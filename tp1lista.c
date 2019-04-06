#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
char *nombre;
int edad;
char *lugarDeNacimiento; // pais o capital
} Persona;

typedef struct _GNodo {
  Persona *persona;
  struct _GNodo *sig;
} GNodo;

typedef GNodo *GList;

typedef struct _SNodo {
  void *dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

GList glist_crear() {
	return NULL;
}

GList glist_agregar_final( GList lista , Persona *persona ) {
	GNodo *nuevoNodo = malloc( sizeof( GNodo ) );
	nuevoNodo->persona = persona;
	nuevoNodo->sig = NULL;
	if( lista == NULL ) {
		return nuevoNodo;
	}
	GList nodo = lista;
	for(; nodo->sig != NULL ; nodo = nodo->sig);
	nodo->sig = nuevoNodo;
	return lista;
}

SList slist_crear() {
	return NULL;
}

SList slist_agregar_final( SList lista , char* string ) {
	SNodo *nuevoNodo = malloc( sizeof( SNodo ) );
	nuevoNodo->dato = string;
	nuevoNodo->sig = NULL;
	if( lista == NULL ) {
		return nuevoNodo;
	}
	SList nodo = lista;
	for(; nodo->sig != NULL ; nodo = nodo->sig);
	nodo->sig = nuevoNodo;
	return lista;
}

int archivo_a_slist( SList lista , char *nombre ) {
	int i = 0;
	FILE *archivo;
	archivo = fopen( nombre , "r" );
	while( !feof( archivo ) ) {
		char *s = malloc( sizeof( char ) * 20 );
		fscanf( archivo , "%s" , s );
		lista = slist_agregar_final( lista , s );
		i++;
	}
	return i;
}

char* elegir( SList opciones , int cant ) {
	int i = 0 , pos = rand() % cant;
	SList l = opciones;
	for(; i < pos ; i++ ) {
		printf( "elegir-for\n" );
		l = l->sig;
		printf( "l-sig\n" );
	}
	return l->dato;
}

Persona* persona_random( SList nombres , int cantN , SList lugares , int cantL ) {
	Persona *persona = malloc( sizeof( Persona ) );
	persona->nombre = elegir( nombres , cantN );
	persona->edad = ( rand() % 100 ) + 1;
	persona->lugarDeNacimiento = elegir( lugares , cantL );
	return persona;
}
	

void lista_personas( GList lista , SList nombres , int cantN , SList lugares , int cantL ) {
	int i = 0 , cant = ( rand() %  8001 ) + 2000;
	for(; i < cant ; i++ ) {
		lista = glist_agregar_final( lista , persona_random( nombres , cantN , lugares , cantL ) );
	}
}

void persona_a_archivo( Persona *persona , FILE *archivo ) {
	fputs( persona->nombre , archivo );
	fputc( ',' , archivo );
	fprintf( archivo , "%d" , persona->edad );
	fputc( ',' , archivo );
	fputs( persona->lugarDeNacimiento , archivo );
	fputc( '\n' , archivo );
}

void glist_a_archivo( GList lista ) {
    FILE *salida ;
    char *nombre = malloc( sizeof( char ) * 20 );
    printf( "Ingrese el nombre del archivo donde guardar la lista generada (max 20 caracteres): " ) ;
    scanf( "%s" , nombre ) ;
    salida = fopen( strcat( nombre , ".txt" ) , "w" ) ;
    GList i = lista ;
    for(; i->sig != NULL ; i = i->sig ) {
		persona_a_archivo( i->persona , salida );
	}
	persona_a_archivo( i->persona , salida );
}

int main() {
	SList nombres = slist_crear() , lugares = slist_crear();
	GList personas = glist_crear();
	int cantN , cantL;
	cantN = archivo_a_slist( nombres , "nombres.txt" );
	cantL = archivo_a_slist( lugares , "paises.txt" );
	
	lista_personas( personas , nombres , cantN , lugares , cantL );
	
	printf( "a archivo\n" );
	glist_a_archivo( personas );
	
	return 0;
}
