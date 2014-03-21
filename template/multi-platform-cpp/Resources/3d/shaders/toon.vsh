// uniform
uniform mat4 u_worldViewProjectionMatrix;      
uniform mat4 u_inverseTransposeWorldViewMatrix;

// attribute
attribute vec4 a_position;						
attribute vec3 a_normal;						

// varying
varying float intensity;

void main(void)
{
	vec3 Normal;
	gl_Position = u_worldViewProjectionMatrix * a_position;
	Normal = (u_inverseTransposeWorldViewMatrix * vec4(a_normal, 0.0)).xyz;
	intensity = abs( dot(vec3(0,0,1),normalize(Normal)) );
}
