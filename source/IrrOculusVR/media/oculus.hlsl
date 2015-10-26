float2 EyeToSourceUVScale, EyeToSourceUVOffset;                                        
float4x4 EyeRotationStart, EyeRotationEnd;             

float2 TimewarpTexCoord(float2 TexCoord, float4x4 rotMat)                              
{                                                                                      
	// Vertex inputs are in TanEyeAngle space for the R,G,B channels (i.e. after chromatic 
	// aberration and distortion). These are now "real world" vectors in direction (x,y,1) 
	// relative to the eye of the HMD.	Apply the 3x3 timewarp rotation to these vectors.
    	float3 transformed = float3( mul ( rotMat, float4(TexCoord.xy, 1, 1) ).xyz); 
    	
	// Project them back onto the Z=1 plane of the rendered images.
    	float2 flattened = (transformed.xy / transformed.z);            
    	
	// Scale them into ([0,0.5],[0,1]) or ([0.5,0],[0,1]) UV lookup space (depending on eye)
    	return(EyeToSourceUVScale * flattened + EyeToSourceUVOffset);                      
}  


struct VS_INPUT
{
	float3 Position:POSITION;
	float2 TexCoord0:TEXCOORD0;
	float2 TexCoord1:TEXCOORD1;
	float4 Color:COLOR0;
	float3 Norm:NORMAL;
};

struct VS_OUTPUT
{
	float4 oPosition:POSITION;
	float2 oTexCoord0:TEXCOORD0;
	float2 oTexCoord1:TEXCOORD1;
	float2 oTexCoord2:TEXCOORD2;
	float oVignette:TEXCOORD3;
};

VS_OUTPUT vs_main(VS_INPUT input)
{             
	VS_OUTPUT output;

	float timewarpLerpFactor = input.Color.a; // extract amount of timewarp
	float Vignette = input.Color.r; // extract vignette

    	float4x4 lerpedEyeRot = lerp(EyeRotationStart, EyeRotationEnd, timewarpLerpFactor);

    	output.oTexCoord0  = TimewarpTexCoord(input.TexCoord0, lerpedEyeRot);                            
    	output.oTexCoord1  = TimewarpTexCoord(input.Norm.xy, lerpedEyeRot);                            
    	output.oTexCoord2  = TimewarpTexCoord(input.TexCoord1, lerpedEyeRot);                            
    	output.oPosition = float4(input.Position.xy, 0.5, 1.0);    
	output.oVignette = Vignette;

	return output;
}

sampler2D Texture : register(s0);

float4 main(float4 oPosition  : POSITION,  float2 oTexCoord0 : TEXCOORD0,     
            	float2 oTexCoord1 : TEXCOORD1, float2 oTexCoord2 : TEXCOORD2,     
            	float  oVignette  : TEXCOORD3) : COLOR                         
{                                                                                      
	// 3 samples for fixing chromatic aberrations
    	float R = tex2D(Texture, oTexCoord0.xy).r;
    	float G = tex2D(Texture, oTexCoord1.xy).g;
    	float B = tex2D(Texture, oTexCoord2.xy).b;

    	return (oVignette*float4(R,G,B,1));                                                
}

