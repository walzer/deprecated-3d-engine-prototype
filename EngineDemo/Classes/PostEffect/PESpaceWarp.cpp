#include "PESpaceWarp.h"
#include "C3DPostProcess.h"
#include "C3DMaterial.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "MaterialParameter.h"
#include "C3DTexture.h"
#include "C3DSampler.h"
#include "C3DFrameBuffer.h"
#include "C3DModel.h"
#include "C3DRenderTarget.h"
#include "C3DFrameBuffer.h"
#include "MaterialParameter.h"
#include "C3DRenderSystem.h"

namespace cocos3d
{
PESpaceWrap::PESpaceWrap(C3DPostProcess* postProcess, const std::string& name)
	: C3DPostEffect(postProcess,name)
	, _time( 0.0 )
{
	_pixelSize = C3DVector2(1.f / 1024.f, 1.f / 768.f);
	_wrapSpeed = 1.5f;
	_wrapScale = 0.05f;

	int w = 256, h = 256;

	unsigned int* data = new unsigned int[w * h];
    
	for(int y = 0; y < h; y++)
	{
		for(int x = 0; x < w; x++)
		{
			float fx = x * (1.0f / 256) - 0.5f;
			float fy = y * (1.0f / 256) - 0.5f;

			int iDu = (int)(32 * cosf(4.0f * (fx + fy) * MATH_PI)) + 128;
			int iDv = (int)(32 * sinf(4.0f * (fx + fy) * MATH_PI)) + 128;

			data[y * w + x] = iDu << 16 | iDv << 8 | 0xff << 24;
		}
	}
	C3DTexture* texture = C3DTexture::create(w, h, C3DTexture::RGBA, data, sizeof(unsigned int)*h*w, false);
	_wrapSampler = new C3DSampler(texture);
	_wrapSampler->setWrapMode(Texture_Wrap_REPEAT, Texture_Wrap_REPEAT);

	delete[] data;
}

PESpaceWrap::~PESpaceWrap()
{
	SAFE_RELEASE(_wrapSampler);
}

C3DPostEffect* PESpaceWrap::create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess )
{
	C3DPostEffect* pe = new PESpaceWrap( postProcess, name );
	bool bInit = pe->init( materialName );
	if (!bInit)
	{
		SAFE_RELEASE(pe);
	}
	return pe;
}

void PESpaceWrap::setShaderParameter()
{
	static C3DVector2 pixelsize(1.f / 1024.f, 1.f / 768.f);

    C3DPass* pass = _material->getTechnique(0u)->getPass(0u);

	pass->getParameter("g_vPixelSize")->setValue(pixelsize);
	pass->getParameter("g_fWarpScale")->setValue(_wrapScale);
	pass->getParameter("g_fWarpSpeed")->setValue(_wrapSpeed);
	pass->getParameter("u_pertubeSampler")->setValue(_wrapSampler);
	pass->getParameter("g_fTime")->setValue(_time);
}
void PESpaceWrap::update( float dt )
{
	_time += dt;
}
}