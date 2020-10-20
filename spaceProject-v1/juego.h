#pragma once

//INCLUDES
#include <windows.h>
#include <glut.h>
#include <gl.h>		 	
#include <glu.h>	
#include <stdio.h>
#include <SOIL.h>
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include "avion.h"
#include "laser.h"
#include "Valores.h"

void constructorAvion(avion *p, float angulo, float angulo2, float incrementoDesplazamiento, float posicionX, float posicionY, float posicionZ);
void constructorLaser(laser *ls, float px, float py, float pz, float angulo1, float angulo2);
void inicializarAvion(GLuint d); 
void inicializarLaser();
void inicializarCajaTexto();
void actualizarAvion(avion *p);
void actualizarLaser(laser *ls, avion p);
void dispararLaser(laser *ls);
bool colisionLaserPlaneta(laser ls, objeto planeta);
bool colisionaLaserPlaneta();

GLuint cargaTextura(char const *nombreArchivo);
void inicializarTexturas(void);
void actualizarColisiones(float x, float z, float y);

void mostrarInformacion(std::string planetaColision);

bool comprobarColisiones(float x, float z, float y);

int cargaTextura(char const *nombreArchivo, objeto* object);
void inicializaTexturas();
void actualizarMovimiento(objeto* object);
void actualizarMovimientoSatelite(objeto* satelite, objeto planeta);
void myMovimiento(int i);

void inicializarNiebla(void);

