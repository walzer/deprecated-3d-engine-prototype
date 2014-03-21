// Uniforms
uniform mat4 u_worldViewProjectionMatrix;           // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix;     // Matrix to transform a normal to view space.
uniform mat4 u_worldViewMatrix;                     // Matrix to tranform a position to view space.
uniform vec3 u_cameraViewPosition;                      // Position of the camera in view space.
uniform mat4 u_worldMatrix;
// Inputs
attribute vec4 a_position;                          // Vertex Position (x, y, z, w)
attribute vec3 a_normal;                            // Vertex Normal (x, y, z)
attribute vec2 a_texCoord;                          // Vertex Texture Coordinate (u, v)

// Outputs
varying vec3 v_normalVector;                        // NormalVector in view space.
varying vec2 v_texCoord;                            // Texture coordinate (u, v).
varying vec3 v_cameraDirection;                     // Camera direction

varying vec3 v_posInView;

#ifdef MAXPOINTLIGHT
struct PointLight
{
    vec3 position;
    vec3 color;
    float rangeInverse;
};
#endif

#ifdef MAXPOINTLIGHT
uniform PointLight u_pointlight[MAXPOINTLIGHT];
#endif

#ifdef PTLIGHT
uniform vec3 u_ligthPos;
uniform mat4 u_lightInvWorld;
varying vec3 v_lightRefDir;
#endif

void main()
{
    vec4 position = a_position;
    vec3 normal = a_normal;

    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Transform normal to view space.
    mat3 normalMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz,
                             u_inverseTransposeWorldViewMatrix[1].xyz,
                             u_inverseTransposeWorldViewMatrix[2].xyz);
    v_normalVector = normalMatrix * normal;

    // Compute the camera direction.
    vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    v_cameraDirection = u_cameraViewPosition - positionWorldViewSpace.xyz;

	v_posInView = positionWorldViewSpace.xyz;

	// Pass on the texture coordinates to Fragment shader.
    v_texCoord = a_texCoord;

	#ifdef PTLIGHT
	vec3 vLightNormal	= normalize((u_worldMatrix * position).xyz - u_ligthPos);
	v_lightRefDir		= normalize((u_lightInvWorld * vec4(vLightNormal, 0.0)).xyz);
	#endif
}
