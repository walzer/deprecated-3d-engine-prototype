// uniform
uniform mat4 u_worldViewProjectionMatrix;       
uniform mat4 u_worldViewMatrix;					
uniform mat4 u_inverseTransposeWorldViewMatrix; 

// attribute
attribute vec4 a_position;						
attribute vec3 a_normal;						

// const 
const vec3		LightPosition = vec3(0.0, 0.0, 4.0);
const float		Scale = 6.25;					

// varying
varying float lightIntensity;
varying vec3 Position;

void main(void)
{
	vec4 pos = u_worldViewMatrix * a_position;
	Position = vec3(a_position) * Scale;
	vec3 tnorm = (u_inverseTransposeWorldViewMatrix * vec4(a_normal, 0.0)).xyz; 
	lightIntensity = abs(dot(normalize(LightPosition - vec3(pos)), tnorm)) * 50.5;
	gl_Position = u_worldViewProjectionMatrix * a_position;
}
