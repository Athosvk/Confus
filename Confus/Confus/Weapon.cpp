#include <Irrlicht/irrlicht.h>
#include <math.h>

#include "Weapon.h"
#include "../ConfusShared/Physics/RigidBody.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include <iostream>

namespace Confus
{
    Weapon::Weapon(irr::scene::ISceneManager* a_SceneManager, Physics::PhysicsWorld& a_World,
		irr::core::vector3df a_Dimensions)
    {
        m_Node = a_SceneManager->addCubeSceneNode(1.0f, nullptr, -1, irr::core::vector3df(), irr::core::vector3df(),
            a_Dimensions);
        m_Node->setVisible(false);

		m_Collider = a_World.createBoxCollider(a_Dimensions, m_Node, Physics::ECollisionFilter::Interactable,
			Physics::ECollisionFilter::Player);
		m_Collider->getRigidBody()->makeKinematic();
		m_Collider->getRigidBody()->enableTriggerState();
        m_Collider->setTriggerEnterCallback([this](Physics::BoxCollider* a_Other)
        {
            if(!m_Collided)
            {
                m_Collided = true;
                damagePlayer(a_Other->getRigidBody()->getAttachedNode());
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
    }

    void Weapon::disableCollider()
    {
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
