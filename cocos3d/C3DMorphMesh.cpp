#include "Base.h"
#include "C3DMorphMesh.h"
#include "C3DMorph.h"
#include "C3DVertexFormat.h"

namespace cocos3d
{
C3DMorphMesh::C3DMorphMesh(C3DVertexFormat* vertexFormat,PrimitiveType primitiveType)
    : C3DMesh(vertexFormat,primitiveType)
{
	_vertexData = NULL;
}

C3DMorphMesh::~C3DMorphMesh()
{
	reload();
}

C3DMorphMesh* C3DMorphMesh::createMesh(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic)
{
	C3DMorphMesh* mesh = new C3DMorphMesh(vertexFormat);
	
	bool res = mesh->init(vertexFormat, vertexCount, dynamic);

	if(res == true)
		return mesh;
	else
	{
		SAFE_RELEASE(mesh);
		return NULL;
	}
}

void C3DMorphMesh::setVertexData(void* vertexData, unsigned int vertexStart, unsigned int vertexCount)
{
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );

    if (vertexStart == 0 && vertexCount == 0)
    {
        GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, _vertexCount * _vertexFormat->getVertexSize(), vertexData, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
    }
    else
    {
        if (vertexCount == 0)
        {
            vertexCount = _vertexCount - vertexStart;
        }

        GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, vertexStart * _vertexFormat->getVertexSize(), vertexCount * _vertexFormat->getVertexSize(), vertexData) );
    }

	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );

	setMorphVertexData(vertexData);
}

void C3DMorphMesh::setMorphVertexData(void* vertexData)
{
	int vertexByteCount = _vertexCount*_vertexFormat->getVertexSize();
    _vertexData = new unsigned char[vertexByteCount];
	memcpy(_vertexData,vertexData,vertexByteCount);
}

void C3DMorphMesh::reload()
{
	C3DMesh::reload();
	SAFE_DELETE_ARRAY(_vertexData);
}

void C3DMorphMesh::clearMorph(C3DMorph* morph)
{
	if(_vertexData == NULL)
		return;

	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );

    GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, 0, _vertexCount * _vertexFormat->getVertexSize(), _vertexData) );
	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    morph->clearCurTarget();
}

void C3DMorphMesh::applyMorph(C3DMorph* morph)
{
	if(_vertexData == NULL)
		return;

	int vertexByteCount = _vertexCount*_vertexFormat->getVertexSize();
	unsigned char* vertexData = new unsigned char[vertexByteCount];
	memcpy(vertexData,_vertexData,vertexByteCount);

	std::vector<unsigned int>* curTargets = morph->getCurTargets();

	unsigned int vertexSize = _vertexFormat->getVertexSize();
	unsigned int floatSize = sizeof(float);

	for( std::vector<unsigned int>::iterator iter1=curTargets->begin(); iter1!=curTargets->end(); ++iter1 )
	{
		MorphTarget* morphTarget = morph->getMorphTarget(*iter1);
		std::vector<VertexOffset>& offsets = morphTarget->offsets;

		VertexOffset vo;
		unsigned int offset = 0;
		for(std::vector<VertexOffset>::iterator iter=offsets.begin();iter!=offsets.end();++iter)
		{
			vo = *iter;
			offset = vo.index*vertexSize;
			float* x = (float*)&vertexData[offset];
			float* y = (float*)&vertexData[offset+floatSize];
			float* z = (float*)&vertexData[offset+2*floatSize];

			(*x) = (*x) + vo.x*morphTarget->weight;
			(*y) = (*y) + vo.y*morphTarget->weight;
			(*z) = (*z) + vo.z*morphTarget->weight;
		}
	}

	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );

	GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, 0, _vertexCount * _vertexFormat->getVertexSize(), vertexData) );
	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
	SAFE_DELETE_ARRAY(vertexData);
}

void C3DMorphMesh::pushMorph(C3DMorph* morph,int morphTargetIndex,float weight)
{
	bool res = morph->pushTarget(morphTargetIndex);
	if(res==false)
		return;

	MorphTarget* morphTarget = morph->getMorphTarget(morphTargetIndex);
	morphTarget->weight = weight;
	applyMorph(morph);
}

void C3DMorphMesh::popMorph(C3DMorph* morph,int morphTargetIndex)
{
	bool res = morph->popTarget(morphTargetIndex);
	if(res==false)
		return;

	applyMorph(morph);
}

void C3DMorphMesh::changeMorph(C3DMorph* morph,int morphTargetIndex,float weight)
{
	MorphTarget* morphTarget = morph->getMorphTarget(morphTargetIndex);
	morphTarget->weight = weight;

	applyMorph(morph);
}
}