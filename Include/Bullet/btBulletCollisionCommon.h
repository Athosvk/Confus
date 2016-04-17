/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BULLET_COLLISION_COMMON_H
#define BULLET_COLLISION_COMMON_H

///Common headerfile includes for Bullet Collision Detection

///Bullet's btCollisionWorld and btCollisionObject definitions
#include "Collision/CollisionDispatch/btCollisionWorld.h"
#include "Collision/CollisionDispatch/btCollisionObject.h"

///Collision Shapes
#include "Collision/CollisionShapes/btBoxShape.h"
#include "Collision/CollisionShapes/btSphereShape.h"
#include "Collision/CollisionShapes/btCapsuleShape.h"
#include "Collision/CollisionShapes/btCylinderShape.h"
#include "Collision/CollisionShapes/btConeShape.h"
#include "Collision/CollisionShapes/btStaticPlaneShape.h"
#include "Collision/CollisionShapes/btConvexHullShape.h"
#include "Collision/CollisionShapes/btTriangleMesh.h"
#include "Collision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "Collision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "Collision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "Collision/CollisionShapes/btTriangleMeshShape.h"
#include "Collision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "Collision/CollisionShapes/btCompoundShape.h"
#include "Collision/CollisionShapes/btTetrahedronShape.h"
#include "Collision/CollisionShapes/btEmptyShape.h"
#include "Collision/CollisionShapes/btMultiSphereShape.h"
#include "Collision/CollisionShapes/btUniformScalingShape.h"

///Narrowphase Collision Detector
#include "Collision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"

//#include "BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h"
#include "Collision/CollisionDispatch/btDefaultCollisionConfiguration.h"

///Dispatching and generation of collision pairs (broadphase)
#include "Collision/CollisionDispatch/btCollisionDispatcher.h"
#include "Collision/BroadphaseCollision/btSimpleBroadphase.h"
#include "Collision/BroadphaseCollision/btAxisSweep3.h"
#include "Collision/BroadphaseCollision/btMultiSapBroadphase.h"
#include "Collision/BroadphaseCollision/btDbvtBroadphase.h"

///Math library & Utils
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btIDebugDraw.h"
#include "LinearMath/btSerializer.h"


#endif //BULLET_COLLISION_COMMON_H

