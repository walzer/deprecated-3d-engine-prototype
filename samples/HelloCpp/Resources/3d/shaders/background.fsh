#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform sampler2D u_diffuseTexture;

// Varyings
varying vec2 v_texCoord;


void main()
{
    gl_FragColor = texture2D(u_diffuseTexture, v_texCoord);
    gl_FragColor.a = 0.0;
}
