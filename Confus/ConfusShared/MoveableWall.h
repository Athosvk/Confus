#pragma once
#include <Irrlicht/irrlicht.h>

namespace ConfusShared
{    
	namespace Physics
	{
		class PhysicsWorld;
		class RigidBody;
	};

    /// <summary>
    /// Represents a wall that transitions into and out of the maze
    /// </summary>
    class MoveableWall
    {
    public:
        /// <summary>
        /// The, maximum, speed at which the wall transitions
        /// </summary>
        float TransitionSpeed = 0.1f;

        /// <summary>
        /// The relative distance, as a percentage, at which the wall becomes solid during transition
        /// <remarks> Measured as difference from the hidden position to the current position </remarks>
        /// </summary>
        float SolifyPoint = 0.2f;

		/// <summary>
		/// The position at which the wall is transparent and not visible within the maze
		/// </summary>
		irr::core::vector3d<float> HiddenPosition;
    private:        
        /// <summary>
        /// The scenenode to represent the wall in Irrlicht
        /// </summary>
        irr::scene::IAnimatedMeshSceneNode* m_MeshNode;

        /// <summary>
        /// The position at which the wall is solid and present in the maze
        /// </summary>
        irr::core::vector3d<float> m_RegularPosition;

        /// <summary>
        /// The position to which the wall is currently transitioning
        /// <remarks> Usually equal to the current position of the wall if 
        /// <see cref="m_Transitioning"/> is false </remarks>
        /// </summary>
        irr::core::vector3d<float> m_TargetPosition;

        /// <summary>
        /// The texture used when the wall is solid present in the maze
        /// </summary>
        irr::video::ITexture* m_RegularTexture;

        /// <summary>
        /// The texture for when the wall is translucent and is transitioning or hidden
        /// </summary>
        irr::video::ITexture* m_TransparentTexture;    

        /// <summary>
        /// Whether the wall is currently transitioning
        /// </summary>
        bool m_Transitioning = false;

		/// <summary>
		/// If the wall is raised or lowered
		/// </summary>
		bool m_Raised = true;
		
		/// <summary> The rigid body for collision detection </summary>
		Physics::RigidBody* m_RigidBody = nullptr;

    public:        
		/// <summary>
		/// Initializes a new instance of the <see cref="MoveableWall" /> class.
		/// </summary>
		/// <param name="a_Device">The current Irrlicht device.</param>
		/// <param name="a_RegularPosition">The position of the wall when present in the maze.</param>
		/// <param name="a_Scale">The scale </param>
		/// <param name="a_PhysicsWorld">The active physics world</param>
		MoveableWall(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RegularPosition,
			irr::core::vector3df a_Scale, Physics::PhysicsWorld& a_PhysicsWorld);

        /// <summary>
        /// Finalizes an instance of the <see cref="MoveableWall"/> class, cleans up Irrlicht resources
        /// </summary>
        ~MoveableWall();

        /// <summary>
        /// Starts the hiding transition
        /// </summary>
        void hide();

		irr::scene::IAnimatedMeshSceneNode* getMeshNode() const { return m_MeshNode; }
		
		/// <summary>
		/// Starts the rising up transition, for moving into the maze
		/// </summary>
        void rise();   

        /// <summary>
        /// The fixed update used to update the state of the wall
        /// </summary>
        void fixedUpdate();
    private:        
        /// <summary>
        /// Loads the necessary textures
        /// </summary>
        /// <param name="a_VideoDriver">The current driver.</param>
        void loadTextures(irr::video::IVideoDriver* a_VideoDriver);

        /// <summary>
        /// Loads the wall mesh into the scene
        /// </summary>
        /// <param name="a_SceneManager">The current scene manager.</param>
        void loadMesh(irr::scene::ISceneManager* a_SceneManager);

        /// <summary>
        /// Updates the position when transitioning
        /// </summary>
        void updatePosition();

        /// <summary>
        /// Updates the transparency, including collision state, when transitioning
        /// </summary>
        void updateTransparency();    

        /// <summary>
        /// Makes the wall solid and enables the collider
        /// </summary>
        void solidify();      

        /// <summary>
        /// Makes the wall transparent and disables the collider
        /// </summary>
        void makeTransparent();

        /// <summary>
        /// Enables the collision.
        /// </summary>
        void enableCollision();   

        /// <summary>
        /// Disables the collision.
        /// </summary>
        void disableCollision();
    };
}