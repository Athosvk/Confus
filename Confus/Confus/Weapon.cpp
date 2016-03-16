#include <Irrlicht/irrlicht.h>

#include "Weapon.h"

namespace Confus
{
    Weapon::Weapon(irr::scene::ISceneNode* a_ParentNode, irr::scene::ISceneManager* a_SceneManager)
    {
        m_TriangleSelector = a_SceneManager->createTriangleSelectorFromBoundingBox(m_Node);

        const irr::core::aabbox3df& boundingBox = m_Node->getBoundingBox();
        auto responseAnimator = a_SceneManager->createCollisionResponseAnimator(m_TriangleSelector, m_Node,
            boundingBox.getExtent() / 2, irr::core::vector3df());
        m_Collider = std::make_unique<Collider>(responseAnimator);

        m_Collider->setCallback([this](irr::scene::ISceneNode* a_CollidedNode)
        {
            damagePlayer();
            return true;
        });
    }

    void Weapon::damagePlayer() const
    {

    }

    void Weapon::enableCollider()
    {
        m_Node->setTriangleSelector(m_TriangleSelector);
    }

    void Weapon::disableCollider()
    {
        m_Node->setTriangleSelector(nullptr);
    }
}
