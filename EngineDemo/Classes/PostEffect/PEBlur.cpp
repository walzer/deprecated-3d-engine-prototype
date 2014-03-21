#include "PEBlur.h"

#include "C3DPostProcess.h"
#include "C3DMaterial.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "C3DFrameBuffer.h"
#include "C3DTexture.h"
#include "C3DDepthStencilTarget.h"
#include "C3DSampler.h"
#include "MaterialParameter.h"
#include "C3DRenderTarget.h"
#include "C3DModel.h"
#include "C3DRenderSystem.h"

namespace cocos3d
{
PEBlur::~PEBlur()
{
}

C3DPostEffect* PEBlur::create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess )
{
	C3DPostEffect* pe = new PEBlur( postProcess, name );
	bool bInit = pe->init( materialName );
	if (!bInit)
	{
		SAFE_RELEASE(pe);
	}
	return pe;
}

void PEBlur::setShaderParameter()
{
	_material->getTechnique(0u)->getPass(0u)->getParameter("u_length")->setValue(1.0f / ((float)_postProcess->getFBWidth() / 2.0f));
}

}