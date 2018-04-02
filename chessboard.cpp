#include <cmath>
#include "chessboard.h"

float ChessBoard::intersect(Point origin, Vector direction, Point* hit) {
	Vector n = normal;

	//n.x(x - lightSquare[0].bottomLeft.x) + n.y(y - lightSquare[0].bottomLeft.y) + n.z(z - lightSquare[0].bottomLeft.z) = 0
	float k = n.x * direction.x + n.y * direction.y + n.z * direction.z;
	float b = n.x * (origin.x - lightSquare[0].bottomLeft.x) + n.y * (origin.y - lightSquare[0].bottomLeft.y) + n.z * (origin.z - lightSquare[0].bottomLeft.z);

	if(k < 1e-3 && k > -1e-3) {
		return -1.0;
	}

	float r = -b / k;
	hit->x = origin.x + r * direction.x;
	hit->y = origin.y + r * direction.y;
	hit->z = origin.z + r * direction.z;

	if (r > 0.0f && hit->x >= lightSquare[0].bottomLeft.x && hit->x <= lightSquare[0].bottomLeft.x + 8 * lightSquare[0].sideLength * cos(lightSquare[0].rotation * 1.0) 
		&& hit->y >= lightSquare[0].bottomLeft.y && hit->y <= lightSquare[0].bottomLeft.y + 8 * lightSquare[0].sideLength * sin(lightSquare[0].rotation * 1.0)
		&& hit->z <= lightSquare[0].bottomLeft.z && hit->z >= lightSquare[0].bottomLeft.z - 8 * lightSquare[0].sideLength) {
		return r;
	} else {
		return -1.0f;
	}
}

bool ChessBoard::blocked(Point origin, Vector direction) {
	Vector n = normal;

	//n.x(x - lightSquare[0].bottomLeft.x) + n.y(y - lightSquare[0].bottomLeft.y) + n.z(z - lightSquare[0].bottomLeft.z) = 0
	float k = n.x * direction.x + n.y * direction.y + n.z * direction.z;
	float b = n.x * (origin.x - lightSquare[0].bottomLeft.x) + n.y * (origin.y - lightSquare[0].bottomLeft.y) + n.z * (origin.z - lightSquare[0].bottomLeft.z);

	if (k < 1e-3 && k > -1e-3) {
		return false;
	}

	float r = -b / k;
	//hit->x = origin.x + r * direction.x;
	//hit->y = origin.y + r * direction.y;
	//hit->z = origin.z + r * direction.z;

	if (origin.x + r * direction.x >= lightSquare[0].bottomLeft.x && origin.x + r * direction.x <= lightSquare[0].bottomLeft.x + 8 * lightSquare[0].sideLength * cos(lightSquare[0].rotation * 1.0)
		&& origin.y + r * direction.y >= lightSquare[0].bottomLeft.y && origin.y + r * direction.y <= lightSquare[0].bottomLeft.y + 8 * lightSquare[0].sideLength * sin(lightSquare[0].rotation * 1.0)
		&& origin.z + r * direction.z <= lightSquare[0].bottomLeft.z && origin.z + r * direction.z >= lightSquare[0].bottomLeft.z - 8 * lightSquare[0].sideLength) {
		return r > 1e-4;
	}
	
	return false;
}

