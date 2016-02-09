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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include <string.h>
#include "triple.h"
#include "camera.h"
#include "light.h"
#include "object.h"
#include "image.h"

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
	std::string renderMode; // 0 is phong, 1 is zBuffer, 2 is normalBuffer
	int superSamplingFactor;
	std::string superSamplingPattern;
	std::string shadows;
	int maxRecursionDepth;

	
public:
    Camera camera;
	double goochB, goochY, goochAlpha, goochBeta;
    Color trace(const Ray &ray, int RecursionDepth);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setCamera(Camera c);
	void setRenderMode(std::string m);
	void setShadows(std::string s);
	void setMaxRecursionDepth(std::string m);
	Vector refractionVector(Vector incident, Vector normal, double eta);
	void setSuperSampling(std::vector<std::string> m);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
	void setGoochParameters(double b, double y, double alpha, double beta)
	{ goochB = b; goochY = y; goochAlpha = alpha; goochBeta = beta; }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
