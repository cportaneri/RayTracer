//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef CAMERA_H_KNBLQLP6
#define CAMERA_H_KNBLQLP6

#include <vector>
#include <string.h>
#include "triple.h"

class Camera
{
private:
    Point eye;
    Point center;
    Vector up;
	int viewSizeX;
	int viewSizeY;

	
public:
    void setEye(Triple e);
    void setCenter(Triple e);
    void setUp(Triple e);
	void setViewSize(int x, int y);    
	Point getEye();
    Point getCenter();
    Vector getUp();
	int getViewSizeX();
	int getViewSizeY();
};

#endif /* end of include guard: CAMERA_H_KNBLQLP6 */
