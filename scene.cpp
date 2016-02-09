//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#define MAX_DISTANCE 1000.0
#define WHITE_COLOR 1

double random(double min, double max){
	return (rand()/(double)RAND_MAX) * (max-min) + min;
}

Color Scene::trace(const Ray &ray, int RecursionDepth)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<=min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector
	V.normalize();
	Color color = material->color;
	
	
    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/
	

		
	Color finalColor = Color(0.0, 0.0, 0.0);
	int mode = 0;
	
	if (renderMode.compare("phong") == 0){
	 mode = 0;
	}
	else if (renderMode.compare("zbuffer") == 0){
	 mode = 1;
	}
	else if (renderMode.compare("normal") == 0){
	 mode = 2;
	}
	else if(renderMode.compare("textureCoordinate") == 0) {
	 mode = 3;
	}
	else if(renderMode.compare("gooch") == 0) {
	 mode = 4;
	}
	
	
	switch (mode) {
	
		/* Phong Illumination mode */
		case 0: 
		{
		
			/* Shadows */
			if (shadows.compare("true") == 0){
			
				/* For each light */
				for (unsigned int i=0; i<lights.size(); i++){
					Light* light = lights[i];
					/* Ray between the light and the hit point */
					Ray ray(light->position, (hit - light->position).normalized());
					/* Intersection distance between the light and the current object */
					double light_obj_distance = obj->intersect(ray).t;
					
					/* For each object on the scene */
					for (unsigned int j = 0; j < objects.size(); j++) {
						/* Except the object where the hit point is */
						if (objects[j] != obj) {
							/* If the intersection distance of the objects[j] is closer than the intersection distance of the current object */
							if (objects[j]->intersect(ray).t < light_obj_distance){			
								/* Return a darker color with the light color*/
								color = color * 0.8 * light->color;
								break;
							}
						}
					}	
					
				}
				
			}
		
			Color light_diffuse;
			Color light_specular;
			Color specular;
			Color diffuse;
			Color ambiant;
			Vector light_reflection_vector;
			
			for (unsigned int i=0; i<lights.size(); i++){
				Light* light = lights[i];
			
					
				/* Calcul lights diffuse */
				Vector l =  -((hit - light->position).normalized());
				light_diffuse +=  max(l.dot(N), 0.0) * light->color;
					
			
				/*Calcul lights specular */
				light_reflection_vector = (2.*max(0.0,N.dot(l))*N-l).normalized();
					
				light_specular += pow(max(0.0,V.dot(light_reflection_vector)),material->n)* light->color;
			
			}
				
			/*Calcul specular */
			specular = material->ks* light_specular;
				
			/* Calcul diffuse */
			diffuse = material->kd * light_diffuse;
				
			/*Calcul ambiant */
			ambiant = material->ka * lights[0]->color;
				
			
			if (material->texture == NULL) {
				finalColor = color * (diffuse+ambiant) + specular;		
			}
			else {
				double u, v;
				obj->computeTextureCoordinate(hit, u, v);
				finalColor = material->texture->colorAt(u, v) * (diffuse+ambiant) + specular;
			}
			
			/* Recursive Reflection */
			
			if (material->ks > 0) {
				if (RecursionDepth>0) {
				
					/* We get the reflective ray*/
					Vector reflection_vector = (2.*max(0.0,N.dot(V))*N-V).normalized();
					Ray reflection_ray( hit + N*1e-12, reflection_vector);
				
					/* Add the value found as a specular component */
					finalColor += material->ks* trace(reflection_ray, RecursionDepth - 1);
				}
			}
			
			/* Recursive Refraction */			
			if (material->kt > 0) {
				if (RecursionDepth>0) {
				
					/* We get the first ray which go into the material */
					Ray transmission_ray( hit - N*0.001, refractionVector(ray.D, N, material->eta));
				
					/* Find the exit of the object through the transmission ray */
					Hit min_hitE(std::numeric_limits<double>::infinity(),Vector());
					for (unsigned int i = 0; i < objects.size(); i++) {
						Hit hitE(objects[i]->intersect(transmission_ray));
						if (hitE.t<=min_hitE.t) {
							min_hitE = hitE;
						}
					}
					
					/* We get the second ray at the outside of the object */
					Vector Ne = min_hitE.N;    
					Point hitE = transmission_ray.at(min_hitE.t);  
					Ray refraction_ray( hitE + Ne*0.001, refractionVector(transmission_ray.D, Ne, material->eta));
					
					/* Add the value found as a specular component */
					finalColor += material->kt*trace(refraction_ray, RecursionDepth - 1);
					
				}
			}

			
			
			
			
			break;
		}	
		
		/*  Z-Buffer grey mode */
		case 1:
		{
			/* F(z): function that calculate the grey level with z the z-distance between the eye and the object 
			* F(MAX_DISTANCE) = 0 (black) / NB: MAX_DISTANCE is defined at the top of this file
			* F(0) = 1 (white)
			*/
			double greyLevel = (- WHITE_COLOR/ MAX_DISTANCE)* (ray.O - hit).z + WHITE_COLOR;

			finalColor = Color(greyLevel, greyLevel, greyLevel );
			break;
		}
		case 2:
		{
			/* F(c): function that calculate the color level (Red or Green or Blue)
			* c value of one of the component of the hit point's normal (x for Red/y for Green/z for Blue)
			* F(1) = 1  (full color)
			* F(-1) = 0 (no color)
			*/
			double redLevel = N.x/2.0+ 0.5;
			double greenLevel = N.y/2.0+ 0.5;
			double blueLevel = N.z/2.0+ 0.5;
			
			finalColor = Color(redLevel ,greenLevel ,blueLevel);
			break;
		}
		
		/* Texture Coordinate */
		case 3:
		{
			double x, y;
			obj->computeTextureCoordinate(hit, x, y);
			
			finalColor = Color(x, 0, y);
			break;
		}
		/*Gooch*/
		case 4:
		{
			/* Shadows */
			if (shadows.compare("true") == 0){
			
				/* For each light */
				for (unsigned int i=0; i<lights.size(); i++){
					Light* light = lights[i];
					/* Ray between the light and the hit point */
					Ray ray(light->position, (hit - light->position).normalized());
					/* Intersection distance between the light and the current object */
					double light_obj_distance = obj->intersect(ray).t;
					
					/* For each object on the scene */
					for (unsigned int j = 0; j < objects.size(); j++) {
						/* Except the object where the hit point is */
						if (objects[j] != obj) {
							/* If the intersection distance of the objects[j] is closer than the intersection distance of the current object */
							if (objects[j]->intersect(ray).t < light_obj_distance){			
								/* Return a darker color with the light color*/
								color = color * 0.2 ;
								break;
							}
						}
					}	
					
				}
				
			}
			
			/* edges */
			double edgeAngle = N.dot(V);
			if (edgeAngle > 0.0 && edgeAngle < 0.25)
			{
				finalColor = Color(0,0,0);
			}
			else {
				Color kD;
				Color light_specular;
				Color specular;
				Color diffuse;
				Color kCool;
				Color kWarm;
				Vector light_reflection_vector;
				
				for (unsigned int i=0; i<lights.size(); i++){
					Light* light = lights[i];
				
					/* Gooch lighting */
					Vector l =  (light->position - hit).normalized();
					double gooshVector = N.dot(l);
					kD = obj->material->kd * color * light->color;
					kCool = Color(0.0, 0.0, goochB) + goochAlpha*kD;
					kWarm = Color(goochY, goochY, 0.0) + goochBeta*kD;
					diffuse += kCool * (1 - gooshVector)/2 + kWarm * (1 + gooshVector)/2;
					
					/*Calcul lights specular */
					light_reflection_vector = (2.*max(0.0,N.dot(l))*N-l).normalized();
					light_specular += pow(max(0.0,V.dot(light_reflection_vector)),material->n)* light->color;
				
				}
					
				/*Calcul specular */
				specular = material->ks* light_specular;
				
				if (obj->material->texture == NULL) {
					finalColor =  diffuse + specular;		
				}
				else {
					double u, v;
					obj->computeTextureCoordinate(hit, u, v);
					finalColor = obj->material->texture->colorAt(u, v) + diffuse + specular;
				}
			}
			
			/* Recursive Reflection */	
			if (material->ks > 0) {
				if (RecursionDepth>0) {
				
					/* We get the reflective ray*/
					Vector reflection_vector = (2.*max(0.0,N.dot(V))*N-V).normalized();
					Ray reflection_ray( hit + N*1e-12, reflection_vector);
				
					/* Add the value found as a specular component */
					finalColor += material->ks* trace(reflection_ray, RecursionDepth - 1);
				}
			}
			
			/* Recursive Refraction */			
			if (material->kt > 0) {
				if (RecursionDepth>0) {
				
					/* We get the first ray which go into the material */
					Ray transmission_ray( hit - N*0.001, refractionVector(ray.D, N, material->eta));
				
					/* Find the exit of the object through the transmission ray */
					Hit min_hitE(std::numeric_limits<double>::infinity(),Vector());
					for (unsigned int i = 0; i < objects.size(); i++) {
						Hit hitE(objects[i]->intersect(transmission_ray));
						if (hitE.t<=min_hitE.t) {
							min_hitE = hitE;
						}
					}
					
					/* We get the second ray at the outside of the object */
					Vector Ne = min_hitE.N;    
					Point hitE = transmission_ray.at(min_hitE.t);  
					Ray refraction_ray( hitE + Ne*0.001, refractionVector(transmission_ray.D, Ne, material->eta));
					
					/* Add the value found as a specular component */
					finalColor += material->kt*trace(refraction_ray, RecursionDepth - 1);
					
				}
			}

			break;
		}
		default:
		{
			break;
		}
	}
	

	
	return finalColor;
}

/* Return the refraction vector */
Vector Scene::refractionVector(Vector incident, Vector N, double eta){
	
	double cosI = incident.dot(N);
	double n1,n2;
					
	/* Ray is inside the material */
	if (cosI > 0){
					
		n1 = eta;
		n2 = 1.0;	
		N = -N;
	}
	/* Ray is outside the material */
	else {
					
		n2 = eta;
		n1 = 1.0;		
		cosI = -cosI;
	}
					
	/* Refracted angle's cosine. */
	double cosT = 1.0 - (n1 / n2) * (n1 / n2) * (1.0 - cosI * cosI);
				
	if (cosT < 0.0){
		/* Total internal reflection */
		return Vector();
	}


	cosT = sqrt(cosT);					
	Vector refraction_vector = incident * (n1 / n2) + N * ((n1 / n2) * cosI - cosT);
	
	return refraction_vector;
}

void Scene::render(Image &img)
{
	srand((unsigned)time(NULL));
	double size = camera.getUp().length();
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
			
			/* Create a list of point depending on the superSamplingFactor */
			std::vector<Point> pixels;
			/* Position for each ray */
			double xp;
			double yp;
			
			/* Grid Pattern */
			if (superSamplingPattern.compare("grid") == 0){
				
				/* space between each ray */
				double a = 1.0/superSamplingFactor*size;
				
				/* Creation of the grid */
				xp = (double) (x)*size + a/2.0 + (camera.getCenter().x - w/2*size);
				yp = (double) (y)*size -a/2.0 - (camera.getCenter().y*size - h/2);
				for (int i = 0; i < superSamplingFactor; i++){				
					 for (int j = 0; j < superSamplingFactor; j++){
						Point pixel(xp, h-1-yp, 0);
						pixels.push_back(pixel);
						yp -= a;
					 }
					xp += a;	
					yp = (double) (y) *size -a/2.0 - (camera.getCenter().y*size - h/2);
				}
				
			}
			
			/* Random Pattern */
			else if (superSamplingPattern.compare("random") == 0){
				
				for (int i = 0; i < superSamplingFactor*superSamplingFactor;i++){	
					xp = random((double)(x), (double)(x) +1.0)*size+ (camera.getCenter().x - w/2*size);
					yp = random((double)(y), (double)(y) +1.0)*size- (camera.getCenter().y*size - h/2);
					Point pixel(xp, h-1-yp, 0);
					pixels.push_back(pixel);
				}
			
			}
			
			/* Jitter Pattern see https://en.wikipedia.org/wiki/Supersampling */
			else if (superSamplingPattern.compare("jitter") ==  0){
			
				/* space between each ray */
				double a = 1.0/superSamplingFactor*size;
				
				/* Creation of the grid */
				
				yp = (double) (y)*size - random(0.0, a)- (camera.getCenter().y*size - h/2);
				for (int i = 0; i < superSamplingFactor; i++){	
					xp = (double) (x)*size + random(i*a,(i+1)*a) + (camera.getCenter().x - w/2*size);
					for (int j = 0; j < superSamplingFactor; j++){
						yp = (double) (y) *size - random(j*a,(j+1)*a) - (camera.getCenter().y*size - h/2);
						Point pixel(xp, h-1-yp, 0);
						pixels.push_back(pixel);
					}
				}

			}
			
			/* Calculate the average value for each ray of the pixel */
			Color col;
			for (unsigned int i = 0; i < pixels.size(); i++){
				Ray ray(camera.getEye(), (pixels[i]-camera.getEye()).normalized());
				col += trace(ray,maxRecursionDepth);
			}
            col = col/pixels.size();
			col.clamp();
			img(x,y) = col;
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setCamera(Camera c)
{
    camera = c;
}

void Scene::setRenderMode(std::string m){
	renderMode = m;
}

void Scene::setShadows(std::string s){
	shadows = s;
}

void Scene::setSuperSampling (std::vector<std::string> m){
    superSamplingFactor = atoi(m[0].c_str());
	superSamplingPattern = m[1];
}

void Scene::setMaxRecursionDepth(std::string m){
	maxRecursionDepth = atoi(m.c_str());
}
