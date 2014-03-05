#ifndef __Enemy_H__
#define __Enemy_H__

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
class Enemy : public C3DActor
{
public:
	Enemy(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer);
	virtual ~Enemy();

	class AnimListenerObject : public cocos3d::ListenerObject
	{
	public:
		AnimListenerObject(Enemy* player): _player(player)
		{
		}
		void onHurtEnd();
		void onStandupEnd();
		void onInjureEnd();

	protected:
		Enemy* _player;
	};

	enum State
	{
		State_None = 0,
		State_Idle = 0x01,
		State_Move = 0x02,
		State_Injure = 0x04,
		State_Die = 0x10,
		State_Standup = 0x20,
	};

	virtual void init();

	virtual C3DActor::Type getType();

	virtual void update(long elapsedTime);

	/**
	  *get injured
	  */
	void hurt(float damage, long hurtTime);

	bool isState(unsigned int state,unsigned int bit) const
	{
		return (state & bit) == bit;
	}

private:
	void updateState(long elapsedTime);

private:

	long _hurtTime;
	float _hurtRandom;
	unsigned int _state;
	Enemy::AnimListenerObject* _animListener;

	unsigned int _lastState;
	unsigned int _curState;
};
}
#endif
