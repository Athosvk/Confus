#pragma once
#include <memory>

#include "Collider.h"

namespace Confus
{
    class Weapon
    {
    private:
        irr::scene::IAnimatedMeshSceneNode* m_MeshNode = nullptr;
        std::unique_ptr<Collider> m_Collider = nullptr;
        irr::scene::ITriangleSelector* m_TriangleSelector;

    public:
        Weapon(irr::scene::ISceneManager* a_Device);

        void enableCollider();
        void disableCollider();
    private:
        void damagePlayer() const;
        void loadMesh(irr::scene::ISceneManager* a_SceneManager);
    };
}
