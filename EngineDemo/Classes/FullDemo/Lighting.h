#ifndef LIGHTING_H
#define LIGHTING_H

#include "BaseBullet.h"

namespace cocos2d
{
class LightingBullet : public BaseBullet
{
public:

#define MAX_TARGET_NUM	3

public:

	LightingBullet(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer);
	virtual ~LightingBullet();

	bool addTargets( C3DActor* target )
	{
		for ( int i = 0; i < MAX_TARGET_NUM; ++i )
		{
			if ( _targets[i] == NULL )
			{
				_targets[i] = target;
				return true;
			}
		}

		return false;
	}

protected:

	virtual void onSpell(void);
	virtual void onFly(void);
	virtual void onHit(void);
	virtual void onFinish(void);

	virtual void stateSpell(long elapsedTime);
	virtual void stateFly(long elapsedTime);
	virtual void stateHit(long elapsedTime);
	virtual void stateFinish(long elapsedTime);

private:
	cocos3d::C3DLineRender* _lighting;
	C3DActor* _targets[MAX_TARGET_NUM];
	unsigned int _curTargetID;
	float _flyTime;
	float _maxFlyTime;
	float _spellDelty;
};
}	// namespace cocos2d

#endif	// #define LIGHTING_H