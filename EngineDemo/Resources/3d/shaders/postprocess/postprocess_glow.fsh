#ifdef OPENGL_ES
precision mediump float;
#endif

// Uniforms
uniform sampler2D u_texture;
uniform sampler2D u_glowTexture;
uniform vec4 u_tintColor;
uniform float u_glowIntensity;

// Inputs
varying vec2 v_texCoord;

void main()
{
 
    vec4 src = texture2D(u_glowTexture, v_texCoord) * u_tintColor;
    vec4 dst = texture2D(u_texture, v_texCoord);
    
    
	vec4 glow = vec4( 0.0 );
	float glowWeight = 0.0;
	float glowParam = 0.5;
	if ( src.a <= glowParam )
	{
		glowWeight = src.a/glowParam;
	}
	else
	{
		glowWeight =pow(1.0-clamp(src.a-glowParam, 0.0, (1.0-glowParam))/(1.0-glowParam), 0.125);
	}
	glow = src*glowWeight*(1.0-dst)*u_glowIntensity;
	
	
	
	gl_FragColor = dst+glow;
	
	//gl_FragColor = vec4(0.0);
	//gl_FragColor.r = u_glowIntensity;
   
}