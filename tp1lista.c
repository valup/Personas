#include <stdio.h>
#include <stdlib.h>

typedef struct {
char * nombre ;
int edad ;
char * lugarDeNacimiento ; // pais o capital
} Persona ;

typedef struct _SNodo {
  int dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;
