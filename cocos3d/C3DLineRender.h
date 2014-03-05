#ifndef C3DLINERENDER_H
#define C3DLINERENDER_H

#include "C3DNode.h"

namespace cocos3d
{
class C3DNode;
class C3DMaterial;
class C3DBatchModel;
class C3DBatchMesh;
class C3DPerlinNoise;

class C3DLineRender : public C3DNode
{
public:
	enum LineType
	{
		LT_Direct,		// 直线
		LT_Random,		// 随机抖动
		LT_PerlinNosie,	// 柏林噪声曲线
	};

	enum StepType
	{
		ST_Begin,
		ST_End,
		ST_Other,
	};

public:
	struct Line
	{
		Line( C3DVector3 lBegin, C3DVector3 lEnd )
			:lineBegin( lBegin )
			,lineEnd( lEnd )
			,beginOffset( 0.0 )
		{
		}
		Line( C3DVector3 lBegin, C3DVector3 lEnd, float offset )
			:lineBegin( lBegin )
			,lineEnd( lEnd )
			,beginOffset( offset )
		{
		}

		C3DVector3 lineBegin;
		C3DVector3 lineEnd;
		float beginOffset;
	};

public:
	C3DLineRender(const std::string& materialPath);
	~C3DLineRender(void);

	// create sprite and add it to autorelease pool
	static C3DLineRender* create(const std::string& materialPath);

	void update(long elapsedTime);

	void draw();

	const std::vector<Line>& getLines() const
	{
		return _lines;
	}

	void setLines( const std::vector<Line>& lines );
	void setWidth( float f )
	{
		_width = f;
	}
	float getWidth(void) const
	{
		return _width;
	}
	void setStep( float f )
	{
		_step = f;
	}
	float getStep(void) const
	{
		return _step;
	}
	void setTextueLength( float f )
	{
		_textureLength = f;
	}
	float getTextureLength(void) const
	{
		return _textureLength;
	}
	void setAmplitude0( float f )
	{
		_amplitude0 = f;
	}
	float getAmplitude0(void)
	{
		return _amplitude0;
	}
	void setAmplitude1( float f )
	{
		_amplitude1 = f;
	}
	float getAmplitude1(void)
	{
		return _amplitude1;
	}
	void setFrequency0( float f )
	{
		_frequency0 = f;
	}
	float getFrequency0(void)
	{
		return _frequency0;
	}
	void setFrequency1( float f )
	{
		_frequency1 = f;
	}
	float getFrequency1(void)
	{
		return _frequency1;
	}
	void setForceTexLoop( bool b )
	{
		_forceTexLoop = b;
	}

	bool getForceTexLoop(void) const
	{
		return _forceTexLoop;
	}

	void setTimeFactor( float f )
	{
		_timeFactor = f;
	}

	float getTimeFactor(void) const
	{
		return _timeFactor;
	}

	void setAttenuation( float f )
	{
		_attenuation = f;
	}
	float getAttenuation(void) const
	{
		return _attenuation;
	}

	void setLineType( LineType lineType )
	{
		_lineType = lineType;
	}
	LineType getLineType(void)
	{
		return _lineType;
	}

	void setColor( const C3DVector4& color )
	{
		_color = color;
	}
	C3DVector4 getColor(void)
	{
		return _color;
	}

	void setForceUpdate( bool b )
	{
		_forceUpdate = b;
	}

	bool getForceUpdate(void) const
	{
		return _forceUpdate;
	}

private:
	void init( const std::string& materialPath );
	void updateMesh(void);

	void addOneStep( C3DBatchModel* mesh, C3DVector3 lineBegin, C3DVector3 lineEnd,
							C3DVector3 widthOffset/*One, C3DVector3 widthOffsetTwo,
							float halfWidth*/, float uvBegin, float uvEnd, StepType stepType, C3DVector4 color );

	C3DVector3 getCenter(void);

private:

	bool _forceTexLoop;	// 是否强制纹理循环
	bool _dirty;
	bool _forceUpdate;

	float _time;	// 秒

	float _width;	// 宽
	float _step;	// 单个面片网格长，越小曲线越平滑，数值过于小可能带来效率问题
	float _textureLength;	// 单张纹理长度，调整这个数值可以避免纹理过度拉伸或挤压
	float _amplitude0;	// 振幅
	float _amplitude1;	// 振幅
	float _frequency0;	// 频率
	float _frequency1;	// 频率
	float _timeFactor;

	float _attenuation;	// 头尾振幅衰减，距离起止点的_attenuation距离内带收敛为三角

	C3DVector4 _color;	// 颜色，具体作用为 _color*纹理颜色

	LineType _lineType;	// 类型

	std::vector<Line> _lines;

	//C3DMaterial* _material;
	C3DBatchModel* _model;
	static C3DPerlinNoise _noise;
};
}

#endif	//#define C3DLINERENDER_H