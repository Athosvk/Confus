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

    public:
        Weapon(irr::scene::ISceneManager* a_Device);

    private:
        void damagePlayer() const;
        void loadMesh(irr::scene::ISceneManager* a_SceneManager);
    };
}
