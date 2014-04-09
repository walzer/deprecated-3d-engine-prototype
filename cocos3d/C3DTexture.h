#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "EnumDef_GL.h"
#include "cocos2d.h"

namespace cocos2d 
{
    class Texture2D;
}

namespace cocos3d
{
    class Image;

    /**
     * Represents a texture.
	 It wraps the cocos2d::ccTexture
 */
    class C3DTexture : public cocos2d::CCObject
    {
        friend class C3DSampler;
		friend class C3DTextureMgr;

    public:

        /**
         * Defines the set of supported texture formats.
         */
        enum Format
        {
            RGB     = GL_RGB,
            RGBA    = GL_RGBA,
            ALPHA   = GL_ALPHA,
            DEPTH   = GL_DEPTH_COMPONENT,
#ifdef USE_PVRTC
            COMPRESSED_RGB_PVRTC_4BPP = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,
            COMPRESSED_RGBA_PVRTC_4BPP = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,
            COMPRESSED_RGB_PVRTC_2BPP = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,
            COMPRESSED_RGBA_PVRTC_2BPP = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
#endif
        };

        /**
         * Creates a texture from the given image resource.
         *
         * @param path The image resource path.
         * @param generateMipmaps true to auto-generate a full mipmap chain, false otherwise.
         *
         * @return The new texture, or NULL if the texture could not be loaded/created.
         */
        static C3DTexture* create(const std::string& path, bool generateMipmaps = false);
        static C3DTexture* create(int width, int height, Format fmt, bool generateMipmaps = false);
	    static C3DTexture* create(int width, int height, Format fmt, const void* data, ssize_t dataLen, bool generateMipmaps = false);

		static cocos2d::CCImage* createImage(const std::string& path);

		void init(int width, int height, C3DTexture::Format fmt, bool generateMipmaps);
		virtual void reload();

        /**
         * Returns the texture width.
         */
        unsigned int getWidth() const;

        /**
         * Returns the texture height.
         */
        unsigned int getHeight() const;

        /**
         * Sets the wrap mode for this texture.
         *
         * @param wrapS Horizontal wrapping mode for the texture.
         * @param wrapT Vertical wrapping mode for the texture.
         */
        void setWrapMode(Texture_Wrap wrapS, Texture_Wrap wrapT);

        /**
         * Sets the minification and magnification filter modes for this texture.
         *
         * @param minificationFilter New texture minification filter.
         * @param magnificationFilter New texture magnification filter.
         */
        void setFilterMode(Texture_Filter minificationFilter, Texture_Filter magnificationFilter);

        /**
         * Generates a full mipmap chain for this texture if it isn't already mipmapped.
         */
        void generateMipmaps();

        /**
         * Determines if this texture currently contains a full mipmap chain.
         *
         * @return True if this texture is currently mipmapped, false otherwise.
         */
        bool isMipmapped() const;

        /**
         * Returns the texture handle.
         *
         * @return The texture handle.
         */
        GLuint getHandle() const;

		/**
         * get texture path
         */
        const std::string& getPath() const { return _path; };
	protected:
		void innerInit2D(const std::string& path, bool generateMipmaps = false);
		void innerInit2D(C3DTexture* rhs);

        C3DTexture();
        C3DTexture(const C3DTexture& copy);
        virtual ~C3DTexture();

#ifdef USE_PVRTC
        static C3DTexture* createCompressedPVRTC(const std::string& path);
#endif

        std::string			_path;

		// zhukaixy: Handle不为0说明是一个Cocos3D纹理，否则为Cocos2D管理的纹理(即与_texture互斥)，历史遗留代码稍后处理。
		GLuint				_handle;
		C3DTexture::Format	_fmt;

		//------------------------------------------
        unsigned int		_width;
        unsigned int		_height;
        bool				_mipmapped;

        cocos2d::Texture2D* _2DTex;
    };

	//-----------------------------------------------------------------------
	class C3DTextureMgr :public cocos2d::Ref
	{
	public:
		typedef std::vector<C3DTexture*> T_CACHE_CONTAINER;
	public:
		static C3DTextureMgr* getInstance();

		void add(C3DTexture* texture);
		void remove(C3DTexture* texture);
		C3DTexture* get(const std::string& strID);

		void reload();
	protected:
		C3DTextureMgr();
		~C3DTextureMgr();
	protected:
		T_CACHE_CONTAINER _texCon;
	};
}

#endif
