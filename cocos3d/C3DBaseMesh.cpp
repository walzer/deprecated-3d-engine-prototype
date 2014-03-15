#include "C3DBaseMesh.h"
#include "C3DVertexFormat.h"

namespace cocos3d
{
C3DBaseMesh::C3DBaseMesh(C3DVertexFormat* vertexFormat,PrimitiveType primitiveType)
	: _vertexFormat(NULL), _primitiveType(primitiveType)
{
	_vertexFormat = new C3DVertexFormat(vertexFormat);
}

C3DBaseMesh::~C3DBaseMesh()
{
	SAFE_DELETE(_vertexFormat);
}

const C3DVertexFormat* C3DBaseMesh::getVertexFormat() const
{
    return _vertexFormat;
}

unsigned int C3DBaseMesh::getVertexSize() const
{
    return _vertexFormat->getVertexSize();
}

PrimitiveType C3DBaseMesh::getPrimitiveType() const
{
    return _primitiveType;
}

void C3DBaseMesh::setPrimitiveType(PrimitiveType type)
{
    _primitiveType = type;
}

unsigned int C3DBaseMesh::getTriangleCount() const
{
	return 0;
}
}