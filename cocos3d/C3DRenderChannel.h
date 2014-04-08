#ifndef VISUAL_CHANNELS_H_
#define VISUAL_CHANNELS_H_

#include <string>
#include <map>
#include <vector>
#include "Base.h"
#include "cocos2d.h"

namespace cocos3d
{
class C3DTexture;
class C3DFrameBuffer;

class BaseChannelDrawItem;
class C3DScene;

class BaseChannelDrawItem
{
public:
	BaseChannelDrawItem( float sortParam )
		: _sortParam( sortParam )
	{
	}

	virtual ~BaseChannelDrawItem()
	{
	}

	// 对外的统一调用接口
	virtual void draw() = 0;
	virtual inline void clear()
	{
		_sortParam = 0.0f;
	}
public:

	virtual float getSortParam(void) const
	{
		return _sortParam;
	}

	inline void setSortParam( float f )
	{
		_sortParam = f;
	}

protected:
	float _sortParam;
};

class C3DBaseModel;

class ModelDrawItem : public BaseChannelDrawItem
{
public:
	typedef std::vector<ModelDrawItem*> ModelItems;

public:
	ModelDrawItem( C3DBaseModel* model, float sortParam )
		: BaseChannelDrawItem( sortParam )
		, _model( model )
	{
	}

	virtual ~ModelDrawItem()
	{
	}

protected:
	virtual void draw();

private:
	C3DBaseModel* _model;

public:
	inline C3DBaseModel* getModel()
	{
		return _model;
	}
	inline void setModel( C3DBaseModel* model )
	{
		_model = model;
	}
	virtual inline void clear()
	{
		BaseChannelDrawItem::clear();
		_model = NULL;
	}
};

class C3DBatchModel;

/**
 * This class is the base class and the manager for the visual channels. A
 * channel is essentially a bucket of objects with similar rendering rules.
 * The channels are traversed fixed global order, and within each channel items
 * are drawn, possibly in a sorted order.
 */
class C3DRenderChannel
{
public:

	enum SortType
	{
		ST_Greater,
		ST_Less,
		ST_None,
	};
	typedef std::list<BaseChannelDrawItem*> ChannelDrawItems;
	typedef std::map<std::string,ChannelDrawItems> ChannelDrawItemsMap;

public:
	/**
	 * Constructor & Destructor
	 */
	C3DRenderChannel()
		: _channelName( "" )
		, _sortType( ST_None )
		, _enable(true)
	{
	}

	virtual ~C3DRenderChannel();

	virtual void resetChannelSize();

	/**
	 * add draw item
	 */
	virtual void addItem( C3DBaseModel* model, float sortParamFlo, const std::string& sortParamStr = "" );

	// 如需进行特殊渲染排序，在此进行
	virtual void preDraw(void);
	/**
	 * draw routine
	 */
	virtual void draw(void);

	virtual void postDraw(void);

	void setSortType( SortType st )
	{
		_sortType = st;
	}

	/**
	 * get channel name
	 */
	inline const std::string& getName() const
	{
		return _channelName;
	}
	inline void setName( const std::string& name )
	{
		_channelName = name;
	}

	int itemCount(void);

	bool getEnable(void) const
	{
		return _enable;
	}

protected:
	// 排序方法
	bool (*compare)( const BaseChannelDrawItem* a, const BaseChannelDrawItem* b );
private:
	SortType _sortType;
	ChannelDrawItemsMap _drawItems;
	unsigned int _drawItemAlloc;
	std::string _channelName;

	bool _enable;

public:
	static ChannelDrawItems ItemPool;
};

/**
 * This class manage all the render channel.
 */
class RenderChannelManager : public cocos2d::CCObject
{
public:

	enum ChannelName
	{
		CN_Background = 0,	// 背景
		CN_Opacity,			// 不透明
		CN_Transparency,	// 透明
		CN_Debug,			// 调试信息
		CN_Count,
	};

	static const std::string SceneBufferName;

	static const std::string ChannelBackground;
	static const std::string ChannelOpacity;
	static const std::string ChannelTransparency;
	static const std::string ChannelDebug;

public:

	static RenderChannelManager* getInstance();

	/**
	 * Constructor & Destructor
	 */
	RenderChannelManager();
	virtual ~RenderChannelManager();

	void init();
	void clear();

	/**
	 * get channel by name
	 */

	C3DRenderChannel* getRenderChannel( const std::string& name );

	C3DRenderChannel* getRenderChannel( ChannelName channel )
	{
		return &(_channels[channel]);
	}

	/**
	 * draw routine
	 */
	void draw( /*C3DScene* scene*/ );

	C3DTexture* get3DSceneTexture(void);

private:
	void createFrameBuffer(void);

private:

	C3DRenderChannel _channels[CN_Count];
	C3DFrameBuffer* _frameBuffer;

	static RenderChannelManager* _instance;
};

}
#endif // VISUAL_CHANNELS_H
