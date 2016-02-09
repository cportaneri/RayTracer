//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "box.h"
#include "plane.h"
#include "triangle.h"
#include "quad.h"
#include "cylinder.h"
#include "mesh.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

std::string parseString(const YAML::Node& node){
	std::string i="";
	node >> i;
	return i;
}

int getArrayElement(const YAML::Node& node, int index){
	return node[index];
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
	if(node.FindValue("color")){
		node["color"] >> m->color;
    }		
	const YAML::Node *textureNode = node.FindValue("texture");
	if (textureNode)
	{
		std::string texture;
		*textureNode >> texture;
		m->texture = new Image(texture.c_str());
	}
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
	node["kt"] >> m->kt;
	node["eta"] >> m->eta;
    node["n"] >> m->n;
    return m;
}

Camera Raytracer::parseCamera(const YAML::Node& node){
	Camera *c = new Camera();
	c->setEye(node["eye"]);
	c->setCenter(node["center"]);
	c->setUp(node["up"]);
	
	c->setViewSize( getArrayElement(node["viewSize"],0),getArrayElement(node["viewSize"],1));
	return *c;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
	std::string shadow;
	
    node["type"] >> objectType;
    
	if (objectType.compare("sphere") == 0) {
        Point pos;
		Vector rot = Vector(0.0,0.0,1.0);
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
		const YAML::Node* rotNode = node.FindValue("rotation");
		if (rotNode != NULL) {
			*rotNode >> rot;
		}
		double angle = 0;
		const YAML::Node* angleNode = node.FindValue("angle");
		if (angleNode != NULL) {
			*angleNode >> angle;
		}
        Sphere *sphere = new Sphere(pos,r,angle,rot);		
        returnObject = sphere;
    }
	else if(objectType.compare("box") == 0) {
        Point min;
        node["min"] >> min;
        Point max;
        node["max"] >> max;
        Box *box = new Box(min,max);		
        returnObject = box;
    }
	else if(objectType.compare("plane") == 0) {
		Point O;
        node["O"] >> O;
        Vector N;
        node["N"] >> N;
        Plane *plane = new Plane(O,N);		
        returnObject = plane;
	}
	else if(objectType.compare("triangle") == 0) {
		Point v1;
        node["v1"] >> v1;
        Point v2;
        node["v2"] >> v2;
		Point v3;
		node["v3"] >> v3;
        Triangle *triangle = new Triangle(v1,v2,v3);		
        returnObject = triangle;
	}
	else if(objectType.compare("quad") == 0) {
		Point p1;
        node["p1"] >> p1;
        Point p2;
        node["p2"] >> p2;
		Point p3;
		node["p3"] >> p3;
		Point p4;
		node["p4"] >> p4;	
		Quad *quad = new Quad(p1,p2,p3,p4);
		returnObject = quad;
	}
		else if(objectType.compare("cylinder") == 0) {
		Point p1;
        node["p1"] >> p1;
        Point p2;
        node["p2"] >> p2;
		double r;
		node["r"] >> r;
		Cylinder *cylinder = new Cylinder(p1,p2,r);
		returnObject = cylinder;
	}
		else if(objectType.compare("mesh") == 0) {
		Point pos;
		node["position"] >> pos;
		std::string file = parseString(node["file"]);
		double size;
		node["size"] >> size;
		Mesh *mesh = new Mesh(pos,file,size);
		returnObject = mesh;
	}


    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
		
		
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

std::vector<std::string> parseSuperSampling(const YAML::Node& node){
	std::vector<std::string> superSampling;
    std::string factor ="";
    node["factor"] >> factor;
	superSampling.push_back(factor);
	std::string pattern ="";
    node["pattern"] >> pattern;
	superSampling.push_back(pattern);
    return superSampling;
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // Read scene configuration options
			Camera c = parseCamera(doc["Camera"]);
            scene->setCamera(c);
			scene->setRenderMode(parseString(doc["RenderMode"]));
			scene->setShadows(parseString(doc["Shadows"]));
			scene->setMaxRecursionDepth(parseString(doc["MaxRecursionDepth"]));
			scene->setSuperSampling(parseSuperSampling(doc["SuperSampling"]));
	    
			// Read Gooch parameters
			if (doc.FindValue("GoochParameters") != NULL)
			{
				double b= 0.55,y=0.3,alpha=0.25,beta=0.5;
				const YAML::Node* bnode = doc["GoochParameters"].FindValue("b");
				if(bnode != NULL) {
					*bnode >> b;
				}
				const YAML::Node* ynode = doc["GoochParameters"].FindValue("y");
				if(ynode != NULL) {
					*ynode >> y;
				}
				const YAML::Node* alphanode = doc["GoochParameters"].FindValue("alpha");
				if(alphanode != NULL) {
					*alphanode >> alpha;
				}
				const YAML::Node* betanode = doc["GoochParameters"].FindValue("beta");
				if(betanode != NULL) {
					*betanode >> beta;
				}		
				scene->setGoochParameters(b,y,alpha,beta);
			}
			else
			{
				scene->setGoochParameters(0.55, 0.3, 0.25, 0.5);
			}
			
            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }
			

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(scene->camera.getViewSizeX() ,scene->camera.getViewSizeY());
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
