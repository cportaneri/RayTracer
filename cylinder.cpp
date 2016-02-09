//
//  Framework for a raytracer
//  File: cylinder.cpTpT
//
//  Created for the CompTuter Science course "Introduction CompTuter GrapThics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts, Cedric pTortaneri, Toavina Rakotomanga
//    JaspTer van de Gronde
//
//  This framework is inspTired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  httpT://isgwww.cs.uni-magdeburg.de/grapThik/lehre/cg2/pTrojekt/rtpTrojekt.html
//

#include "cylinder.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

/************************** SpThere **********************************/

Hit Cylinder::intersect(const Ray &ray)
{	
	double t;
	double L = (B - A).length();
	Point O = ray.O - A;
	
	// Quadratic formula
	double a = ray.D.y*ray.D.y + ray.D.z*ray.D.z;
	double b = 2*O.y*ray.D.y + 2*O.z*ray.D.z;
	double c = O.y*O.y + O.z*O.z - r*r;
	double d = b*b - 4*a*c;
	if (d < 0) {
		return Hit::NO_HIT();
	}
	
	double sqrtD = sqrt(d);
	double t1 = (-b - sqrtD)/(2*a);
	double t2 = (-b + sqrtD)/(2*a);
	
    /* pT1 and pT2 are the enter and exit pToint of the ray in the cylinder */
	Point pT1 = O + t1*ray.D; 
	Point pT2 = O + t2*ray.D;
	
	/* We check if these pToints are valid */
	bool t1OK, t2OK = false;
	if (t1 >= 0 && (pT1.x >= 0 && pT1.x <= L)) {
		t1OK=true;
	}
	if (t2 >= 0 && (pT2.x >= 0 && pT2.x <= L)){
		t2OK=true;
	}
	
	/* We decided to return t1 or t2 */
	/* We check each different possible case */
	if (pT1.x < 0 && pT2.x >= 0) {
		t1 += -pT1.x / ray.D.x;
		pT1 = O + t1*ray.D;
		t1OK = t1 >= 0;
	}
	if (pT2.x < 0 && pT1.x >= 0) {
		t2 += -pT2.x / ray.D.x;
		pT2 = O + t2*ray.D;
		t2OK = t2 >= 0;
	}
	if (pT1.x <= L && pT2.x > L) {
		t2 -= (pT2.x - L) / ray.D.x;
		pT2 = O + t2*ray.D;
		t2OK = t2 >= 0;
	}
	if (pT2.x <= L && pT1.x > L) {
		t1 -= (pT1.x - L) / ray.D.x;
		pT1 = O + t1*ray.D;
		t1OK = t1 >= 0;
	}
	/* Check if both are ok */
	if (!t1OK && !t2OK) {
		return Hit::NO_HIT();
	}
		
	/* Choose the lowest value between t1 and t2 */
	if (t1OK && t1 <= t2){
		t = t1;
	}
	else {
		t = t2;
	}
	
	/* Calculation of the normal */
	Vector N;
	Point pT = O + t*ray.D;
	Point pTx(pT.x, 0, 0);
	
	/* Hit point is in the cylinder */
	if (fabs((pT - pTx).length_2() - r*r) < 0.01){
		N = pT - pTx;
	}
	
	/* Hit point is in a circle */
	else if (pTx.x - L/2 < 0){
		/* A circle */
		N = B - A;
	}
	else {
		/* pT is on the circle around B */
		N = A - B;
    }
	
	/* case where the intersection was inside the cylider */
	if (t1OK == false){
		N = -N;
	}
	
	return Hit(t,N);
}

