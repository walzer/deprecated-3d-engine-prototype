#include "Base.h"
#include "Geo.h"
#include "C3DGeoWireRender.h"
#include "C3DMesh.h"
#include "C3DVector4.h"

#include "C3DVertexFormat.h"
#include "Vertex.h"
#include "C3DAABB.h"
#include "C3DOBB.h"
#include "C3DBatchModel.h"
#include "C3DBatchMesh.h"
#include "C3DMaterial.h"
#include "C3DModel.h"
#include "C3DCylinder.h"
#include "C3DCollitionBox.h"
#include "C3DCapsule.h"
#include "C3DTechnique.h"

namespace cocos3d
{
C3DGeoWireRender::C3DGeoWireRender():_model_3D(NULL), _model_2D(NULL)
{
}
C3DGeoWireRender::~C3DGeoWireRender()
{
	delete (_model_3D);
	delete (_model_2D);
}

bool C3DGeoWireRender::init()
{
	if (_model_3D)
		delete (_model_3D);
	if (_model_2D)
		delete (_model_2D);

	_model_3D = C3DBatchModel::createDebugModel(this);

	return _model_3D != NULL;
}

void C3DGeoWireRender::addBoundingBox(const C3DAABB* box, const C3DVector4& color, const C3DMatrix* pMat)
{
	if (_model_3D)
	{
		static C3DVector3 corners[8];
		static unsigned short index[] = {0,1,  1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,7, 1,6, 2,5, 3,4};
		box->getCorners(corners);

		addLinesRenderData(corners, 8, index, 24, color, pMat);
	}
}

void C3DGeoWireRender::addOBB(const C3DOBB* obb,const C3DVector4& color, const C3DMatrix* pMat)
{
	if (_model_3D)
	{
		static C3DVector3 corners[8];
		static unsigned short index[] = {0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,4, 1,5, 2,6, 3,7};
		obb->getVertices(corners);

		addLinesRenderData(corners, 8, index, 24, color, pMat);
	}
}

void C3DGeoWireRender::add3DRect(const C3DVector3& v0, const C3DVector3& v1, const C3DVector3& v2, const C3DVector3& v3, const C3DVector4& color, const C3DMatrix* pMat)
{
	if (_model_3D)
	{
		static unsigned short index[8] = {0,1, 1,2, 2,3, 3,0};
		static C3DVector3 verts[4];
		verts[0] = v0;
		verts[1] = v1;
		verts[2] = v2;
		verts[3] = v3;
		addLinesRenderData(verts, 4, index, 8, color, pMat);
	}
}

void C3DGeoWireRender::add3DTriangle(const C3DVector3& v0, const C3DVector3& v1, const C3DVector3& v2, const C3DVector4& color, const C3DMatrix* pMat)
{
	if (_model_3D)
	{
		static unsigned short index[6] = {0,1, 1,2, 2,0};
		static C3DVector3 verts[3];
		verts[0] = v0;
		verts[1] = v1;
		verts[2] = v2;
		addLinesRenderData(verts, 3, index, 6, color, pMat);
	}
}

void C3DGeoWireRender::add3DLine(const C3DVector3& start, const C3DVector3& end, const C3DVector4& color, const C3DMatrix* pMat)
{
	if (_model_3D)
	{
		static unsigned short index[2] = {0,1};
		static C3DVector3 verts[2];
		verts[0] = start;
		verts[1] = end;

		addLinesRenderData(verts, 2, index, 2, color, pMat);
	}
}

void C3DGeoWireRender::addLinesRenderData(const C3DVector3* verts, int num, const unsigned short* index, int indexCount, const C3DVector4& color, const C3DMatrix* pMat)
{
	CCAssert(_model_3D, "invalid model");
	if (_model_3D)
	{
		BBVertex* bbvert = new BBVertex[num];
		for (int i = 0; i < num; i++)
		{
			bbvert[i].position = verts[i];
			bbvert[i].color = color;
		}
		if (pMat)
		{
			for (int k = 0; k < num; k++)
			{
				bbvert[k].position = (*pMat) * bbvert[k].position;
			}
		}
		_model_3D->add((unsigned char*)bbvert, num, index, indexCount);
		delete[] bbvert;
	}
}

void C3DGeoWireRender::addCollitionBox(const C3DCollitionBox* box, const C3DVector4& color, const C3DMatrix* pMat)
{
	if (_model_3D)
	{
		static C3DVector3 corners[8];
		static unsigned short index[] = {0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,7, 1,6, 2,5, 3,4};
		box->getCorners(corners);

		addLinesRenderData(corners, 8, index, 24, color, pMat);
	}
}

void C3DGeoWireRender::addCylinder(const C3DCylinder* cylinder, const C3DVector4& color, const C3DMatrix* pMat)
{
	float radius = cylinder->getRadius();
	float height = cylinder->getHeight();
	if (radius == 0.0f || height == 0.0f)
	    return;

	std::vector<unsigned short> aIndices;
	std::vector<BBVertex> aVerts;

	int iNumCol = log(50 * radius);
	if (iNumCol < 10)
		iNumCol = 10;

	C3DVector3 vCenter = cylinder->getCenter() + cylinder->getAxisY() * (height / 2.0f);

	int i;
	float fYawStep = MATH_DEG_TO_RAD(360.0f / iNumCol);
	C3DVector3 vPos, vOff;
	BBVertex bbvertex;
	bbvertex.color = color;

	// vertices
	for (i=0; i < iNumCol; i++)
	{
		float fYaw = i * fYawStep;

		vOff.x = radius * cos(fYaw);
		vOff.z = radius * sin(fYaw);

		vPos = vCenter + vOff.x * cylinder->getAxisX() + vOff.z * cylinder->getAxisZ();
		bbvertex.position = vPos;
		aVerts.push_back(bbvertex);
		vPos -= cylinder->getAxisY() * (height);
		bbvertex.position = vPos;
		aVerts.push_back(bbvertex);
	}

	// cap borders
	for (i=0; i < iNumCol; i++)
	{
		int i1 = i * 2;
		int i2 = (i + 1) % iNumCol * 2;

		aIndices.push_back(i1);
		aIndices.push_back(i2);

		aIndices.push_back(i1 + 1);
		aIndices.push_back(i2 + 1);
	}

	// side faces
	for (i=0; i < iNumCol; i++)
	{
		aIndices.push_back(i * 2);
		aIndices.push_back(i * 2 + 1);
	}

	if (pMat)
	{
		for (size_t k = 0; k < aVerts.size(); k++)
		{
			aVerts[k].position = (*pMat) * aVerts[k].position;
		}
	}
	_model_3D->add((unsigned char*)(&aVerts[0]), aVerts.size(), &aIndices[0], aIndices.size());
}

void C3DGeoWireRender::addCapsule(const C3DCapsule* capsule, const C3DVector4& color, const C3DMatrix* pMat)
{
    float halfHei = capsule->getCylinderHeight() / 2.0f;
    float radius = capsule->getRadius();
    if (radius == 0.0f)
		return;

    if (halfHei == 0.0f)
        return addSphere(capsule->getCenterPos(), capsule->getRadius(), color);

    std::vector<BBVertex> aVerts;
    std::vector<unsigned short> aIndices;
	BBVertex bbvertex;
	bbvertex.color = color;

	int i, j, iNumRow=4, iNumCol;
    iNumCol = log(50 * radius);
	if (iNumCol < 10)
		iNumCol = 10;

    const C3DVector3& vLenAxis = capsule->getAxisY();
    const C3DVector3& vCapusleCenter = capsule->getCenterPos();

    const C3DVector3& vAxisX = capsule->getAxisX();
    const C3DVector3& vAxisY = capsule->getAxisY();
    const C3DVector3& vAxisZ = capsule->getAxisZ();

	//	Top hemisphere's center
	C3DVector3 vCenter = vCapusleCenter + vLenAxis * halfHei;

	//	Top vertex of top hemisphere
    bbvertex.position = vCenter + vLenAxis * radius;
	aVerts.push_back(bbvertex);

	C3DVector3 vPos, vOff;

	//	Vertices of top hemisphere
	for (i=0; i < iNumRow; i++)
	{
		float fPitch = MATH_DEG_TO_RAD(90.0f - (i+1) * 90.0f / iNumRow);
		vOff.y = radius * sin(fPitch);

		float fYawStep = MATH_DEG_TO_RAD(360.0f / iNumCol);
		float fRadius = radius * cos(fPitch);

		for (j=0; j < iNumCol; j++)
		{
			float fYaw = j * fYawStep;

			vOff.x = fRadius * cos(fYaw);
			vOff.z = fRadius * sin(fYaw);

			bbvertex.position = vCenter + vOff.x * vAxisX + vOff.y * vAxisY + vOff.z * vAxisZ;
			aVerts.push_back(bbvertex);
		}
	}

	//	Bottom hemisphere's center
	vCenter = capsule->getCenterPos() - vLenAxis * halfHei;

	//	Vertices of bottom hemisphere
	for (i=0; i < iNumRow; i++)
	{
		float fPitch = MATH_DEG_TO_RAD(i * 90.0f / iNumRow);
		vOff.y = radius * sin(fPitch);

		float fYawStep = MATH_DEG_TO_RAD(360.0f / iNumCol);
		float fRadius = radius * cos(fPitch);

		for (j=0; j < iNumCol; j++)
		{
			float fYaw = j * fYawStep;

			vOff.x = fRadius * cos(fYaw);
			vOff.z = fRadius * sin(fYaw);

			bbvertex.position = vCenter + vOff.x * vAxisX - vOff.y * vAxisY + vOff.z * vAxisZ;
			aVerts.push_back(bbvertex);
		}
	}

	//	Top vertex of bottom hemisphere
    bbvertex.position = vCenter - vLenAxis * radius;
	aVerts.push_back(bbvertex);

	unsigned short wBaseIdx = 1;

	for (i=0; i < iNumCol; i++)
	{
		aIndices.push_back(0);
		aIndices.push_back(wBaseIdx + i);

		if (i == iNumCol - 1)
		{
			aIndices.push_back(wBaseIdx + iNumCol - 1);
			aIndices.push_back(wBaseIdx);
		}
		else
		{
			aIndices.push_back(wBaseIdx + i);
			aIndices.push_back(wBaseIdx + i + 1);
		}
	}

	wBaseIdx += iNumCol;

	int iLoop = iNumRow * 2 - 2 + 1;
	for (j=0; j < iLoop; j++)
	{
		for (i=0; i < iNumCol; i++)
		{
			aIndices.push_back(wBaseIdx + i - iNumCol);
			aIndices.push_back(wBaseIdx + i);

			if (i == iNumCol - 1)
			{
				aIndices.push_back(wBaseIdx + iNumCol - 1);
				aIndices.push_back(wBaseIdx);
			}
			else
			{
				aIndices.push_back(wBaseIdx + i);
				aIndices.push_back(wBaseIdx + i + 1);
			}
		}

		wBaseIdx += iNumCol;
	}

	for (i=0; i < iNumCol; i++)
	{
		aIndices.push_back(wBaseIdx + i - iNumCol);
		aIndices.push_back(wBaseIdx);
	}

	if (pMat)
	{
		for (size_t k = 0; k < aVerts.size(); k++)
		{
			aVerts[k].position = (*pMat) * aVerts[k].position;
		}
	}

    _model_3D->add((unsigned char*)(&aVerts[0]), aVerts.size(), &aIndices[0], aIndices.size());
}

void C3DGeoWireRender::addSphere(const C3DVector3& vPos, float radius, const C3DVector4& color, const C3DMatrix* pMat)
{
	float r = radius;

	int nRow = int(log(r*10) * 2 + 1);
	if (nRow < 3)
		nRow = 3;

	int nCol = nRow * 2 + 2;
	if(nCol < 3) nCol = 3;

	int vertCount = (nRow * 2 - 1) * nCol + 2;
	BBVertex* verts = new BBVertex[vertCount];
	std::vector<unsigned short> Indices;

	float angelStep = MATH_PI * 2.0f / nCol;
	int i,vIdx = 0, j;
	int idx1 = -1,idx2= -1;
	int lastIdx = -1;
	for(i = 0; i < nRow ; i++)
	{
		if(i == 0)
		{
			C3DVector3 vec(0.0f,r,0.0f);
			verts[vIdx].color = color;
			verts[vIdx++].position = vec+vPos;
			vec.y = -r;
			verts[vIdx].color = color;
			verts[vIdx++].position = vec+vPos;
			idx1 = 0;
			idx2 = 1;

			continue;
		}

		lastIdx = vIdx;
		float tmp = (float)sin(MATH_PI/2.0f/nRow* i);
		tmp *= r;
		for(j = 0; j < nCol; j++)
		{
			float z = sin(MATH_PI * 2.0f/nCol*j)*tmp;
			float x = cos(MATH_PI * 2.0f/nCol*j)*tmp;
			float y = sqrt(r*r- x*x - z*z);
			C3DVector3 vec(x,y,z);
			verts[vIdx].color = color;
			verts[vIdx++].position = vec + vPos;
			vec.y = -y;
			verts[vIdx].color = color;
			verts[vIdx++].position = vec + vPos;
			if(i == 1)
			{
				Indices.push_back(idx1);
				Indices.push_back(lastIdx + j*2);
				Indices.push_back(idx2);
				Indices.push_back(lastIdx + j*2+1);
			}
			else
			{
				Indices.push_back(lastIdx +(j - nCol)*2);
				Indices.push_back(lastIdx + j*2);
				Indices.push_back(lastIdx + (j - nCol)*2+1);
				Indices.push_back(lastIdx + j*2 +1);
			}

			if(j == 0)
			{
				Indices.push_back(lastIdx);
				Indices.push_back(lastIdx + (nCol-1)*2);
				Indices.push_back(lastIdx + 1);
				Indices.push_back(lastIdx + (nCol-1)*2 + 1);
			}
			else
			{
				Indices.push_back(lastIdx + (j-1)*2);
				Indices.push_back(lastIdx + j*2);
				Indices.push_back(lastIdx + (j-1)*2 + 1);
				Indices.push_back(lastIdx + j*2 + 1);
			}
		}
	}

	lastIdx = vIdx;
	for(i = 0; i < nCol; i++)
	{
		float z = sin(angelStep*i)*r;
		float x = cos(angelStep*i)*r;
		float y = 0;
		C3DVector3 vec(x,y,z);
		verts[vIdx].color = color;
		verts[vIdx++].position = vec + vPos;

		if(nRow == 1)
		{
			Indices.push_back(0);
			Indices.push_back(lastIdx + i);
			Indices.push_back(1);
			Indices.push_back(lastIdx + i);
		}
		else
		{
			Indices.push_back(lastIdx + (i - nCol)*2);
			Indices.push_back(lastIdx + i);
			Indices.push_back(lastIdx + (i - nCol)*2 + 1);
			Indices.push_back(lastIdx + i);
		}

		if(i == 0)
		{
			Indices.push_back(lastIdx);
			Indices.push_back(lastIdx + (nCol-1));
		}
		else
		{
			Indices.push_back(lastIdx + (i-1));
			Indices.push_back(lastIdx + i);
		}
	}

	if (pMat)
	{
		for (int k = 0; k < vertCount; k++)
		{
			verts[k].position = (*pMat) * verts[k].position;
		}
	}

	_model_3D->add((unsigned char*)(verts), vertCount, &Indices[0], Indices.size());
	delete[] verts;
}

void C3DGeoWireRender::begin()
{
	if ( _model_3D )
	{
		_model_3D->clear();
	}
	if ( _model_2D )
	{
		_model_2D->clear();
	}
}

void C3DGeoWireRender::flush()
{
	if (_model_3D)
	{
	//	_model_3D->draw();
		//_model_3D->end();

		C3DRenderChannel* channel = _model_3D->getRenderChannel();
		if(channel != NULL)
		{
			channel->addItem( _model_3D, 0 );
		}
		else
		{
			_model_3D->draw();
		}
	}

	if (_model_2D)
	{
		_model_2D->draw();
		//_model_2D->end();
	}
}
}