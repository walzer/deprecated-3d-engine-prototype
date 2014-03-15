#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

// Global variables
vec4 _baseColor;                                // Base color

void main()
{
    // Fetch diffuse color from texture.
    _baseColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    // Light the pixel
    gl_FragColor = _baseColor;
}
