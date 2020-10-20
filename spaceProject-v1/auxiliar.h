#pragma once

//INCLUDES
#include <windows.h>
#include <glut.h>
#include <gl.h>	 	
#include <glu.h>	
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "avion.h"
#include "Valores.h"


void camara(GLuint cameraMode, float posX, float posY, float posZ, float ang, float ang2);
void actualizar(int incr);
void teclado(unsigned char tras, int x, int y);