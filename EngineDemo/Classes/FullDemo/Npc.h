#ifndef __Npc_H__
#define __Npc_H__

#include "cocos2d.h"
//#include "cocos3d.h"
#include "C3DActor.h"
#include "C3DVector2.h"

namespace cocos3d
{
	class C3DSprite;
	class C3DVector2;
}

namespace cocos2d
{
class Npc : public C3DActor
{
public:
	Npc(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer);
	virtual ~Npc();

	class AnimListenerObject : public cocos3d::ListenerObject
	{
	public:
		AnimListenerObject(Npc* npc);
		void onEnd();

	protected:
		Npc* _npc;
	};

	enum State
	{
		State_Idle = 0,
		State_Speak,
	};

	virtual C3DActor::Type getType();

	virtual void init();

	virtual void update(long elapsedTime);

	void speak();

private:
	void updateState(long elapsedTime);

private:
	unsigned int _state;

	Npc::AnimListenerObject* _animListener;
};
}
#endif
