#include "C3DModel.h"
#include "C3DMorph.h"
#include "C3DMorphMesh.h"

namespace cocos3d
{

C3DMorph* C3DModel::getMorph()
{
	return _morph;
}

void C3DModel::setMorph(C3DMorph* morph)
{
	if(_morph != morph)
	{
		SAFE_DELETE(_morph);

		_morph = morph;
	}
}

void C3DModel::pushMorph(int morphTargetIndex,float weight)
{
	if(_mesh!=NULL && _morph!=NULL)
	{
		static_cast<C3DMorphMesh*>(_mesh)->pushMorph(_morph,morphTargetIndex,weight);
	}
}

void C3DModel::popMorph(int morphTargetIndex)
{
	if(_mesh!=NULL && _morph!=NULL)
	{
		static_cast<C3DMorphMesh*>(_mesh)->popMorph(_morph,morphTargetIndex);
	}
}

void C3DModel::changeMorph(int morphTargetIndex,float weight)
{
	if(_mesh!=NULL && _morph!=NULL)
	{
		static_cast<C3DMorphMesh*>(_mesh)->changeMorph(_morph,morphTargetIndex,weight);
	}
}
}