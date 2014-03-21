#include "Bullet.h"
#include "FullDemoLayer.h"
#include "MainPlayer.h"

#include "Enemy.h"

using namespace cocos3d;

namespace cocos2d
{
Bullet::Bullet(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer)
	: BaseBullet(name,node,layer)
	, _moveParticle( NULL )
	, _explodeParticle( NULL )
	, _hitTime( 0.0 )
{
}

Bullet::~Bullet()
{
}

//void Bullet::init()
//{
//}

//void Bullet::moveTo(cocos3d::C3DVector3& target)
//{
//	C3DActor::moveTo(target);
//	_state = Bullet::State_Move;
//}

//void Bullet::spell(C3DActor* caster, C3DActor* target)
//{
//	BaseBullet::spell( caster, target );
//}

void Bullet::onSpell(void)
{
	_target.y += 1;

	std::string name = _name + "_move";
	_moveParticle = C3DParticleSystem::create(name.c_str());
	_moveParticle->load("demores/particles/fire.particle");
	_layer->get3DScene()->addChild(_moveParticle);
	_node->addChild(_moveParticle);
}

void Bullet::onFly(void)
{
	// do nothing
}

void Bullet::onHit(void)
{
	if(_moveParticle)
	{
		_node->removeChild(_moveParticle);
		_moveParticle = NULL;
	}

	if(_explodeParticle == NULL)
	{
		std::string name = _name + "_explode";
		_explodeParticle = C3DParticleSystem::create(name.c_str());
		_explodeParticle->load("demores/particles/explosion.particle");

		_layer->get3DScene()->addChild(_explodeParticle);

		_node->addChild(_explodeParticle);

		C3DActor* target = FullDemoLayer::getInstance()->getMainPlayer()->getTargetActor();

		if (target == NULL)
		{
			return;
		}

		Enemy* enemy = dynamic_cast<Enemy*>(target);
		if (enemy)
		{
			enemy->hurt(10.f, 1500L);
		}
	}

	_hitTime = 0.0;
}

void Bullet::onFinish(void)
{
	BaseBullet::onFinish();

	if(_explodeParticle)
	{
		_node->removeChild(_explodeParticle);
		_explodeParticle = NULL;
	}
}

void Bullet::stateSpell(long elapsedTime)
{
	_state = State_Fly;
	onFly();
}

void Bullet::stateFly(long elapsedTime)
{
	// 判断命中
	C3DVector3 curPos = _node->getTranslationWorld();
	_target = _targetActor->getNode()->getTranslationWorld();
	_target.y += 1;
	float dist = curPos.distanceSquared(_target);
	if(dist < 0.04f)
	{
		_state = Bullet::State_Hit;
		onHit();
	}

	// 飞行
	C3DVector3 newFaceDir = _target - curPos;
	newFaceDir.normalize();
	C3DVector3 offset = newFaceDir * 5.0f * (elapsedTime * 0.001f);
	_node->translate(offset.x,offset.y,offset.z);
}

void Bullet::stateHit(long elapsedTime)
{
	_hitTime += elapsedTime;
	if(_hitTime > 1000.0f)
	{
		_state = BaseBullet::State_Finish;
		onFinish();
	}
}
void Bullet::stateFinish(long elapsedTime)
{
	// do nothing
}

/**
void Bullet::update(long elapsedTime)
{
	updateState(elapsedTime);

	if(_state == Bullet::State_Move)
	{
		this->move3D(elapsedTime);
	}
	else if(_state==Bullet::State_Explode)
	{
		if(_moveParticle)
		{
			_node->removeChild(_moveParticle);
			_moveParticle = NULL;
		}

		if(_explodeParticle == NULL)
		{
			std::string name = _name + "_explode";
			_explodeParticle = C3DParticleSystem::create(name.c_str());
			_explodeParticle->load("particles/explosion.particle");

			_layer->getScene()->addChild(_explodeParticle);

			_node->addChild(_explodeParticle);

			C3DActor* target = FullDemoLayer::getInstance()->getMainPlayer()->getTargetActor();

			if (target == NULL)
				return;

			Enemy* enemy = dynamic_cast<Enemy*>(target);
			if (enemy)
				enemy->hurt(10.f, 1500L);
		}
	}
	else if(_state == Bullet::State_Finished)
	{
		if(_explodeParticle)
		{
			_node->removeChild(_explodeParticle);
			_explodeParticle = NULL;
		}

		_state = Bullet::State_Die;
	}
}
static float dieTime = 0;
void Bullet::updateState(long elapsedTime)
{
	if(_state == Bullet::State_Die)
		return;

	if(_state == Bullet::State_Explode)
	{
		dieTime += elapsedTime;
		if(dieTime > 1000.0f)
		{
			dieTime = 0;
			_state = Bullet::State_Finished;
		}
	}
	else if(_state == Bullet::State_Move)
	{
		C3DVector3 curPos = _node->getTranslationWorld();

		_target = _targetActor->getNode()->getTranslationWorld();
		_target.y += 1;
		float dist = curPos.distanceSquared(_target);
		if(dist < 0.04f)
		{
			_state = Bullet::State_Explode;
		}
	}
}

void Bullet::move3D(long elapsedTime)
{
	C3DVector3 curPos = _node->getTranslationWorld();

	C3DVector3 newFaceDir = _target - curPos;

	newFaceDir.normalize();

	C3DVector3 offset = newFaceDir * 5.0f * (elapsedTime * 0.001f);
	_node->translate(offset.x,offset.y,offset.z);
}

//*/
}