//
//  Framework for a raytracer
//  File: box.cpp
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

#include "box.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

/************************** Sphere **********************************/

Hit Box::intersect(const Ray &ray)
{	
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
	* t is the distance between the ray origin and the closest face of the box
    ****************************************************/

	/* Mathematic Method 
	* We consider a cube like the intersection of 3 slabs
	* We first start to calculate the distance between the ray and the 2D slab on x,y of the cube
	* Then repeat it for z
	*/
    double t, tentry, texit, tyentry, tyexit, tzentry, tzexit; 

	/* A box is defined with two point: the minimum and maximum exent points (min and max) */
	/* extents[0] = min extent of the box & extents[1] = max extent of the box*/
	Point extents[2] = {min,max};
	
	/* First we start in 2D 
	* Exemple for tentry initialized with txentry:
	* ray.O.x + txentry*ray.D.x = min.x (= extents[0].x) if ray.D.x >= 0
	* <=> tentryx = (min.x - ray.O.x)/ ray.D.x
	* or
	* ray.O.x + txentry*ray.D.x = max.x (= extents[1].x) if ray.D.x < 0
	* <=> tentryx = (max.x - ray.O.x)/ ray.D.x
	*/
    tentry = (extents[(ray.D.x < 0.0 && ray.D.x != -0.0)].x - ray.O.x) / ray.D.x; 
    texit = (extents[1-(ray.D.x < 0.0 && ray.D.x != -0.0)].x - ray.O.x) / ray.D.x; 
    tyentry = (extents[(ray.D.y < 0.0 && ray.D.y != -0.0)].y - ray.O.y) / ray.D.y; 
    tyexit = (extents[1-(ray.D.y < 0.0 && ray.D.y != -0.0)].y - ray.O.y) / ray.D.y; 
 
	/* Case where there is no Hit*/
    if ((tentry > tyexit) || (tyentry > texit)){ 
        return Hit::NO_HIT();
	}	
	/* Case where the entry point is on y */
    if (tyentry > tentry) {
        tentry = tyentry; 
	}
	/* Case where the exit point is on y */
	if (tyexit < texit) { 
        texit = tyexit; 
    }
 
	/* The we compare tentry and texit with the 3rd dimension */
    tzentry = (extents[(ray.D.z < 0.0 && ray.D.z != -0.0)].z - ray.O.z)  / ray.D.z; 
    tzexit = (extents[1-(ray.D.z < 0.0 && ray.D.z != -0.0)].z - ray.O.z)  / ray.D.z; 
	
	/* Case where there is no Hit*/
    if ((tentry > tzexit) || (tzentry > texit)) 
        return Hit::NO_HIT();
		
	/* Case where the entry point is on z */
    if (tzentry > tentry) {
        tentry = tzentry;
	}
	/* Case where the exit point is on z */
    if (tzexit < texit) {
        texit = tzexit; 
    }
	
	/* Choose t according to the tentry and texit cases*/
	if (tentry <= 0){
		t = texit;
	}
	else{
		t = tentry;
	}	
			

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    ****************************************************/
	Vector N;
	Point a,b;
	Vector v;
	Point center = Point((min.x + max.x)/2.0,(min.y + max.y)/2.0,(min.z + max.z)/2.0);
	Point hit = ray.at(t);
	double side_x = fabs(max.x - min.x)/2.0;
	double side_y = fabs(max.y - min.y)/2.0;
	double side_z = fabs(max.z - min.z)/2.0;
	
	if (hit.y  <= center.y - side_y + 0.1 && hit.y  >= center.y - side_y - 0.1) {
		// Face 6
		a = Point(center.x,center.y - side_y,center.z);
		//cout << "6";
	}
	else if ( hit.y  <= center.y + side_y + 0.1 && hit.y  >= center.y + side_y - 0.1) {
		// Face 3
		a = Point(center.x,center.y + side_y,center.z);
		//cout << "3";
	}
	else if (hit.x  <= center.x - side_x + 0.1 && hit.x  >= center.x - side_x - 0.1) {
		// Face 5
		a = Point(center.x- side_x,center.y,center.z);
		//cout << "5";
	}
	else if ( hit.x  <= center.x + side_x + 0.1 && hit.x  >= center.x + side_x - 0.1) {
		// Face 2
		a = Point(center.x+ side_x,center.y ,center.z);
		//cout << "2";
	}
	else if (hit.z  <= center.z - side_z + 0.1 && hit.z  >= center.z - side_z - 0.1) {
		// Face 4
		a = Point(center.x,center.y,center.z- side_z);
		//cout << "4";
	}
	else if (hit.z  <= center.z + side_z + 0.1 && hit.z  >= center.z + side_z - 0.1) {
		// Face 1
		a = Point(center.x,center.y,center.z+ side_z);
		//cout << "1";
	}
	else {
		int x = center.x + side_x;
		cout << "?- " << hit.x << " c: " << (int)(x) <<"-";
		cout << ((int) (hit.x) >= (int)(x)) <<"-";
	}
	v = hit - a;
	b = center + v;
	N = (hit - b).normalized();
	
	
	
	/* Choose of the good direction 
	if (N.dot(ray.D) > 0){
		N = -N;
	}*/

	//cout << "x :" <<  hit.x <<" y :" <<  hit.y <<" z :" <<  hit.z << "\n" ;

    return Hit(t,N);
}


