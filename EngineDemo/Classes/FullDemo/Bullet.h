#ifndef __Bullet_H__
#define __Bullet_H__

#include "BaseBullet.h"

namespace cocos3d
{
	class C3DSprite;
	class C3DVector2;
}

namespace cocos2d
{
class Bullet : public BaseBullet
{
public:
	Bullet(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer);
	virtual ~Bullet();

	//virtual void init();

	//virtual void moveTo(cocos3d::C3DVector3& target);

	//virtual void update(long elapsedTime);

	//virtual void spell(C3DActor* caster, C3DActor* target);

protected:
	//void updateState(long elapsedTime);
	//void move3D(long elapsedTime);

	virtual void onSpell(void);
	virtual void onFly(void);
	virtual void onHit(void);
	virtual void onFinish(void);

	virtual void stateSpell(long elapsedTime);
	virtual void stateFly(long elapsedTime);
	virtual void stateHit(long elapsedTime);
	virtual void stateFinish(long elapsedTime);

private:

	cocos3d::C3DParticleSystem* _moveParticle;
	cocos3d::C3DParticleSystem* _explodeParticle;

	float _hitTime;
};

}
#endif
