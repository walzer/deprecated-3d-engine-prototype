#include "Vertex.h"
#include "C3DLineRender.h"

#include "C3DVertexFormat.h"
#include "C3DBatchMesh.h"
#include "C3DBatchModel.h"
#include "C3DMaterial.h"
#include "C3DRandom.h"
#include "C3DScene.h"
#include "C3DCamera.h"
#include "C3DNoise.h"
#include "Base.h"
#include "C3DMaterialManager.h"

namespace cocos3d
{
C3DPerlinNoise C3DLineRender::_noise = C3DPerlinNoise( -1.0 );

C3DLineRender::C3DLineRender(const std::string& materialPath)
	: _forceTexLoop( false )
	, _time(0.0)
	, _width(1.0 )
	, _step( 2.0 )
	, _textureLength( 2.0 )
	, _amplitude0( 2.0 )
	, _amplitude1( 2.0 )
	, _frequency0( 2 )
	, _frequency1( 100 )
	, _model( NULL )
	, _timeFactor( 1.0 )
	, _attenuation( 0.0 )
	, _lineType( LT_PerlinNosie )
	, _color( C3DVector4::one() )
	, _dirty( true )
	, _forceUpdate( true )
{
	_lines.clear();

	init( materialPath );
}

C3DLineRender::~C3DLineRender(void)
{
	CC_SAFE_DELETE(_model);
}

C3DLineRender* C3DLineRender::create(const std::string& materialPath)
{
	C3DLineRender* pRet = new C3DLineRender(materialPath);

	pRet->autorelease();
	return pRet;
}

void C3DLineRender::init( const std::string& materialPath )
{
	C3DVertexElement elements[] =
	{
		C3DVertexElement(Vertex_Usage_POSITION, 3),
		C3DVertexElement(Vertex_Usage_COLOR, 4),
		C3DVertexElement(Vertex_Usage_TEXCOORD0, 2)
	};
	C3DVertexFormat vertexformat = C3DVertexFormat(elements, 3);

	C3DBatchMesh* mesh = new C3DBatchMesh(&vertexformat, PrimitiveType_TRIANGLES, true, 512);

	_model = new C3DBatchModel(this);
	_model->setMesh(mesh);

	C3DMaterial* material = static_cast<C3DMaterial*>(C3DMaterialManager::getInstance()->getResource(materialPath));
	_model->setMaterial( material );
}

void C3DLineRender::update(long elapsedTime)
{
	C3DNode::update( elapsedTime );

	float timeSecond = (float)elapsedTime *0.001;
	_time += timeSecond;
}

void C3DLineRender::setLines( const std::vector<Line>& lines )
{
	if ( !_lines.empty() )
	{
		_lines.clear();
		_lines.reserve( lines.size() );
	}
	_lines.insert( _lines.begin(), lines.begin(), lines.end() );

	C3DVector3 center = getCenter();
	setPosition( center );

	_dirty = true;
}

void C3DLineRender::addOneStep( C3DBatchModel* model,
					C3DVector3 lineBegin, C3DVector3 lineEnd,
					C3DVector3 widthOffset,
					float uvBegin, float uvEnd, StepType stepType, C3DVector4 color )
{
	static const int VertexCount = 4;
	static const int IndexCount = 6;
	//static VertexPostionUV vertex[VertexCount];
	static unsigned short index[IndexCount] = {	0, 1, 2,
		0, 2, 3,
		/*4, 5, 7,
		5, 6, 7*/ };
	//static C3DVector2 uv[VertexCount] = { C3DVector2( 0, 0 ),
	//	C3DVector2( 1, 0 ),
	//	C3DVector2( 1, 1 ),
	//	C3DVector2( 0, 1 ),};

	static VertexColorCoord1 vertex[VertexCount];

	switch ( stepType )
	{
	case ST_Begin:
		{
			vertex[0].position = lineBegin;
			vertex[1].position = lineEnd	+ widthOffset;
			vertex[2].position = lineEnd	- widthOffset;
			vertex[3].position = lineBegin;

			vertex[0].u = uvBegin;
			vertex[1].u = uvEnd;
			vertex[2].u = uvEnd;
			vertex[3].u = uvBegin;

			vertex[0].v = 0.5;
			vertex[1].v = 0;
			vertex[2].v = 1;
			vertex[3].v = 0.5;

			vertex[0].color = color;
			vertex[1].color = color;
			vertex[2].color = color;
			vertex[3].color = color;
		}break;
	case  ST_End:
		{
			vertex[0].position = lineBegin	+ widthOffset;
			vertex[1].position = lineEnd;
			vertex[2].position = lineEnd;
			vertex[3].position = lineBegin	- widthOffset;

			vertex[0].u = uvBegin;
			vertex[1].u = uvEnd;
			vertex[2].u = uvEnd;
			vertex[3].u = uvBegin;

			vertex[0].v = 0;
			vertex[1].v = 0.5;
			vertex[2].v = 0.5;
			vertex[3].v = 1;

			vertex[0].color = color;
			vertex[1].color = color;
			vertex[2].color = color;
			vertex[3].color = color;
		}break;
	case  ST_Other:
		{
			vertex[0].position = lineBegin	+ widthOffset;
			vertex[1].position = lineEnd	+ widthOffset;
			vertex[2].position = lineEnd	- widthOffset;
			vertex[3].position = lineBegin	- widthOffset;

			vertex[0].u = uvBegin;
			vertex[1].u = uvEnd;
			vertex[2].u = uvEnd;
			vertex[3].u = uvBegin;

			vertex[0].v = 0;
			vertex[1].v = 0;
			vertex[2].v = 1;
			vertex[3].v = 1;

			vertex[0].color = color;
			vertex[1].color = color;
			vertex[2].color = color;
			vertex[3].color = color;
		}break;
	}

	model->add((unsigned char*)vertex, VertexCount, index, IndexCount);
}

void C3DLineRender::updateMesh(void)
{
	// 每段线，每个三角形
	for (unsigned int i = 0; i < _lines.size(); ++i )
	{
		float halfWidth = _width*0.5;
		float inverseTextueLength = (_textureLength!=0)?(1/_textureLength):1.0;

		C3DVector3 lineBegin( _lines[i].lineBegin );
		C3DVector3 lineEnd( _lines[i].lineEnd );
		float beginOffset( _lines[i].beginOffset );
		C3DVector3 dir = lineEnd - lineBegin;
		float totalLength = dir.length();
		dir.normalize();
		float invTotalLength = (totalLength!=0)?(1/totalLength):1.0;

		float step(_step);
		float stepCount = (totalLength<=_step*2)?2:ceil(totalLength/step);
		step = totalLength/stepCount;

		// 计算展开朝向，展开朝向应该总是尽量向着相机
		C3DVector3 widthOffset( C3DVector3::zero() );
		C3DCamera* camera = _scene->getActiveCamera();
		if ( camera != NULL )
		{
			C3DVector3 camForward = camera->getForwardVector();
			camForward.normalize();

			widthOffset = dir;
			widthOffset.cross( camForward );
			widthOffset.normalize();
		}

		C3DVector3 stepBegin(lineBegin);
		C3DVector3 stepEnd(lineEnd);

		float curLength(0.0);

		for ( int j = 0; j < stepCount-1; ++j )
		{
			C3DVector3 randomPos( C3DVector3::zero() );

			switch ( _lineType )
			{
			case LT_Direct:
				{
				}break;
			case  LT_Random:
				{
					randomPos = C3DVector3( _amplitude0, _amplitude0, _amplitude0 );
					randomPos.x *= RandomFloat();
					randomPos.y *= RandomFloat();
					randomPos.z *= RandomFloat();
				}break;
			case  LT_PerlinNosie:
				{
					float noiseParam = ( _time*_timeFactor + curLength*invTotalLength + beginOffset );
					float noise = _noise.generateNoise1D( noiseParam*_frequency0 )*_amplitude0;
					noise += _noise.generateNoise1D( noiseParam*_frequency1 )*_amplitude1;

					float attenuationDis = C3D_Min(curLength, totalLength-curLength);

					if ( attenuationDis < _attenuation )
					{
						noise *= pow( (attenuationDis/_attenuation), 4);
					}

					randomPos = widthOffset*noise;
				}break;
			}

			stepEnd = lineBegin+dir*(curLength+step)+randomPos;

			addOneStep( _model, (curLength==0)?lineBegin:stepBegin, stepEnd,
				widthOffset*halfWidth,
				curLength*inverseTextueLength-_time, (curLength+step)*inverseTextueLength-_time,
				(curLength==0)?ST_Begin:ST_Other, _color );

			//lastLength = curLength;
			curLength += step;
			stepBegin = stepEnd;
		}

		//尾
		float uvBegin( curLength*inverseTextueLength );
		float uvEnd( totalLength*inverseTextueLength );

		if ( _forceTexLoop )
		{
			uvEnd = ceil( uvEnd );
		}
		addOneStep( _model, stepBegin, lineEnd,
			widthOffset*halfWidth,
			uvBegin-_time, uvEnd-_time, ST_End, _color );
	}

	_dirty = false;
}

void C3DLineRender::draw()
{
	/*if ( _model != NULL )
	{
		if ( _forceUpdate || _dirty )
		{
			_model->clear();
			updateMesh();
		}
		_model->draw();
	}*/

	//.......
	if (_model)
	{
		if ( _forceUpdate || _dirty )
		{
			_model->clear();
			updateMesh();
		}

		C3DRenderChannel* channel = _model->getRenderChannel();
		if(channel != NULL)
		{
			C3DVector3 pos( C3DVector3::zero() );
	        this->getWorldViewMatrix().getPosition( &pos );

			channel->addItem( _model, pos.z );
		}
		else
		{
			_model->draw();
		}
	}

	//.......
}

C3DVector3 C3DLineRender::getCenter(void)
{
	C3DVector3 center(C3DVector3::zero());
	if ( !_lines.empty() )
	{
		for ( unsigned int i = 0; i < _lines.size(); ++i )
		{
			center += _lines[i].lineBegin;
			center += _lines[i].lineEnd;
		}

		center.scale( 0.5/_lines.size());
	}

	return center;
}
}	//namespace cocos3d