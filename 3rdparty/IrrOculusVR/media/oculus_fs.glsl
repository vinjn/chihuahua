

uniform sampler2D Texture0;
    

void main()
{
   float ResultR = texture2D(Texture0, gl_TexCoord[0].xy, 0.0).r;
   float ResultG = texture2D(Texture0, gl_TexCoord[1].xy, 0.0).g;
   float ResultB = texture2D(Texture0, gl_TexCoord[2].xy, 0.0).b;
    
   gl_FragColor = vec4(ResultR * gl_TexCoord[3].x, ResultG * gl_TexCoord[3].x, ResultB * gl_TexCoord[3].x, 1.0);
}
