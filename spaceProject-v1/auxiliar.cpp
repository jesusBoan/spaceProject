
//INCLUDES
#include <windows.h>
#include <glut.h>
#include <gl.h>	 	
#include <glu.h>	
#include <stdio.h>
#include <math.h> 
#include <stdlib.h>
#include <iostream>
#include <mmsystem.h>
#include "avion.h"
#include "laser.h"
#include "Valores.h"

//DEFINES
#define INCREMENTO .05
#define DISTANCIA 200
#define PI 3.1416
#define GRADIANES 0.0174

extern avion jugador;
extern laser ls;
extern GLuint cam;
extern int mostrarInfo;
extern laser cajaTexto;
extern std::string planetaColision;
extern int actualizarSonido;

extern void actualizarAvion(avion *p);
extern void actualizarLaser(laser *ls, avion p);
extern void actualizarCajaTexto(laser *cajaTexto, avion p);
extern void dispararLaser(laser *ls);
extern bool colisionaLaserPlaneta();
extern bool comprobarColisiones(float x, float z, float y);

void camara(GLuint camMod, float posX, float posY, float posZ, float ang, float ang2) {
	glMatrixMode(GL_PROJECTION); 

	glLoadIdentity(); 
	gluPerspective(65, 1, 0.5, 27800.0); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (camMod == 0) //Vista 1ª Persona
		gluLookAt(posX, posY + 1.5 + jugador.angulo2 / 120, posZ, (jugador.detrasDer[0] + jugador.detrasIzq[0])/2,jugador.angulo2/30 + (jugador.detrasDer[2] + jugador.detrasIzq[2]) / 2, (jugador.detrasDer[1] + jugador.detrasIzq[1]) / 2, 0,1,0);
	else if (camMod == 1) // Vista 3ª persona
		gluLookAt(posX + 15.0*sin(ang*GRADIANES), posY + 15.0*-sin(ang2*GRADIANES) + 2 - jugador.angulo2/5, posZ + 15.0*cos(ang*GRADIANES), posX, posY, posZ, 0, 1, 0);
	else if (camMod == 3) //1ª Persona trasera	
		gluLookAt(posX, posY + 2 + jugador.angulo2 / 60, posZ, (jugador.delanteDer[0] + jugador.delanteIzq[0])/2,-jugador.angulo2 + (jugador.delanteDer[2] + jugador.delanteIzq[2]) / 2, (jugador.delanteDer[1] + jugador.delanteIzq[1]) / 2, 0,1,0);
	else if (camMod == 4) //3ª persona trasera
		gluLookAt(posX - 10.0*sin(ang*GRADIANES), posY + 5.0*-sin(ang2*GRADIANES) + 2 + jugador.angulo2/10, posZ - 10.0*cos(ang*GRADIANES), posX, posY, posZ, 0, 1, 0);
	else if (camMod == 2) {	//vista del cartel
		gluLookAt(0, 5, 10, 0,5,-5, 0, 1, 0);
	}
}
 
void actualizar(int incr) {
	actualizarAvion(&jugador);
	/*actualizarCajaTexto(&cajaTexto, jugador);*/
	if (ls.disparado == 0) {
		actualizarLaser(&ls, jugador);
	}
	else {
		dispararLaser(&ls);
		if (colisionaLaserPlaneta()) {
			PlaySound(NULL, NULL, 0);
			ls.disparado = 0;
			mostrarInfo = 1;
			cam = 2;
			jugador.velocidad = 0;
			std::cout << "Colisiona laser" << std::endl;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(40, actualizar, 0);
}

void teclado(unsigned char auxTec, int x, int y)
{
	switch (auxTec) {	//los controles de 's' y 'w' puede parecer que están invertidos pero la realidad es que pensando en un mando de avión, al tirar para atrás (s) subes 
	case 's': //ir para arriba
		if (jugador.angulo2 < 60) jugador.angulo2 += 1;		
		break;
	
	case 'a': // GIRAR A LA IZQUIERDA
		jugador.angulo += 1;
		if (jugador.angulo3 < 45) jugador.angulo3 += 2;
			
		break;
	case 'd': // GIRAR A LA DERECHA		
		jugador.angulo -= 1;
		if (jugador.angulo3 > -45) jugador.angulo3 -= 2;

		break;
	case 'w': //ir para abajo
		if (jugador.angulo2 > -60) jugador.angulo2 -= 1;
		break;
	case 'm': // acelerar
		if (jugador.velocidad > -100) jugador.velocidad -= 10;

		if (jugador.velocidad == 20 || jugador.velocidad == -20 || jugador.velocidad == 40 || jugador.velocidad == -40) {
			actualizarSonido = 1;
		}

		if (jugador.velocidad > -40 && jugador.velocidad < 40) {			
			PlaySound("sonidos/velocidadBase.wav", NULL, SND_LOOP | SND_ASYNC);
		}
		else {			
			PlaySound("sonidos/velocidadAlta.wav", NULL, SND_LOOP | SND_ASYNC);
		}
		break;
	case 'n': // frenar
		if (jugador.velocidad < 100) jugador.velocidad += 10;

		if (jugador.velocidad > -40 && jugador.velocidad < 40) {			
			PlaySound("sonidos/velocidadBase.wav", NULL, SND_LOOP | SND_ASYNC);
		}
		else {			
			PlaySound("sonidos/velocidadAlta.wav", NULL, SND_LOOP | SND_ASYNC);
		}
		break;
	case 'q': // VISTA CABINA
		ls.px = jugador.posicionX;
		ls.py = jugador.posicionY;
		ls.pz = jugador.posicionZ;

		ls.angulo1 = jugador.angulo;
		ls.angulo2 = jugador.angulo2;
		ls.disparado = 1;

		PlaySound(NULL, NULL, 0);
		PlaySound("sonidos/laser.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case '1': // Primera Persona
		cam = 0;
		mostrarInfo = 0;
		break;
	case '2': // Tercera Persona
		cam = 1;
		mostrarInfo = 0;
		break;
	case '3': // Primera Persona trasera
		cam = 3;
		mostrarInfo = 0;
		break;
	case '4': // Tercera Persona trasera
		cam = 4;
		mostrarInfo = 0;
		break;
	case '5': // Tercera Persona trasera
		cam = 2;
		mostrarInfo = 1;
		planetaColision = "controles";
		break;
	default:
		break;
	}
	glutPostRedisplay();
}