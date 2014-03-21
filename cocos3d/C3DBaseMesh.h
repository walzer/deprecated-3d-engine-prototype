#ifndef BASEMESH_H_
#define BASEMESH_H_

#include "cocos2d.h"
#include "EnumDef_GL.h"

namespace cocos3d
{
class C3DVertexFormat;

    class C3DBaseMesh : public cocos2d::Ref
{
public:
	C3DBaseMesh(C3DVertexFormat* vertexFormat,PrimitiveType primitiveType = PrimitiveType_TRIANGLES);
    virtual ~C3DBaseMesh();

	const C3DVertexFormat* getVertexFormat() const;

    unsigned int getVertexSize() const;

	PrimitiveType getPrimitiveType() const;

    void setPrimitiveType(PrimitiveType type);

	virtual unsigned int getTriangleCount() const;

protected:
	const C3DVertexFormat* _vertexFormat;
	PrimitiveType _primitiveType;
};
}

#endif
