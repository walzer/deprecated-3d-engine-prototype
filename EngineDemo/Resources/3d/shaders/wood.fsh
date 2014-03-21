#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

const float GrainSizeRecip = 7.0;
const vec3 DarkColor = vec3(0.6, 0.3, 0.1);
const vec3 spread = vec3(0.15, 0.075, 0.0);

varying float lightIntensity; 
varying vec3 Position;

void main(void)
{
	//
	// cheap noise
	//
	vec3 location = Position;
	vec3 floorvec = vec3(floor(10.0 * Position.x), 0.0, floor(10.0 * Position.z));
	vec3 noise = Position * 10.0 - floorvec - 0.5;
	noise *= noise;
	location += noise * 0.12;
		
	//
	// distance from axis
	//
	float dist = location.x * location.x + location.z * location.z;
	float grain = dist * GrainSizeRecip;

	//
	// grain effects as function of distance
	//
	float brightness = fract(grain);    
	if (brightness > 0.5) 
		brightness = 1.0 - brightness;
	vec3 color = DarkColor + brightness * spread;

	//
	// apply lighting effects from vertex processor
	//
	color = clamp(color * lightIntensity, 0.0, 1.0);

	gl_FragColor = vec4(color, 1.0);
}