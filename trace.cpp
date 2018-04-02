#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "global.h"
#include "sphere.h"
#include "chessboard.h"

//
// Global variables
//
extern int win_width;
extern int win_height;

extern GLfloat frame[WIN_HEIGHT][WIN_WIDTH][3];

extern float image_width;
extern float image_height;

extern Point eye_pos;
extern float image_plane;
extern RGB_float background_clr;
extern RGB_float null_clr;

extern Spheres *scene;
extern ChessBoard chessBoard;

// light 1 position and color
extern Point light1;
extern float light1_ambient[3];
extern float light1_diffuse[3];
extern float light1_specular[3];

// global ambient term
extern float global_ambient[3];

// light decay parameters
extern float decay_a;
extern float decay_b;
extern float decay_c;

extern int shadow_on;
extern int reflection_on;
extern int chessBoard_on;
extern int step_max;

/////////////////////////////////////////////////////////////////////

/*********************************************************************
* Phong illumination - you need to implement this!
*********************************************************************/
bool blocked(Point o, Vector d) {
	for (Spheres* pointer = scene; pointer != NULL; pointer = pointer->next) {
		const float a = vec_dot(d, d);
		const Vector co = get_vec(pointer->center, o);
		const float b = 2 * vec_dot(d, co);
		const float c = vec_dot(co, co) - pointer->radius * pointer->radius;

		float disc = b * b - 4 * a * c;
		if (disc < 1e-3) {
			continue;
		}

		disc = sqrt(disc * 1.0);
		const double root1 = (-b + disc) / 2 * a;
		const double root2 = (-b - disc) / 2 * a;

		if (root1 > 1e-4 || root2 > 1e-4) {
			return true;
		}
	}

	if(chessBoard_on) {
		return chessBoard.blocked(o, d);
	}

	return false;
}

Point reflect(Point p, Point lp, Vector lv) {
	float k = lv.x + (lv.y * lv.y / lv.x) + (lv.z * lv.z / lv.x);
	float b = -p.x * lv.x + (lv.y * lv.y / lv.x) * (p.x - 2 * lp.x) + 2 * lv.y * (lp.y - p.y) + (lv.z * lv.z / lv.x) * (p.x - 2 * lp.x) + 2 * lv.z * (lp.z - p.z);

	float x = -b / k;
	float y = (lv.y / lv.x) * (p.x + x - 2 * lp.x) + 2 * lp.y - p.y;
	float z = (lv.z / lv.x) * (p.x + x - 2 * lp.x) + 2 * lp.z - p.z;

	return Point(x, y, z);
}

RGB_float phong(Point q, Vector v, Vector surf_norm, Spheres *sph, Vector shadow_ray, bool isChessBoard) {
	//
	// do your thing here
	//
	// I = Iga * Kga + Ia * Ka + (1/(a + bd + cd^2)) * (Id * Kd * (n * l) + Is * Ks * (r * v) ^ n)
	Vector l = get_vec(q, light1); //light1 - q;
	Point l1 = get_point(q, l); //q + l;
	Point l2 = get_point(q, vec_scale(l, 2.0f));

	Point r1 = reflect(l1, q, surf_norm);
	Point r2 = reflect(l2, q, surf_norm);

	Vector r = get_vec(r1, r2);
	Vector n = surf_norm;
	//Vector v = get_vec(q, eye_pos); //!!!

	float d = vec_len(v);

	normalize(&l);
	normalize(&n);
	normalize(&r);
	normalize(&v);

	/*
	float mat_ambient[3];    // material property used in Phong model
	float mat_diffuse[3];
	float mat_specular[3];
	float mat_shineness;

	float reflectance;       // this number [0,1] determines how much */
	if (shadow_on) {
		if (blocked(q, shadow_ray) || (chessBoard_on && chessBoard.blocked(q, shadow_ray))) {
			return null_clr;
		}
	}

	float decay = 1.0f / (decay_a + decay_b * d + decay_c * d * d);
	float red, green, blue;
	if(isChessBoard) {
		Square square;

		int k = int((chessBoard.lightSquare[0].bottomLeft.z - q.z) / chessBoard.lightSquare[0].sideLength);
		int j = int((q.x - chessBoard.lightSquare[0].bottomLeft.x) / cos(chessBoard.lightSquare[0].rotation) / chessBoard.lightSquare[0].sideLength);
		if((k + j) % 2 == 0) {
			square = chessBoard.lightSquare[0];
		} else {
			square = chessBoard.darkSquare[0];
		}

		red = light1_ambient[0] * square.mat_ambient[0] + decay * (light1_diffuse[0] * square.mat_diffuse[0] * vec_dot(l, n) + light1_specular[0] * square.mat_specular[0] * pow(vec_dot(r, v), square.mat_shineness));
		green = light1_ambient[1] * square.mat_ambient[1] + decay * (light1_diffuse[1] * square.mat_diffuse[1] * vec_dot(l, n) + light1_specular[1] * square.mat_specular[1] * pow(vec_dot(r, v), square.mat_shineness));
		blue = light1_ambient[2] * square.mat_ambient[2] + decay * (light1_diffuse[2] * square.mat_diffuse[2] * vec_dot(l, n) + light1_specular[2] * square.mat_specular[2] * pow(vec_dot(r, v), square.mat_shineness));
	}
	else {
		red = light1_ambient[0] * sph->mat_ambient[0] + decay * (light1_diffuse[0] * sph->mat_diffuse[0] * vec_dot(l, n) + light1_specular[0] * sph->mat_specular[0] * pow(vec_dot(r, v), sph->mat_shineness));
		green = light1_ambient[1] * sph->mat_ambient[1] + decay * (light1_diffuse[1] * sph->mat_diffuse[1] * vec_dot(l, n) + light1_specular[1] * sph->mat_specular[1] * pow(vec_dot(r, v), sph->mat_shineness));
		blue = light1_ambient[2] * sph->mat_ambient[2] + decay * (light1_diffuse[2] * sph->mat_diffuse[2] * vec_dot(l, n) + light1_specular[2] * sph->mat_specular[2] * pow(vec_dot(r, v), sph->mat_shineness));
	}

	return RGB_float(red, green, blue);
}

RGB_float clamp(RGB_float color) {
	float red;
	float green;
	float blue;

	if (color.r < 0.0f) {
		red = 0.0f;
	}
	else if (color.r > 1.0f) {
		red = 1.0f;
	}
	else {
		red = color.r;
	}

	if (color.g < 0.0f) {
		green = 0.0f;
	}
	else if (color.g > 1.0f) {
		green = 1.0f;
	}
	else {
		green = color.g;
	}

	if (color.b < 0.0f) {
		blue = 0.0f;
	}
	else if (color.b > 1.0f) {
		blue = 1.0f;
	}
	else {
		blue = color.b;
	}

	return RGB_float(red, green, blue);
}

/************************************************************************
* This is the recursive ray tracer - you need to implement this!
* You should decide what arguments to use.
************************************************************************/
RGB_float recursive_ray_trace(Point p, Vector d, int step) {
	//
	// do your thing here
	//
	if (step > step_max) { //!!!
		return background_clr;
	}

	Point *hit = new Point();
	Point *planeHit = new Point();
	Spheres *sphere = intersect_scene(p, d, scene, hit, 0);
	float para = chessBoard.intersect(p, d, planeHit);
	if (!sphere && para < 0.0f || !sphere && !chessBoard_on) {
		delete hit;
		delete planeHit;
		return background_clr;
	}
	
	RGB_float local, reflected;
	float count = 1.0f;
	
	if(chessBoard_on && para > 0.0f && (!sphere || vec_len(get_vec(*planeHit, p)) < vec_len(get_vec(*hit, p)))) {
		// chess board blocked spheres
		Vector v = get_vec(*planeHit, eye_pos);
		Vector surf_norm = chessBoard.normal;
		Vector shadow_ray = get_vec(*planeHit, light1);
		normalize(&shadow_ray);
		local = phong(*planeHit, v, surf_norm, sphere, shadow_ray, true); //

		Point l1 = get_point(*planeHit, vec_scale(d, -1.0f));
		Point l2 = get_point(*planeHit, vec_scale(d, -2.0f));

		Point r1 = reflect(l1, *planeHit, surf_norm);
		Point r2 = reflect(l2, *planeHit, surf_norm);
		Vector r = get_vec(r1, r2);

		if (reflection_on) {
			reflected = recursive_ray_trace(*planeHit, r, step + 1);
			count += chessBoard.lightSquare[0].reflectance;
		} else {
			reflected = null_clr;
		}

		delete hit;
		delete planeHit;

		return clr_scale(clr_add(local, clr_scale(reflected, chessBoard.lightSquare[0].reflectance)), 1 / count);
	} else {
		Vector v = get_vec(*hit, eye_pos);
		Vector surf_norm = get_vec(sphere->center, *hit);
		Vector shadow_ray = get_vec(*hit, light1);
		normalize(&shadow_ray);
		local = phong(*hit, v, surf_norm, sphere, shadow_ray, false);

		Point l1 = get_point(*hit, vec_scale(d, -1.0f));
		Point l2 = get_point(*hit, vec_scale(d, -2.0f));

		Point r1 = reflect(l1, *hit, surf_norm);
		Point r2 = reflect(l2, *hit, surf_norm);
		Vector r = get_vec(r1, r2);

		if (reflection_on) {
			reflected = recursive_ray_trace(*hit, r, step + 1);
			count += sphere->reflectance;
		} else {
			reflected = null_clr;
		}

		delete hit;
		delete planeHit;

		return clr_scale(clr_add(local, clr_scale(reflected, sphere->reflectance)), 1 / count);
	}
}

/*********************************************************************
* This function traverses all the pixels and cast rays. It calls the
* recursive ray tracer and assign return color to frame
*
* You should not need to change it except for the call to the recursive
* ray tracer. Feel free to change other parts of the function however,
* if you must.
*********************************************************************/
void ray_trace() {
	int i, j;
	float x_grid_size = image_width / float(win_width);
	float y_grid_size = image_height / float(win_height);
	float x_start = -0.5 * image_width;
	float y_start = -0.5 * image_height;
	RGB_float ret_color;
	Point cur_pixel_pos;
	Vector ray;

	// ray is cast through center of pixel
	cur_pixel_pos.x = x_start + 0.5 * x_grid_size;
	cur_pixel_pos.y = y_start + 0.5 * y_grid_size;
	cur_pixel_pos.z = image_plane;

	for (i = 0; i<win_height; i++) {
		for (j = 0; j<win_width; j++) {
			ray = get_vec(eye_pos, cur_pixel_pos);

			//
			// You need to change this!!!
			//
			// ret_color = recursive_ray_trace();
			//ray.x = ray.y = 0.0f;
			//ray.z = -1.0f;
			ret_color = recursive_ray_trace(cur_pixel_pos, ray, 1); // just background for now

																	// Parallel rays can be cast instead using below
																	//
																	// ray.x = ray.y = 0;
																	// ray.z = -1.0;
																	// ret_color = recursive_ray_trace(cur_pixel_pos, ray, 1);

																	// Checkboard for testing
																	//RGB_float clr = {float(i/32), 0, float(j/32)};
																	//ret_color = clr;

			frame[i][j][0] = GLfloat(ret_color.r);
			frame[i][j][1] = GLfloat(ret_color.g);
			frame[i][j][2] = GLfloat(ret_color.b);

			cur_pixel_pos.x += x_grid_size;
		}

		cur_pixel_pos.y += y_grid_size;
		cur_pixel_pos.x = x_start;
	}
}
