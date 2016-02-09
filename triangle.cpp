//
//  Framework for a raytracer
//  File: triangle.cpp
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

#include "triangle.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

/************************** Sphere **********************************/

Hit Triangle::intersect(const Ray &ray)
{	
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
	* t is the distance between the ray origin and the intersection hit point
    ****************************************************/

	/* Mathematical Solution */
	/* We consider p1=A p2=B p3=C */	
	double ABx = v1.x - v2.x; 
	double ABy = v1.y - v2.y; 
	double ABz = v1.z - v2.z;
	double ACx = v1.x - v3.x; 
	double ACy = v1.y - v3.y; 
	double ACz = v1.z - v3.z;
	
	double OAx = v1.x - ray.O.x; 
	double OAy = v1.y - ray.O.y; 
	double OAz = v1.z - ray.O.z;

	/* Calcul of the distance */
	double dividende = ABx*(ACy*ray.D.z - ray.D.y*ACz) + ABy*(ray.D.x*ACz - ACx*ray.D.z) + ABz*(ACx*ray.D.y - ACy*ray.D.x);
	double t = -(ACz*(ABx*OAy - OAx*ABy) + ACy*(OAx*ABz - ABx*OAz) + ACx*(ABy*OAz - OAy*ABz))/dividende;	
	if (t < 0) {
		return Hit::NO_HIT();
	}
	
	/* Other cases of No Hit */
	double angle1 = (ray.D.z*(ABx*OAy - OAx*ABy) + ray.D.y*(OAx*ABz - ABx*OAz) + ray.D.x*(ABy*OAz - OAy*ABz))/dividende;
	if (angle1 < 0 || angle1 > 1) {
		return Hit::NO_HIT();
	}
	double angle2 = (OAx*(ACy*ray.D.z - ray.D.y*ACz) + OAy*(ray.D.x*ACz - ACx*ray.D.z) + OAz*(ACx*ray.D.y - ACy*ray.D.x))/dividende;
	if (angle2 < 0 || angle2 > (1 - angle1)){
		return Hit::NO_HIT();
	}
	
	/* Calcul of the normal */
	Vector N = ((v2 - v1).cross(v3 - v1)).normalized();
	
	return Hit(t,N);
	
	
	/* Geometrical Solution */
	/* Möller–Trumbore intersection algorithm 
	
	Vector e1, e2; 
	Vector A, B, N;
	Point T;
	double determinat, inv_determinat, u, v;
	double t;

	//Find vectors for two edges sharing one vertex v1
	e1 = v2 - v1;
	e2 = v3 - v1;
	
	// A is used to calculate the determinant and the u parameter
	A = ray.D.cross(e2);
	//if determinant is near zero, ray lies in plane of triangle
	determinat = e1.dot(A);
	
	if (fabs(determinat) > 1e-6) { 
	
		inv_determinat = 1.f / determinat;

		//vector from v1 to ray origin
		T = ray.O - v1;
		//Calculate u parameter and test bound
		u = T.dot(A) * inv_determinat;
		//The intersection lies outside of the triangle
		if(u < 0.f || u > 1.f) return Hit::NO_HIT();

		//B is to calculate the v parameter and the distance
		B =  T.cross(e1);
		//Calculate V parameter and test bound
		v = ray.D.dot(B) * inv_determinat;
		//The intersection lies outside of the triangle
		if(v < 0.f || u + v  > 1.f) return Hit::NO_HIT();
		
		t = e2.dot(B) * inv_determinat;
		if(t > 0.0001) { 
		
			// Calcul of the normal
			N = e1.cross(e2);
			if (N.dot(ray.D)>0){
				N = -N;
			}
			
			return Hit(t,N);
			
		}		
	}
	
	return Hit::NO_HIT();
	*/
	
    
}

