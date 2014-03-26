#ifndef C3DBASEMODEL_H_
#define C3DBASEMODEL_H_

#include "cocos2d.h"
#include "Base.h"

namespace cocos3d
{
class C3DMaterial;
class C3DBaseMesh;
class C3DNode;

class C3DBaseModel : public cocos2d::Ref
{
public:

	C3DBaseModel();

    virtual ~C3DBaseModel();

	virtual void draw() = 0;

	//virtual void setNode(C3DNode* node) = 0;

	//virtual void setMesh(C3DBatchMesh* mesh) = 0;

public:
	//C3DNode* _node;
	//C3DBaseMesh* _mesh;
	//C3DMaterial* _material;
};
}

#endif
