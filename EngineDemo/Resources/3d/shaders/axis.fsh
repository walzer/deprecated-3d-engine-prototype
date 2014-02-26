#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif
 varying vec4 v_color;
uniform vec4 u_diffuseColor;

void main(void) 
{
   gl_FragColor = v_color * u_diffuseColor;
}