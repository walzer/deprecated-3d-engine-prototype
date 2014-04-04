#include "Base.h"
#include "Geo.h"
#include "C3DMesh.h"
#include "MeshPart.h"
#include "C3DVector4.h"

#include "C3DVertexFormat.h"
#include "Vertex.h"
namespace cocos3d
{

C3DMesh* Geo::createBoard(float x, float y, float width, float height)
{
    float x2 = x + width;
    float y2 = y + height;

	float vertices[] =
    {
        x, y,   0,  0, 0, 1,  0, 1,//lb
        x, y2,  0,  0, 0, 1,  0, 0,//lt
        x2, y,  0,  0, 0, 1,  1, 1,//rb
        x2, y2, 0,  0, 0, 1,  1, 0 //rt
    };

    C3DVertexElement elements[] =
    {
        C3DVertexElement(Vertex_Usage_POSITION, 3),
        C3DVertexElement(Vertex_Usage_NORMAL, 3),
        C3DVertexElement(Vertex_Usage_TEXCOORD0, 2)
    };
	C3DVertexFormat vf(elements, 3);
    C3DMesh* mesh = C3DMesh::createMesh(&vf, 4, false);
    if (mesh == NULL)
    {
        return NULL;
    }

    mesh->_primitiveType = PrimitiveType_TRIANGLE_STRIP;
    mesh->setVertexData(vertices, 0, 4);

    return mesh;
}

#define MAX_QUAT_FULL_SCREEN_SIZE 100

C3DMesh* Geo::createQuadFullscreen( unsigned int sizeX , unsigned int sizeY )
{
	static float xMin = -1.0f;
	static float yMin = -1.0f;
	static float xMax = 1.0f;
	static float yMax = 1.0f;

	static float uMin = 0.0;
	static float uMax = 1.0;
	static float vMin = 0.0;
	static float vMax = 1.0;

	sizeX = (sizeX>1)?sizeX:1;
	sizeY = (sizeY>1)?sizeY:1;
	sizeX = (sizeX<MAX_QUAT_FULL_SCREEN_SIZE)?sizeX:MAX_QUAT_FULL_SCREEN_SIZE;
	sizeY = (sizeY<MAX_QUAT_FULL_SCREEN_SIZE)?sizeY:MAX_QUAT_FULL_SCREEN_SIZE;

	float vertexStepX = (xMax-xMin)/sizeX;
	float vertexStepY = (yMax-yMin)/sizeY;

	float uStep = (uMax-uMin)/sizeX;
	float vStep = (vMax-vMin)/sizeY;

	int vertexCount = (sizeX+1)*(sizeY+1);
	int indexCount = sizeX*sizeY*6;

	VertexPosition2UV* vertices = new VertexPosition2UV[vertexCount];
	for ( unsigned int x = 0; x <= sizeX; ++x )
	{
		for ( unsigned int  y = 0; y <= sizeY; ++y )
		{
			unsigned int offset = x+y*(sizeX+1);
			VertexPosition2UV* ptr = &(vertices[offset]);
			ptr->position.x = xMin+x*vertexStepX;
			ptr->position.y = yMin+y*vertexStepY;

			ptr->uv.x = uMin+x*uStep;
			ptr->uv.y = vMin+y*vStep;
		}
	}

	unsigned short* indices = new unsigned short[indexCount];
	for ( unsigned int x = 0; x < sizeX; ++x )
	{
		for ( unsigned int y = 0; y < sizeY; ++y )
		{
			unsigned short* ptr = &(indices[(x+y*sizeX)*6]);
			*(ptr+0) = (x+0) + (y+0)*(sizeX+1);
			*(ptr+1) = (x+0) + (y+1)*(sizeX+1);
			*(ptr+2) = (x+1) + (y+0)*(sizeX+1);

			*(ptr+3) = (x+0) + (y+1)*(sizeX+1);
			*(ptr+4) = (x+1) + (y+1)*(sizeX+1);
			*(ptr+5) = (x+1) + (y+0)*(sizeX+1);
		}
	}

	C3DVertexElement elements[] =
	{
		C3DVertexElement(Vertex_Usage_POSITION, 2),
		C3DVertexElement(Vertex_Usage_TEXCOORD0, 2)
	};
	C3DVertexFormat vertformat(elements, 2);

	C3DMesh* mesh = C3DMesh::createMesh(&vertformat, vertexCount);
	mesh->setPrimitiveType(PrimitiveType_TRIANGLES);
	mesh->setVertexData(vertices, 0, vertexCount);

	MeshPart* meshPart = mesh->addPart( PrimitiveType_TRIANGLES, IndexFormat_INDEX16, indexCount );

	meshPart->setIndexData( indices, 0, indexCount );

	SAFE_DELETE_ARRAY( vertices );
	SAFE_DELETE_ARRAY( indices );

	return mesh;
}

void Geo::createQuadFullscreen( unsigned int sizeX, unsigned int sizeY, C3DMesh* mesh )
{
	static float xMin = -1.0f;
	static float yMin = -1.0f;
	static float xMax = 1.0f;
	static float yMax = 1.0f;

	static float uMin = 0.0;
	static float uMax = 1.0;
	static float vMin = 0.0;
	static float vMax = 1.0;

	sizeX = (sizeX>1)?sizeX:1;
	sizeY = (sizeY>1)?sizeY:1;
	sizeX = (sizeX<MAX_QUAT_FULL_SCREEN_SIZE)?sizeX:MAX_QUAT_FULL_SCREEN_SIZE;
	sizeY = (sizeY<MAX_QUAT_FULL_SCREEN_SIZE)?sizeY:MAX_QUAT_FULL_SCREEN_SIZE;

	float vertexStepX = (xMax-xMin)/sizeX;
	float vertexStepY = (yMax-yMin)/sizeY;

	float uStep = (uMax-uMin)/sizeX;
	float vStep = (vMax-vMin)/sizeY;

	int vertexCount = (sizeX+1)*(sizeY+1);
	int indexCount = sizeX*sizeY*6;

	VertexPosition2UV* vertices = new VertexPosition2UV[vertexCount];
	for ( unsigned int x = 0; x <= sizeX; ++x )
	{
		for ( unsigned int  y = 0; y <= sizeY; ++y )
		{
			unsigned int offset = x+y*(sizeX+1);
			VertexPosition2UV* ptr = &(vertices[offset]);
			ptr->position.x = xMin+x*vertexStepX;
			ptr->position.y = yMin+y*vertexStepY;

			ptr->uv.x = uMin+x*uStep;
			ptr->uv.y = vMin+y*vStep;
		}
	}

	unsigned short* indices = new unsigned short[indexCount];
	for ( unsigned int x = 0; x < sizeX; ++x )
	{
		for ( unsigned int y = 0; y < sizeY; ++y )
		{
			unsigned short* ptr = &(indices[(x+y*sizeX)*6]);
			*(ptr+0) = (x+0) + (y+0)*(sizeX+1);
			*(ptr+1) = (x+0) + (y+1)*(sizeX+1);
			*(ptr+2) = (x+1) + (y+0)*(sizeX+1);

			*(ptr+3) = (x+0) + (y+1)*(sizeX+1);
			*(ptr+4) = (x+1) + (y+1)*(sizeX+1);
			*(ptr+5) = (x+1) + (y+0)*(sizeX+1);
		}
	}

	C3DVertexElement elements[] =
	{
		C3DVertexElement(Vertex_Usage_POSITION, 2),
		C3DVertexElement(Vertex_Usage_TEXCOORD0, 2)
	};
	C3DVertexFormat vertformat(elements, 2);

	//------------------------------------------------------------------
	mesh->reload();
	mesh->init(&vertformat, vertexCount, false);
	mesh->setPrimitiveType(PrimitiveType_TRIANGLES);
	mesh->setVertexData(vertices, 0, vertexCount);

	// Create mesh parts
	MeshPart* part = mesh->addPart(PrimitiveType_TRIANGLES, IndexFormat_INDEX16, indexCount );
	part->setIndexData( indices, 0, indexCount );
	//---------------------------------------------------------------

	SAFE_DELETE_ARRAY( vertices );
	SAFE_DELETE_ARRAY( indices );
}

bool Geo::CreateSphere(float radius, std::vector< BBVertex > &ppVertices, std::vector< unsigned short > &ppIndices, const C3DVector4& color, int stacks, int slices)
{
	int base = ppVertices.size();

	const float theta_start_degree = 0.0f;
	const float theta_end_degree = 360.0f;
	const float phi_start_degree = -90.0f;
	const float phi_end_degree = 90.0f;

	float ts = /*FastMath::DegreeToRadian*/CC_DEGREES_TO_RADIANS(theta_start_degree);
	float te = /*FastMath::DegreeToRadian*/CC_DEGREES_TO_RADIANS(theta_end_degree);
	float ps = /*FastMath::DegreeToRadian*/CC_DEGREES_TO_RADIANS(phi_start_degree);
	float pe = /*FastMath::DegreeToRadian*/CC_DEGREES_TO_RADIANS(phi_end_degree);

	float theta_total = te - ts;
	float phi_total = pe - ps;
	float theta_inc = theta_total/stacks;
	float phi_inc = phi_total/slices;

	int i,j;
	float theta = ts;

	float sin_theta, cos_theta;
	float sin_phi, cos_phi;

 	BBVertex pVertices;
	for ( i=0; i<=stacks; i++ )
	{
		float phi = ps;
		/*FastMath::*///SinCos(theta, sin_theta, cos_theta);
		//C3DUtility::getInstance().sincos(theta, &sin_theta, &cos_theta);
		sin_theta = sin(theta);
		cos_theta = cos(theta);
		for ( j=0; j<=slices; j++ )
		{
			sin_phi = sin(phi);
			cos_phi = cos(phi);
			/*FastMath::*///SinCos(phi, sin_phi, cos_phi);
			//C3DUtility::getInstance().sincos(phi, &sin_phi, &cos_phi);

			// vertex
			//pVertices[index].m_Position[0] = radius * cos_phi * cos_theta;
			//pVertices[index].m_Position[1] = radius * sin_phi;
			//pVertices[index].m_Position[2] = radius * cos_phi * sin_theta;
			pVertices.position.x = radius * cos_phi * cos_theta;
			pVertices.position.y = radius * sin_phi;
			pVertices.position.z = radius * cos_phi * sin_theta;

			// Color
			//float shading = (float) j / (float) slices;
			////float shading = 1.0f;
			//pVertices[index].m_RGBA[0] = 255 * color[0] * shading;
			//pVertices[index].m_RGBA[1] = 255 * color[1] * shading;
			//pVertices[index].m_RGBA[2] = 255 * color[2] * shading;
			//pVertices[index].m_RGBA[3] = 255 * color[3] * shading;

			pVertices.color = color;
			ppVertices.push_back(pVertices);

			// inc phi
			phi += phi_inc;
		}
		// inc theta
		theta += theta_inc;
	}

	// triangle list
	for ( i=0; i<stacks; i++ )
	{
		for ( j=0; j<slices; j++ )
		{
			/*pIndices[index++] = base;
			pIndices[index++] = base+1;
			pIndices[index++] = base+slices+1;

			pIndices[index++] = base+1;
			pIndices[index++] = base+slices+2;
			pIndices[index++] = base+slices+1;*/
			ppIndices.push_back(base);
			ppIndices.push_back(base + 1);
			ppIndices.push_back(base+slices+1);

			ppIndices.push_back(base+1);
			ppIndices.push_back(base+slices+2);
			ppIndices.push_back(base+slices+1);
			base++;
		}
		base++;
	}

	return true;
}
}