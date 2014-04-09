#ifndef SCENEMODEL_H_
#define SCENEMODEL_H_

#include "C3DRenderNode.h"
using namespace std;
namespace cocos3d
{
class C3DMaterial;
class C3DScene;
class C3DNode;
class MeshBatch;

/**
*Defines this static object of the scene,which can change material.
*/
class C3DStaticObj : public C3DRenderNode
{
	friend class C3DScene;
	friend class C3DRenderNodeManager;
public:
	virtual bool load(C3DResourceLoader* loader, bool isLoadAll = false);

	C3DNode::Type getType() const;

	void calculateBoundingBox_();

	virtual void copyFrom(const C3DTransform* other, C3DNode::CloneContext& context);
	virtual C3DNode* clone(CloneContext& context) const;
protected:
	static C3DStaticObj* create(const std::string& id);

private:

    C3DStaticObj(const std::string& id);
    ~C3DStaticObj();
};
}

#endif
