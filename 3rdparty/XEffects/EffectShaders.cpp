#include "EffectShaders.h"

const char* LIGHT_MODULATE_P[ESE_COUNT] =
{
R"(
    precision mediump float;
 
	uniform sampler2D ColorMapSampler;
	uniform sampler2D ScreenMapSampler;

    varying vec4 vTextureCoord0;

	void main() 
	{		
		vec4 finalCol = texture2D(ColorMapSampler, vTextureCoord0.xy);
		vec4 lightCol = texture2D(ScreenMapSampler, vTextureCoord0.xy);

		gl_FragColor = finalCol * lightCol;
	}
)",

R"(
	sampler2D ColorMapSampler : register(s0);
	sampler2D ScreenMapSampler : register(s1);

	float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0
	{		
		float4 finalCol = tex2D(ColorMapSampler, TexCoords);
		float4 lightCol = tex2D(ScreenMapSampler, TexCoords);

		return finalCol * lightCol;
	}
)"
};


const char* SHADOW_PASS_1P[ESE_COUNT] =
{
R"(
    precision mediump float;
 
    varying vec4 vTextureCoord0;

	void main()
	{
		vec4 vInfo = vTextureCoord0;
		float depth = vInfo.z / vInfo.x;
		gl_FragColor = vec4(depth, depth * depth, 0.0, 0.0);
	}
)",
R"(
	float4 pixelMain(float4 ClipPos: TEXCOORD0) : COLOR0
	{
		float depth = ClipPos.z / ClipPos.x;
		return float4(depth, depth * depth, 0.0, 0.0);
	}
)"
};

const char* SHADOW_PASS_1PT[ESE_COUNT] = 
{
R"(
    precision mediump float;
 
	uniform sampler2D ColorMapSampler;

    varying vec4 vTextureCoord0;
    varying vec4 vTextureCoord1;

	void main() 
	{
		vec4 vInfo = vTextureCoord0;

		float depth = vInfo.z / vInfo.x;

		float alpha = texture2D(ColorMapSampler, vTextureCoord1.xy).a;

	    gl_FragColor = vec4(depth, depth * depth, 0.0, alpha);
	}
)",
R"(
	sampler2D ColorMapSampler : register(s0);

	float4 pixelMain(float4 Color: TEXCOORD0, float2 Texcoords: TEXCOORD1) : COLOR0
	{
		float depth = Color.z / Color.w;
		
		float alpha = tex2D(ColorMapSampler, Texcoords).a;
		
		return float4(depth, depth * depth, 0.0, alpha);
	}
)"
};

const char* SHADOW_PASS_1V[ESE_COUNT] = 
{
R"(
	uniform mat4 mWorldViewProj;
	uniform float MaxD;

    attribute vec2 inTexCoord0;
    attribute vec3 inVertexPosition;

    varying vec4 vTextureCoord0;
    varying vec4 vTextureCoord1;

	void main()
	{
		vec4 tPos = mWorldViewProj * vec4(inVertexPosition, 1.0);
		gl_Position = tPos;
		vTextureCoord0 = vec4(MaxD, tPos.y, tPos.z, tPos.w);

		vTextureCoord1.xy = inTexCoord0.xy;
	}
)",
R"(
	float4x4 mWorldViewProj;
	float MaxD;

	struct VS_OUTPUT 
	{
		float4 Position: POSITION0;
		float4 ClipPos: TEXCOORD0;
		float2 Texcoords: TEXCOORD1;
		float4 VColor: TEXCOORD2;
	};

	VS_OUTPUT vertexMain(float3 Position : POSITION0, float2 Texcoords : TEXCOORD0, float4 vColor : COLOR0)
	{
		VS_OUTPUT  OUT;
		float4 hpos = mul(float4(Position.x, Position.y, Position.z, 1.0), mWorldViewProj);
		OUT.ClipPos = hpos;
		OUT.ClipPos.x = MaxD;
		OUT.Position = hpos;
		OUT.Texcoords = Texcoords;
		OUT.VColor = vColor;
		return(OUT);
	}
)"
};

const char* SHADOW_PASS_2P[ESE_COUNT] = 
{
R"(
    precision mediump float;
 
	uniform sampler2D ShadowMapSampler;
	uniform vec4 LightColour;
	varying float lightVal;

	##ifdef VSM
	float testShadow(vec2 texCoords, vec2 offset, float RealDist)
	{
		vec4 shadTexCol = texture2D(ShadowMapSampler, texCoords + offset);

		float lit_factor = (RealDist <= shadTexCol.x) ? 1.0 : 0.0;

		float E_x2 = shadTexCol.y;
		float Ex_2 = shadTexCol.x * shadTexCol.x;
		float variance = min(max(E_x2 - Ex_2, 0.00001) + 0.000001, 1.0);
		float m_d = (shadTexCol.x - RealDist);
		float p = variance / (variance + m_d * m_d);

		return (1.0 - max(lit_factor, p)) / float(SAMPLE_AMOUNT);
	}
	##else
	float testShadow(vec2 smTexCoord, vec2 offset, float realDistance)
	{
		vec4 texDepth = texture2D(ShadowMapSampler, vec2( smTexCoord + offset));
		float extractedDistance = texDepth.r;
		
		return (extractedDistance <= realDistance) ? (1.0  / float(SAMPLE_AMOUNT)) : 0.0;
	}
	##endif

	vec2 offsetArray[16];

    varying vec4 vTextureCoord0;
    varying vec4 vTextureCoord1;

	void main() 
	{
		vec4 SMPos = vTextureCoord0;
		vec4 MVar = vTextureCoord1;

		offsetArray[0] = vec2(0.0, 0.0);
		offsetArray[1] = vec2(0.0, 1.0);
		offsetArray[2] = vec2(1.0, 1.0);
		offsetArray[3] = vec2(-1.0, -1.0);
		offsetArray[4] = vec2(-2.0, 0.0);
		offsetArray[5] = vec2(0.0, -2.0);
		offsetArray[6] = vec2(2.0, -2.0);
		offsetArray[7] = vec2(-2.0, 2.0);
		offsetArray[8] = vec2(3.0, 0.0);
		offsetArray[9] = vec2(0.0, 3.0);
		offsetArray[10] = vec2(3.0, 3.0);
		offsetArray[11] = vec2(-3.0, -3.0);
		offsetArray[12] = vec2(-4.0, 0.0);
		offsetArray[13] = vec2(0.0, -4.0);
		offsetArray[14] = vec2(4.0, -4.0);
		offsetArray[15] = vec2(-4.0, 4.0);

	    SMPos.xy  = SMPos.xy / SMPos.w / 2.0 + vec2(0.5, 0.5);

		vec4 finalCol = vec4(0.0, 0.0, 0.0, 0.0);

		// If this point is within the light's frustum.
	##ifdef ROUND_SPOTLIGHTS
		float lengthToCenter = length(SMPos.xy - vec2(0.5, 0.5));
		if(SMPos.z - 0.01 > 0.0 && SMPos.z + 0.01 < MVar.z)
	##else
		vec2 clampedSMPos = clamp(SMPos.xy, vec2(0.0, 0.0), vec2(1.0, 1.0));
		if(clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar.z)
	##endif
		{
			float lightFactor = 1.0;
			float realDist = MVar.x / MVar.z - 0.002;
		
			for(int i = 0;i < SAMPLE_AMOUNT; i++)
				lightFactor -= testShadow(SMPos.xy, offsetArray[i] * MVar.w, realDist);

			// Multiply with diffuse.
	##ifdef ROUND_SPOTLIGHTS
			finalCol = LightColour * lightFactor * MVar.y * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);
	##else
			finalCol = LightColour * lightFactor * MVar.y;
	##endif
		}

		gl_FragColor = finalCol;
	}
)",
R"(
	sampler2D ShadowMapSampler : register(s0);
	float4 LightColour;

	##ifdef VSM
	float calcShadow(float2 texCoords, float2 offset, float RealDist)
	{
		float4 shadTexCol = tex2D(ShadowMapSampler, texCoords + offset);

		float lit_factor = (RealDist <= shadTexCol.x);

		float E_x2 = shadTexCol.y;
		float Ex_2 = shadTexCol.x * shadTexCol.x;
		float variance = min(max(E_x2 - Ex_2, 0.00001) + 0.000001, 1.0);
		float m_d = (shadTexCol.x - RealDist);
		float p = variance / (variance + m_d * m_d);
		  
		return (1.0 - max(lit_factor, p)) / SAMPLE_AMOUNT;
	}
	##else
	float calcShadow(float2 texCoords, float2 offset, float RealDist)
	{
	   float4 shadTexCol = tex2D(ShadowMapSampler,texCoords + offset);
	   float extractedDistance = shadTexCol.r;
	      
	   return (extractedDistance <= RealDist ? (1.0  / SAMPLE_AMOUNT) : 0.0);
	}
	##endif

	float4 pixelMain
	(
	   float4 SMPos       : TEXCOORD0,
	   float4 MVar        : TEXCOORD1,
	   float2 TexCoords    : TEXCOORD2
	) : COLOR0
	{
		const float2 offsetArray[16] = 
		{
			float2(0.0, 0.0),
			float2(0.0, 1.0),
			float2(1.0, -1.0),
			float2(-1.0, 1.0),
			float2(-2.0, 0.0),
			float2(0.0, -2.0),
			float2(-2.0, -2.0),
			float2(2.0, 2.0),
			float2(3.0, 0.0),
			float2(0.0, 3.0),
			float2(3.0, -3.0),
			float2(-3.0, 3.0),
			float2(-4.0, 0.0),
			float2(0.0, -4.0),
			float2(-4.0, -4.0),
			float2(4.0, 4.0)
		}
		SMPos.xy = SMPos.xy / SMPos.w + float2(0.5, 0.5);

		float4 finalCol = float4(0.0, 0.0, 0.0, 0.0);

		// If this point is within the light's frustum.
	##ifdef ROUND_SPOTLIGHTS
		float lengthToCenter = length(SMPos.xy - float2(0.5, 0.5));
		if(lengthToCenter < 0.5 && SMPos.z > 0.0 && SMPos.z < MVar[3])
	##else
		float2 clampedSMPos = saturate(SMPos.xy);
		if(clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar[3])
	##endif
		{
			float lightFactor = 1.0;
			float realDistance = MVar[0] / MVar[3] - 0.005;
		
			for(int i = 0;i < SAMPLE_AMOUNT; ++i)
				lightFactor -= calcShadow(SMPos.xy, offsetArray[i] * MVar[2], realDistance);

			// Multiply with diffuse.
	##ifdef ROUND_SPOTLIGHTS
			finalCol = LightColour * lightFactor * MVar[1] * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);
	##else
			finalCol = LightColour * lightFactor * MVar[1];
	##endif
		}
		return finalCol;
	}
)"
};

const char* SHADOW_PASS_2V[ESE_COUNT] = 
{
R"(
	struct VS_OUTPUT 
	{
		vec4 Position;
		vec4 ShadowMapSamplingPos;
		vec4 MVar;
	};

	uniform float MaxD, MAPRES;
	uniform vec3 LightPos;
	uniform mat4 mWorldViewProj;
	uniform mat4 mWorldViewProj2;

    attribute vec3 inVertexPosition;
    attribute vec3 inVertexNormal;
    attribute vec2 inTexCoord0;
    attribute vec2 inTexCoord1;

    varying vec4 vTextureCoord0;
    varying vec4 vTextureCoord1;

	VS_OUTPUT vertexMain( in vec3 Position) 
	{
		VS_OUTPUT OUT;

		OUT.Position = (mWorldViewProj * vec4(Position, 1.0));
		OUT.ShadowMapSamplingPos = (mWorldViewProj2 * vec4(Position, 1.0));

		vec3 lightDir = normalize(LightPos - Position);
		
		OUT.MVar.x = OUT.ShadowMapSamplingPos.z;
		OUT.MVar.y = dot(normalize(inVertexNormal.xyz), lightDir);
		OUT.MVar.z = MaxD;
		OUT.MVar.w = 1.0 / MAPRES;

		return OUT;
	}

	void main() 
	{
		VS_OUTPUT vOut = vertexMain(inVertexPosition.xyz);

		gl_Position = vOut.Position;
		vTextureCoord0 = vOut.ShadowMapSamplingPos;
		vTextureCoord1 = vOut.MVar;
	}
)",
R"(
	float4x4 mWorldViewProj;
	float4x4 mWorldViewProj2;
	float3 LightPos;
	float ShadDark;
	float MaxD;
	float EnableLighting;
	float MAPRES;

	struct VS_OUTPUT 
	{
		float4 Position				: POSITION0;
		float4 ShadowMapSamplingPos : TEXCOORD0; 
		float4 MVar        			: TEXCOORD1;
		float2 TexCoords            : TEXCOORD2;
	};

	VS_OUTPUT vertexMain( 
	   	float3 Position	: POSITION0,
		float2 TexCoords : TEXCOORD0,
		float2 TexCoords2 : TEXCOORD1,
		float3 Normal : NORMAL
	  )
	{
		VS_OUTPUT  OUT;
	    OUT.Position = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj);
		float4 SMPos = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj2);
		SMPos.xy = float2(SMPos.x, -SMPos.y) / 2.0;
		
		OUT.ShadowMapSamplingPos = SMPos;
			
		float3 LightDir = normalize(LightPos - Position.xyz);
		
		OUT.MVar = float4(SMPos.z, dot(Normal, LightDir), 1.0 / MAPRES, MaxD);
		OUT.TexCoords = TexCoords;
		
		return(OUT);
	}
)"
};


const char* SIMPLE_P[ESE_COUNT] = 
{
R"(
    precision mediump float;
 
	uniform sampler2D ColorMapSampler;

    varying vec4 vTextureCoord0;

	void main() 
	{		
		vec4 finalCol = texture2D(ColorMapSampler, vTextureCoord0.xy);
		gl_FragColor = finalCol;
	}
)",
R"(
	sampler2D ColorMapSampler : register(s0);

	float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0
	{		
		float4 finalCol = tex2D(ColorMapSampler, TexCoords);
		return finalCol;
	}
)"
};


const char* WHITE_WASH_P[ESE_COUNT] = 
{
R"(
    precision mediump float;
 
	uniform sampler2D ColorMapSampler;

    varying vec4 vTextureCoord1;

	void main() 
	{
		float alpha = texture2D(ColorMapSampler, vTextureCoord1.xy).a;

	    gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
	}
)",
R"(
	sampler2D ColorMapSampler : register(s0);

	float4 pixelMain(float4 Color: TEXCOORD0, float2 Texcoords: TEXCOORD1) : COLOR0
	{
		float alpha = tex2D(ColorMapSampler, Texcoords).a;

		return float4(1.0, 1.0, 1.0, alpha);
	}
)"
};


const char* WHITE_WASH_P_ADD[ESE_COUNT] = 
{
R"(
    precision mediump float;
 
	uniform sampler2D ColorMapSampler;

    varying vec4 vTextureCoord1;

	float luminance(vec3 color)
	{
		return clamp(color.r * 0.3 + color.g * 0.59 + color.b * 0.11, 0.0, 1.0);
	}
	void main() 
	{
		vec4 diffuseTex = texture2D(ColorMapSampler, vTextureCoord1.xy);
		//diffuseTex *= vTextureCoord2;

	    gl_FragColor = vec4(1.0, 1.0, 1.0, luminance(diffuseTex.rgb));
	}
)",
R"(
	sampler2D ColorMapSampler : register(s0);

	float luminance(float3 color)
	{
		return clamp(color.r * 0.3 + color.g * 0.59 + color.b * 0.11, 0.0, 1.0);
	}

	float4 pixelMain(float4 Color : TEXCOORD0, float2 Texcoords : TEXCOORD1, float4 VColor : TEXCOORD2) : COLOR0
	{
		float4 diffuseTex = tex2D(ColorMapSampler, Texcoords);
		diffuseTex *= VColor;

		return float4(1.0, 1.0, 1.0, luminance(diffuseTex.rgb));
	}
)"
};

const char* SCREEN_QUAD_V[ESE_COUNT] = 
{
R"(
	uniform float screenX, screenY; 
	uniform vec3 LineStarts0, LineStarts1, LineStarts2, LineStarts3; 
	uniform vec3 LineEnds0, LineEnds1, LineEnds2, LineEnds3; 

    attribute vec2 inTexCoord0;
    attribute vec2 inTexCoord1;
    attribute vec3 inVertexPosition;

    varying vec4 vTextureCoord0;
    varying vec4 vTextureCoord1;
    varying vec4 vTextureCoord2;
    varying vec4 vTextureCoord3;

	void main()  
	{ 
		gl_Position = vec4(inVertexPosition.x, inVertexPosition.y, 0.0, 1.0); 
		vec2 tCoords;  
		tCoords.x = 0.5 * (1.0 + inVertexPosition.x);  
		tCoords.y = 0.5 * (1.0 + inVertexPosition.y);  
		vTextureCoord0.xy = tCoords.xy;  
		tCoords.y = 1.0 - tCoords.y;  
		vec3 tLStart = mix(LineStarts0, LineStarts1, tCoords.x);  
		vec3 bLStart = mix(LineStarts2, LineStarts3, tCoords.x);  
		vTextureCoord1.xyz = mix(tLStart, bLStart, tCoords.y);  
		vec3 tLEnd = mix(LineEnds0, LineEnds1, tCoords.x);  
		vec3 bLEnd = mix(LineEnds2, LineEnds3, tCoords.x);  
		vTextureCoord2.xyz = mix(tLEnd, bLEnd, tCoords.y);  
		vTextureCoord3.xy = vec2(screenX, screenY); 
	}
)",
R"(
	float screenX, screenY; 
	float3 LineStarts0, LineStarts1, LineStarts2, LineStarts3; 
	float3 LineEnds0, LineEnds1, LineEnds2, LineEnds3; 
	struct VS_OUTPUT 
	{
		float4 Position		: POSITION0;
		float2 TexCoords	: TEXCOORD0;
		float3 LStart		: TEXCOORD1;
		float3 LEnd			: TEXCOORD2;
		float2 ScreenSize	: TEXCOORD3;
	};

	VS_OUTPUT vertexMain(float3 Position : POSITION0) 
	{ 
		VS_OUTPUT OUT; 
		OUT.Position = float4(Position.x,Position.y, 0.0, 1.0); 
		OUT.TexCoords.x = 0.5 * (1.0 + Position.x + (1.0 / screenX));  
		OUT.TexCoords.y = 1.0 - 0.5 * (1.0 + Position.y - (1.0 / screenY));  
		float3 tLStart = lerp(LineStarts0, LineStarts1, OUT.TexCoords.x);  
		float3 bLStart = lerp(LineStarts2, LineStarts3, OUT.TexCoords.x);  
		OUT.LStart = lerp(tLStart, bLStart, OUT.TexCoords.y);  
		float3 tLEnd = lerp(LineEnds0, LineEnds1, OUT.TexCoords.x);  
		float3 bLEnd = lerp(LineEnds2, LineEnds3, OUT.TexCoords.x);  
		OUT.LEnd = lerp(tLEnd, bLEnd, OUT.TexCoords.y);  
		OUT.ScreenSize = float2(screenX, screenY); 
		return(OUT); 
	}
)"
};


const char* VSM_BLUR_P[ESE_COUNT] = 
{
R"(
    precision mediump float;
 
	uniform sampler2D ColorMapSampler;

	vec2 offsetArray[5];

    varying vec4 vTextureCoord0;
    varying vec4 vTextureCoord3;

	void main() 
	{

	##ifdef VERTICAL_VSM_BLUR
		offsetArray[0] = vec2(0.0, 0.0);
		offsetArray[1] = vec2(0.0, -1.5 / vTextureCoord3.y);
		offsetArray[2] = vec2(0.0, 1.5 / vTextureCoord3.y);
		offsetArray[3] = vec2(0.0, -2.5 / vTextureCoord3.y);
		offsetArray[4] = vec2(0.0, 2.5 / vTextureCoord3.y);
	##else
		offsetArray[0] = vec2(0.0, 0.0);
		offsetArray[1] = vec2(-1.5 / vTextureCoord3.x, 0.0);
		offsetArray[2] = vec2(1.5 / vTextureCoord3.x, 0.0);
		offsetArray[3] = vec2(-2.5 / vTextureCoord3.x, 0.0);
		offsetArray[4] = vec2(2.5 / vTextureCoord3.x, 0.0);
	##endif

		vec4 BlurCol = vec4(0.0, 0.0, 0.0, 0.0);

		for(int i = 0;i < 5;++i)
			BlurCol += texture2D(ColorMapSampler, clamp(vTextureCoord0.xy + offsetArray[i], vec2(0.001, 0.001), vec2(0.999, 0.999)));

		gl_FragColor = BlurCol / 5.0;
	}
)",
R"(
	sampler2D ColorMapSampler : register(s0);

	float4 pixelMain ( float4 Texcoords : TEXCOORD0, float2 ScreenSize : TEXCOORD3 ) : COLOR0
	{
		float2 offsetArray[5];
	##ifdef VERTICAL_VSM_BLUR
		offsetArray[0] = float2(0, 0);
		offsetArray[1] = float2(0, 1.5 / ScreenSize.y);
		offsetArray[2] = float2(0, -1.5 / ScreenSize.y);
		offsetArray[3] = float2(0, 2.5 / ScreenSize.y);
		offsetArray[4] = float2(0, -2.5 / ScreenSize.y);
	##else
		offsetArray[0] = float2(0, 0);
		offsetArray[1] = float2(1.5 / ScreenSize.x, 0);
		offsetArray[2] = float2(-1.5 / ScreenSize.x, 0);
		offsetArray[3] = float2(2.5 / ScreenSize.x, 0);
		offsetArray[4] = float2(-2.5 / ScreenSize.x, 0);
	##endif

		float4 finalVal = float4(0.0, 0.0, 0.0, 0.0);

		for(int i = 0;i < 5;++i)
			finalVal += tex2D(ColorMapSampler, clamp(Texcoords.xy + offsetArray[i], float2(0.001, 0.001), float2(0.999, 0.999)));

		return finalVal / 5.0;
	}
)"
};
