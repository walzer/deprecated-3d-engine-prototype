#ifndef RENDERNODE_H_
#define RENDERNODE_H_

#include <map>
#include <string>

#include "C3DMatrix.h"
#include "C3DVector3.h"

#include "C3DNode.h"
#include "cocos2d.h"

#include "C3DOBB.h"
#include "C3DBone.h"

#include "C3DCollitionBox.h"
#include "C3DResource.h"

using namespace std;
namespace cocos3d
{
class C3DScene;
class C3DNode;
class C3DBatchMesh;
class C3DAABB;
class C3DMaterial;
class C3DRay;
class AttachNode;
class C3DResourceLoader;
/**
*Defines the bass class for all scene objects which includes static object,dynamic object,and so on.
*/
class  C3DRenderNode : public C3DNode, public C3DResource
{
	friend class C3DScene;
	friend class C3DStaticObj;
	friend class C3DSprite;

public:

	C3DRenderNode(const std::string& id);
    ~C3DRenderNode();

	/**
    * load sprite from file.
    *
    * @param fileName sprite filename.
    */
	bool loadFromFile(const std::string& fileName, bool isLoadAll = false);

	virtual bool load(C3DResourceLoader* loader, bool isLoadAll = false){ return false; };
	virtual void reload();

	/**
    * Render for handling rendering routines.
    */
	virtual void draw();

	virtual void drawDebug();
	/**
     * Update routine
     */
    virtual void update(long elapsedTime);

	AttachNode * attachNode( const std::string & nodeName );
	void attach(const std::string& nodeName,C3DNode* attachment);
	void detach(const std::string& nodeName,C3DNode* attachment);

	void drawCollitionBox();

	void transformChanged();

	void showCollitionBox(bool show);
	bool showCollitionBox();

	void showWireFrame(bool _show);

	void showSkeleton(bool _show){ _showSkeleton = _show;}
	bool showSkeleton(){return _showSkeleton;}

	virtual C3DMaterial* getMaterial(const std::string& meshName);
	virtual void setMaterial(const std::string& modelName, const std::string& matName);
    virtual void setMaterial(const std::string& matName);
	virtual void setMaterial(C3DMaterial* material);

	void setDefaultMaterial(const std::string& path);

	virtual void removeMaterial(const std::string& modelName, const std::string& matName);
	virtual void removeMaterial(const std::string& matName);

	void getAllModel(std::list<C3DNode*>& models);

    unsigned int getTriangleCount() const;

	unsigned int getValidCollitionBoxID();
	unsigned int addCollitionBox(const std::string& strAttachBone, const C3DVector3& offset = C3DVector3::zero(), float fRadius = 1.f);
	void removeCollitionBox(C3DCollitionBox* bb);
	const std::vector<C3DCollitionBox*>& getCollitionBoxs()const;
	C3DCollitionBox* pickCollitionBox(const C3DRay *ray);
	bool saveCollitionBox(const std::string& fileName)const;
	bool loadCollitionBox(const std::string& fileName);

	virtual C3DNode* clone(CloneContext& context) const{ return NULL; }

protected:

	virtual void copyFrom(const C3DTransform* other, C3DNode::CloneContext& context);

	AttachNode* accessNode( C3DNode* pNode);

protected:

	std::vector<AttachNode*> _attachNodes;

	std::vector<C3DCollitionBox*> m_collitionBoxs;// collision box list

    bool _showSkeleton;
	bool _showCollitionBox;
    bool _castShadowMap;

	std::string _fileName;
	//C3DResourceLoader* _loader;

    bool _isVisibleByCamera;
};
}

#endif
