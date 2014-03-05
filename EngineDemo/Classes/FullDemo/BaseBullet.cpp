#include "BaseBullet.h"

using namespace cocos3d;

namespace cocos2d
{
BaseBullet::BaseBullet(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer)
	: C3DActor(name, node, layer)
	, _casterActor( NULL )
	, _targetActor( NULL )
	, _state( State_None )
{
}

BaseBullet::~BaseBullet(void)
{
}

C3DActor::Type BaseBullet::getType()
{
	return C3DActor::ActorType_Bullet;
}

void BaseBullet::init()
{
}

void BaseBullet::moveTo(cocos3d::C3DVector3& target)
{
	C3DActor::moveTo(target);
	_state = State_Fly;
}

void BaseBullet::update(long elapsedTime)
{
	updateState( elapsedTime );
}

void BaseBullet::updateState(long elapsedTime)
{
	switch( _state )
	{
	case  State_None:
		{
			// do nothing
		}break;
	case State_Spell:
		{
			stateSpell(elapsedTime);
		}break;
	case State_Fly:
		{
			stateFly(elapsedTime);
		}break;
	case State_Hit:
		{
			stateHit(elapsedTime);
		}break;
	case State_Finish:
		{
			stateFinish(elapsedTime);
		}break;
	}
}

BaseBullet::State BaseBullet::getState()
{
	return _state;
}

void BaseBullet::changeStateTo( State state )
{
	if ( _state == state )
	{
		return;
	}

	_state = state;

	switch( _state )
	{
	case State_Spell:
		{
			onSpell();
		}break;
	case State_Fly:
		{
			onFly();
		}break;
	case State_Hit:
		{
			onHit();
		}break;
	case State_Finish:
		{
			onFinish();
		}break;
	case State_None:
		{
			// do nothing
		}break;
	}
}

void BaseBullet::spell(C3DActor* caster, C3DActor* target)
{
	_casterActor = caster;
	_targetActor = target;

	_target = _targetActor->getNode()->getTranslationWorld();

	changeStateTo( State_Spell );
}

void BaseBullet::onSpell(void)
{
}

void BaseBullet::onFly()
{
}

void BaseBullet::onHit()
{
}
void BaseBullet::onFinish()
{
	_casterActor = NULL;
	_targetActor = NULL;
}

void BaseBullet::stateSpell(long elapsedTime)
{
}

void BaseBullet::stateFly(long elapsedTime)
{
}

void BaseBullet::stateHit(long elapsedTime)
{
}

void BaseBullet::stateFinish(long elapsedTime)
{
}
}	// namespace cocos2d