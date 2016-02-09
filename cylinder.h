//
//  Framework for a raytracer
//  File: cylinder.h
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

#ifndef CYLINDER_H_115209AE
#define CYLINDER_H_115209AE

#include "object.h"

class Cylinder : public Object
{
public:
    Cylinder(Point a,Point b, double r) : A(a), B(b), r(r) { }

    virtual Hit intersect(const Ray &ray);
	
	const Point A, B;
	const double r;
};

#endif /* end of include guard: CYLINDER_H_115209AE */
