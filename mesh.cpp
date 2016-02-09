//
//  Framework for a raytracer
//  File: mesh.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts, Cedric Portaneri, Toavina Rakotomanga
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//


#include "mesh.h"


/************************** Sphere **********************************/

Hit Mesh::intersect(const Ray &ray)
{	
   	/* Use the sphere */
	Hit sphere_hit = intersectSphere->intersect(ray);
	/* If it doesn't hit the sphere then no hit */
	if (!(sphere_hit.t>0)) {
		return Hit::NO_HIT();
    }
	
	/* Find the minimum hit with the triangles */
	Hit minHit(std::numeric_limits<double>::infinity(),Vector());
	bool hasHit = false;
	for (unsigned int i = 0; i < triangles.size(); i++) {
	
		Hit hit = triangles[i]->intersect(ray);
		if (hit.t > 0  && hit.t < minHit.t ) {
			minHit = hit;
			hasHit = true;
		}
		
	}
	if (!hasHit) {
		return Hit::NO_HIT();
	}
	
	return minHit;
}

