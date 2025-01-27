/*
 Written by Xuchen Han <xuchenhan2015@u.northwestern.edu>
 
 Bullet Continuous Collision Detection and Physics Library
 Copyright (c) 2019 Google Inc. http://bulletphysics.org
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef BT_DEFORMABLE_RIGID_DYNAMICS_WORLD_H
#define BT_DEFORMABLE_RIGID_DYNAMICS_WORLD_H

#include "btSoftMultiBodyDynamicsWorld.h"
#include "btDeformableLagrangianForce.h"
#include "btDeformableMassSpringForce.h"
#include "btDeformableBodySolver.h"
#include "btDeformableMultiBodyConstraintSolver.h"
#include "btSoftBodyHelpers.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"
#include <functional>
typedef btAlignedObjectArray<btSoftBody*> btSoftBodyArray;

class btDeformableBodySolver;
class btDeformableLagrangianForce;
struct MultiBodyInplaceSolverIslandCallback;
class btDeformableMultiBodyConstraintSolver;

typedef btAlignedObjectArray<btSoftBody*> btSoftBodyArray;

class btDeformableMultiBodyDynamicsWorld : public btMultiBodyDynamicsWorld
{
    typedef btAlignedObjectArray<btVector3> TVStack;
//    using TVStack = btAlignedObjectArray<btVector3>;
    ///Solver classes that encapsulate multiple deformable bodies for solving
    btDeformableBodySolver* m_deformableBodySolver;
    btSoftBodyArray m_softBodies;
    int m_drawFlags;
    bool m_drawNodeTree;
    bool m_drawFaceTree;
    bool m_drawClusterTree;
    btSoftBodyWorldInfo m_sbi;
    btScalar m_internalTime;
    int m_contact_iterations;
    
    typedef void (*btSolverCallback)(btScalar time, btDeformableMultiBodyDynamicsWorld* world);
    btSolverCallback m_solverCallback;
    
protected:
    virtual void internalSingleStepSimulation(btScalar timeStep);
    
    virtual void integrateTransforms(btScalar timeStep);
    
    void positionCorrection(btScalar timeStep);
    
    void solveConstraints(btScalar timeStep);
    
public:
    btDeformableMultiBodyDynamicsWorld(btDispatcher* dispatcher, btBroadphaseInterface* pairCache, btDeformableMultiBodyConstraintSolver* constraintSolver, btCollisionConfiguration* collisionConfiguration, btDeformableBodySolver* deformableBodySolver = 0)
    : btMultiBodyDynamicsWorld(dispatcher, pairCache, (btMultiBodyConstraintSolver*)constraintSolver, collisionConfiguration),
    m_deformableBodySolver(deformableBodySolver), m_solverCallback(0)
    {
        m_drawFlags = fDrawFlags::Std;
        m_drawNodeTree = true;
        m_drawFaceTree = false;
        m_drawClusterTree = false;
        m_sbi.m_broadphase = pairCache;
        m_sbi.m_dispatcher = dispatcher;
        m_sbi.m_sparsesdf.Initialize();
        m_sbi.m_sparsesdf.setDefaultVoxelsz(0.025);
        m_sbi.m_sparsesdf.Reset();
        
        m_sbi.air_density = (btScalar)1.2;
        m_sbi.water_density = 0;
        m_sbi.water_offset = 0;
        m_sbi.water_normal = btVector3(0, 0, 0);
        m_sbi.m_gravity.setValue(0, -10, 0);
        m_internalTime = 0.0;
    }

    void setSolverCallback(btSolverCallback cb)
    {
        m_solverCallback = cb;
    }
    
    virtual ~btDeformableMultiBodyDynamicsWorld()
    {
    }
    
    virtual btMultiBodyDynamicsWorld* getMultiBodyDynamicsWorld()
    {
        return (btMultiBodyDynamicsWorld*)(this);
    }
    
    virtual const btMultiBodyDynamicsWorld* getMultiBodyDynamicsWorld() const
    {
        return (const btMultiBodyDynamicsWorld*)(this);
    }
    
    virtual btDynamicsWorldType getWorldType() const
    {
        return BT_DEFORMABLE_MULTIBODY_DYNAMICS_WORLD;
    }
    
    virtual void predictUnconstraintMotion(btScalar timeStep);
    
    virtual void addSoftBody(btSoftBody* body, int collisionFilterGroup = btBroadphaseProxy::DefaultFilter, int collisionFilterMask = btBroadphaseProxy::AllFilter);
    
    btSoftBodyArray& getSoftBodyArray()
    {
        return m_softBodies;
    }
    
    const btSoftBodyArray& getSoftBodyArray() const
    {
        return m_softBodies;
    }
    
    btSoftBodyWorldInfo& getWorldInfo()
    {
        return m_sbi;
    }
    
    const btSoftBodyWorldInfo& getWorldInfo() const
    {
        return m_sbi;
    }
    
    void reinitialize(btScalar timeStep);
    
    void applyRigidBodyGravity(btScalar timeStep);
    
    void beforeSolverCallbacks(btScalar timeStep);
    
    void afterSolverCallbacks(btScalar timeStep);
    
    void addForce(btSoftBody* psb, btDeformableLagrangianForce* force);
    
    void removeSoftBody(btSoftBody* body);
    
    int getDrawFlags() const { return (m_drawFlags); }
    void setDrawFlags(int f) { m_drawFlags = f; }
    
    void setupConstraints();
    
    void solveMultiBodyConstraints();
    
    void solveMultiBodyRelatedConstraints();
    
    void sortConstraints();
};

#endif  //BT_DEFORMABLE_RIGID_DYNAMICS_WORLD_H
