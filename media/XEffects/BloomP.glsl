uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

varying vec2 vTextureCoord0;

float luminance(vec4 c)
{
	return dot( c, vec4(0.3, 0.59, 0.11, 0.0) );
}

void main() 
{		
	vec4 bloomCol = texture2D(ColorMapSampler, vTextureCoord0.xy);
	vec4 screenCol = texture2D(ScreenMapSampler, vTextureCoord0.xy);

	gl_FragColor = (screenCol * 0.9 + bloomCol * 0.5);
}