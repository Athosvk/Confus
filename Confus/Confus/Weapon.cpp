#include <Irrlicht/irrlicht.h>
#include <math.h>
#include <iostream>
#include <string>
#include "Weapon.h"
#include "../ConfusShared/Physics/RigidBody.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "../ConfusShared/EHitIdentifier.h"
#include "Player.h"

namespace Confus
{
    Weapon::Weapon(irr::scene::ISceneManager* a_SceneManager, Physics::PhysicsWorld& a_World,
		irr::core::vector3df a_Dimensions, ETeamIdentifier a_TeamIdentifier) : m_TeamIdentifier(a_TeamIdentifier)
    {
        m_Node = a_SceneManager->addCubeSceneNode(1.0f, nullptr, -1, irr::core::vector3df(), irr::core::vector3df(),
            a_Dimensions);
        m_Node->setVisible(false);
		m_Node->setPosition(irr::core::vector3df(0.0f, 0.0f, -1.2f));
        m_TeamIdentifier = a_TeamIdentifier;
		m_Collider = a_World.createBoxCollider(a_Dimensions, m_Node, Physics::ECollisionFilter::Interactable,
			Physics::ECollisionFilter::Player);
		m_Collider->getRigidBody()->makeKinematic();
		m_Collider->getRigidBody()->enableTriggerState();
        m_Collider->setTriggerEnterCallback([this](Physics::BoxCollider* a_Other)
        {
            if(!m_Collided)
            {
				auto playerNode = dynamic_cast<Player*>(*a_Other->getRigidBody()->getAttachedNode()->getChildren().begin());
				if(playerNode != nullptr)
				{
					damagePlayer(playerNode);
					m_Collided = true;
				}
            }
        });
    }

    void Weapon::damagePlayer(Player* a_Player) const
    {
        // Remove iostream include with this comment.
        std::cout << "Player team identifier is " + static_cast<char>(a_Player->TeamIdentifier) << " \n our team id is " + static_cast<char>(m_TeamIdentifier);
        // Only do damage if the other player is in another team than us.
        if(a_Player->TeamIdentifier != m_TeamIdentifier)
        {
            if(getAngle(a_Player->getPosition(), m_Node->getPosition()) <= (180.0f - BackstabAngle))
            {
			if (Damage == Player::LightAttackDamage)
				a_Player->getHealthInstance()->damage(Damage*2, EHitIdentifier::LightBackstab);
			else
				a_Player->getHealthInstance()->damage(Damage*2, EHitIdentifier::HeavyBackstab);
            }
            else
            {
			if(Damage == Player::LightAttackDamage)
				a_Player->getHealthInstance()->damage(Damage,EHitIdentifier::Light);
			else
				a_Player->getHealthInstance()->damage(Damage, EHitIdentifier::Heavy);
            }
        }
    }

    float Weapon::getAngle(irr::core::vector3df a_Vector1, irr::core::vector3df a_Vector2) const
    {
        return irr::core::radToDeg(acos(a_Vector1.dotProduct(a_Vector2)));
    }

    void Weapon::enableCollider()
    {
		m_Collider->getRigidBody()->activate();
    }

    void Weapon::disableCollider()
    {
		m_Collider->getRigidBody()->deactivate();
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
