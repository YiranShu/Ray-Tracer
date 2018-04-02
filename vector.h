/**************************************************************************
 * This header provides you data types and functions to handle vectors
 ************************************************************************/
#ifndef _VECTOR_H_
#define _VECTOR_H_
class Point {
public:
	float x;
	float y;
	float z;
	Point() {}
	Point(float a, float b, float c) {
		x = a;
		y = b; 
		z = c;
	}
};  // geometric 3D point

class Vector {
public:
	float x;
	float y;
	float z;
	Vector() {}
	Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
}; // geometric 3D vector

class RGB_float {
public:
	float r;
	float g;
	float b;
	RGB_float() {}
	RGB_float(float x, float y, float z) {
  		r = x;
  		g = y;
  		b = z;
	}
};

float vec_len(Vector);
float vec_dot(Vector, Vector);
Vector vec_plus(Vector, Vector);
Vector vec_minus(Vector, Vector);
Vector vec_cross(Vector, Vector);
Vector get_vec(Point, Point);
Point get_point(Point, Vector);
void normalize(Vector *);
Vector vec_scale(Vector, float);
RGB_float clr_add(RGB_float, RGB_float);
RGB_float clr_scale(RGB_float, float);
float vec_angle(Vector, Vector);

#endif

