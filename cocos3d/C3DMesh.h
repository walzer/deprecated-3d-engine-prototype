#ifndef MESH_H_
#define MESH_H_

#include "C3DBaseMesh.h"

namespace cocos3d
{
class C3DVertexFormat;
class MeshPart;
class C3DAABB;

/**
 * Defines a mesh supporting various vertex formats and 1 or more
 * MeshPart(s) to define how the vertices are connected.
 */
class C3DMesh : public C3DBaseMesh
{
    friend class C3DModel;
    friend class C3DResourceLoader;
	friend class Geo;

public:

	/**
	create mesh
	@param vertexFormat defines vertex format
	@param vertexCount vertex count
	@param dynamic dynamic increase the vertex number or not
	*/
    static  C3DMesh* createMesh(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic = false);

    const std::string& getUrl() const;

    unsigned int getVertexCount() const;

    VertexBufferHandle getVertexBuffer() const;

    bool isDynamic() const;

    virtual void setVertexData(void* vertexData, unsigned int vertexStart = 0, unsigned int vertexCount = 0);

    MeshPart* addPart(PrimitiveType primitiveType, IndexFormat indexFormat, unsigned int indexCount, bool dynamic = false);

    unsigned int getPartCount() const;

    MeshPart* getPart(unsigned int index);

    virtual unsigned int getTriangleCount() const;

    virtual ~C3DMesh();

	void reset();

	C3DAABB* getBoundingBox() { return _boundingBox; };

protected:
	C3DMesh(C3DVertexFormat* vertexFormat,PrimitiveType primitiveType = PrimitiveType_TRIANGLES);
	bool init(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic);
	virtual void reload();

protected:

    std::string _url;
    unsigned int _vertexCount;
    VertexBufferHandle _vertexBuffer;
    unsigned int _partCount;
    MeshPart** _parts;
    bool _dynamic;
	C3DAABB* _boundingBox;
};

}
#endif
