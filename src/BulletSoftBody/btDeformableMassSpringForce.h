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

#ifndef BT_MASS_SPRING_H
#define BT_MASS_SPRING_H

#include "btDeformableLagrangianForce.h"

class btDeformableMassSpringForce : public btDeformableLagrangianForce
{
    bool m_momentum_conserving;
    btScalar m_elasticStiffness, m_dampingStiffness;
public:
    typedef btAlignedObjectArray<btVector3> TVStack;
    btDeformableMassSpringForce() : m_momentum_conserving(false), m_elasticStiffness(1), m_dampingStiffness(0.05)
    {
    }
    btDeformableMassSpringForce(btScalar k, btScalar d, bool conserve_angular = true) : m_momentum_conserving(conserve_angular), m_elasticStiffness(k), m_dampingStiffness(d)
    {
    }
    
    virtual void addScaledImplicitForce(btScalar scale, TVStack& force)
    {
        addScaledDampingForce(scale, force);
    }
    
    virtual void addScaledExplicitForce(btScalar scale, TVStack& force)
    {
        addScaledElasticForce(scale, force);
    }
    
    virtual void addScaledDampingForce(btScalar scale, TVStack& force)
    {
        int numNodes = getNumNodes();
        btAssert(numNodes <= force.size())
        for (int i = 0; i < m_softBodies.size(); ++i)
        {
            const btSoftBody* psb = m_softBodies[i];
            for (int j = 0; j < psb->m_links.size(); ++j)
            {
                const btSoftBody::Link& link = psb->m_links[j];
                btSoftBody::Node* node1 = link.m_n[0];
                btSoftBody::Node* node2 = link.m_n[1];
                size_t id1 = node1->index;
                size_t id2 = node2->index;
                
                // damping force
                btVector3 v_diff = (node2->m_v - node1->m_v);
                btVector3 scaled_force = scale * m_dampingStiffness * v_diff;
                if (m_momentum_conserving)
                {
                    if ((node2->m_q - node1->m_q).norm() > SIMD_EPSILON)
                    {
                        btVector3 dir = (node2->m_x - node1->m_x).normalized();
                        scaled_force = scale * m_dampingStiffness * v_diff.dot(dir) * dir;
                    }
                }
                force[id1] += scaled_force;
                force[id2] -= scaled_force;
            }
        }
    }
    
    virtual void addScaledElasticForce(btScalar scale, TVStack& force)
    {
        int numNodes = getNumNodes();
        btAssert(numNodes <= force.size())
        for (int i = 0; i < m_softBodies.size(); ++i)
        {
            const btSoftBody* psb = m_softBodies[i];
            for (int j = 0; j < psb->m_links.size(); ++j)
            {
                const btSoftBody::Link& link = psb->m_links[j];
                btSoftBody::Node* node1 = link.m_n[0];
                btSoftBody::Node* node2 = link.m_n[1];
                btScalar r = link.m_rl;
                size_t id1 = node1->index;
                size_t id2 = node2->index;
                
                // elastic force
                // explicit elastic force
                btVector3 dir = (node2->m_q - node1->m_q);
                btVector3 dir_normalized = (dir.norm() > SIMD_EPSILON) ? dir.normalized() : btVector3(0,0,0);
                btVector3 scaled_force = scale * m_elasticStiffness * (dir - dir_normalized * r);
                force[id1] += scaled_force;
                force[id2] -= scaled_force;
            }
        }
    }
    
    virtual void addScaledForceDifferential(btScalar scale, const TVStack& dv, TVStack& df)
    {
        // implicit damping force differential
        for (int i = 0; i < m_softBodies.size(); ++i)
        {
            const btSoftBody* psb = m_softBodies[i];
            btScalar scaled_k_damp = m_dampingStiffness * scale;
            for (int j = 0; j < psb->m_links.size(); ++j)
            {
                const btSoftBody::Link& link = psb->m_links[j];
                btSoftBody::Node* node1 = link.m_n[0];
                btSoftBody::Node* node2 = link.m_n[1];
                size_t id1 = node1->index;
                size_t id2 = node2->index;

                btVector3 local_scaled_df = scaled_k_damp * (dv[id2] - dv[id1]);
                if (m_momentum_conserving)
                {
                    if ((node2->m_q - node1->m_q).norm() > SIMD_EPSILON)
                    {
                        btVector3 dir = (node2->m_x - node1->m_x).normalized();
                        local_scaled_df= scaled_k_damp * (dv[id2] - dv[id1]).dot(dir) * dir;
                    }
                }
                df[id1] += local_scaled_df;
                df[id2] -= local_scaled_df;
            }
        }
    }
    
    virtual btDeformableLagrangianForceType getForceType()
    {
        return BT_MASSSPRING_FORCE;
    }
    
};

#endif /* btMassSpring_h */
