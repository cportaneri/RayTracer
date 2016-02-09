//
//  Framework for a raytracer
//  File: quad.cpp
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

#include "quad.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

/************************** Quad **********************************/

Hit Quad::intersect(const Ray &ray)
{	
   /* We take the closest hit between the two triangles */
	Hit h1 = t1->intersect(ray);
	Hit h2 = t2->intersect(ray);
	/* If h1 or h2 are NoHit, in order to compare them we add the infinity value to their t*/
	if (!(h1.t > 0)){
		h1 = Hit(std::numeric_limits<double>::infinity(),h1.N);
	}
	if (!(h2.t > 0)){
		h2 = Hit(std::numeric_limits<double>::infinity(),h2.N);
	}
	
	if (h1.t < h2.t) {
		return h1;
	}
	else {
		return h2;
	}
}

