//
//  Framework for a raytracer
//  File: plane.h
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

#ifndef PLANE_H_115209AE
#define PLANE_H_115209AE

#include "object.h"

class Plane : public Object
{
public:
    Plane(Point O,Vector N) : O(O), N(N) { }

    virtual Hit intersect(const Ray &ray);

	/* Normal and origine of the normal of the plan */
	Point O;
    Vector N;
   
};

#endif /* end of include guard: PLANE_H_115209AE */
