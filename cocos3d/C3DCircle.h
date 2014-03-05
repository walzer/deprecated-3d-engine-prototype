#ifndef C3DCIRCLE_H_
#define C3DCIRCLE_H_

#include "C3DMatrix.h"
#include "C3DVector3.h"

namespace cocos3d
{
class C3DRay;
class C3DVector3;

/**
 * Defines a class describe 3-dimensional Circle.
 */
class C3DCircle
{
public:
	C3DCircle(const C3DVector3& center, const C3DVector3& normal, float radius);

	virtual ~C3DCircle(){;}

	virtual bool Intersect(const C3DRay *ray)const;

	float dist(const C3DRay *ray)const;

	void setCenter(const C3DVector3& center){_center = center;}

	C3DVector3 getCenter(){return _center;}

	void setRadius(float radius){_radius = radius;}

	float getRadius() const {return _radius;}

private:
	C3DVector3 _center;
	C3DVector3 _normal;
	float _radius;
};
}

#endif//C3DCIRCLE_H_
