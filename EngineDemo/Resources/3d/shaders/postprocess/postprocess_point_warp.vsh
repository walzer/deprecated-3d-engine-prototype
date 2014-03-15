// Inputs
attribute vec2 a_position;
attribute vec2 a_texCoord;

// Varying
varying vec2 v_texCoord;
varying float v_power;


#define MAX_CLICKS	8

// vec4.xy	ÖÐÐÄÎ»ÖÃ
// vec4.z	Ó°Ïì°ë¾¶
// vec4.w	×î´ó°ë¾¶
uniform vec4 u_clicks[MAX_CLICKS];

void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
    
    vec2 offset = vec2(0.0, 0.0);
    for (int i = 0; i < MAX_CLICKS; i++)
    {
	    vec4 click = u_clicks[i];
    //click.z = 0.01;
    	if ( click.z > 0.0 )
    	{
    		vec2 dir = a_texCoord-click.xy;
    		float dis = length(dir);
    			
    			
    		if ( dis < click.z )
    		{
    			// µ¥´¿ÅòÕÍ
    			//float offDis = dis/click.z;
    			//offset -= (1.0-offDis)*dis*normalize(dir);
    			
    			
    			
    			float offDis = dis/click.z;
    			offDis = (1.0-offDis);
    			
    			float halfWavelength = 0.5*click.w;		// ²¨³¤
    			halfWavelength *= max((1.0-click.z/click.w), 0.25)*2.0;
    			float crest = click.z-halfWavelength;	// ²¨·å
    			
				float disToCrest = min(abs(dis-crest), halfWavelength);	// ¾à²¨·å¾àÀë
    			disToCrest = 1.0-(disToCrest/halfWavelength);	// ¾à²¨·åÖµÎª1.±ßÔµÖµÎª0
    			offDis *= disToCrest;	// ²¨·åË¥¼õ
    			
    			offDis *= (1.0-click.z/click.w)*2.0;	//Ëæ°ë¾¶À©´óË¥¼õ
    			
    			offset -= offDis*dis*normalize(dir);
    		}
    	}
    }
    
    v_texCoord = a_texCoord+offset;
}