#include "Base.h"
#include "C3DModelNode.h"
#include "C3DModel.h"
#include "C3DScene.h"
#include "C3DNode.h"
#include "C3DRenderChannel.h"
#include "C3DMaterial.h"

namespace cocos3d
{
C3DModelNode::C3DModelNode(const std::string& id)
    : C3DNode(id), _model(NULL)
{
}

C3DModelNode::~C3DModelNode()
{
	if (_model)
		_model->setNode(NULL);
	SAFE_RELEASE(_model);
}

C3DModelNode* C3DModelNode::create(const std::string& id)
{
    return new C3DModelNode(id);
}

C3DNode::Type C3DModelNode::getType() const
{
	return C3DNode::NodeType_Model;
}

void C3DModelNode::setModel(C3DModel* model)
{
    if (_model != model)
    {
        if (_model)
        {
            _model->setNode(NULL);
            SAFE_RELEASE(_model);
        }

        _model = model;

        if (_model)
        {
            _model->retain();
            _model->setNode(this);
        }
    }
}

C3DModel* C3DModelNode::getModel()
{
    return _model;
}

void C3DModelNode::draw()
{
	if (_model)
	{
		C3DRenderChannel* channel = _model->getRenderChannel();
		if(channel != NULL && !_scene->isInShadowPass() )
		{
			if(channel->getName() == RenderChannelManager::ChannelOpacity)
			{
				channel->addItem( _model, _model->distanceToCamera(), _model->getMaterial()->getResourceName() );
			}
			else
			{
				channel->addItem( _model, _model->distanceToCamera(), "Transparency");
			}
		}
		else
		{
			_model->draw();
		}
	}
}

const C3DMatrix& C3DModelNode::getWorldMatrix()
{
    if (_dirtyBits & NODE_DIRTY_WORLD)
    {
        // Clear our dirty flag immediately to prevent this block from being entered if our
        // parent calls our getWorldMatrix() method as a result of the following calculations.
        _dirtyBits &= ~NODE_DIRTY_WORLD;

        // If we have a parent, multiply our parent world transform by our local
        // transform to obtain our final resolved world transform.
        C3DNode* parent = getParent();
		if (parent)
        {
			if(parent->getType()==C3DNode::NodeType_SuperModel)
			{
				_world = getMatrix();
			}
			else
			{
				C3DMatrix::multiply(parent->getWorldMatrix(), getMatrix(), &_world);
			}
        }
        else
        {
            _world = getMatrix();
        }

        // Our world matrix was just updated, so call getWorldMatrix() on all child nodes
        // to force their resolved world matrices to be updated.
		for(std::vector<C3DNode*>::const_iterator iter=_children.begin(); iter!=_children.end(); ++iter)
        {
			(*iter)->getWorldMatrix();
        }
    }

    return _world;
}

C3DNode* C3DModelNode::clone(CloneContext& context) const
{
	C3DModelNode* other = new C3DModelNode(_id);
	other->copyFrom(this, context);
	other->_id = _id;
	C3DModel* newModel = _model ? _model->clone(context) : NULL;
	other->setModel(newModel);
	other->autorelease();
	return other;
}

}