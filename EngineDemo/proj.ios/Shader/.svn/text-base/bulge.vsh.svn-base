attribute vec4 a_position;
attribute vec2 a_texCoord;
uniform		mat4 u_MVPMatrix;
varying vec2 texCoordVarying;

void main()
{
    gl_Position = u_MVPMatrix*a_position;
    texCoordVarying = a_texCoord;
}
