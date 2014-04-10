#include "C3DRenderNode.h"
#include "C3DModel.h"
#include "C3DModelNode.h"
#include "C3DNode.h"
#include "C3DBatchMesh.h"
#include "C3DAABB.h"
#include "Vertex.h"
#include "C3DMaterial.h"
#include "C3DRenderChannel.h"
#include "C3DMesh.h"
#include "C3DMeshSkin.h"
#include "C3DSkinModel.h"
#include "C3DBone.h"
#include "C3DVector3.h"
#include "C3DScene.h"
#include "C3DTechnique.h"
#include "C3DRay.h"
#include "C3DElementNode.h"
#include "C3DCollitionBox.h"

#include "C3DModel.h"
#include "AttachNode.h"
#include "C3DParticleSystem.h"
#include "C3DScene.h"
#include "C3DCamera.h"
#include "C3DGeoWireRender.h"

#include "C3DResourceLoader.h"

#include "C3DSprite.h"
#include "C3DStaticObj.h"

namespace cocos3d
{
C3DRenderNode::C3DRenderNode(const std::string& id):C3DNode(id),C3DResource(id),_isVisibleByCamera(true)
{
	_scene = NULL;

	_showSkeleton = false;
	_showCollitionBox = false;
}

C3DRenderNode::~C3DRenderNode()
{
	_scene = NULL;

	if (_attachNodes.size() > 0)
    {
		std::vector<AttachNode*>::iterator iter = _attachNodes.begin();
        while (iter != _attachNodes.end())
        {
            AttachNode* an = *iter;
            SAFE_DELETE(an);
            ++iter;
        }
		_attachNodes.clear();
    }

	std::vector<C3DCollitionBox*>::iterator iter = m_collitionBoxs.begin();
	for ( ; iter != m_collitionBoxs.end(); iter++ )
	{
		SAFE_DELETE((*iter));
	}
	m_collitionBoxs.clear();
}

bool C3DRenderNode::loadFromFile(const std::string& fileName, bool isLoadAll /*= false*/)
{
	bool res(false);

	C3DResourceLoader* bundle = C3DResourceLoader::create(fileName);
	if (NULL != bundle)
	{
		res = load(bundle, isLoadAll);
		SAFE_RELEASE(bundle);
	}

	return res;
}

void C3DRenderNode::reload()
{
 	C3DResourceLoader* bundle = C3DResourceLoader::create(_fileName);
	if (bundle == NULL)
		return;

	bundle->reLoadSuperModel(this);
	bundle->release();
}

void C3DRenderNode::draw()
{
	if(_visible == false)
		return;

	if (_isVisibleByCamera)
	{
		for(std::vector<C3DNode*>::const_iterator iter=_children.begin(); iter!=_children.end(); ++iter)
		{
			C3DNode* node = *iter;

			if (node->getType() != C3DNode::NodeType_Model)
				continue;

			node->draw();
		}
	}

	std::vector<AttachNode*>::iterator nit;
	for (nit = _attachNodes.begin(); nit != _attachNodes.end(); nit++)
	{
		(*nit)->draw();
	}
}

/**
* Update routine
*/
void C3DRenderNode::update(long elapsedTime)
{
	//C3DNode::update(deltatime);
    getAABB();
	_isVisibleByCamera = (_scene->getActiveCamera()->isVisible(*_bb));

	if(_attachNodes.size()>0)
	{
		for (std::vector<AttachNode*>::iterator nit = _attachNodes.begin(); nit != _attachNodes.end(); nit++)
		{
			(*nit)->update(elapsedTime);
		}
	}

	// update CollitionBox
	std::vector<C3DCollitionBox*>::iterator iter = m_collitionBoxs.begin();
	for ( ; iter != m_collitionBoxs.end(); iter++)
	{
		(*iter)->update();
	}
}

AttachNode * C3DRenderNode::accessNode( C3DNode* pNode)
{
	std::vector<AttachNode*>::iterator nit;
	for (nit = _attachNodes.begin(); nit != _attachNodes.end(); nit++)
	{
		if ( (*nit)->node() == pNode )
			break;
	}
	if (nit == _attachNodes.end())
	{
		_attachNodes.push_back( new AttachNode( pNode, this ) );
		nit = _attachNodes.end() - 1;
	}
	return *nit;
}

AttachNode * C3DRenderNode::attachNode( const std::string & nodeName )
{
	C3DNode* pNode;

	if (nodeName.empty())
	{
		return NULL;
	}
	else
	{
		pNode = findNode( nodeName.c_str() );
		if (!pNode)
		{
			return NULL;
		}
	}

	AttachNode * pAttachNode = this->accessNode( pNode );

	return pAttachNode;
}

void C3DRenderNode::attach(const std::string& nodeName, C3DNode* attachment)
{
	AttachNode* attachNode = this->attachNode(nodeName);
	if(attachNode==NULL)
		return;

	attachNode->attach(attachment);
}

void C3DRenderNode::detach(const std::string& nodeName, C3DNode* attachment)
{
	AttachNode* attachNode = this->attachNode(nodeName);
	if(attachNode==NULL)
		return;

	attachNode->detach(attachment);
}

void C3DRenderNode::drawCollitionBox()
{
	if(_visible == false)
		return;

	std::vector<C3DCollitionBox*>::iterator iter = m_collitionBoxs.begin();
	for ( ; iter != m_collitionBoxs.end(); iter++)
	{
		C3DCollitionBox* bb = *iter;

		_scene->getGeoWireRender()->add3DLine(bb->getPosition(), bb->getPosition() + bb->getRight()*2, C3DVector4(1,0,0,1));
		_scene->getGeoWireRender()->add3DLine(bb->getPosition(),bb->getPosition() + bb->getUp()*2,C3DVector4(0,1,0,1));
		_scene->getGeoWireRender()->add3DLine(bb->getPosition(),bb->getPosition() + bb->getForward()*2,C3DVector4(0,0,1,1));
		_scene->getGeoWireRender()->addCollitionBox(bb, C3DVector4(1,1,0,1));
	}
}

void C3DRenderNode::drawDebug()
{
	C3DNode::drawDebug();

	if (_showCollitionBox)
		drawCollitionBox();
}

void C3DRenderNode::transformChanged()
{
	C3DNode::transformChanged();
}

void C3DRenderNode::showCollitionBox(bool show)
{
	 _showCollitionBox = show;
}

bool C3DRenderNode::showCollitionBox()
{
	return _showCollitionBox;
}

void C3DRenderNode::copyFrom(const C3DTransform* other, C3DNode::CloneContext& context)
{
	const C3DRenderNode* otherNode = static_cast<const C3DRenderNode*>(other);

	C3DNode::copyFrom(other, context);

	_showSkeleton = otherNode->_showSkeleton;

	if (otherNode->_attachNodes.size())
    {
        for (size_t i = 0; i < otherNode->_attachNodes.size(); i++) {
            C3DNode* node = context.cloneMap[otherNode->_attachNodes[i]->_node];
            C3DNode* owner = context.cloneMap[otherNode->_attachNodes[i]->_owner];
            CCAssert(node == NULL || owner == NULL, "attachment node or owner null");
            if (node == NULL || owner == NULL)
                return;
            AttachNode* attach = new AttachNode(node, owner);
            _attachNodes.push_back(attach);
        }
    }
}

C3DMaterial* C3DRenderNode::getMaterial(const std::string& meshName)
{
	C3DNode* node = findNode(meshName.c_str());
	if(node == NULL)
		return NULL;

	if (node->getType() != C3DNode::NodeType_Model)
		return NULL;

	return static_cast<C3DModelNode*>(node)->getModel()->getMaterial();
}

void C3DRenderNode::getAllModel(std::list<C3DNode*>& models)
{
	models.clear();
	for(std::vector<C3DNode*>::const_iterator iter=_children.begin(); iter!=_children.end(); ++iter)
	{
		C3DNode* node = *iter;

		if (node->getType() != C3DNode::NodeType_Model)
			continue;
		models.push_back(node);
	}
}

void C3DRenderNode::setMaterial(const std::string& modelName, const std::string& matName)
{
	C3DNode* node = findNode(modelName.c_str());
	if (node == NULL)
		return;

	if (node->getType() != C3DNode::NodeType_Model)
		return;

	static_cast<C3DModelNode*>(node)->getModel()->setMaterial(matName.c_str());
}

void C3DRenderNode::setMaterial(const std::string& matName)
{
    for (std::vector<C3DNode*>::const_iterator iter = _children.begin(); iter!=_children.end(); ++iter)
    {
        C3DNode* node = *iter;

        if(node->getType()!=C3DNode::NodeType_Model )
            continue;
        static_cast<C3DModelNode*>(node)->getModel()->setMaterial(matName.c_str());
    }
}

void C3DRenderNode::setMaterial(C3DMaterial* material)
{
	for (std::vector<C3DNode*>::const_iterator iter = _children.begin(); iter!=_children.end(); ++iter)
	{
		C3DNode* node = *iter;

		if(node->getType()!=C3DNode::NodeType_Model )
		{
			continue;
		}
		static_cast<C3DModelNode*>(node)->getModel()->setMaterial( material );
	}
}

void C3DRenderNode::setDefaultMaterial(const std::string& path)
{
    for (std::vector<C3DNode*>::const_iterator iter = _children.begin(); iter!=_children.end(); ++iter)
    {
        C3DNode* node = *iter;

        if(node->getType()!=C3DNode::NodeType_Model )
            continue;
        static_cast<C3DModelNode*>(node)->getModel()->setDefaultMaterial(path);
    }
}

void C3DRenderNode::removeMaterial(const std::string& modelName, const std::string& matName)
{
	C3DNode* node = findNode(modelName.c_str());
	if (node == NULL)
		return;

	if (node->getType() != C3DNode::NodeType_Model)
		return;

	static_cast<C3DModelNode*>(node)->getModel()->removeMaterial();
}

void C3DRenderNode::removeMaterial(const std::string& matName)
{
	for (std::vector<C3DNode*>::const_iterator iter = _children.begin(); iter!=_children.end(); ++iter)
	{
		C3DNode* node = *iter;

		if(node->getType()!=C3DNode::NodeType_Model )
			continue;
		static_cast<C3DModelNode*>(node)->getModel()->removeMaterial();
	}
}

unsigned int C3DRenderNode::getTriangleCount() const
{
    int nTriangle = 0;

    for(std::vector<C3DNode*>::const_iterator iter=_children.begin(); iter!=_children.end(); ++iter)
	{
		C3DNode* node = *iter;

		if(node->getType()!=C3DNode::NodeType_Model )
			continue;

        C3DMesh* mesh = ((C3DModelNode*)node)->getModel()->getMesh();
		nTriangle += mesh->getTriangleCount();
	}
    return nTriangle;
}

unsigned int C3DRenderNode::getValidCollitionBoxID()
{
	for (unsigned int i = 0; i < m_collitionBoxs.size(); i++)
	{
		bool bFind = false;
		for (std::vector<C3DCollitionBox*>::iterator iter = m_collitionBoxs.begin(); iter != m_collitionBoxs.end(); iter++)
		{
			if (i == (*iter)->getID())
			{
				bFind = true;
				break;
			}
		}

		if (!bFind)
			return i;
	}
	return m_collitionBoxs.size();
}

unsigned int C3DRenderNode::addCollitionBox(const std::string& strAttachBone, const C3DVector3& offset, float fRadius)
{
	m_collitionBoxs.push_back((new cocos3d::C3DCollitionBox(this, getValidCollitionBoxID(), strAttachBone, offset, fRadius)));
	return m_collitionBoxs.size();
}

void C3DRenderNode::removeCollitionBox(C3DCollitionBox* bb)
{
	std::vector<C3DCollitionBox*>::iterator iter = m_collitionBoxs.begin();
	for ( ; iter != m_collitionBoxs.end(); iter++)
	{
		if ((*iter) == bb)
		{
			SAFE_DELETE((*iter));
			m_collitionBoxs.erase(iter);
			break;
		}
	}
}

const std::vector<C3DCollitionBox*>& C3DRenderNode::getCollitionBoxs()const
{
	return m_collitionBoxs;
}

C3DCollitionBox* C3DRenderNode::pickCollitionBox(const C3DRay *ray)
{
	std::vector<C3DCollitionBox*>::iterator iter = m_collitionBoxs.begin();
	for ( ; iter != m_collitionBoxs.end(); iter++)
	{
		C3DAABB* tmp = new C3DAABB((*iter)->getMin(), (*iter)->getMax());
		if (ray->intersects(tmp))
		{
			SAFE_DELETE(tmp);
			return *iter;
		}
		SAFE_DELETE(tmp);
	}
	return NULL;
}

bool C3DRenderNode::saveCollitionBox(const std::string& fileName)const
{
	cocos3d::C3DElementNode* tpBoxListNode = C3DElementNode::createEmptyNode("", "collitionbox");
	if (!tpBoxListNode)
		return false;

	unsigned int index = 0;
	std::vector<C3DCollitionBox*>::const_iterator iter = m_collitionBoxs.begin();
	for ( ; iter != m_collitionBoxs.end(); ++iter, ++index)
	{
		StringTool::toString(index);
		cocos3d::C3DElementNode* tpNodeTechnique = cocos3d::C3DElementNode::createEmptyNode(StringTool::toString(index).c_str(), "box");
		C3DVector3 localPos = (*iter)->getLocalPos();
		tpNodeTechnique->setElement("center", &localPos);
		float radius = (*iter)->getRadius();
		tpNodeTechnique->setElement("radius", &radius);
		std::string strBone = (*iter)->getBone();
		tpNodeTechnique->setElement("bone", &strBone);

		tpBoxListNode->addChildNode(tpNodeTechnique);
	}

	tpBoxListNode->writeToFile(fileName.c_str());
	SAFE_DELETE(tpBoxListNode);
	return true;
}

bool C3DRenderNode::loadCollitionBox(const std::string& fileName)
{
	m_collitionBoxs.clear();
	cocos3d::C3DElementNode* tpBoxListNode = C3DElementNode::create(fileName.c_str());
	if (!tpBoxListNode)
		return true;

	tpBoxListNode = tpBoxListNode->getNextChild();
	if (!tpBoxListNode || tpBoxListNode->getNodeType() != "collitionbox")
		return false;

	cocos3d::C3DElementNode* tpNode = NULL;
	while ((tpNode = tpBoxListNode->getNextChild()))
	{
		if (tpNode)
		{
			C3DVector3 center;
			tpNode->getElement("center", &center);

			float radius;
			tpNode->getElement("radius", &radius);

			std::string strBoneName = tpNode->getElement("bone");

			addCollitionBox(std::string(strBoneName), center, radius);
		}
	}
	SAFE_DELETE(tpBoxListNode);
	return true;
}
}