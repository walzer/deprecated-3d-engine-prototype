#include "Lighting.h"
#include "C3DLineRender.h"

using namespace cocos3d;

namespace cocos2d
{
LightingBullet::LightingBullet(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer)
	: BaseBullet( name, node, layer )
	, _lighting( NULL )
	, _curTargetID( 0 )
	, _flyTime( 0.0 )
	, _maxFlyTime( 0.3 )
	, _spellDelty( 1.0 )
{
	memset( _targets, 0, MAX_TARGET_NUM*sizeof(_targets[0]) );
}

LightingBullet::~LightingBullet()
{
}

void LightingBullet::onSpell(void)
{
	_lighting = C3DLineRender::create( "demores/effect/sceneEffect/lighting.material" );
	_lighting->setForceTexLoop( false );
	_lighting->setWidth( 2 );
	_lighting->setTextueLength( 10 );
	_lighting->setStep( 0.2 );
	_lighting->setAmplitude0( 0.3 );
	_lighting->setFrequency0( 5 );
	_lighting->setAmplitude1( 0.05 );
	_lighting->setFrequency1( 200 );
	_lighting->setTimeFactor( 1 );

	_node->addChild( _lighting );
}

void LightingBullet::onFly(void)
{
	_flyTime = 0.0;
}

void LightingBullet::onHit(void)
{
	++_curTargetID;
}

void LightingBullet::onFinish(void)
{
	BaseBullet::onFinish();

	if ( _lighting != NULL && _node != NULL )
	{
		_node->removeChild( _lighting );
		_lighting = NULL;
	}
}

void LightingBullet::stateSpell(long elapsedTime)
{
	_spellDelty -= elapsedTime*0.001;
	if ( _spellDelty < 0 )
	{
		changeStateTo( BaseBullet::State_Fly );
	}
}

void LightingBullet::stateFly(long elapsedTime)
{
	_flyTime += elapsedTime*0.001;

	if ( _flyTime >= _maxFlyTime )
	{
		changeStateTo( BaseBullet::State_Hit );
	}
	else
	{
		/**
		C3DVector3 startPos = (_curTargetID==0)
								?_casterActor->getNode()->getTranslationWorld()
								:_targets[_curTargetID-1]->getNode()->getTranslationWorld();
		C3DVector3 endPos = _targets[_curTargetID]->getNode()->getTranslationWorld();
		startPos.y += 1.0;
		endPos.y += 1.0;

		endPos = startPos+(endPos-startPos)*C3D_Min(1, (4*_flyTime/_maxFlyTime));

		std::vector<C3DLineRender::Line> lines;
		lines.push_back( C3DLineRender::Line( startPos, endPos ) );
		lines.push_back( C3DLineRender::Line( startPos, endPos, 100 ) );
		//*/

		std::vector<C3DLineRender::Line> lines;

		for ( unsigned int i = 0; i <= _curTargetID; ++i )
		{
			C3DVector3 startPos = (i==0)?_casterActor->getNode()->getTranslationWorld()
										:_targets[i-1]->getNode()->getTranslationWorld();
			C3DVector3 endPos = _targets[i]->getNode()->getTranslationWorld();
			startPos.y += 1.0;
			endPos.y += 1.0;

			if ( i == _curTargetID )
			{
				endPos = startPos+(endPos-startPos)*C3D_Min(1, (4*_flyTime/_maxFlyTime));
			}

			lines.push_back( C3DLineRender::Line( startPos, endPos, i*100 ) );
			lines.push_back( C3DLineRender::Line( startPos, endPos, i*100+1000 ) );
			lines.push_back( C3DLineRender::Line( startPos, endPos, i*100+2000 ) );
		}

		if ( _lighting )
		{
			_lighting->setLines( lines );
		}
	}
}

void LightingBullet::stateHit(long elapsedTime)
{
	if ( _curTargetID < MAX_TARGET_NUM && _targets[_curTargetID] != NULL )
	{
		changeStateTo( BaseBullet::State_Fly );
	}
	else
	{
		changeStateTo( BaseBullet::State_Finish );
	}
}

void LightingBullet::stateFinish(long elapsedTime)
{
	// do nothing
}
}