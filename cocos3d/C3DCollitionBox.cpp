#include "Base.h"
#include "C3DCollitionBox.h"
#include "C3DRenderNode.h"
#include "C3DSprite.h"

namespace cocos3d
{
	C3DCollitionBox::C3DCollitionBox(C3DRenderNode* pRenderNode, unsigned int nID, const std::string& strBone, const C3DVector3& offset, float fRadius)
	:m_pParentNode(pRenderNode),
	m_nID(nID),
	m_strBone(strBone),
	m_fRadius(fRadius),
	m_fScale(1.f),
	m_Rotate(C3DQuaternion::zero()),
	m_vLocalPos(offset),
	_min(C3DVector3::zero()),
	_max(C3DVector3::zero()),
	m_vUp(C3DVector3::unitY()),
	m_vRight(C3DVector3::unitX()),
	m_vForward(C3DVector3::unitZ())
	{
		assert(m_pParentNode);
		update();
	}

	float C3DCollitionBox::getRadius()
	{
		float fRadius = (getScale() + (m_pParentNode->getScale().x - 1.f)) * m_fRadius;
		return fRadius;
	}

	C3DVector3 C3DCollitionBox::getLocalPos()
	{
		return m_vLocalPos;
	}

	void C3DCollitionBox::updatePosition()
	{
		if (!m_pParentNode) return;

		float scaleParent = m_pParentNode->getScale().x;
		if (m_pParentNode->getType() == C3DNode::NodeType_SuperModel)
		{
			C3DBone* tpBone = static_cast<C3DSprite*>(m_pParentNode)->getBone(m_strBone);
			if (!tpBone) return ;

			C3DVector3 boneWorldPos = tpBone->getTranslationWorld();
			C3DQuaternion quat = tpBone->getRotationWorld();
			C3DVector3 v1 = quat * (m_vLocalPos * scaleParent);
			_position = boneWorldPos + v1;
		}
		else if (m_pParentNode->getType() == C3DNode::NodeType_SceneModel)
		{
			C3DQuaternion quat = m_pParentNode->getRotationWorld();
			C3DVector3 v1 = quat * (m_vLocalPos * scaleParent);
			_position = m_pParentNode->getPosition() + v1;
		}
		else
		{
			assert(0);
		}

		//C3DBone* tpBone = static_cast<C3DSprite*>(m_pParentNode)->getBone(m_strBone);
		//if (!tpBone) return ;

		//C3DVector3 boneWorldPos = tpBone->getTranslationWorld();
		//C3DQuaternion quat = tpBone->getRotationWorld();
		//float scaleParent = m_pParentNode->getScale().x;
		//C3DVector3 v1 = quat * (m_vLocalPos * scaleParent);
		//_position = boneWorldPos + v1;
	}

	void C3DCollitionBox::getCorners(C3DVector3* dst) const
	{
		assert(dst);

		// Near face, specified counter-clockwise looking towards the origin from the positive z-axis.
		// Left-top-front.
		dst[0].set(_min.x, _max.y, _max.z);
		// Left-bottom-front.
		dst[1].set(_min.x, _min.y, _max.z);
		// Right-bottom-front.
		dst[2].set(_max.x, _min.y, _max.z);
		// Right-top-front.
		dst[3].set(_max.x, _max.y, _max.z);

		// Far face, specified counter-clockwise looking towards the origin from the negative z-axis.
		// Right-top-back.
		dst[4].set(_max.x, _max.y, _min.z);
		// Right-bottom-back.
		dst[5].set(_max.x, _min.y, _min.z);
		// Left-bottom-back.
		dst[6].set(_min.x, _min.y, _min.z);
		// Left-top-back.
		dst[7].set(_min.x, _max.y, _min.z);
	}

	void C3DCollitionBox::update()
	{
		if (m_strBone == "")
		{
			rotate(m_pParentNode->getRotationWorld());
		}
		else
		{
			C3DBone* tpBone = static_cast<C3DSprite*>(m_pParentNode)->getBone(m_strBone);
			if (tpBone)
			{
				rotate(tpBone->getRotationWorld());
			}
		}

		updatePosition();
		C3DVector3 vOffset = C3DVector3(getRadius(), getRadius(), getRadius());
		C3DVector3 vWorldPos = getPosition();
		_min = vWorldPos - vOffset;
		_max = vWorldPos + vOffset;
	}

	void C3DCollitionBox::translate(float cx, float cy, float cz)
	{
		C3DVector3 offset = C3DVector3(cx, cy, cz);
		m_vLocalPos += offset;
	}

	void C3DCollitionBox::rotate(const C3DQuaternion& rotate)
	{
		m_Rotate = rotate;
	}

	void C3DCollitionBox::scale(float fScale)
	{
		m_fScale = fScale;
	}

	float C3DCollitionBox::getScale()
	{
		return m_fScale;
	}

	float C3DCollitionBox::getScaleLocal()
	{
		return m_fScale;
	}

	C3DVector3 C3DCollitionBox::getUp()
	{
		return m_Rotate * m_vUp;
	}

	C3DVector3 C3DCollitionBox::getRight()
	{
		return m_Rotate * m_vRight;
	}

	C3DVector3 C3DCollitionBox::getForward()
	{
		return m_Rotate * m_vForward;
	}
}