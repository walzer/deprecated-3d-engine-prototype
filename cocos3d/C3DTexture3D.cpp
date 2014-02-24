#include "Base.h"
#include "C3DTexture3D.h"
#include "C3DStream.h"
#include "cocos2d.h"
#include "textures/CCTexture2D.h"
#include "textures/CCTextureCache.h"
#include "support/ccUtils.h"
#include "platform/CCImage.h"


using namespace std;
using namespace cocos2d;

namespace cocos3d
{
    
    static std::vector<C3DTexture3D*> __textureCache;
    
    C3DTexture3D::C3DTexture3D() : _handle(0)
    {
		for (int i = 0; i < 6; i++)
		{
			_image[i] = NULL;
		}
    }
    
    C3DTexture3D::~C3DTexture3D()
    {
        std::vector<C3DTexture3D*>::iterator itr = std::find(__textureCache.begin(), __textureCache.end(), this);
        if (itr != __textureCache.end())
        {
            __textureCache.erase(itr);
        }
        
		for (int i = 0; i < 6; i++)
		{
			CC_SAFE_RELEASE(_image[i]);
		}

		if (_handle)
		{
			glDeleteTextures(1, &_handle);
			_handle = NULL;
		}
    }

	cocos2d::CCImage* C3DTexture3D::createImage(const std::string& path)
	{
		if (path.empty())
			return NULL;

		std::string pathKey = path;

		pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
		if (pathKey.size() == 0)
		{
			return NULL;
		}

		std::string fullpath = pathKey; // (CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(path));
		std::string lowerCase(pathKey);
		for (unsigned int i = 0; i < lowerCase.length(); ++i)
		{
			lowerCase[i] = tolower(lowerCase[i]);
		}
		CCImage::EImageFormat eImageFormat = CCImage::kFmtUnKnown;
		if (std::string::npos != lowerCase.find(".png"))
		{
			eImageFormat = CCImage::kFmtPng;
		}
		else if (std::string::npos != lowerCase.find(".jpg") || std::string::npos != lowerCase.find(".jpeg"))
		{
			eImageFormat = CCImage::kFmtJpg;
		}
		else if (std::string::npos != lowerCase.find(".tif") || std::string::npos != lowerCase.find(".tiff"))
		{
			eImageFormat = CCImage::kFmtTiff;
		}
		else if (std::string::npos != lowerCase.find(".webp"))
		{
			eImageFormat = CCImage::kFmtWebp;
		}

		cocos2d::CCImage* pImage = new CCImage();
		if(NULL == pImage)
			return NULL;

		bool bRet = pImage->initWithImageFile(fullpath.c_str(), eImageFormat);
		if (!bRet)
			SAFE_DELETE(pImage);

		return pImage;
	}

	unsigned char* C3DTexture3D::getImageData(cocos2d::CCImage* image, cocos2d::CCTexture2DPixelFormat&    pixelFormat)
	{
		unsigned char*            tempData = image->getData();
		unsigned int*             inPixel32  = NULL;
		unsigned char*            inPixel8 = NULL;
		unsigned short*           outPixel16 = NULL;
		bool                      hasAlpha = image->hasAlpha();
		CCSize                    imageSize = CCSizeMake((float)(image->getWidth()), (float)(image->getHeight()));
		
		size_t                    bpp = image->getBitsPerComponent();

		int width = image->getWidth();
		int height = image->getHeight();
		// compute pixel format
		if (hasAlpha)
		{
			pixelFormat = kCCTexture2DPixelFormat_Default;
		}
		else
		{
			if (bpp >= 8)
			{
				pixelFormat = kCCTexture2DPixelFormat_RGB888;
			}
			else 
			{
				pixelFormat = kCCTexture2DPixelFormat_RGB565;
			}

		}

		// Repack the pixel data into the right format
		unsigned int length = width * height;

		if (pixelFormat == kCCTexture2DPixelFormat_RGB565)
		{
			if (hasAlpha)
			{
				// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"

				tempData = new unsigned char[width * height * 2];
				outPixel16 = (unsigned short*)tempData;
				inPixel32 = (unsigned int*)image->getData();

				for(unsigned int i = 0; i < length; ++i, ++inPixel32)
				{
					*outPixel16++ = 
						((((*inPixel32 >>  0) & 0xFF) >> 3) << 11) |  // R
						((((*inPixel32 >>  8) & 0xFF) >> 2) << 5)  |  // G
						((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);    // B
				}
			}
			else 
			{
				// Convert "RRRRRRRRRGGGGGGGGBBBBBBBB" to "RRRRRGGGGGGBBBBB"

				tempData = new unsigned char[width * height * 2];
				outPixel16 = (unsigned short*)tempData;
				inPixel8 = (unsigned char*)image->getData();

				for(unsigned int i = 0; i < length; ++i)
				{
					*outPixel16++ = 
						(((*inPixel8++ & 0xFF) >> 3) << 11) |  // R
						(((*inPixel8++ & 0xFF) >> 2) << 5)  |  // G
						(((*inPixel8++ & 0xFF) >> 3) << 0);    // B
				}
			}    
		}
		else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444)
		{
			// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"

			inPixel32 = (unsigned int*)image->getData();  
			tempData = new unsigned char[width * height * 2];
			outPixel16 = (unsigned short*)tempData;

			for(unsigned int i = 0; i < length; ++i, ++inPixel32)
			{
				*outPixel16++ = 
					((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
					((((*inPixel32 >> 8) & 0xFF) >> 4) <<  8) | // G
					((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
					((((*inPixel32 >> 24) & 0xFF) >> 4) << 0);  // A
			}
		}
		else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1)
		{
			// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
			inPixel32 = (unsigned int*)image->getData();   
			tempData = new unsigned char[width * height * 2];
			outPixel16 = (unsigned short*)tempData;

			for(unsigned int i = 0; i < length; ++i, ++inPixel32)
			{
				*outPixel16++ = 
					((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
					((((*inPixel32 >> 8) & 0xFF) >> 3) <<  6) | // G
					((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
					((((*inPixel32 >> 24) & 0xFF) >> 7) << 0);  // A
			}
		}
		else if (pixelFormat == kCCTexture2DPixelFormat_A8)
		{
			// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "AAAAAAAA"
			inPixel32 = (unsigned int*)image->getData();
			tempData = new unsigned char[width * height];
			unsigned char *outPixel8 = tempData;

			for(unsigned int i = 0; i < length; ++i, ++inPixel32)
			{
				*outPixel8++ = (*inPixel32 >> 24) & 0xFF;  // A
			}
		}

		if (hasAlpha && pixelFormat == kCCTexture2DPixelFormat_RGB888)
		{
			// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRGGGGGGGGBBBBBBBB"
			inPixel32 = (unsigned int*)image->getData();
			tempData = new unsigned char[width * height * 3];
			unsigned char *outPixel8 = tempData;

			for(unsigned int i = 0; i < length; ++i, ++inPixel32)
			{
				*outPixel8++ = (*inPixel32 >> 0) & 0xFF; // R
				*outPixel8++ = (*inPixel32 >> 8) & 0xFF; // G
				*outPixel8++ = (*inPixel32 >> 16) & 0xFF; // B
			}
		}
		return tempData;
	}

	C3DTexture3D* C3DTexture3D::create(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4, const std::string& path5, const std::string& path6)
	{
		//CCAssert(path1 && path2 && path3 && path4 && path5 && path6, "6 textures needed");
		C3DTexture3D* texture = new C3DTexture3D();
		if (texture == NULL)
			return NULL;
		texture->_image[0] = createImage(path1);
		texture->_image[1] = createImage(path2);
		texture->_image[2] = createImage(path3);
		texture->_image[3] = createImage(path4);
		texture->_image[4] = createImage(path5);
		texture->_image[5] = createImage(path6);

        glGenTextures(1, &texture->_handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->_handle);
		
		for (int i=0; i<6; i++) {
			CCImage* image = texture->_image[i];
			
			CCTexture2DPixelFormat    pixelFormat;
			unsigned char* data = getImageData(image, pixelFormat);
			if (pixelFormat == kCCTexture2DPixelFormat_RGBA8888)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,                  //level
                             GL_RGBA,            //internal format
                             image->getWidth(),                 //width
                             image->getHeight(),                 //height
                             0,                  //border
                             GL_RGBA,             //format
                             GL_UNSIGNED_BYTE,   //type
                             data); // pixel data
			else if (pixelFormat == kCCTexture2DPixelFormat_RGB888)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,                  //level
                             GL_RGB,            //internal format
                             image->getWidth(),                 //width
                             image->getHeight(),                 //height
                             0,                  //border
                             GL_RGB,             //format
                             GL_UNSIGNED_BYTE,   //type
                             data); // pixel data
			}
			if (data != image->getData())
                delete[] data;
            
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,  
                        GL_LINEAR);  
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,  
                        GL_LINEAR);  
        
		
		return texture;
	}
    
    
    
    
}
