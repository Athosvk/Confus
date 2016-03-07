#pragma once
#include <Irrlicht/irrlicht.h>
/// <summary>
/// Class StaticWall places a mesh with a collider the player cannot go through. 
/// </summary>
class StaticWall
{
    irr::scene::ISceneManager* m_SceneManager;
    irr::scene::IAnimatedMesh* m_MeshNode;
    irr::scene::ISceneNode* m_SceneNode;
    irr::scene::ICameraSceneNode* m_Camera;
    irr::video::IVideoDriver* m_VideoDriver;

public:    
    /// <summary>
    /// Initializes a new instance of the <see cref="StaticWall"/> class.
    /// </summary>
    /// <param name="a_Device">The active irrlichtdevice in this context.</param>
    /// <param name="a_Position">The position of the wall in the world.</param>
    /// <param name="a_Rotation">The rotation of the wall in the world.</param>
    /// <param name="a_Camera">The player camera to create an automatic collision with.</param>
    StaticWall(irr::IrrlichtDevice* a_Device, irr::core::vector3d<float> a_Position, irr::core::vector3d<float> a_Rotation, irr::scene::ICameraSceneNode* a_Camera);    
    /// <summary>
    /// Finalizes an instance of the <see cref="StaticWall"/> class.
    /// </summary>
    ~StaticWall();

    /// <summary>
    /// Loads the mesh.
    /// </summary>
    void loadMesh();    
    /// <summary>
    /// Assigns the texture to the scene node.
    /// </summary>
    void setTexture();    
    /// <summary>
    /// Configures the scene node.
    /// </summary>
    /// <param name="a_Position">The world position of the scene node.</param>
    /// <param name="a_Rotation">The world rotation of the scene node.</param>
    void setSceneNode(irr::core::vector3d<float> a_Position, irr::core::vector3d<float> a_Rotation);    
    /// <summary>
    /// Creates a collision response animator between the player and the wall.
    /// </summary>
    void addMeshCollider();    
    /// <summary>
    /// Adds the light to the front of the wall.
    /// </summary>
    /// <param name="a_Position">The position of the light relative to the wall.</param>
    void addLight(irr::core::vector3df a_Position);
};

