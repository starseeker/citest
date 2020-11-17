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

#ifndef BT_COMPOUND_FROM_GIMPACT
#define BT_COMPOUND_FROM_GIMPACT

#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "btGImpactShape.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"

struct MyCallback : public btTriangleRaycastCallback
		{
			int	m_ignorePart;
			int	m_ignoreTriangleIndex;
			

			MyCallback(const btVector3& from, const btVector3& to, int ignorePart, int ignoreTriangleIndex)
			:btTriangleRaycastCallback(from,to),
			m_ignorePart(ignorePart),
			m_ignoreTriangleIndex(ignoreTriangleIndex)
			{
				
			}
			virtual btScalar reportHit(const btVector3& hitNormalLocal, btScalar hitFraction, int partId, int triangleIndex)
			{
				if (partId!=m_ignorePart || triangleIndex!=m_ignoreTriangleIndex)
				{
					if (hitFraction < m_hitFraction)
						return hitFraction;
				}

				return m_hitFraction;
			}
		};
		struct MyInternalTriangleIndexCallback :public btInternalTriangleIndexCallback
		{
			const btGImpactMeshShape*		m_gimpactShape;
			btCompoundShape*			m_colShape;
			btScalar	m_depth;

			MyInternalTriangleIndexCallback (btCompoundShape* colShape, const btGImpactMeshShape* meshShape, btScalar depth)
			:m_colShape(colShape),
			m_gimpactShape(meshShape),
			m_depth(depth)
			{
			}
			
			virtual void internalProcessTriangleIndex(btVector3* triangle,int partId,int  triangleIndex)
			{
				btVector3 scale = m_gimpactShape->getLocalScaling();
				btVector3 v0=triangle[0]*scale;
				btVector3 v1=triangle[1]*scale;
				btVector3 v2=triangle[2]*scale;
				
				btVector3 centroid = (v0+v1+v2)/3;
				btVector3 normal = (v1-v0).cross(v2-v0);
				normal.normalize();
				btVector3 rayFrom = centroid;
				btVector3 rayTo = centroid-normal*m_depth;
				
				MyCallback cb(rayFrom,rayTo,partId,triangleIndex);
				
				m_gimpactShape->processAllTrianglesRay(&cb,rayFrom, rayTo);
				if (cb.m_hitFraction<1)
				{
					rayTo.setInterpolate3(cb.m_from,cb.m_to,cb.m_hitFraction);
					//rayTo = cb.m_from;
					//rayTo = rayTo.lerp(cb.m_to,cb.m_hitFraction);
					//gDebugDraw.drawLine(tr(centroid),tr(centroid+normal),btVector3(1,0,0));
				}
				

				
				btBU_Simplex1to4* tet = new btBU_Simplex1to4(v0,v1,v2,rayTo);
				btTransform ident;
				ident.setIdentity();
				m_colShape->addChildShape(ident,tet);
			}
		};
		
btCompoundShape*	btCreateCompoundFromGimpactShape(const btGImpactMeshShape* gimpactMesh, btScalar depth)
{
	btCompoundShape* colShape = new btCompoundShape();
		
		btTransform tr;
		tr.setIdentity();
		
		MyInternalTriangleIndexCallback cb(colShape,gimpactMesh, depth);
		btVector3 aabbMin,aabbMax;
		gimpactMesh->getAabb(tr,aabbMin,aabbMax);
		gimpactMesh->getMeshInterface()->InternalProcessAllTriangles(&cb,aabbMin,aabbMax);

	return colShape;	
}	

#endif //BT_COMPOUND_FROM_GIMPACT