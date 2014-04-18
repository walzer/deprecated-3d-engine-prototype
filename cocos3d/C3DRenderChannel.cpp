#include "C3DRenderChannel.h"
#include "Base.h"
#include "C3DBaseModel.h"
#include "C3DBatchModel.h"
#include "C3DParticleRender.h"
#include "C3DFrameBuffer.h"
#include "C3DTexture.h"
#include "C3DDepthStencilTarget.h"
#include "C3DRenderTarget.h"
#include "C3DRenderSystem.h"
#include "C3DScene.h"
#include "C3DPostProcess.h"
#include "CCGLView.h"

namespace cocos3d
{
bool greater( const BaseChannelDrawItem* a, const BaseChannelDrawItem* b )
{
	return a->getSortParam() > b->getSortParam();
}
bool less(const BaseChannelDrawItem* a, const BaseChannelDrawItem* b)
{
	return a->getSortParam() < b->getSortParam();
}

void ModelDrawItem::draw()
{
	_model->draw();
}

C3DRenderChannel::ChannelDrawItems C3DRenderChannel::ItemPool;

C3DRenderChannel::~C3DRenderChannel()
{
	resetChannelSize();
}

void C3DRenderChannel::resetChannelSize()
{
	for ( ChannelDrawItems::iterator ite = ItemPool.begin(); ite != ItemPool.end(); ++ite )
	{
		SAFE_DELETE( (*ite) );
	}
	ItemPool.clear();
}

void C3DRenderChannel::addItem( C3DBaseModel* model, float sortParamFlo, const std::string& sortParamStr )
{
	if ( model == NULL )
	{
		LOG_ERROR( "C3DRenderChannel::addItem C3DModel* is NULL" );
		return;
	}
	static ModelDrawItem* item(NULL);
	if ( ItemPool.empty() )
	{
		item = new ModelDrawItem( model, sortParamFlo );
	}
	else
	{
		item = (ModelDrawItem*)(*ItemPool.begin());
		ItemPool.pop_front();
		item->setModel( model );
		item->setSortParam( sortParamFlo );
	}

	_drawItems[sortParamStr].push_back( item );
}

void C3DRenderChannel::preDraw(void)
{
	switch ( _sortType )
	{
	case ST_Greater:
		{
			compare = greater;
		}break;
	case  ST_Less:
		{
			compare = less;
		}break;
	case ST_None:
		{
			compare = NULL;
		}break;
	}

	if ( compare != NULL )
	{
		std::map<std::string, ChannelDrawItems>::iterator iter_map = _drawItems.begin();
		for (;iter_map != _drawItems.end(); iter_map++)
		{
			iter_map->second.sort( compare );
		}
	}
}

void C3DRenderChannel::draw(void)
{
	std::map<std::string, ChannelDrawItems>::iterator iter_map = _drawItems.begin();
	for (;iter_map != _drawItems.end(); iter_map++)
	{
		ChannelDrawItems::iterator iter = iter_map->second.begin();
		for (;iter != iter_map->second.end(); ++iter)
		{
			(*iter)->draw();
		}
	}
}

void C3DRenderChannel::postDraw(void)
{
	std::map<std::string, ChannelDrawItems>::iterator iter_map = _drawItems.begin();
	for (;iter_map != _drawItems.end(); iter_map++)
	{
		for ( ChannelDrawItems::iterator ite = iter_map->second.begin(); ite != iter_map->second.end(); ++ite )
		{
			(*ite)->clear();
		}
		ItemPool.insert( ItemPool.end(), iter_map->second.begin(), iter_map->second.end() );
	}
	_drawItems.clear();
}

int C3DRenderChannel::itemCount(void)
{
	int count = 0;
	std::map<std::string, ChannelDrawItems>::iterator iter_map = _drawItems.begin();
	for (;iter_map != _drawItems.end(); iter_map++)
	{
		count += iter_map->second.size();
	}
	return count;
}

///////////////////////////////////////////////////////////////////////////////////

const std::string RenderChannelManager::ChannelBackground = std::string( "background" );
const std::string RenderChannelManager::ChannelOpacity = std::string( "opacity" );
const std::string RenderChannelManager::ChannelTransparency = std::string( "transparency" );
const std::string RenderChannelManager::ChannelDebug = std::string( "debug" );

const std::string RenderChannelManager::SceneBufferName = std::string( "@SceneBuffer" );

RenderChannelManager* RenderChannelManager::_instance = NULL;

RenderChannelManager* RenderChannelManager::getInstance()
{
	if ( _instance == NULL )
	{
		_instance = new RenderChannelManager();
		_instance->autorelease();
	}
	return _instance;
}

RenderChannelManager::RenderChannelManager()
	: _frameBuffer( NULL )
{
	_channels[CN_Background].setSortType( C3DRenderChannel::ST_Less );
	_channels[CN_Background].setName( ChannelBackground );
	_channels[CN_Opacity].setSortType( C3DRenderChannel::ST_None );
	_channels[CN_Opacity].setName( ChannelOpacity );
	_channels[CN_Transparency].setSortType( C3DRenderChannel::ST_Greater );
	_channels[CN_Transparency].setName( ChannelTransparency );
	_channels[CN_Debug].setSortType( C3DRenderChannel::ST_None );
	_channels[CN_Debug].setName( ChannelDebug );
}

RenderChannelManager::~RenderChannelManager()
{
	_instance = NULL;
}

void RenderChannelManager::init()
{
	_frameBuffer = NULL;
}
void RenderChannelManager::clear()
{
	SAFE_RELEASE( _frameBuffer );
}

C3DRenderChannel* RenderChannelManager::getRenderChannel( const std::string& name )
{
	if ( name == ChannelBackground )
	{
		return &(_channels[CN_Background]);
	}
	else if ( name == ChannelOpacity )
	{
		return &(_channels[CN_Opacity]);
	}
	else if ( name == ChannelTransparency )
	{
		return &(_channels[CN_Transparency]);
	}
	else if ( name == ChannelDebug )
	{
		return &(_channels[CN_Debug]);
	}
	else
	{
		return NULL;
	}
}

void RenderChannelManager::createFrameBuffer(void)
{
	static unsigned int fmtColor = C3DTexture::RGBA;
	static unsigned int fmtDepth = C3DDepthStencilTarget::DEPTH16;
		 	
	float width = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().width;
	float height = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().height;

	_frameBuffer = C3DFrameBuffer::create( SceneBufferName, width, height, fmtColor, fmtDepth );
	if (!_frameBuffer) return;

	_frameBuffer->retain();
}

void RenderChannelManager::draw( /*C3DScene* scene*/ )
{
	/**
	C3DPostProcess* process = scene->getActivePostProcess();
	if ( process != NULL && process->active() )
	{
		//if ( _frameBuffer == NULL )
		//{
		//	createFrameBuffer();
		//}
		process->beginDraw();

		//_frameBuffer->bind();

		static C3DVector4 clearColor(0,0,0,0);
		C3DRenderSystem::getInstance()->clear(CLEAR_COLOR_DEPTH,&clearColor,1.0f,0);
	}
	//*/

	for ( unsigned int i = 0; i < CN_Count; ++i )
	{
		C3DRenderChannel* channel = &(_channels[i]);
		channel->preDraw();
		channel->draw();
		channel->postDraw();
	}
	/**
	if ( process != NULL && process->active() )
	{
		//_frameBuffer->unbind();
		//process->draw();

		process->endDraw();
		process->draw();
	}
	//*/
}

C3DTexture* RenderChannelManager::get3DSceneTexture(void)
{
	if ( _frameBuffer == NULL )
	{
		createFrameBuffer();
	}

	return _frameBuffer->getRenderTarget()->getTexture();
}
} 