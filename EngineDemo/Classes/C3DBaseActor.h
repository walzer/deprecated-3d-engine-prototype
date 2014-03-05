#ifndef __C3DBaseActor_H__
#define __C3DBaseActor_H__

#include "cocos2d.h"
#include "cocos3d.h"
#include <string>
#include "C3DBaseActor.h"
namespace cocos3d
{
	class C3DNode;
	class C3DSprite;
	class C3DVector3;
	class C3DLayer;
}

namespace cocos2d
{
class C3DBaseActor : public CCObject
{
public:
	C3DBaseActor(std::string& name,cocos3d::C3DNode* node, cocos3d::C3DLayer* layer);
	virtual ~C3DBaseActor();

	enum Type
	{
		ActorType_None = 0,
		ActorType_MainPlayer,
		ActorType_Enemy,
		ActorType_Npc,
		ActorType_Bullet,
	};

	virtual void init(){};

	virtual void update(long elapsedTime){};

	virtual C3DBaseActor::Type getType(){return ActorType_None;};

	virtual void moveTo(cocos3d::C3DVector3& target);

	cocos3d::C3DNode* getNode();

protected:
	std::string _name;
	cocos3d::C3DNode* _node;
	cocos3d::C3DVector3 _target;
	cocos3d::C3DLayer* _layer;
	float _life;
};
}
#endif
