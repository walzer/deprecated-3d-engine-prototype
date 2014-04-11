#include "MainPlayer.h"
#include "Bullet.h"
#include "Npc.h"
#include "Enemy.h"
#include "C3DNode.h"
#include "StringTool.h"
#include "C3DLayer.h"
#include "C3DScene.h"
#include "C3DCamera.h"
#include "C3DMaterial.h"
#include "MaterialParameter.h"
#include "C3DRenderNodeManager.h"

#include "C3DRenderNode.h"
#include "Lighting.h"
#include "FullDemoLayer.h"

using namespace cocos3d;

namespace cocos2d
{
MainPlayer::MainPlayer(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer)
	:C3DActor(name,node,layer)
	, _attackType( AT_Melee )
{
	_targetActor = NULL;
	_lastState = MainPlayer::State_Idle;
	_curState = MainPlayer::State_Idle;

}

MainPlayer::~MainPlayer()
{
	SAFE_DELETE(_animListener);

	if (_bullets.size() > 0)
	{
		std::list<BaseBullet*>::iterator iter = _bullets.begin();
        while (iter != _bullets.end())
        {
            BaseBullet* an = *iter;
            SAFE_DELETE(an);
            ++iter;
        }
		_bullets.clear();
    }
}

void MainPlayer::init()
{
	_animListener = new MainPlayer::AnimListenerObject(this);

	C3DAnimationClip* attackClip = (static_cast<cocos3d::C3DSprite*>(_node))->getAnimationClip("attack_normal");
	if(attackClip != NULL)
	{
		cocos3d::C3DActionListener* endAction = cocos3d::C3DActionListener::create(_animListener,((cocos3d::ListenerFunction)(&cocos2d::MainPlayer::AnimListenerObject::onMeleeAttackEnd)));
		attackClip->addActionEvent(endAction,attackClip->getDuration()*0.5f);
	}

	attackClip = (static_cast<cocos3d::C3DSprite*>(_node))->getAnimationClip("attack_skill");
	if(attackClip != NULL)
	{
		cocos3d::C3DActionListener* endAction = cocos3d::C3DActionListener::create(_animListener,((cocos3d::ListenerFunction)(&cocos2d::MainPlayer::AnimListenerObject::onRemoteAttackEnd)));
		attackClip->addActionEvent(endAction,attackClip->getDuration());
	}

	_offset = C3DVector3(10,8,8);
	//_offset *= 10.0f;
	_camera = _layer->get3DScene()->getActiveCamera();
	_camera->lookAt(_node->getTranslationWorld()+_offset, C3DVector3(0, 1, 0), _node->getTranslationWorld());

	_terrainFlag = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("demores/fulldemo/cursor/terrainflag.ckb"));
	//_terrainFlag->setMaterial("demores/fulldemo/cursor/terrainflag.material");

	_terrainFlag->translate(0, 1, 0);
    _terrainFlag->scale(2, 2, 2);
    _layer->get3DScene()->addChild(_terrainFlag);
}

MainPlayer::AnimListenerObject::AnimListenerObject(MainPlayer* mainPlayer)
{
	_mainPlayer = mainPlayer;
}

void MainPlayer::AnimListenerObject::onMeleeAttackEnd()
{
	_mainPlayer->_lastState = MainPlayer::State_Idle;
	_mainPlayer->_curState = MainPlayer::State_Idle;

	_mainPlayer->meleeAttack();
}

void MainPlayer::AnimListenerObject::onRemoteAttackEnd()
{
	_mainPlayer->_lastState = MainPlayer::State_Idle;
	_mainPlayer->_curState = MainPlayer::State_Idle;
}

void MainPlayer::meleeAttack()
{
	if(_targetActor)
			static_cast<Enemy*>(_targetActor)->hurt(10.f, 15000L);
}

void MainPlayer::moveTo(cocos3d::C3DVector3& target)
{
	C3DActor::moveTo(target);
	_lastState = MainPlayer::State_Move | MainPlayer::State_Rotate;

	if(_terrainFlag)
	{
		_terrainFlag->setVisible(true);
		C3DVector3 m = target;
		//target.y -= 5;
		_terrainFlag->setPosition(target);
	}
}

C3DActor::Type MainPlayer::getType()
{
	return C3DActor::ActorType_MainPlayer;
}

void MainPlayer::speak(C3DActor* targetActor)
{
	_lastState = MainPlayer::State_Speak;
	_targetActor = targetActor;
	_target = _targetActor->getNode()->getTranslationWorld();
}

void MainPlayer::meleeAttack(C3DActor* targetActor)
{
	_lastState = MainPlayer::State_MeleeAttack;
	_targetActor = targetActor;
	_target = _targetActor->getNode()->getTranslationWorld();
}

void MainPlayer::remoteAttack(C3DActor* targetActor)
{
	_lastState = MainPlayer::State_RemoteAttack;
	_targetActor = targetActor;
	_target = _targetActor->getNode()->getTranslationWorld();
}

void MainPlayer::update(long elapsedTime)
{
	updateState(elapsedTime);
	if(isState(_lastState,MainPlayer::State_Speak) && isState(_curState,MainPlayer::State_Idle))
	{
		Npc* npc = static_cast<Npc*>(_targetActor);
		npc->speak();
		_lastState = MainPlayer::State_Idle;
	}

	if(isState(_curState,MainPlayer::State_Idle))
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip("idle");
		if(_terrainFlag)
			_terrainFlag->setVisible(false);
	}
	else if(isState(_curState,MainPlayer::State_Move) || isState(_curState,MainPlayer::State_RemoteAttack) || isState(_curState,MainPlayer::State_MeleeAttack))
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip( "run" );
		this->move3D(elapsedTime);
		if(isState(_curState,MainPlayer::State_Rotate))
		{
			rotate(elapsedTime);
		}
	}
	else if(isState(_curState,MainPlayer::State_Rotate))
	{
		rotate(elapsedTime);
	}
	else if(isState(_lastState,MainPlayer::State_MeleeAttack) && isState(_curState,MainPlayer::State_Attack))
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip( "attack_normal" );

		if(_terrainFlag)
			_terrainFlag->setVisible(false);
	}
	else if(isState(_lastState,MainPlayer::State_RemoteAttack) && isState(_curState,MainPlayer::State_Attack))
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip( "attack_skill" );

		_lastState = MainPlayer::State_Attack;
		if(_terrainFlag)
		{
			_terrainFlag->setVisible(false);
		}

		static unsigned int counter = 0;
		std::string name = "bullet" + StringTool::toString(counter++);

		C3DNode* node = C3DNode::create(name.c_str());
		node->setPosition(_node->getTranslationWorld());
		_layer->get3DScene()->addChild(node);

		switch( _attackType )
		{
		case AT_Range:
			{
				Bullet* bullet = new Bullet(name, node,_layer);

				bullet->spell( this, _targetActor);
				_bullets.push_back(bullet);
			}break;
		case AT_Lighting:
			{
				LightingBullet* bullet = new LightingBullet(name, node,_layer);

				// 计算目标begin
				FullDemoLayer* layer = static_cast<FullDemoLayer*>(_layer);
				std::list<C3DActor*> actors = layer->getActors();
				bullet->addTargets( _targetActor );
				C3DVector3 targetPos = _targetActor->getNode()->getTranslationWorld();
				for ( int i = 0; i < MAX_TARGET_NUM-1; ++i )
				{
					for ( std::list<C3DActor*>::iterator ite = actors.begin(); ite != actors.end(); ++ite )
					{
						if ( (*ite) == _targetActor )
						{
							continue;
						}
						C3DVector3 nextPos = (*ite)->getNode()->getTranslationWorld();
						if ( nextPos.length() < 10.0 )
						{
							bullet->addTargets( (*ite) );
							actors.erase( ite );
							targetPos = nextPos;
							break;
						}
					}
				}

				// 计算目标end
				bullet->spell( this, _targetActor);
				_bullets.push_back(bullet);
			}break;
            
            default:
            {
            }break;
		}
	}

	BaseBullet* bullet = NULL;
	for (std::list<BaseBullet*>::iterator iter = _bullets.begin(); iter != _bullets.end(); )
	{
		bullet = *iter;

		if(bullet->getState() == BaseBullet::State_Finish)
		{
			SAFE_DELETE(bullet);
			_bullets.erase(iter++);
		}
		else
		{
			bullet->update(elapsedTime);
			++iter;
		}
	}

    static float time = 0;
    time += elapsedTime * 0.005f;
    if (_terrainFlag)
    {
		_terrainFlag->rotateY(time);
    }
}

void MainPlayer::updateState(long elapsedTime)
{
	if((static_cast<cocos3d::C3DSprite*>(_node)) == NULL)
		return;

	if(_lastState == _curState && isState(_curState,MainPlayer::State_Idle))
		return;

	C3DVector3 curPos = _node->getTranslationWorld();

	//...
    C3DVector3 curFaceDir = -_node->getForwardVectorWorld();
	curFaceDir.normalize();
	//...
	if(isState(_lastState,MainPlayer::State_Speak))
	{
		_target = _targetActor->getNode()->getTranslationWorld();
		C3DVector3 newFaceDir = _target - curPos;
		newFaceDir.y = 0;
		newFaceDir.normalize();
		float cosAngle = std::fabs(C3DVector3::dot(curFaceDir,newFaceDir) - 1.0f);
		float dist = curPos.distanceSquared(_target);

		if(dist<=4.0f)
		{
			if(cosAngle<=0.01f)
				_curState = MainPlayer::State_Idle;
			else
				_curState = MainPlayer::State_Rotate;
		}
		else
		{
			if(cosAngle>0.01f)
				_curState = MainPlayer::State_Rotate | MainPlayer::State_Move;
			else
				_curState = MainPlayer::State_Move;
		}
	}
	else if(isState(_lastState,MainPlayer::State_MeleeAttack))
	{
		_target = _targetActor->getNode()->getTranslationWorld();
		C3DVector3 newFaceDir = _target - curPos;
		newFaceDir.y = 0;
		newFaceDir.normalize();
		float cosAngle = std::fabs(C3DVector3::dot(curFaceDir,newFaceDir) - 1.0f);
		float dist = curPos.distanceSquared(_target);

		if(dist<=4.0f)
		{
			if(cosAngle<=0.01f)
				_curState = MainPlayer::State_Attack;
			else
				_curState = MainPlayer::State_Rotate;
		}
		else
		{
			if(cosAngle>0.01f)
				_curState = MainPlayer::State_Rotate | MainPlayer::State_Move;
			else
				_curState = MainPlayer::State_Move;
		}
	}
	else if(isState(_lastState,MainPlayer::State_RemoteAttack))
	{
		_target = _targetActor->getNode()->getTranslationWorld();
		C3DVector3 newFaceDir = _target - curPos;
		newFaceDir.y = 0;
		newFaceDir.normalize();
		float cosAngle = std::fabs(C3DVector3::dot(curFaceDir,newFaceDir) - 1.0f);
		float dist = curPos.distanceSquared(_target);

		if(dist<=100.0f)
		{
			if(cosAngle<=0.01f)
				_curState = MainPlayer::State_Attack;
			else
				_curState = MainPlayer::State_Rotate;
		}
		else
		{
			if(cosAngle>0.01f)
				_curState = MainPlayer::State_Rotate | MainPlayer::State_Move;
			else
				_curState = MainPlayer::State_Move;
		}
	}
	else if(isState(_lastState,MainPlayer::State_Move) | isState(_lastState,MainPlayer::State_Rotate))
	{
		C3DVector3 newFaceDir = _target - curPos;
		newFaceDir.y = 0;
		newFaceDir.normalize();

		float cosAngle = std::fabs(C3DVector3::dot(curFaceDir,newFaceDir) - 1.0f);
		float dist = curPos.distanceSquared(_target);

		if(dist<=1.0f)
		{
			if(cosAngle<=0.01f)
			{
				_lastState = MainPlayer::State_Idle;
				_curState = MainPlayer::State_Idle;
			}
			else
			{
				_lastState = MainPlayer::State_Rotate;
				_curState = MainPlayer::State_Rotate;
			}
		}
		else
		{
			if(cosAngle>0.01f)
			{
				_curState = MainPlayer::State_Rotate | MainPlayer::State_Move;
				_lastState = MainPlayer::State_Rotate | MainPlayer::State_Move;
			}
			else
			{
				_lastState = MainPlayer::State_Move;
			    _curState = MainPlayer::State_Move;
			}
		}
	}
}

void MainPlayer::move3D(long elapsedTime)
{
	C3DVector3 curPos = _node->getTranslationWorld();

	C3DVector3 newFaceDir = _target - curPos;
	newFaceDir.y = 0.0f;
	newFaceDir.normalize();

	C3DVector3 offset = newFaceDir * 5.0f * (elapsedTime * 0.001f);
	_node->translate(offset.x,offset.y,offset.z);

	_camera->lookAt(_node->getTranslationWorld()+_offset, C3DVector3(0, 1, 0), _node->getTranslationWorld());

	//_layer->getScene()->updateOctree(static_cast<C3DRenderNode*>(_node));
}

void MainPlayer::rotate(long elapsedTime)
{
	C3DVector3 curPos = _node->getTranslationWorld();

	C3DVector3 newFaceDir = _target - curPos;
	newFaceDir.y = 0;
	newFaceDir.normalize();

	_node->setForwardVectorWorld(newFaceDir);
}
}