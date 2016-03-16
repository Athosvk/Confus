#include <Irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>

#include "Weapon.h"

namespace Confus
{
    Weapon::Weapon(irr::scene::ISceneManager* a_SceneManager)
    {
        loadMesh(a_SceneManager);
        m_TriangleSelector = a_SceneManager->createTriangleSelectorFromBoundingBox(m_MeshNode);

        const irr::core::aabbox3df& boundingBox = m_MeshNode->getBoundingBox();
        auto responseAnimator = a_SceneManager->createCollisionResponseAnimator(m_TriangleSelector, m_MeshNode,
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
        m_MeshNode->setTriangleSelector(m_TriangleSelector);
    }

    void Weapon::disableCollider()
    {
        m_MeshNode->setTriangleSelector(nullptr);
    }

    void Weapon::loadMesh(irr::scene::ISceneManager* a_SceneManager)
    {
        IrrAssimp importer(a_SceneManager);
        m_MeshNode = a_SceneManager->addAnimatedMeshSceneNode(importer.getMesh("Media/Meshes/Knife.3DS"));
    }
}
