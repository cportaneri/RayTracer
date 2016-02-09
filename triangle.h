//
//  Framework for a raytracer
//  File: triangle.h
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

#ifndef TRIANGLE_H_115209AE
#define TRIANGLE_H_115209AE

#include "object.h"

class Triangle : public Object
{
public:
    Triangle(Point v1,Point v2, Point v3) : v1(v1), v2(v2), v3(v3) { }

    virtual Hit intersect(const Ray &ray);

	/* vertices of the triangle */
    Point v1;
	Point v2; 
	Point v3;
};

#endif /* end of include guard: TRIANGLE_H_115209AE */
