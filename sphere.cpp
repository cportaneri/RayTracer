//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>
#include <stdio.h>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/sphere intersection here.
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

	/* Algebraic method */
	
	double a = ray.D.dot(ray.D);
	double b = ray.D.dot(2.0f * (ray.O - position));
	double c = position.dot(position) + ray.O.dot(ray.O) -2.0f*ray.O.dot(position) - r*r;
	double D = b*b + (-4.0f)*a*c;
	
	// If ray can not intersect then stop
	if (D < 0)
        return Hit::NO_HIT();
	D=sqrtf(D);

	// Ray can intersect the sphere, solve the closer hitpoint
	double tA = (-0.5f)*(b+D)/a;
	if (tA > 0.0) {
        double t =sqrt(a)*tA;  
		
		/****************************************************
		* RT1.2: NORMAL CALCULATION
		*
		* Given: t, C, r
		* Sought: N
		*
		* Insert calculation of the sphere's normal at the intersection point.
		****************************************************/
		Vector N = (ray.at(t) - position).normalized();
		return Hit(t,N);
    }
	else{
       return Hit::NO_HIT();
	}
	
	/* Old Geometric Method */
	
    // Vector OC = position - ray.O;
	// double OClength = OC.length();

	// /* We project OC on the ray direction to get OA using the dot product */
	// double OA = OC.dot(ray.D);
	// /* Use the Pythagore formula to find CA */
	// double CASquare = OClength*OClength - OA*OA;
	// /* We remove the ray that don't intercept the sphere */
	// if (sqrt(CASquare) > r) {
		
		// return Hit::NO_HIT();
	// }
	// /* Use the Pythagore formula to find PA where P is the hit point  */
	// double PA = sqrt(r*r - CASquare );
	// /* Finally we get t the distance between the hit point and the ray origin */
    // double t = OA-PA;

}

void Sphere::computeTextureCoordinate(const Point &p, double &x, double &y)
{
	/* Rotation using the rot vector --> we map it to (0,0,1) with the simplest rotation */
	double angleRot = acos((r*rot).normalized().dot(Vector(0,0,1)));
	/* Rotation matrice */
	Vector r1 = Vector(1,0,0);
	Vector r2 = Vector(0,cos(angleRot),-sin(angleRot));
	Vector r3 = Vector(0,sin(angleRot),cos(angleRot));
	Vector v = p - position;
	Point rotP = Vector(r1.dot(v), r2.dot(v), r3.dot(v)) + position;
	
	double anglePhi;
	double angleTheta;
	/* Minus the rotation with the z axis */
	anglePhi = atan2(rotP.y - position.y, rotP.x - position.x) - (angle*3.14/180);
	angleTheta = acos((rotP.z - position.z)/r);
		
	/* Issues with negative values */
	while (anglePhi < 0.0)
		anglePhi += 2*3.14;
	
	/* To have x and y from 0 to 1 */
	x = anglePhi/(2*3.14);
	y = angleTheta/3.14;
}
