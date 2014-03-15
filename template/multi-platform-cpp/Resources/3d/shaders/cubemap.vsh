// Uniforms
uniform mat4 u_worldViewProjectionMatrix;           // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix;     // Matrix to transform a normal to view space.
uniform mat4 u_worldViewMatrix;                     // Matrix to tranform a position to view space.
uniform vec3 u_cameraViewPosition;                  // Position of the camera in view space.

// Inputs
attribute vec4 a_position;                          // Vertex Position (x, y, z, w)
attribute vec3 a_normal;                            // Vertex Normal (x, y, z)

// Outputs
varying vec3 v_normal;
varying vec3 v_normalVector;                        // NormalVector in view space.
varying vec3 v_cameraDirection;                     // Camera direction

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

#ifdef OPENGL_ES
uniform lowp int u_npointlight;
#else
uniform int u_npointlight;
#endif

#ifdef MAXPOINTLIGHT
varying vec4 v_vertexToPointLightDirection[MAXPOINTLIGHT];
#endif

#ifdef MAXPOINTLIGHT
void applyPointLight(vec4 position)
{
    vec4 positionWorldViewSpace = u_worldViewMatrix * position;

	v_cameraDirection = u_cameraViewPosition - positionWorldViewSpace.xyz;
    
	{
        vec3 lightDirection = u_pointlight[0].position - positionWorldViewSpace.xyz;
        
		vec4 vertexToPointLightDirection;
        vertexToPointLightDirection.xyz = lightDirection;
        
        // Attenuation
        vertexToPointLightDirection.w = 1.0 - dot(lightDirection * u_pointlight[0].rangeInverse, lightDirection * u_pointlight[0].rangeInverse);
        vertexToPointLightDirection.w = clamp(vertexToPointLightDirection.w, 0.0, 1.0);

        // Output light direction.
        v_vertexToPointLightDirection[0] =  vertexToPointLightDirection;
    }
}
#endif

void main()
{
    vec4 position = a_position;
    vec3 normal	  = -a_normal;

	v_normal = a_normal;

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

    #ifdef MAXPOINTLIGHT
    if (u_npointlight > 0)
        applyPointLight(position);
    #endif
}
