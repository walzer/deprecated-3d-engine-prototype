#include "PEVortex.h"
#include "C3DMaterial.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "MaterialParameter.h"
#include "C3DTexture.h"

namespace cocos3d
{
PEVortex::PEVortex(C3DPostProcess* postProcess, const std::string& name)
	: C3DPostEffect( postProcess, name )
	, _angle( 0.f )
	, _radius( 0.f )
	, _paramRadus( NULL )
	, _paramAngle( NULL )
	, _angleSpeed(1.5)
	, _radiusSpeed(0.5)
	, _Time( 0.0 )
	, _maxTime( 2.0 )
{
}

PEVortex::~PEVortex(void)
{
}

C3DPostEffect* PEVortex::create( const std::string& name, const std::string& materialName, C3DPostProcess* postProcess )
{
	C3DPostEffect* pe = new PEVortex( postProcess, name );
	bool bInit = pe->init( materialName );
	if (!bInit)
	{
		SAFE_RELEASE(pe);
	}
	return pe;
}
bool PEVortex::init(const std::string& szMaterial)
{
	if ( !C3DPostEffect::init(szMaterial) )
	{
		return false;
	}

	C3DTechnique* tech = _material->getTechnique(0u);
	if ( tech == NULL )
	{
		return false;
	}

	C3DPass* pass = tech->getPass(0u);
	if ( pass == NULL )
	{
		return false;
	}
	_paramRadus = pass->getParameter( "radius" );
	_paramAngle = pass->getParameter( "angle" );

	return _paramRadus!=NULL && _paramAngle != NULL;
}

void PEVortex::update( float dt )
{
	if ( _Time < _maxTime )
	{
		setAngle( getAngel()+_angleSpeed*dt );
		setRadius( getRadius()+_radiusSpeed*dt );
		_Time += dt;
	}
	else
	{
		_Time = 0.0;
		setAngle( 0.0f );
		setRadius( 0.0f );
	}
}

void PEVortex::setShaderParameter()
{
	if ( _paramRadus != NULL )
	{
		_paramRadus->setValue( _radius );
	}

	if ( _paramAngle != NULL )
	{
		_paramAngle->setValue( _angle );
	}
}
}	//namespace cocos3d