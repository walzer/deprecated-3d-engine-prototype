#include "Base.h"

#include "C3DBatchMesh.h"
#include "C3DVertexFormat.h"
#include "Vertex.h"

namespace cocos3d
{
C3DBatchMesh::C3DBatchMesh(C3DVertexFormat* vertexFormat, PrimitiveType primitiveType, bool bIndex, unsigned int initialCapacity, unsigned int growSize)
    : C3DBaseMesh(vertexFormat,primitiveType)
	/*, _primitiveType(primitiveType)*/
	, _growSize(growSize)
	, _vertexCapacity(initialCapacity)
	, _vertexCount(0)
	, _vertices(NULL)
	, _verticesPtr(NULL)
	, _indices(NULL)
	, _indicesPtr(NULL)
	, _indexCount(0)
	, _indexCapacity( 0 )
{
	//_primitiveType = primitiveType;
	//_vertexFormat = new C3DVertexFormat(vertexFormat);
    _bUseIndex = bIndex;
	resizeVertex(_vertexCapacity);
	if ( _bUseIndex )
	{
		resizeIndex( _indexCapacity );
	}
}

C3DBatchMesh::~C3DBatchMesh()
{
    SAFE_DELETE_ARRAY(_vertices);
    SAFE_DELETE_ARRAY(_indices);

	_verticesPtr = NULL;
	_indicesPtr = NULL;
}

void C3DBatchMesh::setVertexCapacity(unsigned int capacity)
{
	resizeVertex( capacity );
}

void C3DBatchMesh::setIndexCapacity(unsigned int capacity)
{
    resizeIndex(capacity);
}

bool C3DBatchMesh::resizeVertex(unsigned int capacity)
{
    if (capacity == 0)
	{
		return false;
	}

    if (capacity == _vertexCapacity && _verticesPtr != NULL )
	{
		return true;
	}

    unsigned char* oldVertices = _vertices;

    // Allocate new data and reset pointers
    unsigned int voffset = _verticesPtr - _vertices;
    unsigned int vBytes = capacity * _vertexFormat->getVertexSize();
    _vertices = new unsigned char[vBytes];
    if (voffset >= vBytes)
	{
        voffset = vBytes - 1;
	}
    _verticesPtr = _vertices + voffset;

    // Copy old data back in
    if (oldVertices)
	{
        memcpy(_vertices, oldVertices, std::min(_vertexCapacity, capacity) * _vertexFormat->getVertexSize());
	}

	SAFE_DELETE_ARRAY(oldVertices);

    // Assign new capacities
    _vertexCapacity = capacity;

    return true;
}

bool C3DBatchMesh::resizeIndex(unsigned int capacity)
{
    if (_bUseIndex)
    {
        unsigned short* oldIndex = _indices;
        unsigned int voffset = _indicesPtr - _indices;

        _indices = new unsigned short[capacity];

        _indicesPtr = _indices + voffset;

        if (oldIndex)
        {
            if (voffset)
			{
				memcpy(_indices, oldIndex, std::min(_indexCapacity, capacity) * sizeof(_indices[0]) );
			}

			SAFE_DELETE_ARRAY(oldIndex);
        }
        _indexCapacity = capacity;
    }

    return true;
}
void C3DBatchMesh::add(const unsigned char* vertices, unsigned int vertexCount)
{
	addVertex( vertices, vertexCount );
}

void C3DBatchMesh::add(const unsigned char* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
{
	unsigned int vertexOffset = _vertexCount;
	addVertex( vertices, vertexCount );
	addIndex( indices, indexCount, vertexOffset );
}

void C3DBatchMesh::addVertex( const unsigned char* vertices, unsigned int vertexCount )
{
	unsigned int newVertexCount = _vertexCount + vertexCount;

	if (newVertexCount > _vertexCapacity)
	{
		if (_growSize == 0)
		{
			LOG_ERROR( "growSize is 0" );
			return;
		}

		unsigned int capacity( _vertexCapacity );
		while ( newVertexCount > capacity )
		{
			capacity += _growSize;
		}

		if ( !resizeVertex( capacity ) )
		{
			LOG_ERROR_VARG( "resizeVeterx(%d) falied", capacity );
			return;
		}
	}

	unsigned int vBytes = vertexCount * _vertexFormat->getVertexSize();
	memcpy(_verticesPtr, vertices, vBytes);

	_verticesPtr += vBytes;
	_vertexCount = newVertexCount;
}

void C3DBatchMesh::addIndex( const unsigned short* indices, unsigned int indexCount, unsigned int vertexOffset )
{
	unsigned int newIndexCount = _indexCount + indexCount;

	if (newIndexCount > _indexCapacity)
	{
		if (_growSize == 0)
		{
			LOG_ERROR( "growSize is 0" );
			return;
		}

		unsigned int capacity( _indexCapacity );
		while ( newIndexCount > capacity )
		{
			capacity += _growSize;
		}

		if (!resizeIndex( capacity ) )
		{
			LOG_ERROR_VARG( "resizeIndex(%d) falied", capacity );
			return;
		}
	}

	for (unsigned int i = 0; i < indexCount; ++i)
	{
		*_indicesPtr = indices[i] + vertexOffset;
		++_indicesPtr;
	}
	_indexCount = newIndexCount;
}

void C3DBatchMesh::clear()
{
    _vertexCount = 0;
    _verticesPtr = _vertices;
    _indicesPtr = _indices;
    _indexCount = 0;
}
}