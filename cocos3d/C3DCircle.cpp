#include "Base.h"
#include "C3DCircle.h"
#include "C3DRay.h"
#include "C3DVector3.h"
#include "C3DPlane.h"

namespace cocos3d
{
C3DCircle::C3DCircle(const C3DVector3& center, const C3DVector3& normal, float radius)
:_center(center)
,_normal(normal)
,_radius(radius)
{
}

bool C3DCircle::Intersect(const C3DRay *ray)const
{
	C3DPlane *plane = new C3DPlane(_normal, _center);
    float t = ray->dist(plane);
    if ( t > FLT_EPSILON)
    {
        C3DVector3 hitpoint = ray->intersects(plane);;
        if ( (hitpoint - _center).length() < _radius )
        {
			SAFE_DELETE(plane);
            return true;
        }
        else
		{
			SAFE_DELETE(plane);
            return false;
		}
    }
	SAFE_DELETE(plane);
    return false;
};

float C3DCircle::dist(const C3DRay *ray)const
{
	C3DPlane *plane = new C3DPlane(_normal, _center);
	float t = ray->dist(plane);
	if ( t > FLT_EPSILON)
	{
		C3DVector3 hitpoint = ray->intersects(plane);;
		if ( (hitpoint - _center).length() < _radius )
		{
			SAFE_DELETE(plane);
			return t;
		}
	}
	SAFE_DELETE(plane);
	return -1;
}
}