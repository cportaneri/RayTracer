//
//  Framework for a raytracer
//  File: camera.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts , Cedric Portaneri, Toavina Rakotomanga
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "camera.h"
#include <stdio.h>
#include <stdlib.h> 

Point Camera::getEye(){
	return eye;
}

Point Camera::getCenter(){
	return center;
}

Vector Camera::getUp(){
	return up;
}

int Camera::getViewSizeX(){
	return viewSizeX;
}

int Camera::getViewSizeY(){
	return viewSizeY;
}

void Camera::setEye(Triple e)
{
    eye = e;
}

void Camera::setCenter(Triple e)
{
	center = e;
}

void Camera::setUp(Triple e)
{
	up = e;
}

void Camera::setViewSize(int x, int y)
{
	viewSizeX = x;
	viewSizeY = y;
}