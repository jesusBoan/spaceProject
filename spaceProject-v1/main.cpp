//INCLUDES
#include "auxiliar.h"
#include "objetos.h"
#include "juego.h"
#include <string.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <mmsystem.h>
#include "stb_image.h"
#include "Valores.h"

//TAMAÑO DE LA VENTANA
const int W_WIDTH = 1200; 
const int W_HEIGHT = 650; 
extern int myEsfera();

GLuint texturaAbajoSkybox, texturaArribaSkybox, texturaDerechaSkybox, texturaIzquierdaSkybox, skyboxNor, skyboxSur, texturaAvion;

avion jugador;
laser ls;
laser cajaTexto;
std::string planetaColision = "controles";
int mostrarInfo = 1;
int cartel;
int actualizarSonido = 1;

//MODO DE LA CÁMARA
GLuint cam = 2;
//ILUMINACIÓN
GLfloat amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat pos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat dir[3] = { 0.0f, 0.0f, 1.0f };

GLfloat Ambient[] = { 1.0f,1.0f, 1.0f,1.0f };
GLfloat Diffuse[] = { 1.0f,1.0f, 1.0f,1.0f };
GLfloat SpecRef[] = { 1.0f,1.0f, 1.0f,1.0f };
GLfloat Specular[] = { 1.0f,1.0f, 1.0f,1.0f };

GLfloat LuzPos[] = { 0.0f,0.0f, 0.0f,1.0f };
GLfloat SpotDir[] = { 1.0f,0.0f, 1.0f };

float escalaVelocidadPlanetas = 0.0005;
float escalaDistanciasPlanetas = 6;
float escalaTamanoPlanetas = 2;
objeto sol = { 0, 0, 0, 100*escalaVelocidadPlanetas, 0, 450 * escalaTamanoPlanetas,0, 0, 0, {1.0, 1.0, 1.0}, 0 };
objeto mercurio = { 200 * escalaDistanciasPlanetas, 5 * escalaVelocidadPlanetas, 0, 5000 * escalaVelocidadPlanetas, 0, 25 * escalaTamanoPlanetas,0, 0 , 0, {0.7, 0.09, 0.09}, 0 };
objeto venus = { 350 * escalaDistanciasPlanetas, 3.5*escalaVelocidadPlanetas, -90, 1000 * escalaVelocidadPlanetas, 0, 35 * escalaTamanoPlanetas,0, 0 , 0, {0.81, 0.71, 0.23}, 0 };
objeto tierra = { 500 * escalaDistanciasPlanetas, 2.5*escalaVelocidadPlanetas, -270, 1000 * escalaVelocidadPlanetas, 0, 45 * escalaTamanoPlanetas,0, 0 , 0, {0.1 , 0.1 , 0.48}, 0 };
objeto luna = { 50 * escalaDistanciasPlanetas, 20 * escalaVelocidadPlanetas, 0, 1000 * escalaVelocidadPlanetas, 0, 10 * escalaTamanoPlanetas,0, 0 , 0, {0.4078, 0.4078, 0.4078}, 0 };
objeto iss = { 35 * escalaDistanciasPlanetas, 25*escalaVelocidadPlanetas, -80, 1000 * escalaVelocidadPlanetas, 0, 9 * escalaTamanoPlanetas,0, 0 , 0, {1.0,0.0,0.0}, 0 };
objeto marte = { 680 * escalaDistanciasPlanetas, 2.25*escalaVelocidadPlanetas, -10, 1000 * escalaVelocidadPlanetas, 0, 30 * escalaTamanoPlanetas,0, 0 , 0, {1.0, 0.0, 0.0}, 0 };
objeto jupiter = { 850 * escalaDistanciasPlanetas, 1.5*escalaVelocidadPlanetas, -300, 1000 * escalaVelocidadPlanetas, 0, 65 * escalaTamanoPlanetas,0, 0 , 0, {0.81, 0.76, 0.35},0 };
objeto saturno = { 1000 * escalaDistanciasPlanetas, 1.325*escalaVelocidadPlanetas, -180, 1000 * escalaVelocidadPlanetas, 0, 55 * escalaTamanoPlanetas,0, 0 , 0, {1.0, 0.25, 0.0}, 0 };
objeto urano = { 1175 * escalaDistanciasPlanetas, 1.175*escalaVelocidadPlanetas, -135, 1000 * escalaVelocidadPlanetas, 0, 45 * escalaTamanoPlanetas,0, 0 , 0, {0.137255, 0.419608, 0.556863}, 0 };
objeto neptuno = { 1280 * escalaDistanciasPlanetas, 1 * escalaVelocidadPlanetas, -15, 1000 * escalaVelocidadPlanetas, 0, 40 * escalaTamanoPlanetas,0, 0 , 0, {0.137255, 0.419608, 0.556863}, 0 };

//CREACIÓN DEL ESCENARIO
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camara(cam, jugador.posicionX, jugador.posicionY, jugador.posicionZ, jugador.angulo, jugador.angulo2);
	
	dibujarLaser(ls);
	dibujarAvion(&jugador);
	skybox();
	dibujarSistema();
		
	if (mostrarInfo) {	
		dibujarCartel();
		mostrarInformacion(planetaColision);				
	}
	else {
		dibujarEsquinaControles();
	}

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv); 
	glutInitWindowPosition(0, 0); 
	glutInitWindowSize(W_WIDTH, W_HEIGHT); 
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); 
	glutCreateWindow("Odisea Espacial (por Jesús Boán)"); 
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_NORMALIZE); 
	glEnable(GL_CULL_FACE); 
	glEnable(GL_TEXTURE_2D); 
	glShadeModel(GL_SMOOTH);

	// Inicializamos cosas
	sol.listaRender = mercurio.listaRender = venus.listaRender = tierra.listaRender = luna.listaRender = iss.listaRender =
		marte.listaRender = jupiter.listaRender = saturno.listaRender = urano.listaRender = neptuno.listaRender = myEsfera();

	inicializarAvion(0);
	inicializarLaser();
	inicializarTexturas();
	inicializaTexturas();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_NORMALIZE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, LuzPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, SpotDir);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecRef);
	glMateriali(GL_FRONT, GL_SHININESS, 1);

	inicializarNiebla();
	
	ls.listarender = cartel = myCubo();	

	glutKeyboardFunc(teclado);
	glutDisplayFunc(myDisplay);

	myMovimiento(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	actualizar(0);

	glutMainLoop();

	return 0;
}
