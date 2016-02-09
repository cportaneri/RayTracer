//
//  Framework for a raytracer
//  File: box.h
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

#ifndef BOX_H_115209AE
#define BOX_H_115209AE

#include "object.h"

class Box : public Object
{
public:
    Box(Point min,Point max) : min(min), max(max) { }

    virtual Hit intersect(const Ray &ray);

	/* minimum and maximum extent of the box */
    Point min;
    Point max;
};

#endif /* end of include guard: BOX_H_115209AE */
