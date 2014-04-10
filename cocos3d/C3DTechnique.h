#ifndef TECHNIQUE_H_
#define TECHNIQUE_H_

#include<vector>
#include "C3DRenderState.h"

namespace cocos3d
{
class C3DMaterial;
class C3DRenderChannel;

/**
 * Defines a technique for an model to be rendered.
 *
 * one technique contains several pass for render an model.
 * when multiple pass are loaded using a material file,
 * the current pass can be set at runtime.
 */
class C3DTechnique : public C3DRenderState
{
    friend class C3DMaterial;
    friend class C3DPass;
    friend class C3DRenderState;

public:

	C3DTechnique();

	C3DTechnique(const std::string& id, C3DMaterial* material);

    ~C3DTechnique();

    const std::string& getId() const;

    unsigned int getPassCount() const;

    C3DPass* getPass(unsigned int index) const;

    C3DPass* getPass(const std::string& id) const;

	void setChannel(const std::string& channelName);
	C3DRenderChannel* getChannel();
	C3DTechnique* clone() const;

	 /**
     * load info from the elementnode
     *
     * @param nodes elementnode which contains the renderstate info.
     *
     */
	virtual bool load(C3DElementNode* node);

	 /**
     * save the technique info into the elementnode
     *
     * @param nodes elementnode which contains the renderstate info.
     *
     */
	virtual bool save(C3DElementNode* node);

	virtual void reload();

private:

    std::string _id;
    C3DMaterial* _material;
    std::vector<C3DPass*> _passes;

	std::string _renderChannelName;
	//C3DRenderChannel* _renderChannel;
};
}

#endif
