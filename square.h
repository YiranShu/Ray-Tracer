#ifndef	_Square_H_
#define _Square_H_
#include "vector.h"

class Square {
public:
	float sideLength;
	float rotation;
	Point bottomLeft;

	float mat_ambient[3];    // material property used in Phong model
	float mat_diffuse[3];
	float mat_specular[3];

	float mat_shineness;
	float reflectance;

	Square(float sideLength, float rotation, Point bottomLeft, const float mat_ambient[],
		const float mat_diffuse[], const float mat_specular[], float mat_shineness, float reflectance) {
		this->sideLength = sideLength;
		this->rotation = rotation;
		this->bottomLeft = bottomLeft;

		for (int i = 0; i < 3; i++) {
			this->mat_ambient[i] = mat_ambient[i];
			this->mat_diffuse[i] = mat_diffuse[i];
			this->mat_specular[i] = mat_specular[i];
		}

		this->mat_shineness = mat_shineness;
		this->reflectance = reflectance;
	}

	Square() {}
};

#endif

