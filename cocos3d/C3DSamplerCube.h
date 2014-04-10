#ifndef SAMPLECUBE_H_
#define SAMPLECUBE_H_

#include "EnumDef_GL.h"
#include "cocos2d.h"

namespace cocos3d
{
class C3DTexture3D;
class C3DElementNode;

/**
* Defnies a texture sampler.
*
* A texture sampler is basically an instance of a texture that can be
* used to sample a texture from a material. In addition to the texture
* itself, a sampler stores per-instance texture state information, such
* as wrap and filter modes.
*/
class C3DSamplerCube : public cocos2d::CCObject
{
    friend class C3DTexture3D;

public:
	C3DSamplerCube();
	C3DSamplerCube(C3DTexture3D* texture);
	C3DSamplerCube(const std::string& path_left,const std::string& path_right,const std::string& path_top,const std::string& path_bottom,const std::string& path_front,const std::string& path_back);
    ~C3DSamplerCube();

    /**
        * Binds the texture of this sampler to the renderer and applies the sampler state.
        */
    void bind();

 	static bool isSupport();

	const std::string& getPathLeft() const{return _path_left;}
	const std::string& getPathRight() const{return _path_right;}
	const std::string& getPathTop() const{return _path_top;}
	const std::string& getPathBottom() const{return _path_bottom;}
	const std::string& getPathFront() const{return _path_front;}
	const std::string& getPathBack() const{return _path_back;}

	bool load(C3DElementNode* node);
	bool save(C3DElementNode* node);

	void reload();

private:

    C3DTexture3D* _texture;

    std::string _path_left;
    std::string _path_right;
    std::string _path_top;
    std::string _path_bottom;
    std::string _path_front;
    std::string _path_back;
};
}

#endif
