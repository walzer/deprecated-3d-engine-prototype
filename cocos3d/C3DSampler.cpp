#include "Base.h"
#include "C3DSampler.h"
#include "C3DTexture.h"
#include "C3DStream.h"
#include "C3DElementNode.h"

namespace cocos3d
{
C3DSampler::C3DSampler()
    : _3DTexture(NULL),
	_wrapS(Texture_Wrap_CLAMP),
	_wrapT(Texture_Wrap_CLAMP), 
	_minFilter(Texture_Filter_LINEAR),
	_magFilter(Texture_Filter_LINEAR),
	_dirtyBit(Texture_All_Dirty)
{
}

C3DSampler::C3DSampler(C3DTexture* texture)
    : _3DTexture(texture),
	_magFilter(Texture_Filter_LINEAR),
	_dirtyBit(Texture_All_Dirty)
{
	texture->retain();
	setWrapMode(Texture_Wrap_CLAMP, Texture_Wrap_CLAMP);
    _minFilter = texture->isMipmapped() ? Texture_Filter_LINEAR_MIPMAP_LINEAR : Texture_Filter_LINEAR;
}

C3DSampler::~C3DSampler()
{
    SAFE_RELEASE(_3DTexture);
}

C3DSampler* C3DSampler::create(C3DTexture* texture)
{
    assert(texture != NULL);

	C3DSampler* sample = new C3DSampler(texture);

    return sample;
}

C3DSampler* C3DSampler::create(const std::string& path, bool generateMipmaps)
{
    C3DTexture* texture = C3DTexture::create(path, generateMipmaps);

	assert(texture != NULL);

	C3DSampler* sample = new C3DSampler(texture);

    return sample;
}

void C3DSampler::setTexture(const std::string& path, bool generateMipmaps)
{
	SAFE_RELEASE(_3DTexture);
    C3DTexture* texture = C3DTexture::create(path, generateMipmaps);

	assert(texture != NULL);

	_3DTexture = texture;
	_3DTexture->retain();

	_dirtyBit = Texture_All_Dirty;
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

void C3DSampler::setWrapMode(Texture_Wrap wrapS, Texture_Wrap wrapT)
{
	bool sizePot(true);
	if ( _3DTexture != NULL )
	{
		if (   nextPOT( _3DTexture->getWidth() ) != _3DTexture->getWidth()
			|| nextPOT( _3DTexture->getHeight() ) != _3DTexture->getHeight() )
		{
			sizePot = false;
			if ( wrapS != Texture_Wrap_CLAMP || wrapT != Texture_Wrap_CLAMP )
			{
				LOG_ERROR("C3DSampler::setWrapMode texture size is not pot, can only use wrao mode CLAMP" );
			}
		}
	}

    _wrapS = sizePot?wrapS:Texture_Wrap_CLAMP;
    _wrapT = sizePot?wrapT:Texture_Wrap_CLAMP;
    _dirtyBit |= Texture_Wrap_Dirty;
}

void C3DSampler::setFilterMode(Texture_Filter minificationFilter, Texture_Filter magnificationFilter)
{
	assert(_3DTexture);

	if(_3DTexture)
	{
		_minFilter = _3DTexture->isMipmapped() ? Texture_Filter_NEAREST_MIPMAP_LINEAR : Texture_Filter_LINEAR;
		_magFilter = Texture_Filter_LINEAR;
	}
	else
	{
		_minFilter = minificationFilter;
		_magFilter = magnificationFilter;
	}

	_dirtyBit |= Texture_Filter_Dirty;
}

C3DTexture* C3DSampler::getTexture() const
{
    return _3DTexture;
}

void C3DSampler::bind()
{
	//GLint currentTextureId;
   // glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId);
	if(_3DTexture == NULL)
		return;

    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _3DTexture->getHandle()) );
    if (_dirtyBit & Texture_Wrap_Dirty)
    {
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)_wrapS) );
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)_wrapT) );
        _dirtyBit &= ~Texture_Wrap_Dirty;
    }
    if (_dirtyBit & Texture_Filter_Dirty)
    {
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)_minFilter) );
        GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)_magFilter) );
        _dirtyBit &= ~Texture_Filter_Dirty;
    }

	//glBindTexture(GL_TEXTURE_2D, (GLuint)currentTextureId) ;
}

void C3DSampler::reload()
{
	_dirtyBit = Texture_All_Dirty;
}

const std::string C3DSampler::getPath() const
{
    if (_3DTexture)
        return _3DTexture->getPath();
    else
        return "";
}

Texture_Wrap parseTextureWrapMode(const std::string& str, Texture_Wrap defaultValue)
{
    if (str.empty())
    {
        return defaultValue;
    }
    else if (str == "REPEAT")
    {
        return Texture_Wrap_REPEAT;
    }
    else if (str == "CLAMP")
    {
        return Texture_Wrap_CLAMP;
    }
    return defaultValue;
}

Texture_Filter parseTextureFilterMode(const std::string& str, Texture_Filter defaultValue)
{
    if (str.empty())
    {
        return defaultValue;
    }
    else if (str == "NEAREST")
    {
        return Texture_Filter_NEAREST;
    }
    else if (str == "LINEAR")
    {
        return Texture_Filter_LINEAR;
    }
    else if (str == "NEAREST_MIPMAP_NEAREST")
    {
        return Texture_Filter_NEAREST_MIPMAP_NEAREST;
    }
    else if (str == "LINEAR_MIPMAP_NEAREST")
    {
        return Texture_Filter_LINEAR_MIPMAP_NEAREST;
    }
    else if (str == "NEAREST_MIPMAP_LINEAR")
    {
        return Texture_Filter_NEAREST_MIPMAP_LINEAR;
    }
    else if (str == "LINEAR_MIPMAP_LINEAR")
    {
        return Texture_Filter_LINEAR_MIPMAP_LINEAR;
    }
    return defaultValue;
}

const std::string textureFilterModeToString(Texture_Filter filter)
{
    switch (filter)
    {
    case Texture_Filter_LINEAR:
        return "LINEAR";
    case Texture_Filter_NEAREST:
        return "NEAREST";
    case Texture_Filter_LINEAR_MIPMAP_LINEAR:
        return "LINEAR_MIPMAP_LINEAR";
    case Texture_Filter_LINEAR_MIPMAP_NEAREST:
        return "LINEAR_MIPMAP_NEAREST";
    case Texture_Filter_NEAREST_MIPMAP_LINEAR:
        return "NEAREST_MIPMAP_LINEAR";
    case Texture_Filter_NEAREST_MIPMAP_NEAREST:
        return "NEAREST_MIPMAP_NEAREST";
    }

    return "NEAREST";
}

bool textureFilterModeHasMipmap(Texture_Filter filter)
{
    return filter != Texture_Filter_LINEAR && filter != Texture_Filter_NEAREST;
}

const std::string textureWrapModeToString(Texture_Wrap wrap)
{
    switch (wrap)
    {
    case Texture_Wrap_CLAMP:
        return "CLAMP";
    case Texture_Wrap_REPEAT:
        return "REPEAT";
    }
    return "CLAMP";
}

bool C3DSampler::load(C3DElementNode* node)
{
	// Read the texture uniform name
    const std::string& name = node->getNodeName();
    if (name.empty())
        return false; // missing texture uniform name

    // Get the texture path
    const std::string path = node->getElement("path");
    if (path.empty())
        return false; // missing texture path

    // Read texture state (booleans default to 'false' if not present)
    bool mipmap = node->getElement("mipmap",&mipmap);
    Texture_Wrap wrapS = parseTextureWrapMode(node->getElement("wrapS"), Texture_Wrap_REPEAT);
    Texture_Wrap wrapT = parseTextureWrapMode(node->getElement("wrapT"), Texture_Wrap_REPEAT);
    Texture_Filter minFilter = parseTextureFilterMode(node->getElement("minFilter"), mipmap ? Texture_Filter_NEAREST_MIPMAP_LINEAR : Texture_Filter_LINEAR);
    Texture_Filter magFilter = parseTextureFilterMode(node->getElement("magFilter"), Texture_Filter_LINEAR);

	this->setTexture(path, mipmap);

    this->setWrapMode(wrapS, wrapT);
    this->setFilterMode(minFilter, magFilter);

	return true;
}

bool C3DSampler::save(C3DElementNode* node)
{
    node->setElement("path", this->getPath());
    node->setElement("mipmap", textureFilterModeHasMipmap(this->getMinFilter()) ? "true" : "false");
    node->setElement("wrapS", textureWrapModeToString(this->getWrapS()));
    node->setElement("wrapT", textureWrapModeToString(this->getWrapS()));
    node->setElement("minFilter", textureFilterModeToString(this->getMinFilter()));
    node->setElement("magFilter", textureFilterModeToString(this->getMagFilter()));
	return true;
}
}