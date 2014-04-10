#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform vec3 u_lightColor;                      // Light color
uniform vec3 u_ambientColor;                    // Ambient color
uniform float u_specularExponent;               // Specular exponent or shininess property.
uniform sampler2D u_diffuseTexture;             // Diffuse texture.
#if defined(GLOBAL_ALPHA)
uniform float u_globalAlpha;                    // Global alpha value
#endif

// Inputs
varying vec3 v_normalVector;                    // NormalVector in view space.
varying vec2 v_texCoord;                        // Texture coordinate (u, v).
varying vec3 v_cameraDirection;                 // Camera direction

varying vec3 v_posInView;

// Global variables
vec4 _baseColor;                                // Base color
vec3 _ambientColor;                             // Ambient Color
vec3 _diffuseColor;                             // Diffuse Color
vec3 _specularColor;                            // Specular color

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

#ifdef PTLIGHT
uniform samplerCube u_shellTex;
varying vec3 		v_lightRefDir;
#endif

vec3 diffuselight(vec3 lightcolor, vec3 normalVector, vec3 lightDirection, float attenuation)
{
    // Diffuse
    float ddot = dot(normalVector, lightDirection);
    float diffuseIntensity = attenuation * ddot;
    diffuseIntensity = max(0.0, diffuseIntensity);
    return lightcolor * diffuseIntensity;
}

vec3 specularlight(vec3 lightcolor, vec3 normalVector, vec3 halfVector, float attenuation)
{
    // Specular
    float specularIntensity = attenuation * max(0.0, pow(dot(normalVector, halfVector), u_specularExponent));
    specularIntensity = max(0.0, specularIntensity);
    return lightcolor * _baseColor.rgb * specularIntensity;
}

#ifdef MAXPOINTLIGHT
void applyPointLight()
{
    // Normalize the vectors.
    vec3 normalVector = normalize(v_normalVector);
    vec3 cameraDirection = normalize(v_cameraDirection);
    
    //for (int i = 0; i < u_npointlight; i++)
	{
		vec3 lightDirection = u_pointlight[0].position - v_posInView;
        
		vec3 vertexToPointLightDirection;
        vertexToPointLightDirection = normalize(lightDirection);
        
        // Attenuation
        float atten = 1.0 - dot(lightDirection * u_pointlight[0].rangeInverse, lightDirection * u_pointlight[0].rangeInverse);
		atten = clamp(atten, 0.0, 1.0);
		//  
        _diffuseColor += diffuselight(u_pointlight[0].color, normalVector, vertexToPointLightDirection, atten);
        
        vec3 halfVector = normalize(vertexToPointLightDirection + cameraDirection);
        
        _specularColor += specularlight(u_pointlight[0].color, normalVector, halfVector, atten); 
    }
}
#endif

void main()
{
    // Fetch diffuse color from texture.
    _baseColor = texture2D(u_diffuseTexture, v_texCoord);

    // Apply light
    _specularColor = vec3(0.0, 0.0, 0.0);
    _diffuseColor = vec3(0.0, 0.0, 0.0);

    // Ambient
    _ambientColor = _baseColor.rgb * u_ambientColor;
    
    #ifdef MAXPOINTLIGHT
    if (u_npointlight > 0)
        applyPointLight();
    #endif
    
    // Light the pixel
    gl_FragColor.a = _baseColor.a;
	gl_FragColor.rgb = _ambientColor + _diffuseColor + _specularColor;
	
	#ifdef PTLIGHT
	gl_FragColor *= textureCube(u_shellTex, normalize(v_lightRefDir));
	#endif

#if defined(GLOBAL_ALPHA)
    gl_FragColor.a *= u_globalAlpha;
#endif

	gl_FragColor.rgba = vec4(u_ambientColor, 0.8);
}
