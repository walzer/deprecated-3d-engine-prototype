#ifdef GL_ES
precision lowp float;
#endif
uniform float radius;
uniform float strength;
uniform vec2 center;

uniform sampler2D SamplerY;
uniform  vec2 texSize ;
varying  vec2 texCoordVarying;
void main()
{
    vec2 coord=texCoordVarying*texSize;
    coord-=center;
    float distance=length(coord);
    
    if(distance<radius)
    {
        float percent=distance/radius;
        if(strength>0.0)
        {
            coord*=mix(1.0,smoothstep(0.0,radius/distance,percent),strength*0.75);
        }
        else
        {
            coord*=mix(1.0,pow(percent,1.0+strength*0.75)*radius/distance,1.0-percent);
        }
        coord+=center;
        gl_FragColor=texture2D(SamplerY,coord/texSize);
        vec2 clampedCoord=clamp(coord,vec2(0.0),texSize);
        if(coord!=clampedCoord)
        {
            gl_FragColor.a*=max(0.0,1.0-length(coord-clampedCoord));
        }
    }
    else{
        gl_FragColor=texture2D(SamplerY, texCoordVarying);
    }
}