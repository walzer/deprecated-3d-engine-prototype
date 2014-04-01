#include "C3DActor.h"
#include "C3DNode.h"
#include "C3DSprite.h"
#include "C3DModel.h"
#include "C3DModelNode.h"
#include "C3DMaterial.h"
#include "FacialAnimManager.h"
#include "StringTool.h"
namespace cocos2d
{
/**
* part of the avatar model.
*/
class BodyPart
{
public:
	std::string _modelName; //mesh name
	std::string _matName; //material name

	BodyPart(){};

	BodyPart(const std::string& modelName,const std::string& matName)
	{
		_modelName = modelName;
		_matName = matName;
    }

	~BodyPart(){};
};

/**
* config for the part of the avatar model.
*/
class BodyPartConfig
{
public:
	BodyPartConfig(){};

	BodyPartConfig(bool canNone)
	{
		if(canNone==true)
		{
			_curIndex = 1;
			_parts.push_back(NULL);
		}
		else
		{
			_curIndex = 0;
		}
	}

	~BodyPartConfig()
	{
		std::vector<BodyPart*>::iterator iter;
		std::vector<BodyPart*>::iterator endIter = _parts.end();

		endIter = _parts.end();
		for(iter=_parts.begin();iter!=endIter;iter++)
		{
			delete *iter;
		}

		_parts.clear();
	}

	void addPart(BodyPart *part)
	{
		_parts.push_back(part);
	}

	bool delPart(const std::string& modelName, const std::string& matName)// lvlong
	{
		std::vector<BodyPart*>::iterator iter = _parts.begin();
		for (;iter != _parts.end(); ++iter)
		{
			if ((*iter)->_modelName == modelName && (*iter)->_matName == matName)
			{
				delete *iter;
				_parts.erase(iter);
				return true;
			}
		}
		return false;
	}

	void setPart()
	{
	}

	void changePart()
	{
		int index = _curIndex+1;
		if (index < 0 || index >= (int)_parts.size())
			index = 0;

		_curIndex = index;
	}

	void changePart(int index)
	{
		if (index < 0 || index >= (int)_parts.size())
			index = 0;

		_curIndex = index;
	}

	BodyPart* getCurPart()
	{
		return _parts[_curIndex];
	}

	BodyPart* findPart(const std::string& modelName, const std::string& matName)// lvlong
	{
		std::vector<BodyPart*>::iterator iter = _parts.begin();
		for (;iter != _parts.end(); ++iter)
		{
			if ((*iter) && (*iter)->_modelName == modelName && (*iter)->_matName == matName)
			{
				return *iter;
			}
		}
		return NULL;
	}

public:
	std::vector<BodyPart*> _parts;
	int _curIndex;
};

class MorphInfo
{
public:
	unsigned int _morphTargetIndex;
	float _weight;
	MorphInfo(unsigned int morphTargetIndex,float weight)
	{
		_morphTargetIndex = morphTargetIndex;
		_weight = weight;
	}

	~MorphInfo()
	{
	}
};

class MorphConfig
{
public:
	MorphConfig(const std::string& meshName)
	{
		_meshName = meshName;
		_morphInfos = new std::map<std::string,MorphInfo*>();
	}
	~MorphConfig()
	{
		for(std::map<std::string,MorphInfo*>::iterator iter=_morphInfos->begin(); iter!=_morphInfos->end(); ++iter)
		{
			delete iter->second;
		}
		delete _morphInfos;
	}

	void add(const std::string& type, unsigned int morphTargetIndex,float weight)
	{
		MorphInfo*& info = (*_morphInfos)[type];
		if(info == NULL)
		{
			info = new MorphInfo(morphTargetIndex,weight);
		}
		else
		{
			info->_morphTargetIndex = morphTargetIndex;
			info->_weight = weight;
		}
	}

	MorphInfo* get(const std::string& type)
	{
		return (*_morphInfos)[type];
	}

	std::string _meshName;
	std::map<std::string,MorphInfo*>* _morphInfos;
};

C3DActor::C3DActor(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer):C3DBaseActor(name,node,layer)
{
	_life = 30.f;

	_models = new std::map<std::string,cocos3d::C3DNode*>();

	_partConfigs = new std::map<std::string,BodyPartConfig*>();

	_morphConfig = NULL;
}

C3DActor::~C3DActor()
{
	_models->clear();
	SAFE_DELETE(_models);

	for(std::map<std::string,BodyPartConfig*>::iterator iter=_partConfigs->begin();iter!=_partConfigs->end();iter++)
	{
		delete iter->second;
	}
	_partConfigs->clear();
	SAFE_DELETE(_partConfigs);

	SAFE_DELETE(_morphConfig);
}

bool C3DActor::addPartConfig(const std::string& type,bool canNone)
{
	BodyPartConfig*& partConfig = (*_partConfigs)[type];
	if(partConfig == NULL)
	{
		partConfig = new BodyPartConfig(canNone);
		return true;
	}
	return false;
}

bool C3DActor::delPartConfig(const std::string& type)
{
	std::map<std::string,BodyPartConfig*>::iterator iter = _partConfigs->find(type);
	if (iter != _partConfigs->end())
	{
		delete(iter->second);
		_partConfigs->erase(iter);
		return true;
	}
	return false;
}

bool C3DActor::addPart(const std::string& type,const std::string& modelName,const std::string& matName)
{
	BodyPartConfig*& partConfig = (*_partConfigs)[type];
	if(partConfig == NULL)
		return false;

	// is this exist?
	BodyPart* tpBodyPart = partConfig->findPart(modelName, matName);
	if (tpBodyPart)
		return false;

	partConfig->addPart(new BodyPart(modelName,matName));
	return true;
}

bool C3DActor::delPart(const std::string& type,const std::string& modelName,const std::string& matName)
{
	BodyPartConfig*& partConfig = (*_partConfigs)[type];
	if(partConfig == NULL)
		return false;

	partConfig->delPart(modelName, matName);
	return true;
}

void C3DActor::setPart(const std::string& type,int index)
{
	BodyPartConfig* partConfig = (*_partConfigs)[type];
	partConfig->_curIndex = index;
}
void C3DActor::loadParts()
{
	std::list<std::string> models;
	for(std::map<std::string,BodyPartConfig*>::iterator iter=_partConfigs->begin();iter!=_partConfigs->end();++iter)
	{
		BodyPart* curPart = iter->second->getCurPart();
		if(curPart == NULL)
			continue;
		if(std::find(models.begin(),models.end(),curPart->_modelName) == models.end())
		{
			models.push_back(curPart->_modelName);
		}
	}
	static_cast<cocos3d::C3DSprite*>(_node)->loadNodes(&models);

	for(std::map<std::string,BodyPartConfig*>::iterator iter=_partConfigs->begin();iter!=_partConfigs->end();++iter)
	{
		BodyPart* curPart = iter->second->getCurPart();
		if(curPart == NULL)
			continue;

		setMaterial(iter->first,curPart->_modelName,curPart->_matName);
	}
}

void C3DActor::changePart(const std::string& type)
{
	BodyPartConfig* partConfig = (*_partConfigs)[type];
	if(partConfig == NULL)
		return;

	partConfig->changePart();
	BodyPart* curPart = partConfig->getCurPart();

	if(curPart == NULL)
	{
		removePart(type);
	}
	else
	{
		changePart(type,curPart->_modelName,curPart->_matName);
	}

	//setSheetAnim((static_cast<cocos3d::C3DSprite*>(_node))->getFacialAnimManager());
}

void C3DActor::changePart(const std::string& type,int index)
{
	BodyPartConfig* partConfig = (*_partConfigs)[type];
	if(partConfig == NULL)
		return;

	partConfig->changePart(index);
	BodyPart* curPart = partConfig->getCurPart();

	if(curPart == NULL)
	{
		removePart(type);
	}
	else
	{
		changePart(type,curPart->_modelName,curPart->_matName);
	}

	//setSheetAnim((static_cast<cocos3d::C3DSprite*>(_node))->getFacialAnimManager());
}

void C3DActor::setMaterial(const std::string& type,const std::string& modelName,const std::string& matName)
{
	cocos3d::C3DNode* node = (static_cast<cocos3d::C3DSprite*>(_node))->findNode(modelName.c_str());
	if( node == NULL)
	{
		return;
	}

	static_cast<cocos3d::C3DModelNode*>(node)->getModel()->setMaterial(matName.c_str());
	(*_models)[type] = node;
}

void C3DActor::removePart(const std::string& type)
{
	if(_models->find(type)!=_models->end())
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->removeChild((*_models)[type]);
		_models->erase(type);
	}
}

void C3DActor::changePart(const std::string& type,const std::string& modelName,const std::string& matName)
{
	std::map<std::string,cocos3d::C3DNode*>::iterator iter = _models->find(type);

	if(iter!=_models->end() && iter->second->getId()!=modelName)
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->removeChild((*_models)[type]);
		_models->erase(type);
	}

	cocos3d::C3DNode* node = (static_cast<cocos3d::C3DSprite*>(_node))->findNode(modelName.c_str());
	if(node==NULL)
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->loadNode(modelName);
		node = (static_cast<cocos3d::C3DSprite*>(_node))->findNode(modelName.c_str());
		if(node==NULL)
			return;
	}

	if((static_cast<cocos3d::C3DModelNode*>(node))->getModel()->getMaterial()==NULL || (static_cast<cocos3d::C3DModelNode*>(node))->getModel()->getMaterial()->getID() != matName)
	{
		static_cast<cocos3d::C3DModelNode*>(node)->getModel()->setMaterial((matName.c_str()));
	}

	(*_models)[type] = node;
}

void C3DActor::createMorph(const std::string& meshName)
{
	_morphConfig = new MorphConfig(meshName);
}

void C3DActor::setMorphToMesh(const std::string& type, unsigned int morphTargetIndex,float weight)
{
	_morphConfig->add(type,morphTargetIndex,weight);

	addMorph(_morphConfig->_meshName,morphTargetIndex,weight);
}

void C3DActor::addMorph(const std::string &modelName, unsigned int morphTargetIndex,float weight)
{
	cocos3d::C3DNode* node = (static_cast<cocos3d::C3DSprite*>(_node))->findNode(modelName.c_str());

	if( node == NULL)
	{
		(static_cast<cocos3d::C3DSprite*>(_node))->loadNode(modelName);
		node = (static_cast<cocos3d::C3DSprite*>(_node))->findNode(modelName.c_str());
		if(node==NULL)
			return;
	}

	static_cast<cocos3d::C3DModelNode*>(node)->getModel()->pushMorph(morphTargetIndex,weight);
}

void C3DActor::changeMorph(const std::string& type,float weight)
{
	cocos3d::C3DNode* node = (static_cast<cocos3d::C3DSprite*>(_node))->findNode(_morphConfig->_meshName.c_str());

	if( node==NULL )
		return;

	MorphInfo* info = _morphConfig->get(type);
	if(info == NULL)
		return;

	static_cast<cocos3d::C3DModelNode*>(node)->getModel()->changeMorph(info->_morphTargetIndex,weight);
}

//void C3DActor::createFacialAnim()
//{
//	cocos3d::C3DSprite* sprite = static_cast<cocos3d::C3DSprite*>(_node);
//	if(sprite == NULL)
//		return;
//
//	sprite->createFacialAnim();
//	cocos3d::FacialAnimManager* facialAnimMan = sprite->getFacialAnimManager();
//	if(facialAnimMan == NULL)
//		return;
//
//	initExpression(facialAnimMan);
//
//}
//
//void C3DActor::initExpression(cocos3d::FacialAnimManager* facialAnimManager)
//{
//	if(facialAnimManager == NULL)
//		return;
//
//	/*facialAnimManager->addAnim("eye","cry",2,2,4,   "0-1-2-3-0-0-0-0-3-0-3");//uv anim
//	facialAnimManager->addAnim("eye","smile",2,2,4, "0-1-2-3-0-0-0-0-3-0-3");
//
//	facialAnimManager->addAnim("brow","cry",2,2,4  , "0-0-0-3-0-0-0-0-3-0-3");
//	facialAnimManager->addAnim("brow","smile",2,2,4, "0-0-0-3-0-0-0-0-3-0-3");
//
//	facialAnimManager->addAnim("mouse","cry",2,2,4,   "0-0-0-3-0-0-0-0-3-0-3");
//	facialAnimManager->addAnim("mouse","smile",2,2,4, "0-0-0-3-0-0-0-0-3-0-3");*/
//
//	facialAnimManager->addTextureAnim("eye","cry",4, "0-1-2-3-0-0-0-0-3-0-3");//texture anim
//	/*facialAnimManager->addTextureAnim("eye","smile",4, "0-1-2-3-0-0-0-0-3-0-3");
//
//	facialAnimManager->addTextureAnim("brow","cry",4, "0-0-0-3-0-0-0-0-3-0-3");
//	facialAnimManager->addTextureAnim("brow","smile",4, "0-0-0-3-0-0-0-0-3-0-3");
//
//	facialAnimManager->addTextureAnim("mouse","cry",4, "0-0-0-3-0-0-0-0-3-0-3");
//	facialAnimManager->addTextureAnim("mouse","smile",4, "0-0-0-3-0-0-0-0-3-0-3");*/
//
//}
//
//void C3DActor::setSheetAnim(cocos3d::FacialAnimManager* facialAnimManager)
//{
//	if(facialAnimManager == NULL)
//		return;
//
//	cocos3d::C3DMaterial* mat = (static_cast<cocos3d::C3DSprite*>(_node))->getMaterial("Girl_Yanjing_01");
//	string name = mat->getID();
//	name = cocos3d::StringTool::getFileName(name,"material");
//
//	if(name.find("cry"))
//		facialAnimManager->setAnim("eye","cry",mat);
//	else
//		facialAnimManager->setAnim("eye","smile",mat);
//
//	/*C3DMaterial* mat1 = _superModel->getFaceMat("brow");
//	if(mat->getName().find("brow_cry"))
//		facialAnimManager->setAnim("brow","cry",mat);
//	else
//		facialAnimManager->setAnim("brow","smile",mat);
//
//	C3DMaterial* mat2 = _superModel->getFaceMat("mouth");
//	if(mat->getName().find("mouth_cry"))
//		facialAnimManager->setAnim("mouth","cry",mat);
//	else
//		facialAnimManager->setAnim("mouth","smile",mat);*/
//
//}
}