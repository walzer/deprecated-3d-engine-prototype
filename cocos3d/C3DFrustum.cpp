#include "Base.h"
#include "C3DFrustum.h"
#include "C3DCamera.h"

namespace cocos3d
{

    bool C3DFrustum::initFrustum(C3DCamera* pCamera)
    {
        _bInit = true;
        createPlane(pCamera);
        return true;
    }
    bool C3DFrustum::isOutFrustum(const C3DAABB& aabb) const
    {
        if (_bInit)
        {
            C3DVector3 point;

            int nplane = _bClipZ ? 6 : 4;
            for (int i = 0; i < nplane; i++) {
                const C3DVector3& normal = _plane[i].getNormal();
                point.x = normal.x < 0 ? aabb._max.x : aabb._min.x;
                point.y = normal.y < 0 ? aabb._max.y : aabb._min.y;
                point.z = normal.z < 0 ? aabb._max.z : aabb._min.z;
                if (_plane[i].pointClassify(point) == C3DPlane::FRONT_PLANE )//kmPlaneClassifyPoint(&_plane[i], &point) == POINT_INFRONT_OF_PLANE)
                    return true;
            }
        }
        return false;
    }

    bool C3DFrustum::isOutFrustum(const C3DOBB& obb) const
    {
        if (_bInit)
        {
            C3DVector3 point;

            int nplane = _bClipZ ? 6 : 4;

            for (int i = 0; i < nplane; i++) {
                const C3DVector3& normal = _plane[i].getNormal();

                point = obb.center;
                point = normal.dot(obb.xAxis) > 0 ? point - obb.extX : point + obb.extX;//obb.xAxis.dot()
                point = normal.dot(obb.yAxis) > 0 ? point - obb.extY : point + obb.extY;
                point = normal.dot(obb.zAxis) > 0 ? point - obb.extZ : point + obb.extZ;

                if (_plane[i].pointClassify(point) == C3DPlane::FRONT_PLANE )//kmPlaneClassifyPoint(&_plane[i], &point) == POINT_INFRONT_OF_PLANE)
                    return true;
            }
        }
        return  false;
    }

    void C3DFrustum::createPlane(C3DCamera* pcamera)
    {
		const C3DMatrix& mat = pcamera->getViewProjectionMatrix();
		//ref http://www.lighthouse3d.com/tutorials/view-frustum-culling/clip-space-approach-extracting-the-planes/
		//extract frustum plane
		_plane[0].initPlane(-C3DVector3(mat.m[3] + mat.m[0], mat.m[7] + mat.m[4], mat.m[11] + mat.m[8]), (mat.m[15] + mat.m[12]));//left
		_plane[1].initPlane(-C3DVector3(mat.m[3] - mat.m[0], mat.m[7] - mat.m[4], mat.m[11] - mat.m[8]), (mat.m[15] - mat.m[12]));//right
		_plane[2].initPlane(-C3DVector3(mat.m[3] + mat.m[1], mat.m[7] + mat.m[5], mat.m[11] + mat.m[9]), (mat.m[15] + mat.m[13]));//bottom
		_plane[3].initPlane(-C3DVector3(mat.m[3] - mat.m[1], mat.m[7] - mat.m[5], mat.m[11] - mat.m[9]), (mat.m[15] - mat.m[13]));//top
		_plane[4].initPlane(-C3DVector3(mat.m[3] + mat.m[2], mat.m[7] + mat.m[6], mat.m[11] + mat.m[10]), (mat.m[15] + mat.m[14]));//near
		_plane[5].initPlane(-C3DVector3(mat.m[3] - mat.m[2], mat.m[7] - mat.m[6], mat.m[11] - mat.m[10]), (mat.m[15] - mat.m[14]));//far
    }
}