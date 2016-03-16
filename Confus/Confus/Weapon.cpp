#include <Irrlicht/irrlicht.h>
#include <math.h>

#include "Weapon.h"

namespace Confus
{
    Weapon::Weapon(irr::scene::ISceneManager* a_SceneManager, irr::core::vector3df a_Dimensions)
    {
        m_Node = a_SceneManager->addCubeSceneNode(1.0f, nullptr, -1, irr::core::vector3df(), irr::core::vector3df(),
            a_Dimensions);
        m_Node->setVisible(false);

        m_TriangleSelector = a_SceneManager->createTriangleSelectorFromBoundingBox(m_Node);
        m_Node->setTriangleSelector(m_TriangleSelector);

        auto responseAnimator = a_SceneManager->createCollisionResponseAnimator(m_TriangleSelector, m_Node,
            a_Dimensions, irr::core::vector3df());
        m_Collider = std::make_unique<Collider>(responseAnimator);

        m_Collider->setCallback([this](irr::scene::ISceneNode* a_CollidedNode)
        {
            if(!m_Collided)
            {
                m_Collided = true;
                damagePlayer(a_CollidedNode);
            }
            return true;
        });
    }

    void Weapon::damagePlayer(irr::scene::ISceneNode* a_CollidedNode) const
    {
        if(getAngle(a_CollidedNode->getPosition(), m_Node->getPosition()) <= (180.0f - BackstabAngle))
        {
            backstabPlayer();
        }
    }

    void Weapon::backstabPlayer() const
    {

    }

    float Weapon::getAngle(irr::core::vector3df a_Vector1, irr::core::vector3df a_Vector2) const
    {
        return irr::core::radToDeg(acos(a_Vector1.dotProduct(a_Vector2)));
    }

    void Weapon::enableCollider()
    {
        m_Node->setTriangleSelector(m_TriangleSelector);
    }

    void Weapon::disableCollider()
    {
        m_Node->setTriangleSelector(nullptr);
    }

    void Weapon::resetCollider()
    {
        m_Collided = false;
    }

    void Weapon::setParent(irr::scene::ISceneNode* a_Parent)
    {
        m_Node->setParent(a_Parent);
    }
}
