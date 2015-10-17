uniform sampler2D ColorMapSampler;

varying vec2 vTextureCoord0;

void main() 
{		
	vec4 finalVal = texture2D(ColorMapSampler, vTextureCoord0.xy);
	finalVal *= 2.0;
	finalVal = finalVal * finalVal * finalVal;

	gl_FragColor = finalVal;
}