#include "C3DBaseActor.h"
#include "C3DNode.h"
#include "C3DSprite.h"
#include "C3DModel.h"
#include "C3DModelNode.h"
#include "C3DMaterial.h"
namespace cocos2d
{
C3DBaseActor::C3DBaseActor(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer)
{
	_node = node;
	_name = name;
	_layer = layer;
	_life = 30.f;
}

C3DBaseActor::~C3DBaseActor()
{
	_layer = NULL;
}

void C3DBaseActor::moveTo(cocos3d::C3DVector3& target)
{
	_target = target;
}

cocos3d::C3DNode* C3DBaseActor::getNode()
{
	return _node;
}
}