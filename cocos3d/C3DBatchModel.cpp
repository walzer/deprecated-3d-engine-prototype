#include "Base.h"
#include "C3DBatchModel.h"
#include "C3DBatchMesh.h"

#include "C3DScene.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "C3DMaterial.h"
#include "C3DVertexFormat.h"

#include "C3DVertexDeclaration.h"
#include "C3DNode.h"

#include "C3DRenderSystem.h"
#include "C3DRenderChannel.h"
#include "C3DMaterialManager.h"

namespace cocos3d
{
C3DBatchModel* C3DBatchModel::createDebugModel(C3DNode* node)
{
	C3DBatchModel* model = new C3DBatchModel(node);

	C3DVertexElement elements[] =
	{
		C3DVertexElement(Vertex_Usage_POSITION, 3),
		C3DVertexElement(Vertex_Usage_COLOR, 4)
    };
    C3DVertexFormat v = C3DVertexFormat(elements, 2);
    C3DBatchMesh* mesh = new C3DBatchMesh(&v, PrimitiveType_LINES,true);

    model->_mesh = mesh;
	C3DMaterial* material = static_cast<C3DMaterial*>(C3DMaterialManager::getInstance()->getResource("common/debug.material"));

	for ( unsigned int i = 0; i < material->getTechniqueCount(); ++i )
	{
		C3DTechnique* tech = material->getTechnique( i );
		tech->setChannel( RenderChannelManager::ChannelDebug.c_str() );
	}

	model->setMaterial(material);

	return model;
}

C3DBatchModel::C3DBatchModel(C3DNode* node)
{
	_material = NULL;
	_mesh = NULL;

	_node = node;
}

C3DBatchModel::~C3DBatchModel()
{
    SAFE_RELEASE(_material);

    SAFE_DELETE(_mesh);
	_node = NULL;
}

C3DBatchMesh* C3DBatchModel::getMesh() const
{
    return _mesh;
}

void C3DBatchModel::setMesh(C3DBatchMesh* mesh)
{
	_mesh = mesh;
}

C3DMaterial* C3DBatchModel::getMaterial()
{
    return _material;
}

void C3DBatchModel::setMaterialNodeBinding(void)
{
	if ( _node != NULL && _material != NULL )
	{
		_material->setNodeAutoBinding(_node);

		unsigned int techniqueCount = _material->getTechniqueCount();
		for (unsigned int i = 0; i < techniqueCount; ++i)
		{
			C3DTechnique* technique = _material->getTechnique(i);

			technique->setNodeAutoBinding(_node);

			unsigned int passCount = technique->getPassCount();
			for (unsigned int j = 0; j < passCount; ++j)
			{
				C3DPass* pass = technique->getPass(j);

				pass->setNodeAutoBinding(_node);
			}
		}
	}
}

void C3DBatchModel::setMaterial(C3DMaterial* material)
{
	if (material != _material)
	{
		SAFE_RELEASE(_material);
		if (material)
		{
			_material = material;
			material->retain();
		}
		// Apply node binding for the new material.
		setMaterialNodeBinding();
	}
	bindVertex();
}

void C3DBatchModel::bindVertex(void)
{
	if (_material)
	{
		for (unsigned int i = 0, tCount = _material->getTechniqueCount(); i < tCount; ++i)
		{
			C3DTechnique* t = _material->getTechnique(i);
			for (unsigned int j = 0, pCount = t->getPassCount(); j < pCount; ++j)
			{
				C3DPass* p = t->getPass(j);
				
				C3DVertexDeclaration* b = C3DVertexDeclaration::create(_mesh->_vertexFormat, _mesh->_vertices, p->getEffect());
				p->setVertexAttributeBinding(b);
			}
		}
	}
}

void C3DBatchModel::add(const unsigned char* vertices, unsigned int vertexCount)
{
	if ( _mesh != NULL )
	{
		unsigned char* lastVeretices = _mesh->getVertices();
		_mesh->add( vertices, vertexCount );
		if ( lastVeretices != _mesh->getVertices() )
		{
			bindVertex();
		}
	}
}

void C3DBatchModel::add(const unsigned char* vertices, unsigned int vertexCount,
		const unsigned short* indices, unsigned int indexCount)
{
	if ( _mesh != NULL )
	{
		unsigned char* lastVeretices = _mesh->getVertices();
		_mesh->add( vertices, vertexCount, indices, indexCount );
		if ( lastVeretices != _mesh->getVertices() )
		{
			bindVertex();
		}
	}
}

void C3DBatchModel::clear()
{
	if ( _mesh != NULL )
	{
		_mesh->clear();
	}
}

C3DRenderChannel* C3DBatchModel::getRenderChannel()
{
	// Bind the material
	C3DTechnique* technique = _material->getTechnique(C3DMaterial::TECH_USAGE_SCREEN);
	C3DRenderChannel* channel = technique->getChannel();
	if ( channel == NULL )
	{
		channel = C3DRenderSystem::getInstance()->getRenderChannelManager()->getRenderChannel(RenderChannelManager::CN_Transparency);
	}

	return channel;
}

//void C3DBatchModel::draw( float sortParam )
//{
//	if ( _mesh->_vertexCount == 0 )
//	{
//		return;
//	}
//
//	// Bind the material
//	C3DTechnique* technique = _material->getTechnique(C3DMaterial::TECH_USAGE_SCREEN);
//	C3DRenderChannel* channel = technique->getChannel();
//	if ( channel == NULL )
//	{
//		channel = C3DRenderSystem::getInstance()->getRenderChannelManager()->getRenderChannel(RenderChannelManager::ChannelSorted);
//	}
//
//	if ( channel != NULL )
//	{
//		channel->addItem( this, sortParam );
//	}
//	else
//	{
//		LOG_ERROR( "C3DBatchModel::channelDraw is NULL" );
//	}
//
//}
//
//void C3DBatchModel::draw()
//{
//    if ( _node == NULL )
//	{
//        return;
//	}
//	C3DVector3 pos( C3DVector3::zero() );
//	_node->getWorldViewMatrix().getPosition( &pos );
//
//	draw( pos.z );
//}

// ChannelDrawItem
void C3DBatchModel::draw(void)
{
	if ( _mesh->_vertexCount == 0 )
	{
		return;
	}
	// Bind the material
	C3DTechnique* technique = _material->getTechnique(C3DMaterial::TECH_USAGE_SCREEN);

	unsigned int passCount = technique->getPassCount();
	for (unsigned int i = 0; i < passCount; ++i)
	{
		C3DPass* pass = technique->getPass(i);
		pass->bind();

		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ) );
		if (_mesh->_bUseIndex)
		{
			GL_ASSERT( glDrawElements(_mesh->_primitiveType, _mesh->_indexCount, GL_UNSIGNED_SHORT, _mesh->_indices));
		}
		else
		{
			GL_ASSERT( glDrawArrays(_mesh->_primitiveType, 0, _mesh->_vertexCount) );
		}

		pass->unbind();
	}
}

unsigned int C3DBatchModel::getVertexCapacity() const
{
	return (_mesh!=NULL)?_mesh->getVertexCapacity():0;
}

void C3DBatchModel::setVertexCapacity(unsigned int capacity)
{
	if ( _mesh != NULL )
	{
		unsigned char* lastVeretices = _mesh->getVertices();
		_mesh->setVertexCapacity( capacity );
		if ( lastVeretices != _mesh->getVertices() )
		{
			bindVertex();
		}
	}
}

unsigned int C3DBatchModel::getIndexCapacity() const
{
	return (_mesh!=NULL)?_mesh->getIndexCapacity():0;
}

void C3DBatchModel::setIndexCapacity(unsigned int capacity)
{
	if ( _mesh != NULL )
	{
		_mesh->setIndexCapacity( capacity );
	}
}
}