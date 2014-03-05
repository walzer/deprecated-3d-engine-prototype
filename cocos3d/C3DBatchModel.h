#ifndef BATCHMODEL_H_
#define BATCHMODEL_H_

#include "C3DRenderChannel.h"
#include "C3DBaseModel.h"

namespace cocos3d
{
class C3DMaterial;
class C3DBatchMesh;
class C3DNode;
class C3DRenderChannel;

/**
 * Defines a C3DBatchModel which contain a C3DBatchMesh and a material.
 */
class C3DBatchModel : public C3DBaseModel
{
    friend class C3DBatchMesh;

public:

	C3DBatchModel(C3DNode* node);

    virtual ~C3DBatchModel();

	static C3DBatchModel* createDebugModel(C3DNode* node);

    C3DBatchMesh* getMesh() const;
	void setMesh(C3DBatchMesh* mesh);

	C3DMaterial* getMaterial();
    void setMaterial(C3DMaterial* material);

	void add(const unsigned char* vertices, unsigned int vertexCount);
	void add(const unsigned char* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount);

	unsigned int getVertexCapacity() const;
	void setVertexCapacity(unsigned int capacity);

	unsigned int getIndexCapacity() const;
	void setIndexCapacity(unsigned int capacity);

	void clear();

	virtual	void draw(void);

	C3DRenderChannel* getRenderChannel();

private:
	void bindVertex(void);
	void setMaterialNodeBinding(void);

private:
	C3DNode* _node;
	C3DBatchMesh* _mesh;
    C3DMaterial* _material;
};
}

#endif
