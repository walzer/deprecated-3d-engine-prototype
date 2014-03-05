#include "PEOutLine.h"

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
PEOutLine::~PEOutLine()
{
}

C3DPostEffect* PEOutLine::create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess )
{
	C3DPostEffect* pe = new PEOutLine( postProcess, name );
	bool bInit = pe->init( materialName );
	if (!bInit)
	{
		SAFE_RELEASE(pe);
	}
	return pe;
}

void PEOutLine::setShaderParameter()
{
	_material->getTechnique(0u)->getPass(0u)->getParameter("u_w")->setValue(2.0f/(float)_postProcess->getFBWidth());
	_material->getTechnique(0u)->getPass(0u)->getParameter("u_h")->setValue(2.0f/(float)_postProcess->getFBHeight());
}
}