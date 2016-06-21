#include <Irrlicht/irrlicht.h>

#include "Weapon.h"
#include "Physics/RigidBody.h"
#include "Physics/PhysicsWorld.h"
#include "EHitIdentifier.h"
//Refactor to shared player
#include "Player.h"

namespace ConfusShared
{
    Weapon::Weapon(irr::scene::ISceneManager* a_SceneManager, Physics::PhysicsWorld& a_World,
		irr::core::vector3df a_Dimensions, Player& a_AttachedPlayer) 
        : m_AttachedPlayer(a_AttachedPlayer)
    {
        m_Node = a_SceneManager->addCubeSceneNode(1.0f, nullptr, -1, irr::core::vector3df(), irr::core::vector3df(),
            a_Dimensions);
        m_Node->setVisible(false);
		m_Node->setPosition(irr::core::vector3df(0.0f, 0.0f, -1.2f));

		m_Collider = a_World.createBoxCollider(a_Dimensions, m_Node, Physics::ECollisionFilter::Interactable,
			Physics::ECollisionFilter::Player);
		m_Collider->getRigidBody()->makeKinematic();
		m_Collider->getRigidBody()->enableTriggerState();
        m_Collider->setTriggerEnterCallback([this](Physics::BoxCollider* a_Other)
        {
            if(!m_Collided)
            {
				auto playerNode = dynamic_cast<Player*>(a_Other->getRigidBody()->getAttachedNode());
				if(playerNode != nullptr && playerNode->getTeamIdentifier() != m_AttachedPlayer.getTeamIdentifier())
				{
					damagePlayer(playerNode);
                    m_AttachedPlayer.stopAttacking();
					m_Collided = true;
				}
            }
        });
    }

	Weapon::~Weapon()
	{
		m_Collider->remove();
	}

	void Weapon::damagePlayer(Player* a_Player) const
    {
        bool backHit = getAngle(a_Player->getPosition(), m_Node->getPosition()) <= (180.0f - BackstabAngle);
        if(Damage == Player::LightAttackDamage)
        {
            a_Player->getHealthInstance()->damage(backHit ? Damage * 2 : Damage, backHit ? EHitIdentifier::LightBackstab : EHitIdentifier::Light);
        }
        else if (Damage == Player::HeavyAttackDamage)
        {
            a_Player->getHealthInstance()->damage(backHit ? Damage * 2 : Damage, backHit ? EHitIdentifier::HeavyBackstab : EHitIdentifier::Heavy);
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
