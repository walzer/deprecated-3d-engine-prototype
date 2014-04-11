#include "Base.h"
#include "C3DModel.h"
#include "C3DMesh.h"
#include "MeshPart.h"
#include "C3DMeshSkin.h"
#include "C3DScene.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "C3DNode.h"

#include "C3DMaterial.h"

#include "BonePart.h"
#include "C3DMorph.h"
#include "C3DVertexDeclaration.h"

#include "C3DLightFilter.h"
#include "MaterialParameter.h"

#include "C3DStat.h"
#include "C3DAnimatedLight.h"
#include "C3DShadowMap.h"
#include "C3DMaterialManager.h"
#include "C3DRenderChannel.h"
#include "C3DRenderSystem.h"
#include "C3DMorphMesh.h"
#include "C3DDeviceAdapter.h"

namespace cocos3d
{
C3DLightFilter* C3DModel::s_lightFilter = NULL;

C3DModel::C3DModel() :
    _mesh(NULL), _material(NULL), _partCount(0), _partMaterials(NULL), _node(NULL), _wireframe(false)
{
	_morph = NULL;
	_materialName = "";
}

C3DModel::~C3DModel()
{
    SAFE_RELEASE(_material);

    if (_partMaterials)
    {
        for (unsigned int i = 0; i < _partCount; ++i)
        {
            SAFE_RELEASE(_partMaterials[i]);
        }
        SAFE_DELETE_ARRAY(_partMaterials);
    }

    SAFE_RELEASE(_mesh);

	SAFE_DELETE(_morph);
}

C3DModel* C3DModel::create()
{
    return new C3DModel();
}

void C3DModel::setMesh(C3DMesh* mesh)
{
	if (_mesh != mesh)
    {
        // Free the old mesh
        SAFE_RELEASE(_mesh);

        // Assign the new mesh
		if(mesh != NULL)
		{
			_mesh = mesh;
			mesh->retain();

			_partCount = mesh->getPartCount();
		}
    }
}

C3DMesh* C3DModel::getMesh() const
{
    return _mesh;
}

unsigned int C3DModel::getMeshPartCount() const
{
    return _mesh->getPartCount();
}

C3DMaterial* C3DModel::getMaterial(int partIndex)
{
    assert(partIndex == -1 || (partIndex >= 0 && partIndex < (int)getMeshPartCount()));

    C3DMaterial* m = NULL;

    if (partIndex >= 0 && partIndex < (int)_partCount)
    {
        // Look up explicitly specified part material.
        if (_partMaterials)
        {
            m = _partMaterials[partIndex];
        }
    }

    if (m == NULL)
    {
        // Return the shared material.
         m = _material;
    }

    return m;
}

void C3DModel::setMaterial(C3DMaterial* material, int partIndex)
{
    assert(partIndex == -1 || (partIndex >= 0 && partIndex < (int)getMeshPartCount()));

    C3DMaterial* oldMaterial = NULL;

    if (partIndex == -1)
    {
        oldMaterial = _material;

        // Set new shared material.
        if (material)
        {
            _material = material;
            _material->retain();
        }
    }
    else if (partIndex >= 0 && partIndex < (int)getMeshPartCount())
    {
        // Ensure mesh part count is up-to-date.
        validatePartCount();

        // Release existing part material and part binding.
        if (_partMaterials)
        {
            oldMaterial = _partMaterials[partIndex];
        }
        else
        {
            // Allocate part arrays for the first time.
            if (_partMaterials == NULL)
            {
                _partMaterials = new C3DMaterial*[_partCount];
                memset(_partMaterials, 0, sizeof(C3DMaterial*) * _partCount);
            }
        }

        // Set new part material.
        if (material)
        {
            _partMaterials[partIndex] = material;
            material->retain();
        }
    }

    // Release existing material and binding.
    if (oldMaterial)
    {
       for (unsigned int i = 0, tCount = oldMaterial->getTechniqueCount(); i < tCount; ++i)
        {
            C3DTechnique* t = oldMaterial->getTechnique(i);
            for (unsigned int j = 0, pCount = t->getPassCount(); j < pCount; ++j)
            {
                t->getPass(j)->setVertexAttributeBinding(NULL);
            }
        }
        SAFE_RELEASE(oldMaterial);
    }

    if (material)
    {
        // Hookup vertex attribute bindings for all passes in the new material.
        for (unsigned int i = 0, tCount = material->getTechniqueCount(); i < tCount; ++i)
        {
            C3DTechnique* t = material->getTechnique(i);
            for (unsigned int j = 0, pCount = t->getPassCount(); j < pCount; ++j)
            {
                C3DPass* p = t->getPass(j);
                C3DVertexDeclaration* b = C3DVertexDeclaration::create(_mesh, p->getEffect());
                p->setVertexAttributeBinding(b);
            }
        }

        // Apply node binding for the new material.
        if (_node)
        {
            setMaterialNodeBinding(material);
        }
    }
}

//  C3DMaterial* C3DModel::setMaterial(const std::string& vshPath, const std::string& fshPath, const std::string& defines, int partIndex )
//  {
//      // Try to create a C3DMaterial with the given parameters.
//      C3DMaterial* material = C3DMaterial::create(vshPath, fshPath, defines);
//      if (material == NULL)
//      {
//          return NULL;
//      }
//  
//      // Assign the material to us.
//      setMaterial(material, partIndex);
//  
//      return material;
//  }

C3DMaterial* C3DModel::setMaterial(const std::string& materialPath, int partIndex)
{
    // Try to create a C3DMaterial from the specified material file.
	C3DMaterial* material = static_cast<C3DMaterial*>(C3DMaterialManager::getInstance()->getResource(materialPath));

    if (material == NULL)
    {
        return NULL;
    }

    // Assign the material to us
    setMaterial(material, partIndex);

    return material;
}

C3DMaterial* C3DModel::setDefaultMaterial(const std::string& path,int partIndex)
{
	std::string materialPath;
	// Try to create a C3DMaterial from the specified material file.
	if(_materialName.length() == 0)
	{
		materialPath = getDefaultMaterialName();
	}
	else
	{
		materialPath = path + "material"+ "/" + _materialName;
	}

	return setMaterial(materialPath, partIndex);
}

bool C3DModel::removeMaterial(int partIndex)
{
	assert(partIndex == -1 || (partIndex >= 0 && partIndex < (int)getMeshPartCount()));

	if (partIndex == -1)
	{
		SAFE_RELEASE(_material);
		return true;
	}
	else if (partIndex >= 0 && partIndex < (int)getMeshPartCount())
	{
		validatePartCount();

		if (_partMaterials)
		{
			SAFE_RELEASE(_partMaterials[partIndex]);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool C3DModel::hasMaterial(unsigned int partIndex) const
{
    return (partIndex < _partCount && _partMaterials && _partMaterials[partIndex]);
}

C3DRenderChannel* C3DModel::getRenderChannel()
{
	C3DRenderChannel* channel = NULL;

	unsigned int partCount = _mesh->getPartCount();

    C3DMaterial::TechniqueUsage techUsage =
        _node->get3DScene()->isInShadowPass() ? C3DMaterial::TECH_USAGE_SHADOWMAP : C3DMaterial::TECH_USAGE_SCREEN;

    if (partCount == 0)
    {
        // No mesh parts (index buffers).
        if (_material)
        {
            C3DTechnique* technique = _material->getTechnique(techUsage);

			channel = technique->getChannel();

			if (channel == NULL)
			{
				RenderChannelManager* mgr = C3DRenderSystem::getInstance()->getRenderChannelManager();
				channel = mgr->getRenderChannel(RenderChannelManager::CN_Opacity);
			}
        }
    }
    else
	{
        for (unsigned int i = 0; i < partCount; ++i)
        {
            // Get the material for this mesh part.
            C3DMaterial* material = getMaterial(i);

			C3DTechnique* technique = material->getTechnique(techUsage);
			channel = technique->getChannel();

			if(channel != NULL)
				break;
        }
    }

	if (channel == NULL)
	{
		RenderChannelManager* mgr = C3DRenderSystem::getInstance()->getRenderChannelManager();
		channel = mgr->getRenderChannel(RenderChannelManager::ChannelOpacity);
	}

	return channel;
}

C3DNode* C3DModel::getNode() const
{
    return _node;
}

void C3DModel::setNode(C3DNode* node)
{
    _node = node;

    // Re-bind node related material parameters
    if (node)
    {
        if (_material)
        {
           setMaterialNodeBinding(_material);
        }
        if (_partMaterials)
        {
            for (unsigned int i = 0; i < _partCount; ++i)
            {
                if (_partMaterials[i])
                {
                    setMaterialNodeBinding(_partMaterials[i]);
                }
            }
        }
    }
}

void C3DModel::validatePartCount()
{
    unsigned int partCount = _mesh->getPartCount();

    if (_partCount != partCount)
    {
        // Allocate new arrays and copy old items to them.
        if (_partMaterials)
        {
            C3DMaterial** oldArray = _partMaterials;
            _partMaterials = new C3DMaterial*[partCount];
            memset(_partMaterials, 0, sizeof(C3DMaterial*) * partCount);
            for (unsigned int i = 0; i < _partCount; ++i)
            {
                _partMaterials[i] = oldArray[i];
            }
            SAFE_DELETE_ARRAY(oldArray);
        }

        // Update local part count.
        _partCount = _mesh->getPartCount();
    }
}

void C3DModel::setMaterialNodeBinding(C3DMaterial *material)
{
    if (_node)
    {
        material->setNodeAutoBinding(_node);

        unsigned int techniqueCount = material->getTechniqueCount();
        for (unsigned int i = 0; i < techniqueCount; ++i)
        {
            C3DTechnique* technique = material->getTechnique(i);

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

void C3DModel::applyInternalParam(C3DPass* pass)
{
	applyLightParam(pass);
	applyShadowMap(pass);
	applyFogParam(pass);
}

void C3DModel::applyFogParam(C3DPass* pass)
{
	if (_node == NULL || _node->get3DScene() == NULL)
		return;

	const C3DEnvConf& env = _node->get3DScene()->getLayer()->getEnvConf();
	pass->getParameter("u_fogcolor")->setValue(&env._fogColor);
	//C3DVector4 fogparam(env._fogDensity, env._fogStart, env._fogEnd, (float)((int)env._fogType));
	pass->getParameter("u_fogparam")->setValue(&env._fogParam);
}

void C3DModel::applyLightParam(C3DPass* pass)
{
    if (_node == NULL)
        return;

    int nDirlight = 0, nPointlight = 0, nSpotlight = 0, nAnimlight = 0;
    int nMaxDirLight = pass->getMaxDirLight();
    int nMaxPointLight = pass->getMaxPointLight();
    int nMaxSpotLight = pass->getMaxSpotLight();
	int nMaxAnimLight = pass->getMaxAnimLight();

    char paraName[128];
    int i;
    for (i = 0; i < _node->get3DScene()->getLightCount(); i++) {
        C3DLight* light = _node->get3DScene()->getLight(i);
		if (!light->isLightEnable())
			continue;
		C3DLightComponent* lightComp = light->getComponent();
		if (!lightComp)
			continue;

        if (s_lightFilter == NULL || (s_lightFilter && s_lightFilter->isLightValidate(this, light)))
        {
            C3DLight::Type type = light->getLightType();
            C3DPointLight* pPointLight = NULL;
			C3DDirectionalLight* pDirLight = NULL;
			C3DSpotLight* pSpotLight = NULL;
			C3DAnimatedLight* pAnimLight = NULL;
			C3DVector3 upVector, rightVector;
			C3DVector3 posVector;

			switch (type) {
                case C3DLight::DIRECTIONAL:
                    if (nDirlight >= nMaxDirLight)
                        break;
					pDirLight = static_cast<C3DDirectionalLight*> (lightComp);
                    sprintf(paraName, "u_dirlight[%d].dir", nDirlight);
                    pass->getParameter(paraName)->setValue(light->getForwardVectorWorld());
                    sprintf(paraName, "u_dirlight[%d].color", nDirlight);
                    pass->getParameter(paraName)->setValue(pDirLight->getColor());
                    nDirlight++;
                    break;

                case C3DLight::POINT:
                    if (nPointlight >= nMaxPointLight)
                        break;
					pPointLight = static_cast<C3DPointLight*> (lightComp);

                    sprintf(paraName, "u_pointlight[%d].position", nPointlight);
                    pass->getParameter(paraName)->setValue(light->getTranslationView());
					//pass->getParameter(paraName)->setValue(light->getTranslationWorld());
                    sprintf(paraName, "u_pointlight[%d].color", nPointlight);
                    pass->getParameter(paraName)->setValue(pPointLight->getColor());
                    sprintf(paraName, "u_pointlight[%d].rangeInverse", nPointlight);
                    pass->getParameter(paraName)->setValue(pPointLight->getRangeInverse());
                    nPointlight++;

                    break;

                case C3DLight::SPOT:
                    if (nSpotlight >= nMaxSpotLight)
                        break;
					pSpotLight = static_cast<C3DSpotLight*> (lightComp);

                    sprintf(paraName, "u_spotlight[%d].position", nSpotlight);
                    pass->getParameter(paraName)->setValue(light->getTranslationView());
                    sprintf(paraName, "u_spotlight[%d].color", nSpotlight);
                    pass->getParameter(paraName)->setValue(pSpotLight->getColor());
                    sprintf(paraName, "u_spotlight[%d].rangeInverse", nSpotlight);
                    pass->getParameter(paraName)->setValue(pSpotLight->getRangeInverse());
                    sprintf(paraName, "u_spotlight[%d].dir", nSpotlight);
                    pass->getParameter(paraName)->setValue(light->getForwardVectorView());
                    sprintf(paraName, "u_spotlight[%d].innerAngleCos", nSpotlight);
                    pass->getParameter(paraName)->setValue(pSpotLight->getInnerAngleCos());
                    sprintf(paraName, "u_spotlight[%d].outerAngleCos", nSpotlight);
                    pass->getParameter(paraName)->setValue(pSpotLight->getOuterAngleCos());
                    nSpotlight++;

                    break;

				case C3DLight::ANIMATE:
					if (nAnimlight >= min(nMaxAnimLight, 1)) // no more than one animated light allowed
						break;
					pAnimLight = static_cast<C3DAnimatedLight*> (lightComp);

					light->getWorldMatrix().getRightVector(&rightVector);
					light->getWorldMatrix().getUpVector(&upVector);
					light->getWorldMatrix().getPosition(&posVector);

					sprintf(paraName, "u_animLightAxisU");
					pass->getParameter(paraName)->setValue(rightVector);
					sprintf(paraName, "u_animLightAxisV");
					pass->getParameter(paraName)->setValue(upVector);
					sprintf(paraName, "u_animLightPos");
					pass->getParameter(paraName)->setValue(posVector);

					sprintf(paraName, "u_animLightTexOffset");
					pass->getParameter(paraName)->setValue(pAnimLight->getUVOffset());
					sprintf(paraName, "u_animLightTexRotScale1");
					pass->getParameter(paraName)->setValue(pAnimLight->getUV1RotScale());
					sprintf(paraName, "u_animLightTexRotScale2");
					pass->getParameter(paraName)->setValue(pAnimLight->getUV2RotScale());

					sprintf(paraName, "u_animLightIntensity");
					pass->getParameter(paraName)->setValue(pAnimLight->getIntensity());
					sprintf(paraName, "u_lightTexture");
					pass->getParameter(paraName)->setValue(pAnimLight->getSampler());

					nAnimlight++;

					break;

                default:
                    break;
            }
        }
    }

    if (nMaxDirLight > 0)
        pass->getParameter("u_ndirlight")->setValue(nDirlight);
    if (nMaxPointLight > 0)
        pass->getParameter("u_npointlight")->setValue(nPointlight);
    if (nMaxSpotLight > 0)
        pass->getParameter("u_nspotlight")->setValue(nSpotlight);
	if (nMaxAnimLight > 0)
		pass->getParameter("u_nanimlight")->setValue(nAnimlight);

	if ( pass->getParameter( "u_ambientColor" ) != NULL )
	{
		pass->getParameter( "u_ambientColor" )->setValue( getNode()->get3DScene()->getAmbientColor() );
	}
}

void C3DModel::applyShadowMap(C3DPass* pass)
{
	if( C3DDeviceAdapter::getInstance()->isSupportShadow() == false )
		return;

    if (_node == NULL)
        return;

    int nMaxShadowMap = pass->getMaxShadowMap();

    if (nMaxShadowMap <= 0)
        return;

    C3DShadowMap* shadowMap = _node->get3DScene()->getActiveShadowMap();
	if (!shadowMap || !shadowMap->isVisible())
        return;

    pass->getParameter("u_shadowMatrix")->setValue(shadowMap->getTextureMatrix());
    pass->getParameter("u_shadowTexture")->setValue(shadowMap->getSampler());
}

void C3DModel::copyFrom(const C3DModel* other)
{
	//other->_mesh->retain();
	//SAFE_RELEASE(_mesh);
	//_mesh = other->_mesh;
	setMesh(other->_mesh);
	_partCount = other->_partCount;
	C3DMaterial* newMaterial;

	std::string matName = other->_material ? other->_material->getResourceName() : "";
	if ( !matName.empty() )
	{ 
		newMaterial = static_cast<C3DMaterial*>(C3DMaterialManager::getInstance()->getResource(matName));
		setMaterial(newMaterial, -1);
	}

	if (other->_partMaterials)
	{
		for (unsigned int i = 0; i < _partCount; i++)
		{
			newMaterial = other->_partMaterials[i] ? other->_partMaterials[i]->clone() : NULL;
			setMaterial(newMaterial, i);
		}
	}

	setMorph(other->_morph);
	setNode(other->_node);
}

C3DModel* C3DModel::clone(C3DNode::CloneContext& context) const
{
	C3DModel* other = new C3DModel();
	other->copyFrom(this);
	return other;
}

void C3DModel::setMaterialName(const std::string& matName)
{
	_materialName = matName;
}

void C3DModel::draw(void)
{
	unsigned int partCount = _mesh->getPartCount();
	bool bStatEnable = C3DStat::getInstance()->isStatEnable();

	C3DMaterial::TechniqueUsage techUsage =
		_node->get3DScene()->isInShadowPass() ? C3DMaterial::TECH_USAGE_SHADOWMAP : C3DMaterial::TECH_USAGE_SCREEN;

	if ( partCount == 0 )
	{
		// No mesh parts (index buffers).
		if (_material)
		{
			C3DTechnique* technique = _material->getTechnique(techUsage);

			unsigned int passCount = technique->getPassCount();
			if (bStatEnable)
			{
				C3DStat::getInstance()->incDrawCall(passCount);
			}
			for (unsigned int i = 0; i < passCount; ++i)
			{
				if (bStatEnable)
				{
					C3DStat::getInstance()->incTriangleDraw(_mesh->getTriangleCount());
				}

				C3DPass* pass = technique->getPass(i);
				//applyLightParam(pass);
				//applyShadowMap(pass);
				applyInternalParam(pass);
				pass->bind();
				GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
				if (_wireframe && (_mesh->getPrimitiveType() == GL_TRIANGLES || _mesh->getPrimitiveType() == GL_TRIANGLE_STRIP))
				{
					unsigned int vertexCount = _mesh->getVertexCount();
					for (unsigned int j = 0; j < vertexCount; j += 3)
					{
						GL_ASSERT( glDrawArrays(GL_LINE_LOOP, j, 3) );
					}
				}
				else
				{
					GL_ASSERT( glDrawArrays(_mesh->getPrimitiveType(), 0, _mesh->getVertexCount()) );
				}

				pass->unbind();
			}
		}
	}
	else
	{
		for ( size_t i = 0; i < partCount; ++i )
		{
			channelDrawPart( i );
		}
	}
}

void C3DModel::channelDrawPart( int partIndex )
{
	C3DMaterial::TechniqueUsage techUsage =
		getNode()->get3DScene()->isInShadowPass() ? C3DMaterial::TECH_USAGE_SHADOWMAP : C3DMaterial::TECH_USAGE_SCREEN;

	C3DMaterial* material = getMaterial(partIndex);
	MeshPart* meshPart = _mesh->getPart(partIndex);

	if (material)
	{
		C3DTechnique* technique = material->getTechnique(techUsage);
		unsigned int passCount = technique->getPassCount();

		for (unsigned int j = 0; j < passCount; ++j)
		{
			C3DPass* pass = technique->getPass(j);
			//applyLightParam(pass);
			//applyShadowMap(pass);
			applyInternalParam(pass);
			pass->bind();
			GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshPart->getIndexBuffer()) );

			// check whether show the wireframe
			if (_wireframe && (_mesh->getPrimitiveType() == PrimitiveType_TRIANGLES || _mesh->getPrimitiveType() == PrimitiveType_TRIANGLE_STRIP))
			{
				unsigned int indexSize = 0;
				switch (meshPart->getIndexFormat())
				{
				case IndexFormat_INDEX8:
					indexSize = 1;
					break;
				case IndexFormat_INDEX16:
					indexSize = 2;
					break;
				case IndexFormat_INDEX32:
					indexSize = 4;
					break;
				}

				int indexCount = meshPart->getIndexCount();
				for (int k = 0; k < indexCount; k += 3)
				{
					GL_ASSERT( glDrawElements(GL_LINE_LOOP, 3, meshPart->getIndexFormat(), ((const GLvoid*)(k*indexSize))));
				}
			}
			else
			{
				GL_ASSERT( glDrawElements(meshPart->getPrimitiveType(), meshPart->getIndexCount(), meshPart->getIndexFormat(), 0) );
			}
			pass->unbind();
		}
	}
}

float C3DModel::distanceToCamera(void)
{
	C3DAABB worldSpaceBox(*(_mesh->getBoundingBox()));

	worldSpaceBox.transform(_node->getWorldMatrix());

	worldSpaceBox.transform(_node->getViewMatrix());

	C3DVector3 center = C3DVector3((worldSpaceBox.getCenter()).x,(worldSpaceBox.getCenter()).y,(worldSpaceBox.getCenter()).z);

	return abs( center.z );
}
}
