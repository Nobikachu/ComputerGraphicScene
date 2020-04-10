//  ========================================================================
//  COSC363 Assignment 1
//
//  FILE NAME: City.cpp
//  Author: Nobutaka Sato (56572069)
//  ========================================================================

#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include "loadTGA.h"
#include <fstream>
#include <climits>
#include <GL/freeglut.h>
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F   //To get rid of seams between textures
float lookAngle = 0.0;      //Camera rotation
float look_x, look_z=-1., eye_x, eye_z;  //Camera parameters
float cdr=3.14159265/180.0; //Conversion from degrees to radians
bool fpv = false; //First person view

int door_height = 0; //The current position of a garage door
bool door_opening = true; //Checks if the door is either opening or closing

float fan_theta = 0.0; //Current angle of the fan

//////////////////////////////Security camera inside the garage/////////////////////
float camera_theta = 0.0; //Current security camera angle
bool camera_positive_direction = false; //To keep track of the direction of the security camera

//Light position
float gx = -200.0;
float gy = 700.0;
float gz = -999.9;

/////////////////Current bus location (in terms of coordinates////////////
float bus_x = 0.0;
float bus_z = 0.0;

int bus_stop_time = 0; //Bus stop times (For garage and bus stop)

bool bus_moving = false; //Checks if the bus is moving
bool bus_on_driveway = true; //Checks if the bus is on the driveway
bool bus_reversing = false; //Checks if the bus is reversing

float bus_theta = 0; // When the bus turns

//////////////Keeps track of the direction of the bus///////////////////////
bool bus_south = false;
bool bus_east = false;
bool bus_west = false;
bool bus_north = false;

//////////////////Moonwalking man//////////////////////////////////////
float human1_theta = 0; //Left arm and right leg movement
float human2_theta = 0; //Right arm and left leg movement
float human_x = -20;
float human_z = 20;

bool human_moonwalk = false;
bool negative_theta = false; //When the left arm and right leg are moving towards the negative angle

//////////////////////Human////////////////////////////////////////////
bool human_moving = true;
float human2_x = -150;
float human2_z = -600;
float human2_theta1 = 0; //Left arm and right leg movement
float human2_theta2 = 0; //Right arm and left leg movement
float human2_body_theta = -90.0;

//Camera view when on first person view
float human_eye_x = 0;
float human_eye_y = 140;
float human_eye_z = -430;
float human_look_x = 0;
float human_look_z = - 431;

////////////////////////Vase///////////////////////////////////////////
const int N = 50;  // Total number of vertices on the base curve
float vx[N] = {0, 8, 8, 7.5, 6.7, 5, 5.5, 4, 4, 5, 5.6, 6.1, 6.8, 7.1, 7.5, 8, 8.4, 8.7, 9, 9.3,
              9.8, 10, 10.2, 10.4, 10.6, 10.9, 11, 11.1, 11.2, 11.3, 11.4, 11.3, 11.2, 11.1, 11, 10.5, 9.5, 8.2, 7, 6.2,
              6, 6.2, 6.8, 7.6, 8.5, 7, 6.1, 5.3, 4.7, 4.5};
float vy[N] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
               19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
               39, 40, 41, 42, 43, 43, 42, 41, 40, 39};
float vz[N] = {0};

GLuint texId[10]; //Array of textures

/** Loads all the textures.*/
void loadGLTextures()               // Load bitmaps And Convert To Textures
{
    glGenTextures(10, texId);        // Create texture ids
    // *** left ***
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    loadTGA("skybox2/left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** front ***
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    loadTGA("skybox2/front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** right ***
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    loadTGA("skybox2/right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** back***
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    loadTGA("skybox2/back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** top ***
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    loadTGA("skybox2/up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** down ***
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    loadTGA("skybox2/down.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[6]);
    loadTGA("Building/wall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[7]);
    loadTGA("Building/floor.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[8]);
    loadTGA("Building/garage_door.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[9]);
    loadTGA("Building/road.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

/** Animates the garage door by opening/closing the door.*/
void door_animation(void) {
    if (door_opening) {
        if (door_height < 180) { // When the door is 100% opened
            door_height += 2;
        }
    } else{
        if (door_height > 0) { // When the door is 100% closed
            door_height -= 2;
        }
    }
}

/** Handles all the movement of the bus.*/
void bus_movement(void) {
    if (bus_moving) {
        if (bus_south) {
            if (bus_x <= -700.0) {
                bus_theta += 2.5;
                if (bus_theta == 90.0) {
                    door_opening = false; //Sets the garage door closed as soon as the bus exits the driveway
                    bus_south = false;
                    bus_east = true;
                }
            } else {
                bus_x -= 15.0;
                if (bus_x >= -10.0 && bus_x <= 4.9) {
                    bus_moving = false; //Bus is stopped at the bus stop
                }
            }
        } else if (bus_east) {
            if (bus_z >= 700.0) {
                bus_theta += 2.5;
                if (bus_theta == 180.0) {
                    bus_east = false;
                    bus_north = true;
                }
            } else {
                bus_z += 15.0;
                if (bus_z >= -10.0 && bus_z <= 4.9) {
                    bus_moving = false;
                    bus_reversing = true;
                    door_opening = true; //Garage door should open as the bus reverses it back to the garage
                }
            }
        } else if (bus_north) {
            if (bus_x >= 700.0) {
                bus_theta += 2.5;
                if (bus_theta == 270.0) {
                    bus_north = false;
                    bus_west = true;
                }
            } else {
                bus_x += 15.0;
            }
        } else if (bus_west) {
            if (bus_z <= -700.0) {
                bus_theta += 2.5;
                if (bus_theta == 360.0) {
                    bus_west = false;
                    bus_south = true;
                    bus_theta = 0;
                }
            } else {
                bus_z -= 15.0;
            }
        }

    } else if (bus_reversing) {
            if (bus_theta >= 0.0) {
                bus_theta -= 2.5;
            } else {
                bus_x += 5.0;
                if (bus_x >= 0.0) {
                    bus_reversing = false;
                }
            }
        }
    else { //When the bus stops at a bus stop/ garage
        bus_stop_time++;
        if (bus_stop_time >= 200) {
            bus_moving = true;
            bus_stop_time = 0;
            bus_south = true;
        }
    }

}

/** Handles all the movement of a animated human model by moving forward,
 * then moonwalk backward after it reaches certain point.*/
void moonwalking_man(void) {
    if (!human_moonwalk) {
        human_x -= 2.5;
        if (human_x <= -200) {
            human_moonwalk = true;
        }
    } else {
        human_x += 1.25;
        if (human_x >= 5.0) {
            human_moonwalk = false;
        }
    }
    if (negative_theta) {
        human1_theta -= 2.5;
        human2_theta += 2.5;
        if (human1_theta <= -45.0) {
            negative_theta = false;
        }
    } else {
        human1_theta += 2.5;
        human2_theta -= 2.5;
        if (human1_theta >= 45.0) {
            negative_theta = true;
        }
    }
}

/** Ceiling fan moves by changing the theta (To rotate the fan).*/
void fan_movement(void) {
    fan_theta += 8.5;
}

/** Handles the movement of the security camera (inside the garage)*/
void camera_movement(void) {
    if (camera_positive_direction) {
        camera_theta += 0.5;
        if (camera_theta >= 25) {
            camera_positive_direction = false;
        }
    } else {
        camera_theta -= 0.5;
        if (camera_theta <= -35.0) {
            camera_positive_direction = true;
        }
    }
}

/** Timer to control all five animated models.
 * @param value To add amount of time to hold before moving the animated models*/
void myTimer(int value)
{
    door_animation();
    bus_movement();
    moonwalking_man();
    fan_movement();
    camera_movement();
    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0);
}

/** Adds textures to the skybox by plotting the vertexes.*/
void skybox(void){
  glEnable(GL_TEXTURE_2D);

  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-1000,  0, 1000);
  glTexCoord2f(1. ,0.);
  glVertex3f(-1000, 0., -1000);
  glTexCoord2f(1. ,1.);
  glVertex3f(-1000, 1000., -1000);
  glTexCoord2f(0. ,1.);
  glVertex3f(-1000, 1000, 1000);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-1000,  0, -1000);
  glTexCoord2f(1. ,0.);
  glVertex3f(1000, 0., -1000);
  glTexCoord2f(1. ,1.);
  glVertex3f(1000, 1000, -1000);
  glTexCoord2f(0. ,1.);
  glVertex3f(-1000,  1000, -1000);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(1000,  0, -1000);
  glTexCoord2f(1. ,0.);
  glVertex3f(1000, 0, 1000);
  glTexCoord2f(1. ,1.);
  glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0. ,1.);
  glVertex3f(1000,  1000,  -1000);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f( 1000, 0, 1000);
  glTexCoord2f(1. ,0.);
  glVertex3f(-1000, 0,  1000);
  glTexCoord2f(1. ,1.);
  glVertex3f(-1000, 1000,  1000);
  glTexCoord2f(0. ,1.);
  glVertex3f( 1000, 1000, 1000);
  glEnd();

  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-1000, 1000, -1000);
  glTexCoord2f(1. ,0.);
  glVertex3f(1000, 1000,  -1000);
  glTexCoord2f(1. ,1.);
  glVertex3f(1000, 1000, 1000);
  glTexCoord2f(0. ,1.);
  glVertex3f(-1000, 1000, 1000);
  glEnd();

  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-1000, -0.1, 1000);
  glTexCoord2f(1. ,0.);
  glVertex3f(1000, -0.1,  1000);
  glTexCoord2f(1. ,1.);
  glVertex3f(1000, -0.1, -1000);
  glTexCoord2f(0. ,1.);
  glVertex3f(-1000, -0.1, -1000);
  glEnd();

}

/** Adds textures to the building by plotting the vertexes.*/
void building(void) {
  glEnable(GL_TEXTURE_2D);

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[6]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-200,  0, -200);
  glTexCoord2f(1. ,0.);
  glVertex3f(200, 0., -200);
  glTexCoord2f(1. ,1.);
  glVertex3f(200, 200, -200);
  glTexCoord2f(0. ,1.);
  glVertex3f(-200,  200, -200);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[6]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(200,  0, -200);
  glTexCoord2f(1. ,0.);
  glVertex3f(200, 0, 200);
  glTexCoord2f(1. ,1.);
  glVertex3f(200, 200,  200);
  glTexCoord2f(0. ,1.);
  glVertex3f(200,  200,  -200);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[6]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f( 200, 0, 200);
  glTexCoord2f(1. ,0.);
  glVertex3f(-200, 0,  200);
  glTexCoord2f(1. ,1.);
  glVertex3f(-200, 200,  200);
  glTexCoord2f(0. ,1.);
  glVertex3f( 200, 200, 200);
  glEnd();

  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-200, 200, -200);
  glTexCoord2f(1. ,0.);
  glVertex3f(200, 200,  -200);
  glTexCoord2f(1. ,1.);
  glVertex3f(200, 200,  200);
  glTexCoord2f(0. ,1.);
  glVertex3f(-200, 200, 200);
  glEnd();

  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-200, 0.1, 200);
  glTexCoord2f(1. ,0.);
  glVertex3f(200, 0.1,  200);
  glTexCoord2f(1. ,1.);
  glVertex3f(200, 0.1, -200);
  glTexCoord2f(0. ,1.);
  glVertex3f(-200, 0.1, -200);
  glEnd();

  ////////////////////// Door ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[8]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-200,  door_height, 180);
  glTexCoord2f(1. ,0.);
  glVertex3f(-200, door_height, -180);
  glTexCoord2f(1. ,1.);
  glVertex3f(-200, 180, -180);
  glTexCoord2f(0. ,1.);
  glVertex3f(-200, 180, 180);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, texId[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-200,  0, -200);
  glTexCoord2f(1. ,0.);
  glVertex3f(-200, 200, -200);
  glTexCoord2f(1. ,1.);
  glVertex3f(-200, 200, -180);
  glTexCoord2f(0. ,1.);
  glVertex3f(-200, 0, -180);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, texId[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-200,  200, -180);
  glTexCoord2f(1. ,0.);
  glVertex3f(-200, 200, 180);
  glTexCoord2f(1. ,1.);
  glVertex3f(-200, 180, 180);
  glTexCoord2f(0. ,1.);
  glVertex3f(-200, 180, -180);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, texId[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-200,  200, 180);
  glTexCoord2f(1. ,0.);
  glVertex3f(-200, 0, 180);
  glTexCoord2f(1. ,1.);
  glVertex3f(-200, 0, 200);
  glTexCoord2f(0. ,1.);
  glVertex3f(-200, 200, 200);
  glEnd();
}

/** Adds textures to the road by plotting the vertexes.*/
void road(void) {
  glBindTexture(GL_TEXTURE_2D, texId[9]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-800, 0.1, -800);
  glTexCoord2f(1. ,0.);
  glVertex3f(800, 0.1, -800);
  glTexCoord2f(1. ,1.);
  glVertex3f(800, 0.1, -600);
  glTexCoord2f(0. ,1.);
  glVertex3f(-800, 0.1, -600);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, texId[9]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(800, 0.1, -600);
  glTexCoord2f(1. ,0.);
  glVertex3f(800, 0.1, 600);
  glTexCoord2f(1. ,1.);
  glVertex3f(600, 0.1, 600);
  glTexCoord2f(0. ,1.);
  glVertex3f(600, 0.1, -600);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, texId[9]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(800, 0.1, 600);
  glTexCoord2f(1. ,0.);
  glVertex3f(-800, 0.1, 600);
  glTexCoord2f(1. ,1.);
  glVertex3f(-800, 0.1, 800);
  glTexCoord2f(0. ,1.);
  glVertex3f(800, 0.1, 800);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, texId[9]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-800, 0.1, 600);
  glTexCoord2f(1. ,0.);
  glVertex3f(-800, 0.1, -600);
  glTexCoord2f(1. ,1.);
  glVertex3f(-600, 0.1, -600);
  glTexCoord2f(0. ,1.);
  glVertex3f(-600, 0.1, 600);
  glEnd();

  /////////////////////// Driveway //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0. ,0.);
  glVertex3f(-200, 0.1, -180);
  glTexCoord2f(1. ,0.);
  glVertex3f(-200, 0.1,  180);
  glTexCoord2f(1. ,1.);
  glVertex3f(-600, 0.1, 180);
  glTexCoord2f(0. ,1.);
  glVertex3f(-600, 0.1, -180);
  glEnd();
}

/** Draws the bus having several glut objects.*/
void bus(void)
{
    ////////////////Base////////////////
    glPushMatrix();
    glTranslatef(0, 75, 0);
    glScalef(250, 150, 125);
    glColor3f(1, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    /////////////Front Window////////////////
    glPushMatrix();
    glTranslatef(-125.1, 85, 0);
    glScalef(0, 75, 100);
    glColor3f(0, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    ////////////Left front light/////////////
    glPushMatrix();
    glTranslatef(-125.1, 25, -40);
    glColor3f(1, 1, 0);
    glutSolidSphere(12.5, 25, 25);
    glPopMatrix();

    ////////////Right front light/////////////
    glPushMatrix();
    glTranslatef(-125.1, 25, 40);
    glColor3f(1, 1, 0);
    glutSolidSphere(12.5, 25, 25);
    glPopMatrix();

    ////////////Inside the bus//////////////
    glPushMatrix();
    glTranslatef(-50 , 75,62.6);
    glScalef(100, 125, 0);
    glColor3f(0, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    /////////////Door//////////////////////
    glPushMatrix();
    glTranslatef(-25 , 75, 62.7);
    glScalef(50, 125, 0);
    glColor3f(0, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-25 , 75, 62.8);
    glScalef(47.5, 122.5, 0);
    glColor3f(0, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-75 , 75, 62.7);
    glScalef(50, 125, 0);
    glColor3f(0, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-75 , 75, 62.8);
    glScalef(47.5, 122.5, 0);
    glColor3f(0, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    ////////////////Wheel//////////////////
    glPushMatrix();
    glTranslatef(-75 , 5, 62.9);
    glColor3f(0, 0, 0);
    glutSolidTorus(5, 15, 30, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(75 , 5, 62.9);
    glColor3f(0, 0, 0);
    glutSolidTorus(5, 15, 30, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-75 , 5, -62.9);
    glColor3f(0, 0, 0);
    glutSolidTorus(5, 15, 30, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(75 , 5, -62.9);
    glColor3f(0, 0, 0);
    glutSolidTorus(5, 15, 30, 10);
    glPopMatrix();
}

/** Draws a bus stop by adding two block of glut cubes.*/
void bus_stop(void) {
    glPushMatrix();
    glTranslatef(0, 0, -450);
    glScalef(200, 300, 25);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 165, -477);
    glRotatef(45.0, 1.0, 0, 0);
    glScalef(200, 25, 80);
    glutSolidCube(1);
    glPopMatrix();

}

/** Draws a human model from using several glut objects.
 * @param theta1 The theta of the left arm and right leg
 * @param theta2 The theta of the right arm and left leg*/
void human(float theta1, float theta2) {
    //Body
    glPushMatrix();
    glTranslatef(100, 80, -150);
    glScalef(15, 80, 40);
    glColor3f(0, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    //Head
    glPushMatrix();
    glTranslatef(100, 135.5, -150);
    glColor3f(1, 1, 0);
    glutSolidSphere(25, 50, 50);
    glPopMatrix();

    //Mouth
    glPushMatrix();
    glTranslatef(75, 120, -150);
    glScalef(5, 10, 20);
    glColor3f(1, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    //Eyes
    glPushMatrix();
    glTranslatef(80, 140, -160);
    glColor3f(0, 0, 0);
    glutSolidSphere(5, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(80, 140, -140);
    glColor3f(0, 0, 0);
    glutSolidSphere(5, 10, 10);
    glPopMatrix();

    //Left arm
    glPushMatrix();
    glTranslatef(100, 100, -175);
    glRotatef(theta1, 0, 0, 1);
    glScalef(15, 40, 10);
    glColor3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();

    //Right arm
    glPushMatrix();
    glTranslatef(100, 100, -125);
    glRotatef(theta2, 0, 0, 1);
    glScalef(15, 40, 10);
    glColor3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();

    //Left leg
    glPushMatrix();
    glTranslatef(100, 25, -175);
    glRotatef(theta2, 0, 0, 1);
    glScalef(15, 50, 10);
    glColor3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();

    //Right leg
    glPushMatrix();
    glTranslatef(100, 25, -125);
    glRotatef(theta1, 0, 0, 1);
    glScalef(15, 50, 10);
    glColor3f(0, 1, 0);
    glutSolidCube(1);
    glPopMatrix();
}

/** To calculate the normal vector.*/
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
              float x3, float y3, float z3 )
{
      float nx, ny, nz;
      nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
      ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
      nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}

/** Drawing a camera base using surface of revolution by using comple mathematical formula.*/
void camera_base(void) {
    float wx[N], wy[N], wz[N];
    float degree = -10;
    glColor3f(1, 1, 1);
    for (int j =0;j< 36;j++){
        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i < N; i++)
        {
            wx[i] = vx[i] * cos(degree * cdr) +  vz[i] * sin(degree * cdr);
            wy[i] = vy[i];
            wz[i] = -vx[i] * sin(degree * cdr) + vz[i] * cos(degree * cdr);
            if(i > 0) {
                normal(wx[i-1], wy[i-1], wz[i-1], vx[i-1], vy[i-1], vz[i-1], vx[i], vy[i], vz[i]);
                normal(wx[i-1], wy[i-1], wz[i-1], vx[i], vy[i], vz[i], wx[i], wy[i], wz[i] );
            }
            float new_j = j;
            float new_i = i;
            glTexCoord2f(new_j/36.0, new_i/N);
            glVertex3f(vx[i], vy[i], vz[i]);
            glTexCoord2f((new_j + 1)/36.0, new_i/N);
            glVertex3f(wx[i], wy[i], wz[i]);
        }
        glEnd();

        for (int i = 0; i<N; i++){
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
    }

}

/** Draws a camera and translate it to the correct location.*/
void camera(void) {
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(-90, 0, 1, 1);
    camera_base();
    glPopMatrix();
}

/** Draws a ceiling fan on the ceiling (in the garage).*/
void ceiling_fan(void) {
    glPushMatrix();
    glTranslatef(0, 180, 0);
    glColor3f(1, 1, 0);
    glutSolidSphere(10, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 190, 0);
    glScalef(5, 20, 5);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 180, 0);
    glRotatef(fan_theta, 0, 1, 0);
    glScalef(10, 5, 80);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 180, 0);
    glRotatef(180 + fan_theta, 0, 1, 0);
    glScalef(80, 5, 10);
    glColor3f(1, 1, 1);
    glutSolidCube(1);
    glPopMatrix();
}

/** Initialise the program.*/
void initialise(void)
{
    loadGLTextures();

    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    //  Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    //  Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(70.0, 1.0, 100.0, 3500.0);   //Perspective projection
}

/** Displays the scene.*/
void display(void)
{
    float lpos[4] = {gx, gy, gz, 1.0f};  //light's position
    float spotdir[] = {1, -1, 0};
    float shadowMat[16] = {gy, 0, 0, 0, -gx, 0, -gz, -1, 0, 0, gy, 0, 0, 0, 0, gy};

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (!fpv) {
        gluLookAt(eye_x, 100, eye_z,  look_x, 100, look_z,   0, 1, 0);  // Open view camera
    } else {
        gluLookAt(human_eye_x, human_eye_y, human_eye_z,  human_look_x, 140, human_look_z,   0, 1, 0);
    }


    glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotdir);

    float lgt_pos1[] = {0, 174.9, 0, -1}; //light position for light1
    float spotdir1[] = {0, -1, 0}; //Spot light directoin
    glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos1);   //light
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir1);

    skybox();

    building();
    road();

    glDisable(GL_TEXTURE_2D);

    //Drawing the shadow (For the bus stop)
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf(shadowMat);
    glColor4f(0.2, 0.2, 0.2, 1.0);
    bus_stop();
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glColor3f(1, 1, 1); //Colour on the bus stop added here instead (To add the planar shadow)
    bus_stop();


    glPushMatrix();
    glTranslatef(bus_x, 10, bus_z);
    glRotatef(bus_theta, 0.0, 1.0, 0.0);
    bus();
    glPopMatrix();

    ceiling_fan();

    glDisable(GL_LIGHT1);
    glPushMatrix();
    glTranslatef(human2_x, 0, human2_z);
    glRotatef(human2_body_theta, 0.0, 1.0, 0.0);
    human(human2_theta1, human2_theta2);
    glPopMatrix();

    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glPushMatrix();
    glTranslatef(human_x, 0, human_z);
    human(human1_theta, human2_theta);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(160, 160, 160);
    glRotatef(camera_theta, 0, 1, 0);
    camera();
    glPopMatrix();

    glFlush();
}

/** Camera mode for open view
 * @param key The key that has been pressed.*/
void open_view(int key)
{
    if(key==GLUT_KEY_LEFT) lookAngle-=5;         //Turn left
    else if(key==GLUT_KEY_RIGHT) lookAngle+=5;   //Turn right
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 5*sin(lookAngle * cdr);
        eye_z += 5*cos(lookAngle * cdr);

    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 5*sin(lookAngle * cdr);
        eye_z -= 5*cos(lookAngle * cdr);
    }

    look_x = eye_x + 100*sin(lookAngle * cdr);
    look_z = eye_z - 100*cos(lookAngle * cdr);

}

/** Camera mode for first person view
 * @param key The key that has been pressed.*/
void first_person_view(int key) {
    if(key == GLUT_KEY_LEFT) {
        if (lookAngle >= -5) {
            lookAngle-=5;         //Looking left
        }
    } else if(key == GLUT_KEY_RIGHT) {
        if (lookAngle <= 5) {
            lookAngle+=5;   //Looking right
        }
    } else if (key == GLUT_KEY_DOWN) { //Looking down
        if (human_eye_y <= 145) {
            human_eye_y += 2.5;
        }
    } else if (key == GLUT_KEY_UP) { //Looking up
        if (human_eye_y >= 120) {
            human_eye_y -= 2.5;
        }
    }

    human_look_x = human_eye_x + 100*sin(lookAngle * cdr);
    human_look_z = human_eye_z - 100*cos(lookAngle * cdr);
}

/** Special key function
 * @param key The key that has been pressed.
 * @param x
 * @param y*/
 void special(int key, int x, int y)
 {
    if (key==GLUT_KEY_F1) {
        if (fpv) {
            fpv = false;
        } else {
            fpv = true;
        }
    }
    if (!fpv) {
        open_view(key);
    } else {
        first_person_view(key);
    }

    glutPostRedisplay();
}

/** Main function*/
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH );
   glutInitWindowSize (700, 700);
   glutInitWindowPosition (50, 50);

   glutCreateWindow ("Sky Box");
   initialise();
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutTimerFunc(500, myTimer, 0);

   glutMainLoop();
   return 0;
}
