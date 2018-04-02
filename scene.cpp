//
// this provide functions to set up the scene
//
#include <cmath>
#include "sphere.h"
#include "square.h"
#include "chessboard.h"

#define PI 3.14159265

extern Point light1;
extern float light1_ambient[3];
extern float light1_diffuse[3];
extern float light1_specular[3];

extern float global_ambient[3];
extern Spheres *scene;
extern ChessBoard chessBoard;

extern RGB_float background_clr;
extern float decay_a;
extern float decay_b;
extern float decay_c;

extern int chessBoard_on;

//////////////////////////////////////////////////////////////////////////

/*******************************************
* set up the default scene - DO NOT CHANGE
*******************************************/
void set_up_default_scene() {
  // set background color
  background_clr.r = 0.5;
  background_clr.g = 0.05;
  background_clr.b = 0.8;

  // setup global ambient term
  global_ambient[0] = global_ambient[1] = global_ambient[2] = 0.2;

  // setup light 1
  light1.x = -2.0;
  light1.y = 5.0;
  light1.z = 1.0;
  light1_ambient[0] = light1_ambient[1] = light1_ambient[2] = 0.1;
  light1_diffuse[0] = light1_diffuse[1] = light1_diffuse[2] = 1.0;
  light1_specular[0] = light1_specular[1] = light1_specular[2] = 1.0;

  // set up decay parameters
  decay_a = 0.5;
  decay_b = 0.3;
  decay_c = 0.0;

  // sphere 1
  Point sphere1_ctr = { 1.5f, -0.2f, -3.23f }; //{1.5f, -0.2f, -3.2f}
  float sphere1_rad = 1.23f;
  float sphere1_ambient[] = { 0.7f, 0.7f, 0.7f };
  float sphere1_diffuse[] = { 0.1f, 0.5f, 0.8f };
  float sphere1_specular[] = { 1.0f, 1.0f, 1.0f };
  float sphere1_shineness = 10.0f;
  float sphere1_reflectance = 0.4f;
  scene = add_sphere(scene, sphere1_ctr, sphere1_rad, sphere1_ambient,
    sphere1_diffuse, sphere1_specular, sphere1_shineness,
    sphere1_reflectance, 1);

  // sphere 2
  Point sphere2_ctr = { -1.5f, 0.0f, -3.5f };
  float sphere2_rad = 1.5f;
  float sphere2_ambient[] = { 0.6f, 0.6f, 0.6f };
  float sphere2_diffuse[] = { 1.0f, 0.0f, 0.25f };
  float sphere2_specular[] = { 1.0f, 1.0f, 1.0f };
  float sphere2_shineness = 6.0f;
  float sphere2_reflectance = 0.3f;
  scene = add_sphere(scene, sphere2_ctr, sphere2_rad, sphere2_ambient,
    sphere2_diffuse, sphere2_specular, sphere2_shineness,
    sphere2_reflectance, 2);

  // sphere 3
  Point sphere3_ctr = { -0.35f, 1.75f, -2.5f }; //{-0.35f, 1.75f, -2.25f}
  float sphere3_rad = 0.5f;
  float sphere3_ambient[] = { 0.2f, 0.2f, 0.2f };
  float sphere3_diffuse[] = { 0.0f, 1.0f, 0.25f };
  float sphere3_specular[] = { 0.0f, 1.0f, 0.0f };
  float sphere3_shineness = 30.0f;
  float sphere3_reflectance = 0.3f;
  scene = add_sphere(scene, sphere3_ctr, sphere3_rad, sphere3_ambient,
    sphere3_diffuse, sphere3_specular, sphere3_shineness,
    sphere3_reflectance, 3);

  if(chessBoard_on) {
    Square lightSquare[32];
    Square darkSquare[32];

    for(int i = 0; i < 8; i++) {
      for (int j = 0; j < 4; j++) {
        lightSquare[4 * i + j].sideLength = 0.25f;
        lightSquare[4 * i + j].rotation = 1.0f / 6.0f * PI;
        if (i % 2 == 0) {
          lightSquare[4 * i + j].bottomLeft = { -0.2f + 2 * j * lightSquare[4 * i + j].sideLength * cos(lightSquare[4 * i + j].rotation * 1.0),
            -3.0f + 2 * j * lightSquare[4 * i + j].sideLength * sin(lightSquare[4 * i + j].rotation * 1.0),
            -2.5f - i * lightSquare[4 * i + j].sideLength };
        } else {
          lightSquare[4 * i + j].bottomLeft = { -0.2f + (2 * j + 1) * lightSquare[4 * i + j].sideLength * cos(lightSquare[4 * i + j].rotation * 1.0),
            -3.0f + (2 * j + 1) * lightSquare[4 * i + j].sideLength * sin(lightSquare[4 * i + j].rotation * 1.0),
            -2.5f - i * lightSquare[4 * i + j].sideLength };
        }
        lightSquare[4 * i + j].mat_ambient[0] = lightSquare[4 * i + j].mat_ambient[1] = lightSquare[4 * i + j].mat_ambient[2] = 0.6f;
        lightSquare[4 * i + j].mat_diffuse[0] = 1.0f;
        lightSquare[4 * i + j].mat_diffuse[1] = 0.925f;
        lightSquare[4 * i + j].mat_diffuse[2] = 0.545f;
        lightSquare[4 * i + j].mat_specular[0] = lightSquare[4 * i + j].mat_specular[1] = lightSquare[4 * i + j].mat_specular[2] = 1.0f;
        lightSquare[4 * i + j].mat_shineness = 32.0f;
        lightSquare[4 * i + j].reflectance = 0.4f;
      }
    }

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 4; j++) {
        darkSquare[4 * i + j].sideLength = 0.25f;
        darkSquare[4 * i + j].rotation = 1.0f / 6.0f * PI;
        if (i % 2 == 1) {
          darkSquare[4 * i + j].bottomLeft = { -0.2f + 2 * j * darkSquare[4 * i + j].sideLength * cos(darkSquare[4 * i + j].rotation * 1.0),
            -3.0f + 2 * j * darkSquare[4 * i + j].sideLength * sin(darkSquare[4 * i + j].rotation * 1.0),
            -2.5f - i * darkSquare[4 * i + j].sideLength };
        }
        else {
          darkSquare[4 * i + j].bottomLeft = { -0.2f + (2 * j + 1) * darkSquare[4 * i + j].sideLength * cos(darkSquare[4 * i + j].rotation * 1.0),
            -3.0f + (2 * j + 1) * darkSquare[4 * i + j].sideLength * sin(darkSquare[4 * i + j].rotation * 1.0),
            -2.5f - i * darkSquare[4 * i + j].sideLength };
        }
        darkSquare[4 * i + j].mat_ambient[0] = darkSquare[4 * i + j].mat_ambient[1] = darkSquare[4 * i + j].mat_ambient[2] = 0.6f;
        darkSquare[4 * i + j].mat_diffuse[0] = 0.212f;
        darkSquare[4 * i + j].mat_diffuse[1] = 0.392f;
        darkSquare[4 * i + j].mat_diffuse[2] = 0.545f;
        darkSquare[4 * i + j].mat_specular[0] = darkSquare[4 * i + j].mat_specular[1] = darkSquare[4 * i + j].mat_specular[2] = 1.0f;
        darkSquare[4 * i + j].mat_shineness = 32.0f;
        darkSquare[4 * i + j].reflectance = 0.4f;
      }
    }

    for(int i = 0; i < 32; i++) {
      chessBoard.lightSquare[i] = lightSquare[i];
      chessBoard.darkSquare[i] = darkSquare[i];
    }

    Vector p(1.0f, 0.0f, 0.0f);
    Vector q(0.0f, 0.0f, -1.0f);

    p.x = 1.0f * cos(lightSquare[0].rotation * 1.0);
    p.y = 1.0f * sin(lightSquare[0].rotation * 1.0);

    chessBoard.normal = vec_cross(p, q);
  }
}

/***************************************
* You can create your own scene here
***************************************/
void set_up_user_scene() {
  background_clr.r = 0.5;
  background_clr.g = 0.05;
  background_clr.b = 0.8;

  // setup global ambient term
  global_ambient[0] = global_ambient[1] = global_ambient[2] = 0.2;

  // setup light 1
  light1.x = -2.0;
  light1.y = 5.0;
  light1.z = 1.0;
  light1_ambient[0] = light1_ambient[1] = light1_ambient[2] = 0.1;
  light1_diffuse[0] = light1_diffuse[1] = light1_diffuse[2] = 1.0;
  light1_specular[0] = light1_specular[1] = light1_specular[2] = 1.0;

  // set up decay parameters
  decay_a = 0.5;
  decay_b = 0.3;
  decay_c = 0.0;

  // sphere 1
  Point sphere1_ctr = { 1.5f, -0.2f, -3.23f }; //{1.5f, -0.2f, -3.2f}
  float sphere1_rad = 1.23f;
  float sphere1_ambient[] = { 0.7f, 0.7f, 0.7f };
  float sphere1_diffuse[] = { 0.1f, 0.5f, 0.8f };
  float sphere1_specular[] = { 1.0f, 1.0f, 1.0f };
  float sphere1_shineness = 10.0f;
  float sphere1_reflectance = 0.4f;
  scene = add_sphere(scene, sphere1_ctr, sphere1_rad, sphere1_ambient,
    sphere1_diffuse, sphere1_specular, sphere1_shineness,
    sphere1_reflectance, 1);

  // sphere 2
  Point sphere2_ctr = { -1.5f, 0.0f, -3.5f };
  float sphere2_rad = 1.5f;
  float sphere2_ambient[] = { 0.6f, 0.6f, 0.6f };
  float sphere2_diffuse[] = { 1.0f, 0.0f, 0.25f };
  float sphere2_specular[] = { 1.0f, 1.0f, 1.0f };
  float sphere2_shineness = 6.0f;
  float sphere2_reflectance = 0.3f;
  scene = add_sphere(scene, sphere2_ctr, sphere2_rad, sphere2_ambient,
    sphere2_diffuse, sphere2_specular, sphere2_shineness,
    sphere2_reflectance, 2);

  // sphere 3
  Point sphere3_ctr = { -0.35f, 1.75f, -2.5f }; //{-0.35f, 1.75f, -2.25f}
  float sphere3_rad = 0.5f;
  float sphere3_ambient[] = { 0.2f, 0.2f, 0.2f };
  float sphere3_diffuse[] = { 0.0f, 1.0f, 0.25f };
  float sphere3_specular[] = { 0.0f, 1.0f, 0.0f };
  float sphere3_shineness = 30.0f;
  float sphere3_reflectance = 0.3f;
  scene = add_sphere(scene, sphere3_ctr, sphere3_rad, sphere3_ambient,
    sphere3_diffuse, sphere3_specular, sphere3_shineness,
    sphere3_reflectance, 3);

  if(chessBoard_on) {
    Square lightSquare[32];
    Square darkSquare[32];

    for(int i = 0; i < 8; i++) {
      for (int j = 0; j < 4; j++) {
        lightSquare[4 * i + j].sideLength = 0.25f;
        lightSquare[4 * i + j].rotation = 1.0f / 6.0f * PI;
        if (i % 2 == 0) {
          lightSquare[4 * i + j].bottomLeft = { -0.2f + 2 * j * lightSquare[4 * i + j].sideLength * cos(lightSquare[4 * i + j].rotation * 1.0),
            -3.0f + 2 * j * lightSquare[4 * i + j].sideLength * sin(lightSquare[4 * i + j].rotation * 1.0),
            -2.5f - i * lightSquare[4 * i + j].sideLength };
        } else {
          lightSquare[4 * i + j].bottomLeft = { -0.2f + (2 * j + 1) * lightSquare[4 * i + j].sideLength * cos(lightSquare[4 * i + j].rotation * 1.0),
            -3.0f + (2 * j + 1) * lightSquare[4 * i + j].sideLength * sin(lightSquare[4 * i + j].rotation * 1.0),
            -2.5f - i * lightSquare[4 * i + j].sideLength };
        }
        lightSquare[4 * i + j].mat_ambient[0] = lightSquare[4 * i + j].mat_ambient[1] = lightSquare[4 * i + j].mat_ambient[2] = 0.6f;
        lightSquare[4 * i + j].mat_diffuse[0] = 1.0f;
        lightSquare[4 * i + j].mat_diffuse[1] = 0.925f;
        lightSquare[4 * i + j].mat_diffuse[2] = 0.545f;
        lightSquare[4 * i + j].mat_specular[0] = lightSquare[4 * i + j].mat_specular[1] = lightSquare[4 * i + j].mat_specular[2] = 1.0f;
        lightSquare[4 * i + j].mat_shineness = 32.0f;
        lightSquare[4 * i + j].reflectance = 0.4f;
      }
    }

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 4; j++) {
        darkSquare[4 * i + j].sideLength = 0.25f;
        darkSquare[4 * i + j].rotation = 1.0f / 6.0f * PI;
        if (i % 2 == 1) {
          darkSquare[4 * i + j].bottomLeft = { -0.2f + 2 * j * darkSquare[4 * i + j].sideLength * cos(darkSquare[4 * i + j].rotation * 1.0),
            -3.0f + 2 * j * darkSquare[4 * i + j].sideLength * sin(darkSquare[4 * i + j].rotation * 1.0),
            -2.5f - i * darkSquare[4 * i + j].sideLength };
        }
        else {
          darkSquare[4 * i + j].bottomLeft = { -0.2f + (2 * j + 1) * darkSquare[4 * i + j].sideLength * cos(darkSquare[4 * i + j].rotation * 1.0),
            -3.0f + (2 * j + 1) * darkSquare[4 * i + j].sideLength * sin(darkSquare[4 * i + j].rotation * 1.0),
            -2.5f - i * darkSquare[4 * i + j].sideLength };
        }
        darkSquare[4 * i + j].mat_ambient[0] = darkSquare[4 * i + j].mat_ambient[1] = darkSquare[4 * i + j].mat_ambient[2] = 0.6f;
        darkSquare[4 * i + j].mat_diffuse[0] = 0.212f;
        darkSquare[4 * i + j].mat_diffuse[1] = 0.392f;
        darkSquare[4 * i + j].mat_diffuse[2] = 0.545f;
        darkSquare[4 * i + j].mat_specular[0] = darkSquare[4 * i + j].mat_specular[1] = darkSquare[4 * i + j].mat_specular[2] = 1.0f;
        darkSquare[4 * i + j].mat_shineness = 32.0f;
        darkSquare[4 * i + j].reflectance = 0.4f;
      }
    }

    for(int i = 0; i < 32; i++) {
      chessBoard.lightSquare[i] = lightSquare[i];
      chessBoard.darkSquare[i] = darkSquare[i];
    }

    Vector p(1.0f, 0.0f, 0.0f);
    Vector q(0.0f, 0.0f, -1.0f);

    p.x = 1.0f * cos(lightSquare[0].rotation * 1.0);
    p.y = 1.0f * sin(lightSquare[0].rotation * 1.0);

    chessBoard.normal = vec_cross(p, q);
  }
}
