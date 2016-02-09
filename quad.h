//
//  Framework for a raytracer
//  File: quad.h
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

#ifndef QUAD_H_115209AE
#define QUAD_H_115209AE

#include "object.h"
#include "triangle.h"

class Quad : public Object
{
public:
    Quad(Point pp1, Point pp2, Point pp3, Point pp4) :
		p1(pp1), p2(pp2), p3(pp3), p4(pp4)
	{
		t1 = new Triangle(p1, p2, p3);
		t2 = new Triangle(p1, p3, p4);
	}
	
    virtual Hit intersect(const Ray &ray);

	Point p1, p2, p3, p4;
	Triangle *t1, *t2;
};

#endif /* end of include guard: QUAD_H_115209AE */
