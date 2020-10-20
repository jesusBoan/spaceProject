#pragma once

#include <windows.h>
#include <glut.h>
#include <gl.h>		 	
#include <glu.h>	
#include <stdio.h>
#include <SOIL.h>
#include <math.h>

//DATOS DEL AVIÓN
typedef struct laser {
	float px, py, pz;
	float angulo1;
	float angulo2;
	float sx, sy, sz;
	unsigned int listarender;
	float velocidad;
	unsigned int textura;
	int disparado;
};