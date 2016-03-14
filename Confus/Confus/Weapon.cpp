#include <Irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>

#include "Weapon.h"

namespace Confus
{
    Weapon::Weapon(irr::scene::ISceneManager* a_SceneManager)
    {
        loadMesh(a_SceneManager);
        auto triangleSelector = m_MeshNode->getTriangleSelector();

        const irr::core::aabbox3df& boundingBox = m_MeshNode->getBoundingBox();
        auto responseAnimator = a_SceneManager->createCollisionResponseAnimator(triangleSelector, m_MeshNode,
            boundingBox.getExtent() / 2, irr::core::vector3df());
        m_Collider = std::make_unique<Collider>(responseAnimator);

        m_Collider->setCallback([this](irr::scene::ISceneNode* a_CollidedNode)
        {
            damagePlayer();
        });
    }

    void Weapon::damagePlayer() const
    {

    }

    void Weapon::loadMesh(irr::scene::ISceneManager* a_SceneManager)
    {
        IrrAssimp importer(a_SceneManager);
        m_MeshNode = a_SceneManager->addAnimatedMeshSceneNode(importer.getMesh("Media/Meshes/Knife.3DS"));
    }
}
