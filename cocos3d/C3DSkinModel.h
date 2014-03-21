#ifndef C3DSKINMODEL_H_
#define C3DSKINMODEL_H_

#include "C3DModel.h"
#include "C3DNode.h"

namespace cocos3d
{
class C3DMeshSkin;

/**
 * Defines a model which have skin mesh.
 */
class C3DSkinModel : public C3DModel
{
public:
	C3DMeshSkin* getSkin();
	void setSkin(C3DMeshSkin* skin);

    C3DSkinModel();

    ~C3DSkinModel();

	static C3DModel* create();

	void draw(void);
	//void channelDraw();

    virtual C3DSkinModel* clone(C3DNode::CloneContext& context) const;

	virtual std::string getDefaultMaterialName();

private:
	 C3DMeshSkin* _skin;
};
}

#endif
