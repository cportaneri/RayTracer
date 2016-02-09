//
//  Framework for a raytracer
//  File: plane.cpp
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

#include "plane.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

/************************** Sphere **********************************/

Hit Plane::intersect(const Ray &ray)
{	
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
	* t is the distance between the ray origin and the intersection hit point
    ****************************************************/
		
	/** Plane intersection **/
	double 	denominator,t;
	Point hit;
	
	/* Check if the ray and the face are not parallel*/ 
	denominator = N.dot(ray.D);
	if (fabs(denominator) > 1e-6) 
	{
		
		hit = (O - N.dot(ray.O)) / denominator;
		t = (hit - ray.O).length();
	}
	else {
		return Hit::NO_HIT();
	}
		
	

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    ****************************************************/
	Vector Nreturn;
	if (denominator > 0){
		Nreturn = -N;
	}
	else{
		Nreturn = N;
	}

    return Hit(t,Nreturn);
}
