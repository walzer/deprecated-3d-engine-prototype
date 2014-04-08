#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "EnumDef_GL.h"
#include "cocos2d.h"

namespace cocos3d
{
class C3DTexture;
class C3DElementNode;

    enum
    {
       Texture_None_Dirty = 0,
       Texture_Wrap_Dirty = 1,
       Texture_Filter_Dirty = 2,
       Texture_All_Dirty = Texture_Wrap_Dirty | Texture_Filter_Dirty,
    };

/**
* Defnies a texture sampler.
*
* A texture sampler is basically an instance of a texture that can be
* used to sample a texture from a material. In addition to the texture
* itself, a sampler stores per-instance texture state information, such
* as wrap and filter modes.
*/
class C3DSampler : public cocos2d::CCObject
{
    friend class C3DTexture;

public:
	C3DSampler();
	C3DSampler(C3DTexture* texture);
    ~C3DSampler();

    /**
        * Creates a sampler for the specified texture.
        *
        * @param texture The texture.
        *
        * @return The new sampler.
        */
    static C3DSampler* create(C3DTexture* texture);

    /**
        * Creates a sampler for the specified texture.
        *
        * @param path Path to the texture to create a sampler for.
        * @param generateMipmaps True to force a full mipmap chain to be generated for the texture, false otherwise.
        *
        * @return The new sampler.
        */
    static C3DSampler* create(const std::string& path, bool generateMipmaps = false);

    /**
        * Sets the wrap mode for this sampler.
        *
        * @param wrapS The horizontal wrap mode.
        * @param wrapT The vertical wrap mode.
        */
    void setWrapMode(Texture_Wrap wrapS, Texture_Wrap wrapT);

    /**
        * Sets the texture filter modes for this sampler.
        *
        * @param minificationFilter The texture minification filter.
        * @param magnificationFilter The texture magnification filter.
        */
    void setFilterMode(Texture_Filter minificationFilter, Texture_Filter magnificationFilter);

    /**
        * Returns the texture for this sampler.
        */
    C3DTexture* getTexture() const;

    /**
        * Binds the texture of this sampler to the renderer and applies the sampler state.
        */
    void bind();

    const std::string getPath() const;

    Texture_Wrap getWrapS() const { return _wrapS; }
    Texture_Wrap getWrapT() const { return _wrapT; }
    Texture_Filter getMinFilter() const { return _minFilter; }
    Texture_Filter getMagFilter() const { return _magFilter; }

	void setTexture(const std::string& path, bool generateMipmaps = false);

	bool load(C3DElementNode* node);
	bool save(C3DElementNode* node);

	void reload();
private:

    C3DTexture* _3DTexture;
    Texture_Wrap _wrapS;
    Texture_Wrap _wrapT;
    Texture_Filter _minFilter;
    Texture_Filter _magFilter;

    int _dirtyBit;
};

extern Texture_Wrap parseTextureWrapMode(const std::string& str, Texture_Wrap defaultValue);
extern Texture_Filter parseTextureFilterMode(const std::string& str, Texture_Filter defaultValue);
extern const std::string textureFilterModeToString(Texture_Filter filter);
extern bool textureFilterModeHasMipmap(Texture_Filter filter);
extern const std::string textureWrapModeToString(Texture_Wrap wrap);
}

#endif
