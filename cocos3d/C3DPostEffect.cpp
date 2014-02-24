#include "C3DPostEffect.h"
#include "C3DPostProcess.h"

#include "C3DMaterial.h"
#include "C3DRenderState.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "MaterialParameter.h"

#include "Base.h"
#include "C3DVector3.h"
#include "C3DTexture.h"
#include "C3DSampler.h"
#include "C3DFrameBuffer.h"
#include "C3DMaterial.h"
#include "C3DModel.h"
#include "C3DSkinlessModel.h"
#include "Geo.h"
#include "C3DDepthStencilTarget.h"
#include "C3DRenderTarget.h"

#include "C3DRenderSystem.h"
#include "C3DMesh.h"

#include "C3DMaterialManager.h"

namespace cocos3d
{
    
C3DPostEffect::C3DPostEffect(C3DPostProcess* postProcess, const std::string& szName)
	: _model( NULL )
{
    _name = szName;
    
    _postProcess = postProcess;
    //_postProcess->retain();
    _material = NULL;
    
}
    
bool C3DPostEffect::init(const std::string& szMaterial)
{
    SAFE_RELEASE(_material);
    //_material = C3DMaterial::create(szMaterial);
	_material = static_cast<C3DMaterial*>(C3DMaterialManager::getInstance()->getResource(szMaterial));
    if (_material == NULL)
        return false;
    
    _material->retain();
    C3DTechnique* technique = _material->getTechnique(0u);
    
	technique->getPass(0u)->getParameter("u_texture")->setValue(_postProcess->getFramebufferSampler());


	C3DMesh* mesh = Geo::createQuadFullscreen( 1, 1 );
	_model = C3DSkinlessModel::create();
	_model->setMesh( mesh );
	SAFE_RELEASE(mesh);


	_model->setNode(_postProcess);

    return true;
}

void C3DPostEffect::setGridSize( unsigned int x, unsigned int y )
{
	C3DMesh* mesh = Geo::createQuadFullscreen( x, y );
	_model->setMesh( mesh );
	SAFE_RELEASE(mesh);
}

C3DPostEffect::~C3DPostEffect()
{
	SAFE_RELEASE(_model);
    SAFE_RELEASE(_material);
	//SAFE_RELEASE(_postProcess);
}

    
void C3DPostEffect::draw()
{
    if (_material == NULL)
        return;
    
    if( _model->getMaterial() != _material)
    {
        _model->setMaterial(_material);
    }
    
    setShaderParameter();
    
    _model->draw();
}    



}