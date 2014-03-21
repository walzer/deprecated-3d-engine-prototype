#include "Npc.h"
#include "MainLayer.h"

using namespace cocos3d;

namespace cocos2d
{
Npc::Npc(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer):C3DActor(name,node,layer)
{
	_state = Npc::State_Idle;

	//_animListener = new Npc::AnimListenerObject(this);

	/*C3DAnimationClip* speakClip = (static_cast<cocos3d::C3DSprite*>(_node))->getAnimationClip("speak");
	if(speakClip != NULL)
	{
		cocos3d::C3DActionListener* endAction = cocos3d::C3DActionListener::create(_animListener,((cocos3d::ListenerFunction)(&cocos2d::Npc::AnimListenerObject::onEnd)));

		speakClip->addActionEvent(endAction,speakClip->getDuration());
	}*/
}

Npc::~Npc()
{
	//SAFE_DELETE(_animListener);
}

void Npc::init()
{
}

Npc::AnimListenerObject::AnimListenerObject(Npc* npc)
{
	_npc = npc;
}

void Npc::AnimListenerObject::onEnd()
{
	_npc->_state = Npc::State_Idle;
}

C3DActor::Type Npc::getType()
{
	return C3DActor::ActorType_Npc;
}

void Npc::speak()
{
	_state = Npc::State_Speak;
	(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip( "speak" );
	CCLabelTTF* label = CCLabelTTF::create();
	label->initWithString("kill the enemy", "Arial", 30);
	MainLayer::getMainLayer()->addChild(label);
	label->setTag(10000);
	C3DLayer* layer = MainLayer::getMainLayer()->m_pLayer3D;
	C3DVector3 pos = _node->getTranslationWorld();
	pos.y -= 5.0f;
	C3DVector2 labelpos;
	layer->get3DScene()->getActiveCamera()->project(layer->getViewport(), &pos, &labelpos);

	label->setPosition(ccp(labelpos.x, labelpos.y));
}

void Npc::update(long elapsedTime)
{
	updateState(elapsedTime);

	if(_state == Npc::State_Speak)
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip( "speak" );
	}
	else
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip( "idle" );
		CCNode* node = MainLayer::getMainLayer()->getChildByTag(10000);
		if (node)
			MainLayer::getMainLayer()->removeChild(node);
	}
}

static float speakTime = 0;
void Npc::updateState(long elapsedTime)
{
	if(_state == Npc::State_Idle)
		return;

	if(_state == Npc::State_Speak)
	{
		speakTime += elapsedTime;
		if(speakTime > 2000.0f)
		{
			speakTime = 0;
			_state = Npc::State_Idle;
		}
	}

}
}