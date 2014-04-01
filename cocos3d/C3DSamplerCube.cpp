#include "Base.h"
#include "C3DSamplerCube.h"
#include "C3DTexture3D.h"
#include "C3DStream.h"
#include "C3DElementNode.h"

namespace cocos3d
{
C3DSamplerCube::C3DSamplerCube():_texture(NULL)
{
}

C3DSamplerCube::C3DSamplerCube(C3DTexture3D* texture)
{
	_texture = texture;
	texture->retain();
}

C3DSamplerCube::C3DSamplerCube(const std::string& path_left,const std::string& path_right,const std::string& path_top,const std::string& path_bottom,const std::string& path_front,const std::string& path_back)
:_path_left(path_left)
,_path_right(path_right)
,_path_top(path_top)
,_path_bottom(path_bottom)
,_path_front(path_front)
,_path_back(path_back)
{
	_texture = C3DTexture3D::create(_path_left, _path_right, _path_top, _path_bottom, _path_front, _path_back);
}

C3DSamplerCube::~C3DSamplerCube()
{
	CC_SAFE_RELEASE(_texture);
}

bool C3DSamplerCube::isSupport()
{
	return cocos2d::CCConfiguration::sharedConfiguration()->checkForGLExtension("GL_OES_texture_cube_map");
}

/**
* Binds the texture of this sampler to the renderer and applies the sampler state.
*/
void C3DSamplerCube::bind()
{
	if(_texture == NULL)
		return;

    //glEnable(GL_TEXTURE_CUBE_MAP);

	GL_ASSERT( glBindTexture(GL_TEXTURE_CUBE_MAP, _texture->getHandle()) );

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void C3DSamplerCube::reload()
{

}

bool C3DSamplerCube::load(C3DElementNode* node)
{
	// Read the texture uniform name
	const std::string& name = node->getNodeName();
	if (name.empty())
		return false; // missing texture uniform name

#define GET_PATH(valRef, pathName)\
	valRef = node->getElement(#pathName);\
	if(valRef.empty())\
		return false;

	// Get the texture path
	GET_PATH(_path_left, path_left);
	GET_PATH(_path_right, path_right);
	GET_PATH(_path_top, path_top);
	GET_PATH(_path_bottom, path_bottom);
	GET_PATH(_path_front, path_front);
	GET_PATH(_path_back, path_back);

	SAFE_RELEASE(_texture);
	_texture = C3DTexture3D::create(_path_left, _path_right, _path_top, _path_bottom, _path_front, _path_back);
	if (_texture == NULL)
		return false;

	return true;
}

bool C3DSamplerCube::save(C3DElementNode* node)
{
	node->setElement("path_left", _path_left);
    node->setElement("path_right", _path_right);
    node->setElement("path_top", _path_top);
    node->setElement("path_bottom", _path_bottom);
    node->setElement("path_front", _path_front);
    node->setElement("path_back", _path_back);
	return true;
}

}