#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform float u_specularExponent;               // Specular exponent or shininess property.
uniform samplerCube u_shellTex;

#if defined(GLOBAL_ALPHA)
uniform float u_globalAlpha;                    // Global alpha value
#endif

// Inputs
varying vec3 v_normalVector;                    // NormalVector in view space.
varying vec3 v_cameraDirection;                 // Camera direction
varying vec3 v_normal;

// Global variables
vec4 _baseColor;                                // Base color
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

#ifdef MAXPOINTLIGHT
varying vec4 v_vertexToPointLightDirection[MAXPOINTLIGHT];
#endif

vec3 diffuselight(vec3 lightcolor, vec3 normalVector, vec3 lightDirection, float attenuation)
{
    // Diffuse
    float ddot = dot(normalVector, lightDirection);
    float diffuseIntensity = attenuation * ddot;
    diffuseIntensity = max(0.0, diffuseIntensity);
    return lightcolor * _baseColor.rgb * diffuseIntensity;
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
    
	{
        vec3 vertexToPointLightDirection = normalize(v_vertexToPointLightDirection[0].xyz);
        
        // Fetch point light attenuation.
        float pointLightAttenuation = v_vertexToPointLightDirection[0].w;
        _diffuseColor += diffuselight(u_pointlight[0].color, normalVector, vertexToPointLightDirection, pointLightAttenuation);
        
        vec3 halfVector = normalize(vertexToPointLightDirection + cameraDirection);
        _specularColor += specularlight(u_pointlight[0].color, normalVector, halfVector, pointLightAttenuation);
    }
}
#endif

void main()
{
    // Fetch diffuse color from texture.
	_baseColor = textureCube(u_shellTex, normalize(v_normal))/* * vec4(u_lightColor, 1.0)*/;

    // Apply light
    _specularColor = vec3(0.0, 0.0, 0.0);
    _diffuseColor = vec3(0.0, 0.0, 0.0);
	
    #ifdef MAXPOINTLIGHT
    if (u_npointlight > 0)
        applyPointLight();
    #endif
    
    // Light the pixel
    gl_FragColor.a =	0.9;
	gl_FragColor.rgb =  _diffuseColor + _specularColor;

#if defined(GLOBAL_ALPHA)
    gl_FragColor.a *= u_globalAlpha;
#endif
}
