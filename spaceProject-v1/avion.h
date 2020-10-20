#pragma once

#include <windows.h>
#include <glut.h>
#include <gl.h>		 	
#include <glu.h>	
#include <stdio.h>
#include <SOIL.h>
#include <math.h>

//DATOS DEL AVIÓN
typedef struct avion {
	float angulo;	//angulo en X
	float incrementoDesplazamiento;
	float angulo2;	//angulo en Y
	float angulo3;	//angulo en Z
	float posicionX;
	float posicionY;
	float posicionZ;
	float velocidad;
	float incrementoVelocidad;
	float altura;
	GLuint textura;

	float delanteIzq[3];
	float delanteDer[3];
	float detrasIzq[3];
	float detrasDer[3];
};