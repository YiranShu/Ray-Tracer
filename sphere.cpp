#include "sphere.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>

/**********************************************************************
 * This function intersects a ray with a given sphere 'sph'. You should
 * use the parametric representation of a line and do the intersection.
 * The function should return the parameter value for the intersection, 
 * which will be compared with others to determine which intersection
 * is closest. The value -1.0 is returned if there is no intersection
 *
 * If there is an intersection, the point of intersection should be
 * stored in the "hit" variable
 **********************************************************************/
float intersect_sphere(Point o, Vector u, Spheres *sph, Point *hit, bool isRefraction) {
	float a = vec_dot(u, u);
	Vector co = get_vec(sph->center, o); //o - sph->center;
	float b = 2 * vec_dot(u, co);
	float c = vec_dot(co, co) - sph->radius * sph->radius;

	float disc = b * b - 4 * a * c;
	if(disc < 1e-3) {
		return -1.0;
	}

	disc = sqrt(disc * 1.0);
	float r;
	if(isRefraction) {
		r = (-b + disc) / (2 * a);
	} else {
		r = (-b - disc) / (2 * a);
	}
  //float r2 = (-b + disc) / (2 * a);

  // maybe need to allocate
	hit->x = o.x + r * u.x;
	hit->y = o.y + r * u.y;
	hit->z = o.z + r * u.z;

	return r;
}

/*********************************************************************
 * This function returns a pointer to the sphere object that the
 * ray intersects first; NULL if no intersection. You should decide
 * which arguments to use for the function. For exmaple, note that you
 * should return the point of intersection to the calling function.
 **********************************************************************/
Spheres *intersect_scene(Point origin, Vector direction, Spheres *scene, Point *hit, bool isRefraction) {
//
// do your thing here
//
	float length = FLT_MAX;
	int index = -1;
	Spheres *pointer;

	for(pointer = scene; pointer != NULL; pointer = pointer->next) {
		float temp = intersect_sphere(origin, direction, pointer, hit, isRefraction);
		if(temp > 1e-4 && temp < length) {
			length = temp;
			index = pointer->index;
		}
	}

	if(index == -1) {
		return NULL;
	} 

	for(pointer = scene; pointer->index != index; pointer = pointer->next);
	float temp = intersect_sphere(origin, direction, pointer, hit, isRefraction);
  
	return pointer;
}

/*****************************************************
 * This function adds a sphere into the sphere list
 *
 * You need not change this.
 *****************************************************/
Spheres *add_sphere(Spheres *slist, Point ctr, float rad, float amb[],
		    float dif[], float spe[], float shine, 
		    float refl, int sindex) {
	Spheres *new_sphere;

	new_sphere = (Spheres *)malloc(sizeof(Spheres));
	new_sphere->index = sindex;
	new_sphere->center = ctr;
	new_sphere->radius = rad;
	(new_sphere->mat_ambient)[0] = amb[0];
	(new_sphere->mat_ambient)[1] = amb[1];
	(new_sphere->mat_ambient)[2] = amb[2];
	(new_sphere->mat_diffuse)[0] = dif[0];
	(new_sphere->mat_diffuse)[1] = dif[1];
	(new_sphere->mat_diffuse)[2] = dif[2];
	(new_sphere->mat_specular)[0] = spe[0];
	(new_sphere->mat_specular)[1] = spe[1];
	(new_sphere->mat_specular)[2] = spe[2];
	new_sphere->mat_shineness = shine;
	new_sphere->reflectance = refl;
	new_sphere->next = NULL;

	if (slist == NULL) { // first object
		slist = new_sphere;
	} else { // insert at the beginning
		new_sphere->next = slist;
		slist = new_sphere;
	}

	return slist;
}

/******************************************
 * computes a sphere normal - done for you
 ******************************************/
Vector sphere_normal(Point q, Spheres *sph) {
	Vector rc;

	rc = get_vec(sph->center, q);
	normalize(&rc);
	return rc;
}
