#ifndef DEPTHSTENCILTARGET_H_
#define DEPTHSTENCILTARGET_H_

#include "Base.h"
#include "C3DTexture.h"

namespace cocos3d
{

/**
 * Defines a contain for depth and stencil targets in a frame buffer object.
 *
 * This class assumes that the target hardware supports depth textures, since
 * creation of a C3DDepthStencilTarget always attempts to create an underlying
 * depth texture.
 */
class C3DDepthStencilTarget : public C3DTexture
{
    friend class C3DFrameBuffer;

public:

    /**
     * Defines the accepted formats for DepthStencilTargets.
     */
    enum Format
    {
        /**
         * A target with 24-bits of depth data.
         *
         * This format may be internally stored as a 32-bit buffer with 8 bits of unused data.
         */
        DEPTH16 = 1,

        /**
         * A target with 24 bits of depth data and 8 bits stencil data.
         */
        DEPTH24_STENCIL8
    };

	/**
     * create DepthStencilTarget
     */
    static C3DDepthStencilTarget* create(const std::string& id, Format format, unsigned int width, unsigned int height);
    static C3DDepthStencilTarget* getDepthStencilTarget(const std::string& id);

	/**
     * id string
     */
    const std::string& getID() const;

	/**
     * DepthStencilTarget format.
     */
    Format getFormat() const;

	/**
     * Texture associate to DepthStencilTarge.
     */
	virtual void reload();

private:
	/**
     * Constructor & Destructor.
     */
    C3DDepthStencilTarget(const std::string& id, Format format);
    ~C3DDepthStencilTarget();

	//properties
    std::string _id;
    Format _format;
    RenderBufferHandle _stencilBuffer;
};
}

#endif
