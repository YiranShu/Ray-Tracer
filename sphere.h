/**********************************************************************
 * Some stuff to handle spheres
 **********************************************************************/
#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "vector.h"

class Spheres {
public:
	int index;               // identifies a sphere; must be greater than 0

	Point center;
	float radius;

	float mat_ambient[3];    // material property used in Phong model
	float mat_diffuse[3];
	float mat_specular[3];
	float mat_shineness;

	float reflectance;       // this number [0,1] determines how much 
                           // reflected light contributes to the color
                           // of a pixel
	Spheres *next;
};   // a list of spheres

// intersect ray with sphere
Spheres *intersect_scene(Point, Vector, Spheres *, Point *, bool);
// return the unit normal at a point on sphere
Vector sphere_normal(Point, Spheres *);
// add a sphere to the sphere list
Spheres *add_sphere(Spheres *, Point, float, float [], float [], float [], float, float, int);

#endif



