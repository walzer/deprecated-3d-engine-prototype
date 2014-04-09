#include "Base.h"
#include "C3DVector3.h"
#include "C3DSprite.h"

#include "FacialAnimation.h"
#include "FacialAnimManager.h"
#include "StringTool.h"
#include "C3DMaterial.h"
#include "C3DResourceLoader.h"
#include "C3DScene.h"
#include "C3DLayer.h"
#include "C3DNode.h"

#include "C3DAnimation.h"
#include "C3DAnimationClip.h"
#include "C3DModel.h"
#include "C3DMesh.h"
#include "C3DAABB.h"
#include "C3DMeshSkin.h"
#include "C3DSkinModel.h"
#include "C3DBone.h"
#include "C3DSkeleton.h"
#include "C3DLight.h"

#include "C3DMaterial.h"
#include "MaterialParameter.h"

#include "C3DStat.h"
#include "C3DModelNode.h"

namespace cocos3d
{
C3DSprite::C3DSprite(const std::string& id) :C3DRenderNode(id)
{
	//_active = true;

	//_facialAnimManager = NULL;

	_skeleton = NULL;

	_animation = NULL;
}

C3DSprite::~C3DSprite()
{
	//SAFE_DELETE(_facialAnimManager);

	_animation = NULL;

    SAFE_DELETE(_skeleton);
}

C3DSprite* C3DSprite::create(const std::string& id)
{
	C3DSprite* pRet = new C3DSprite(id);

    pRet->autorelease();
    return pRet;
}


bool C3DSprite::load(C3DResourceLoader* loader, bool isLoadAll)
{
	_fileName = loader->getFilePath();

	if(isLoadAll==true)
	{
		loader->loadSuperModel(this);
		setDefaultMaterial(StringTool::getFilePath(_fileName));

		getAABB();
		getOBB();
	}
	else
	{
		loader->loadSkeleton(this,"Bip01");
	}

    return true;
}

void C3DSprite::loadNode(const std::string& nodeName)
{
	// Load mesh/scene from file
	C3DResourceLoader* bundle = C3DResourceLoader::create(_fileName);

	bundle->loadSuperModel(this,nodeName);
	SAFE_RELEASE(bundle);
}

void C3DSprite::loadNodes(std::list<std::string>* models)
{
	// Load mesh/scene from file
	C3DResourceLoader* bundle = C3DResourceLoader::create(_fileName);

	bundle->loadSuperModel(this,models);
	SAFE_RELEASE(bundle);

	getAABB();
    getOBB();
}

C3DAnimation* C3DSprite::createAnimation()
{
	// Load mesh/scene from file
	C3DResourceLoader* bundle = C3DResourceLoader::create(_fileName);
	CCAssert(bundle, "Can not load bundle!");

	//C3DAnimation* animation = C3DAnimation::create("movements");

	bundle->loadAnimation(this);

	SAFE_RELEASE(bundle);

	return _animation;
}

C3DAnimationClip* C3DSprite::addAnimationClip(const std::string& name,unsigned int startFrame,unsigned int endFrame,float repeatCount,float speed)
{
	if( _animation == NULL )
		_animation = createAnimation();

	if(_animation == NULL)
		return NULL;

	//_animation = _skeleton->getAnimation("movements");
	return _animation->addClip(name,startFrame,endFrame,repeatCount,speed);
}

bool C3DSprite::replaceAnimationClip(const std::string& name,unsigned int startFrame,unsigned int endFrame,float repeatCount,float speed)
{
	if(!_animation || !_skeleton)
		return false;

	_animation->replaceClip(name,startFrame,endFrame,repeatCount,speed);
	return true;
}

bool C3DSprite::loadMesh(const std::string& meshName)
{
	loadNode(meshName);
    return false;
}

//FacialAnimManager* C3DSprite::getFacialAnimManager()
//{
//	return _facialAnimManager;
//}

//void C3DSprite::createFacialAnim()
//{
//	_facialAnimManager = new FacialAnimManager();
//}

void C3DSprite::update(long elapsedTime)
{
	if(isVisible() == false)
		return;

	C3DRenderNode::update(elapsedTime);

    bool updatePose = _isVisibleByCamera || m_collitionBoxs.size() || _attachNodes.size();
	if(_animation != NULL)
		_animation ->update(elapsedTime, updatePose);

	//if(_facialAnimManager != NULL)
	//	_facialAnimManager->update(elapsedTime);
}

void C3DSprite::drawDebug()
{
	C3DRenderNode::drawDebug();
}

void C3DSprite::calculateBoundingBox_()
{
	C3DAABB box;

	box._min.set(99999.0f, 99999.0f, 99999.0f);
	box._max.set(-99999.0f, -99999.0f, -99999.0f);

	for(std::vector<C3DNode*>::const_iterator iter=_children.begin(); iter!=_children.end(); ++iter)
	{
		C3DNode* node = *iter;
		if(node->getType()!=C3DNode::NodeType_Model )
			continue;
		C3DModel* model = static_cast<C3DModelNode*>(node)->getModel();

		if (model)
		{
			C3DAABB worldSpaceBox(*model->getMesh()->getBoundingBox());
			box.merge(worldSpaceBox);
		}
	}

	if (!_bbOrigin)
		_bbOrigin = new C3DAABB();
	_bbOrigin->_min = box._min;
	_bbOrigin->_max = box._max;

	if (!_bb)
		_bb = new C3DAABB();
	_bb->_min = box._min;
	_bb->_max = box._max;
}

C3DNode::Type C3DSprite::getType() const
{
	return C3DNode::NodeType_SuperModel;
}

void C3DSprite::playAnimationClip(const std::string& name)
{
	if( _animation == NULL)
		return;

	_animation->play(name);
}

void C3DSprite::stopAnimationClip(const std::string& name)
{
	if( _animation == NULL)
		return;
	_animation->stop(name);
}

void C3DSprite::pauseAnimationClip(const std::string& name)
{
	if( _animation == NULL)
		return;
	_animation->pause(name);
}

void C3DSprite::resumeAnimationClip(const std::string& name)
{
	if( _animation == NULL)
		return;
	_animation->resume(name);
}

bool C3DSprite::isAnimationClipPlaying(const std::string& clipName)
{
	return false;
}

void C3DSprite::setSkeleton(C3DBone* joint)
{
	if(_skeleton == NULL)
		_skeleton = new C3DSkeleton();
	_skeleton->set(joint);
}

C3DBone* C3DSprite::getSkeletonRootBone()const
{
	if (_skeleton)
		return _skeleton->getRootBone();
	return NULL;
}

C3DBone* C3DSprite::getBone(const std::string& name)const
{
	if (_skeleton)
		return _skeleton->getBone(name);
	return NULL;
}

C3DAnimationClip* C3DSprite::getAnimationClip(const std::string& strClip)
{
    if (_animation == NULL)
        return NULL;

    return _animation->getClip(strClip);
}

C3DAnimationClip* C3DSprite::getCurAnimationClip()
{
    if (_animation == NULL)
        return NULL;

    return _animation->getCurAnimationClip();
}

int C3DSprite::getAnimationFrameCount()
{
	if( _animation == NULL )
		createAnimation();

	_animation = _skeleton->getAnimation("movements");
	if (_animation)
		return _animation->getFrameCount();

	return 0;
}

void C3DSprite::copyFrom(const C3DTransform* other, C3DNode::CloneContext& context)
{
	const C3DSprite* otherNode = static_cast<const C3DSprite*>(other);

	C3DRenderNode::copyFrom(other, context);

	C3DSkeleton* ske = otherNode->_skeleton;
	_skeleton = ske ? ske->clone() : NULL;

    _fileName = otherNode->_fileName;

    if (otherNode->_skeleton)
    {
        C3DBone* bone = (C3DBone*)context.cloneMap[otherNode->_skeleton->getRootBone()];
        if (bone != NULL)
        {
            setSkeleton(bone);
            _animation = _skeleton->getAnimation("movements");
        }
    }

	if (otherNode->m_collitionBoxs.size() > 0)
	{
		std::vector<C3DCollitionBox*>::const_iterator iter = otherNode->m_collitionBoxs.begin();
		for ( ; iter != otherNode->m_collitionBoxs.end(); iter++)
		{
			C3DCollitionBox* pMyCbb = new C3DCollitionBox(this, (*iter)->getID(), (*iter)->getBone(), (*iter)->getLocalPos(), (*iter)->getRadius());
			m_collitionBoxs.push_back(pMyCbb);
		}
	}
}

C3DNode* C3DSprite::clone(C3DNode::CloneContext& context) const
{
	C3DSprite* other = new C3DSprite("");
	other->copyFrom(this, context);

	((C3DResource*)other)->copyFrom(this);

	other->autorelease();
	return other;
}

void C3DSprite::stopAllAnimationClip()
{
	_animation->stopAll();
	_animation->update(0);
}

}