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

int archivo_a_glist( GList lista , char *nombre ) {
	int i = 0;
	FILE *archivo;
	archivo = fopen( nombre , "r" );
	while( !feof( archivo ) ) {
		char *nombre;
		fscanf( archivo , nombre );
		lista = glist_agregar_final( lista , nombre );
		i++;
	}
	return i;
}

int main() {
	GList nombres = glist_crear()  , lugares = glist_crear()  , personas = glist_crear();
	int cantN , cant L;
	cantN = archivo_a_glist( nombres , "nombres.txt" );
	cantL = archivo_a_glist( lugares , "lugares.txt" );
	
	return 0;
}
