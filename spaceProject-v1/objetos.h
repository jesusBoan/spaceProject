#pragma once

// Includes
#include <windows.h>
#include <glut.h>
#include <gl.h>	 	
#include <glu.h>	
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "avion.h"
#include "laser.h"
#include "Valores.h"


void crearAvion();
void ejes(void);
void dibujarAvion(avion *p);
void dibujarCajaTexto(laser x);
void skybox(void);
int myCubo();
void dibujarLaser(laser ls);

void dibujarPlaneta(objeto object);
void dibujarISS();
void dibujarSistema();
void dibujarCilindro();

void dibujarCartel();
void dibujarEsquinaControles();

int myEsfera();