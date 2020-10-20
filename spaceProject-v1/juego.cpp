//INCLUDES
#include <windows.h>
#include <glut.h>
#include <gl.h>		 	
#include <glu.h>	
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <string>
#include <mmsystem.h>
#include <fstream>
#include "avion.h"
#include "laser.h"
#include "Valores.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//DEFINES
#define DIAGONAL 10
#define PI 3.1416
#define GL_PI 3.14f
#define N 1.85


extern avion jugador;
extern laser ls;
extern laser cajaTexto;
extern std::string planetaColision;
extern int actualizarSonido;

extern objeto sol, mercurio, venus, tierra, luna, iss, marte, jupiter, saturno, urano, neptuno;

int length = 0;
float x = 0;
float y = 510;
float interlineado = 25;
double *matrix;

//ILUMINACIÓN
extern GLfloat amb[4];
extern GLfloat pos[4];
extern GLfloat dir[3];

extern GLuint texturaAbajoSkybox, texturaArribaSkybox, texturaDerechaSkybox, texturaIzquierdaSkybox, skyboxNor, skyboxSur, texturaAvion;


bool comprobarColisiones(float x, float z, float y);

void constructorAvion(avion *a, float angulo, float angulo2, float incrementoDesplazamiento, float posicionX, float posicionY, float posicionZ) {
	a->angulo = angulo;
	a->angulo2 = angulo2;
	a->incrementoDesplazamiento = incrementoDesplazamiento;
	a->angulo3 = 0;
	a->posicionX = posicionX;
	a->posicionY = posicionY;
	a->posicionZ = posicionZ;
	a->velocidad = 0;

	a->delanteIzq[0] = posicionX - 5;
	a->delanteIzq[1] = posicionZ + 5;
	a->delanteDer[0] = posicionX + 5;
	a->delanteDer[1] = posicionZ + 5;
	a->detrasIzq[0] = posicionX - 5;
	a->detrasIzq[1] = posicionZ - 5;
	a->detrasDer[0] = posicionX + 5;
	a->detrasDer[1] = posicionZ - 5;
}

void constructorLaser(laser *ls, float px, float py, float pz, float angulo1, float angulo2) {
	ls->px = px;
	ls->py = py;
	ls->pz = pz;
	ls->angulo1 = angulo1;
	ls->angulo2 = angulo2;
	ls->velocidad = 0;
	ls->disparado = 0;
}

float randVel(void) {
	srand(time(NULL));
	return 2 + rand() % 2 + (rand() % 5) * 0.03;
}

void inicializarAvion(GLuint d) {
	constructorAvion(&jugador, 67, 0, 0, 7000.0f, 0, 4000);
}

void inicializarLaser() {
	constructorLaser(&ls, jugador.posicionX, jugador.posicionY, jugador.posicionZ, jugador.angulo, jugador.angulo2);
}

void inicializarCajaTexto() {
	constructorLaser(&cajaTexto, 0, 0, 0, 0, 0);
}

void actualizarAvion(avion *p) {

	if (p->angulo3 > 0) {
		p->angulo3 -= 1;
	}
	else if (p->angulo3 < 0) {
		p->angulo3 += 1;
	}

	float velocidadX, velocidadZ, velocidadY;

	float proximoX = p->posicionX, proximoZ = p->posicionZ, proximoY = p->posicionY;

	velocidadX = p->velocidad * sin(p->angulo / 360.0 * 2 * GL_PI);
	velocidadZ = p->velocidad * cos(p->angulo / 360.0 * 2 * GL_PI);

	velocidadY = p->velocidad * -sin(p->angulo2 / 360.0 * 2 * GL_PI);

	proximoX += velocidadX / 25.0;
	proximoZ += velocidadZ / 25.0;
	proximoY += velocidadY / 25.0;

	if (!comprobarColisiones(proximoX, proximoZ, proximoY)) {
		p->posicionX = proximoX;
		p->posicionZ = proximoZ;
		p->posicionY = proximoY;
	}
	else {
		p->velocidad = 0;
	}
}

void actualizarCajaTexto(laser *cajaTexto, avion p) {
	
	cajaTexto->px = cajaTexto->px;
	cajaTexto->py = cajaTexto->py;
	cajaTexto->pz = cajaTexto->pz;

	cajaTexto->angulo1 = cajaTexto->angulo1;
	cajaTexto->angulo2 = cajaTexto->angulo2;
	cajaTexto->velocidad = cajaTexto->velocidad;
}

void actualizarLaser(laser *ls, avion p) {	
		
		ls->px = p.posicionX;
		ls->py = p.posicionY;
		ls->pz = p.posicionZ;
		
		ls->angulo1 = p.angulo;
		ls->angulo2 = p.angulo2;
		ls->velocidad = p.velocidad;
}

void dispararLaser(laser *ls) {
	float velocidadX, velocidadZ, velocidadY;

	float proximoX = ls->px, proximoZ = ls->pz, proximoY = ls->py;	
	   
	velocidadX = -600 * sin(ls->angulo1 / 360.0 * 2 * GL_PI);
	velocidadZ = -600 * cos(ls->angulo1 / 360.0 * 2 * GL_PI);

	velocidadY = -600 * -sin(ls->angulo2 / 360.0 * 2 * GL_PI);

	proximoX += velocidadX / 15.0;
	proximoZ += velocidadZ / 15.0;
	proximoY += velocidadY / 15.0;

	ls->px = proximoX;
	ls->pz = proximoZ;
	ls->py = proximoY;	
}

bool colisionLaserPlaneta(laser ls, objeto planeta) {
	float distancia= sqrt((ls.px - planeta.px) * (ls.px - planeta.px) +
		(ls.py - 0) * (ls.py - 0) +
		(ls.pz - planeta.pz) * (ls.pz - planeta.pz));	
	return distancia < planeta.tamano;	
}

bool colisionaLaserPlaneta(){
	bool colision = 0;
	if (colisionLaserPlaneta(ls, sol)) {
		planetaColision = "sol";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, mercurio)){
		planetaColision = "mercurio";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, venus)) {
		planetaColision = "venus";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, tierra)) {
		planetaColision = "tierra";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, luna)) {
		planetaColision = "luna";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, iss)) {
		planetaColision = "iss";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, marte)) {
		planetaColision = "marte";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, jupiter)) {
		planetaColision = "jupiter";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, saturno)) {
		planetaColision = "saturno";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, urano)) {
		planetaColision = "urano";
		colision = 1;
	}
	else if (colisionLaserPlaneta(ls, neptuno)) {
		planetaColision = "neptuno";
		colision = 1;
	}
	return colision;
}

unsigned int cargaTextura(const char *nombre) {
	//TEXTURAS

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(nombre, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		/*glGenerateMipmap(GL_TEXTURE_2D)*/;
	}
	else {
		/*std::cout << "Failed to load texture" << std::endl;*/
	}
	stbi_image_free(data);
	return texture;
}

void inicializarTexturas(void) {
	texturaAbajoSkybox = cargaTextura("spaceSkybox/lightblue/bot.png");
	texturaArribaSkybox = cargaTextura("spaceSkybox/lightblue/top.png");
	texturaDerechaSkybox = cargaTextura("spaceSkybox/lightblue/right.png");
	texturaIzquierdaSkybox = cargaTextura("spaceSkybox/lightblue/left.png");
	skyboxNor = cargaTextura("spaceSkybox/lightblue/front.png");
	skyboxSur = cargaTextura("spaceSkybox/lightblue/back.png");
	texturaAvion = cargaTextura("texturas/texturaAvion.bmp");
}

void actualizarColisiones(float x, float z, float y) {
	jugador.delanteIzq[0] = x + DIAGONAL * sin((jugador.angulo - 45.0f) / 360.0 * 2 * PI);
	jugador.delanteIzq[1] = z + DIAGONAL * cos((jugador.angulo - 45.0f) / 360.0 * 2 * PI);
	jugador.delanteIzq[2] = y + DIAGONAL * -sin((jugador.angulo2 - 100.0f) / 360.0 * 2 * PI);

	jugador.delanteDer[0] = x + DIAGONAL * sin((jugador.angulo + 45.0f) / 360.0f * 2 * PI);
	jugador.delanteDer[1] = z + DIAGONAL * cos((jugador.angulo + 45.0f) / 360.0f * 2 * PI);
	jugador.delanteDer[2] = y + DIAGONAL * -sin((jugador.angulo2 + 100.0f) / 360.0 * 2 * PI);

	jugador.detrasIzq[0] = x + DIAGONAL * sin((jugador.angulo - 135.0f) / 360.0f * 2 * PI);
	jugador.detrasIzq[1] = z + DIAGONAL * cos((jugador.angulo - 135.0f) / 360.0f * 2 * PI);
	jugador.detrasIzq[2] = y + DIAGONAL * -sin((jugador.angulo2 - 100.0f) / 360.0 * 2 * PI);

	jugador.detrasDer[0] = x + DIAGONAL * sin((jugador.angulo + 135.0f) / 360.0f * 2 * PI);
	jugador.detrasDer[1] = z + DIAGONAL * cos((jugador.angulo + 135.0f) / 360.0f * 2 * PI);
	jugador.detrasDer[2] = y + DIAGONAL * -sin((jugador.angulo2 + 100.0f) / 360.0 * 2 * PI);
}
bool comprobarColisiones(float x, float z, float y) {
	actualizarColisiones(x, z, y);

	if (jugador.detrasIzq[0] < -4000.0*N || jugador.detrasIzq[0] > 4000.0*N || jugador.detrasIzq[1] < -4000.0*N || jugador.detrasIzq[1] > 4000.0*N || jugador.detrasIzq[2] < -4000.0*N || jugador.detrasIzq[2] > 4000.0*N// Comprobación de que esté fuera del escenario
		|| jugador.detrasDer[0] < -4000.0*N || jugador.detrasDer[0] > 4000.0*N || jugador.detrasDer[1] < -4000.0*N || jugador.detrasDer[1] > 4000.0*N || jugador.detrasDer[2] < -4000.0*N || jugador.detrasDer[2] > 4000.0*N
		|| jugador.delanteDer[0] < -4000.0*N || jugador.delanteDer[0] > 4000.0*N || jugador.delanteDer[1] < -4000.0*N || jugador.delanteDer[1] > 4000.0*N || jugador.delanteDer[2] < -4000.0*N || jugador.delanteDer[2] > 4000.0*N
		|| jugador.delanteIzq[0] < -4000.0*N || jugador.delanteIzq[0] > 4000.0*N || jugador.delanteIzq[1] < -4000.0*N || jugador.delanteIzq[1] > 4000.0*N || jugador.delanteIzq[2] < -4000.0*N || jugador.delanteIzq[2] > 4000.0*N) {

		return 1;
	}
	else {
		return 0;
	}
}

void mostrarInformacion(std::string planetaColision) {
	
	std::string str;
	std::string strTXT;
	std::string text;
	int j = 0;
	y = 510;
	if (planetaColision == "sol") {
		strTXT = "textos/sol.txt";		
		std::cout << "SOL" << std::endl;
	}
	else if (planetaColision == "mercurio"){
		strTXT = "textos/mercurio.txt";
		std::cout << "Mercurio" << std::endl;
	}
	else if (planetaColision == "venus") {
		strTXT = "textos/venus.txt";
		std::cout << "Venus" << std::endl;
	}
	else if (planetaColision == "tierra") {
		strTXT = "textos/tierra.txt";
		std::cout << "tierra" << std::endl;
	}
	else if (planetaColision == "luna") {
		strTXT = "textos/luna.txt";
		std::cout << "luna" << std::endl;
	}
	else if (planetaColision == "iss") {
		strTXT = "textos/iss.txt";
		std::cout << "iss" << std::endl;
	}
	else if (planetaColision == "marte") {
		strTXT = "textos/marte.txt";
		std::cout << "marte" << std::endl;
	}
	else if (planetaColision == "jupiter") {
		strTXT = "textos/jupiter.txt";
		std::cout << "jupiter" << std::endl;
	}
	else if (planetaColision == "saturno") {
		strTXT = "textos/saturno.txt";
		std::cout << "saturno" << std::endl;
	}
	else if (planetaColision == "urano") {
		strTXT = "textos/urano.txt";
		std::cout << "urano" << std::endl;
	}
	else if (planetaColision == "neptuno") {
		strTXT = "textos/neptuno.txt";
		std::cout << "neptuno" << std::endl;
	}
	else if (planetaColision == "controles") {
		strTXT = "textos/controles.txt";
		std::cout << "controles" << std::endl;
	}
	std::ifstream file(strTXT);
	while (std::getline(file, str)) {
		text = str;
		length = text.size();
		x = 110;
		y -= interlineado;

		glMatrixMode(GL_PROJECTION);
		matrix = new double[16];
		glGetDoublev(GL_PROJECTION_MATRIX, matrix);
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glLoadIdentity();
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2i(x, y);
		for (int i = 0; i < length; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
		}
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(matrix);
		glMatrixMode(GL_MODELVIEW);
		j++;
	}
}

int cargaTextura(char const *nombreArchivo, objeto* object) {
	glGenTextures(1, &object->textura);
	//glBindTexture(GL_TEXTURE_2D, object->textura[0]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

	glBindTexture(GL_TEXTURE_2D, object->textura);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(1);

	unsigned char *data = stbi_load(nombreArchivo, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); //con mimap 
		return 1;
	}
	else
	{
		return 0;	//en caso de error
	}
}
void inicializaTexturas() {
	cargaTextura("texturas/sol.bmp", &sol);
	cargaTextura("texturas/mercurio.bmp", &mercurio);
	cargaTextura("texturas/venus.bmp", &venus);
	cargaTextura("texturas/tierra.bmp", &tierra);
	cargaTextura("texturas/luna.bmp", &luna);
	cargaTextura("texturas/iss.bmp", &iss);
	cargaTextura("texturas/marte.bmp", &marte);
	cargaTextura("texturas/jupiter.bmp", &jupiter);
	cargaTextura("texturas/saturno.bmp", &saturno);
	cargaTextura("texturas/urano.bmp", &urano);
	cargaTextura("texturas/neptuno.bmp", &neptuno);
}
void actualizarMovimiento(objeto* object) {
	object->angulo_trans -= object->velocidad_trans;
	if (object->angulo_trans == -360) object->angulo_trans = 0;

	object->angulo_rot += object->velocidad_rot;
	if (object->angulo_rot == 360) object->angulo_rot = 0;

	object->px = cos(object->angulo_trans)*object->distancia;
	object->pz = sin(object->angulo_trans)*object->distancia;

}
void actualizarMovimientoSatelite(objeto* satelite, objeto planeta) {
	satelite->angulo_trans -= satelite->velocidad_trans;
	if (satelite->angulo_trans == -360) satelite->angulo_trans = 0;

	satelite->angulo_rot += satelite->velocidad_rot;
	if (satelite->angulo_rot == 360) satelite->angulo_rot = 0;

	satelite->px = cos(planeta.angulo_trans)*planeta.distancia + cos(satelite->angulo_trans)*satelite->distancia;
	satelite->pz = sin(planeta.angulo_trans)*planeta.distancia + sin(satelite->angulo_trans)*satelite->distancia;

}
void myMovimiento(int i) {
	//sol
	actualizarMovimiento(&sol);
	//mercurio
	actualizarMovimiento(&mercurio);
	//venus
	actualizarMovimiento(&venus);
	//tierra
	actualizarMovimiento(&tierra);
	//luna
	actualizarMovimientoSatelite(&luna, tierra);
	//iss
	actualizarMovimientoSatelite(&iss, tierra);
	//marte
	actualizarMovimiento(&marte);
	//jupiter
	actualizarMovimiento(&jupiter);
	//saturno
	actualizarMovimiento(&saturno);
	//urano
	actualizarMovimiento(&urano);
	//neptuno
	actualizarMovimiento(&neptuno);

	glutPostRedisplay();
	glutTimerFunc(MYTIEMPO, myMovimiento, 0);
}

void inicializarNiebla(void) {
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, color);
	glFogf(GL_FOG_DENSITY, 0.15f);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 170.0f);
	glFogf(GL_FOG_END, 25000.0f);
}
