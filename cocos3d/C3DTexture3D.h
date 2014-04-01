#ifndef TEXTURE3D_H_
#define TEXTURE3D_H_

#include "EnumDef_GL.h"
#include "C3DTexture.h"

namespace cocos2d 
{
	class Image;
}

namespace cocos3d
{
    /**
     * texture cube map.
	 It wraps the cocos2d::ccTexture
 */
    class C3DTexture3D : public C3DTexture
    {
        friend class C3DSampler;

    public:

        /**
         * create cube texture from 6 textures.
         *
         * @param path The image resource path.
         * @param generateMipmaps true to auto-generate a full mipmap chain, false otherwise.
         *
         * @create cube texture from 6 textures.
         */
		static C3DTexture3D* create(const std::string& positive_x,const std::string& negative_x,const std::string& positive_y,const std::string& negative_y,const std::string& positive_z,const std::string& negative_z);

		void init(const std::string& positive_x,const std::string& negative_x,const std::string& positive_y,const std::string& negative_y,const std::string& positive_z,const std::string& negative_z);
		void reload();

	protected:

		static unsigned char* getImageData(cocos2d::CCImage* image, cocos2d::CCTexture2DPixelFormat&    pixelFormat);

        C3DTexture3D();
        C3DTexture3D(const C3DTexture3D& copy);

        virtual ~C3DTexture3D();

    private:
		bool init();

		cocos2d::CCImage* _image[6];
		std::string _id;
    };
}

#endif
