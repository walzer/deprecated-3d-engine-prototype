#include "Base.h"
#include "C3DTexture.h"
#include "C3DStream.h"
#include "cocos2d.h"
#include "cocos2d.h"
#include "CCTexture2D.h"
#include "CCTextureCache.h"
#include "ccUtils.h"

using namespace std;
using namespace cocos2d;

namespace cocos3d
{
C3DTexture::C3DTexture() :
	_handle(0),
	_mipmapped(false),
	_width(0),
	_height(0),
	_2DTex(NULL)
{
	C3DTextureMgr::getInstance()->add(this);
}

C3DTexture::C3DTexture(const C3DTexture& copy)
{
	C3DTextureMgr::getInstance()->add(this);
}

C3DTexture::~C3DTexture()
{
	C3DTextureMgr::getInstance()->remove(this);
    SAFE_RELEASE(_2DTex);
}

void C3DTexture::reload()
{
	if(NULL != _2DTex && _mipmapped)
	{
		_2DTex->generateMipmap();
	}

	if(_handle)
	{
		// Cocos3D managed resource
		init(_width, _height, _fmt, false);
	}
}

C3DTexture* C3DTexture::create(const std::string& path, bool generateMipmaps)
{
    C3DTexture* retTexture = new C3DTexture();

	retTexture->innerInit2D(path, generateMipmaps);
	retTexture->autorelease();

    return retTexture;
}

void C3DTexture::innerInit2D(const std::string& path, bool generateMipmaps)
{
    CCTexture2D* tex2D = CCTextureCache::sharedTextureCache()->addImage(path);
    if (tex2D == NULL)
    {
        LOG_ERROR_VARG("failed to load texture file: %s", path.c_str());
        return;
    }

	SAFE_RELEASE(_2DTex);

    if (generateMipmaps)
        tex2D->generateMipmap();

    tex2D->retain();
    _2DTex = tex2D;
    _path = path;
    _width = tex2D->getPixelsWide();
    _height = tex2D->getPixelsHigh();
	_mipmapped = generateMipmaps;
}

void C3DTexture::innerInit2D(C3DTexture* rhs)
{
	rhs->_2DTex->retain();
	_2DTex = rhs->_2DTex;
	_path = rhs->_path;
	_width = _2DTex->getPixelsWide();
	_height = _2DTex->getPixelsHigh();
	_mipmapped = rhs->_mipmapped;
}

inline unsigned long nextPOT(unsigned long x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return x + 1;
}

C3DTexture* C3DTexture::create(int width, int height, C3DTexture::Format fmt, const void* data, ssize_t dataLen, bool generateMipmaps)
{
	CCTexture2DPixelFormat format;
	switch(fmt)
	{
		case C3DTexture::RGBA:
			format = kCCTexture2DPixelFormat_RGBA8888;
			break;
		case C3DTexture::RGB:
			format = kCCTexture2DPixelFormat_RGB888;
			break;

		default:
			return nullptr;
	}

	CCTexture2D* texture2D = new CCTexture2D();
	texture2D->initWithData(data, dataLen, format, width, height, CCSize(width, height));

	C3DTexture* retTexture = new C3DTexture();
	retTexture->_2DTex = texture2D;
	retTexture->_width = width;
	retTexture->_height = height;
	retTexture->_mipmapped = generateMipmaps;
	retTexture->autorelease();

	if (generateMipmaps)
		retTexture->generateMipmaps();

	return retTexture;
}

C3DTexture* C3DTexture::create(int width, int height, C3DTexture::Format fmt, bool generateMipmaps)
{
	C3DTexture* retTexture = new C3DTexture();
	retTexture->init(width, height, fmt, generateMipmaps);

	C3DTextureMgr::getInstance()->add(retTexture);

	retTexture->autorelease();
	return retTexture;
}

cocos2d::CCImage* C3DTexture::createImage(const std::string& path)
{
	if (path.empty())
		return NULL;

	std::string pathKey = path;

	pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey);
	if (pathKey.size() == 0)
	{
		return NULL;
	}

	std::string fullpath = pathKey;
	std::string lowerCase(pathKey);
	for (unsigned int i = 0; i < lowerCase.length(); ++i)
	{
		lowerCase[i] = tolower(lowerCase[i]);
	}

	cocos2d::CCImage* pImage = new CCImage();
	if(NULL == pImage)
		return NULL;

	bool bRet = pImage->initWithImageFile(fullpath.c_str());
	if (!bRet)
		SAFE_DELETE(pImage);

	return pImage;
}

void C3DTexture::init(int width, int height, C3DTexture::Format fmt, bool generateMipmaps)
{
    GLuint handle;
    GL_ASSERT(glGenTextures(1, &handle));
    GL_ASSERT(glBindTexture(GL_TEXTURE_2D, handle));

    // Specify OpenGL texture image
    GLenum internalFormat = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    switch(fmt)
    {
        case RGBA:
            internalFormat = GL_RGBA;
            break;
        case RGB:
            internalFormat = GL_RGB;
            break;
        case ALPHA:
            internalFormat = GL_ALPHA;
            break;
        case DEPTH:
            internalFormat = GL_DEPTH_COMPONENT;
            type = GL_UNSIGNED_SHORT;
            break;
        default:
            LOG_ERROR_VARG("Unknown texture formmat: %d", fmt);
            return;
    }

    GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, (GLenum)fmt, type, 0));

    _handle = handle;
	_fmt = fmt;

    _2DTex = NULL;
    _width = width;
    _height = height;
    _mipmapped = false;
}

#ifdef USE_PVRTC
C3DTexture* C3DTexture::createCompressedPVRTC(const std::string& path)
{
    return create(path, false);
}
#endif

unsigned int C3DTexture::getWidth() const
{
    return _width;
}

unsigned int C3DTexture::getHeight() const
{
    return _height;
}

TextureHandle C3DTexture::getHandle() const
{
    if (_handle)
        return _handle;

    if (_2DTex == NULL)
    {
        LOG_ERROR("getHandle from NULL texture");
        return -1;
    }

    return _2DTex->getName();
}

void C3DTexture::setWrapMode(Texture_Wrap wrapS, Texture_Wrap wrapT)
{
    if (_2DTex != NULL)
    {
        GLint currentTextureId;

        GL_ASSERT( glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId) );
        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _2DTex->getName()) );
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapS) );
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapT) );
        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, (GLuint)currentTextureId) );
    }
}

void C3DTexture::setFilterMode(Texture_Filter minificationFilter, Texture_Filter magnificationFilter)
{
    if (_2DTex != NULL)
    {
        GLint currentTextureId;
        GL_ASSERT( glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId) );
        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _2DTex->getName()) );
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minificationFilter) );
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)magnificationFilter) );
        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, (GLuint)currentTextureId) );
    }
}

void C3DTexture::generateMipmaps()
{
    if (_2DTex != NULL && !_2DTex->hasMipmaps() )
    {
        _2DTex->generateMipmap();
        GLint currentTextureId;
        GL_ASSERT( glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId) );
        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _2DTex->getName()) );
        glGenerateMipmap(GL_TEXTURE_2D);

        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, (GLuint)currentTextureId) );
    }
}

bool C3DTexture::isMipmapped() const
{
    if (_2DTex == NULL)
    {
        //LOG_ERROR("isMipmapped from NULL texture");
        return false;
    }
    return _2DTex->hasMipmaps();
}

//-----------------------------------------------------------------------------------
static C3DTextureMgr* g_3DTextureMgr = NULL;

C3DTextureMgr::C3DTextureMgr()
{
}

C3DTextureMgr::~C3DTextureMgr()
{
	LOG_TRACE_VARG("@C3DTextureMgr::%d", _texCon.size());
	
	g_3DTextureMgr = NULL;
}

C3DTextureMgr* C3DTextureMgr::getInstance()
{
	if (!g_3DTextureMgr)
	{
		g_3DTextureMgr = new C3DTextureMgr();
		g_3DTextureMgr->autorelease();
	}
	return g_3DTextureMgr;
}

void C3DTextureMgr::add(C3DTexture* texture)
{
	T_CACHE_CONTAINER::iterator itr = std::find(_texCon.begin(), _texCon.end(), texture);
	if (itr == _texCon.end())
	{
		_texCon.push_back(texture);
	}
}

void C3DTextureMgr::remove(C3DTexture* texture)
{
	T_CACHE_CONTAINER::iterator itr = std::find(_texCon.begin(), _texCon.end(), texture);
	if (itr != _texCon.end())
	{
		_texCon.erase(itr);
	}
}

C3DTexture* C3DTextureMgr::get(const std::string& strID)
{
	for(T_CACHE_CONTAINER::iterator iter = _texCon.begin(); iter!=_texCon.end(); ++iter)
	{
		if((*iter)->getPath() == strID)
			return *iter;
	}
	return NULL;
}

void C3DTextureMgr::reload()
{
	for(T_CACHE_CONTAINER::iterator iter = _texCon.begin(); iter!=_texCon.end(); ++iter)
	{
		(*iter)->reload();
	}
}

}