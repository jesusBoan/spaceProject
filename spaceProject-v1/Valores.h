#pragma once
typedef struct
{
	float x;
	float y;
	float z;
}punto;

typedef struct {
	float x;
	float y;
	float z;
}vector;


typedef struct {
	float distancia;
	float velocidad_trans;
	float angulo_trans;
	float velocidad_rot;
	float angulo_rot;
	int tamano;
	int listaRender;
	int listaOrbita;
	float iluminacion;
	float colores[3];
	GLuint textura;
	float px;
	float pz;
}objeto;


#define CR 0.0175
#define MYTIEMPO 41

void myCamara();
void myTeclado(unsigned char trans, int x, int y);
void myTeclasespeciales(int cursor, int x, int y);
void myEjes();
