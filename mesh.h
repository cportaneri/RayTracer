//
//  Framework for a raytracer
//  File: mesh.h
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

#ifndef MESH_H_115209AE
#define MESH_H_115209AE

#include <string>
#include <vector>
#include "object.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "glm.h"

class Mesh : public Object
{
public:
    Mesh(Point pos, const std::string& filename, double size):  position(pos), size(size) { 
		
		/* Load the mesh */
		GLMmodel *mesh = glmReadOBJ((char *)filename.c_str());
		
		/* Use of a sphere for the intersection */
		glmUnitize(mesh);
		
		/* Scale the mesh */
		glmScale(mesh, (GLfloat)size/1.5);
	
		/* Get the array of triangles and its number in the GLModel */
		unsigned int nbTriangle = 0;
		double *triArray = glmModelIntoArray(mesh, &nbTriangle);
		
		/* Put it in the triangle array of our Mesh object*/
		triangles.reserve(nbTriangle);
		for (unsigned int i=0; i<nbTriangle; i++){
			/* triArray is composed by the triangle points coordinates (9 per triangle) */
			Triangle *tri = new Triangle(Point(triArray[i*9+0], triArray[i*9+1], triArray[i*9+2]) + position,Point(triArray[i*9+3], triArray[i*9+4], triArray[i*9+5]) + position,Point(triArray[i*9+6], triArray[i*9+7], triArray[i*9+8]) + position);
			triangles.push_back(tri);
		}
	
		free(triArray);
		
		/* Add our intersection Sphere */
		intersectSphere = new Sphere(position, size, 0,Vector(0,0,1));	
	}

    virtual Hit intersect(const Ray &ray);

	std::vector<Triangle*> triangles;
	Sphere *intersectSphere;
	
	const Point position;
	const double size;
	
};

#endif /* end of include guard: MESH_H_115209AE */
