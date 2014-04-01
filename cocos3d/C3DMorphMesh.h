#ifndef MORPHMESH_H_
#define MORPHMESH_H_

#include "C3DVector3.h"
#include "Base.h"
#include "EnumDef_GL.h"
#include "cocos2d.h"
#include "C3DMesh.h"

namespace cocos3d
{
class C3DVertexFormat;
class C3DMorph;

/**
 * Defines a mesh supporting morph deform.
 */
class C3DMorphMesh : public C3DMesh
{
    friend class C3DModel;
    friend class C3DResourceLoader;
	friend class Geo;

public:
	static C3DMorphMesh* createMesh(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic);
    virtual void setVertexData(void* vertexData, unsigned int vertexStart = 0, unsigned int vertexCount = 0);

	void pushMorph(C3DMorph* morph,int morphTargetIndex,float weight);
	void popMorph(C3DMorph* morph,int morphTargetIndex);
	void changeMorph(C3DMorph* morph,int morphTargetIndex,float weight);
	void clearMorph(C3DMorph* morph);

    virtual ~C3DMorphMesh();
protected:
	virtual void reload();
private:
	void setMorphVertexData(void* vertexData);
	C3DMorphMesh(C3DVertexFormat* vertexFormat,PrimitiveType primitiveType = PrimitiveType_TRIANGLES);
	void applyMorph(C3DMorph* morph);

private:

	unsigned char* _vertexData;
};
}

#endif
