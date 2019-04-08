#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef struct {
char *nombre;
int edad;
char *lugarDeNacimiento; // pais o capital
} Persona;

typedef struct _GNodo {
  Persona *dato;
  struct _GNodo *sig;
} GNodo;

typedef GNodo *GList;

GList glist_crear() {
	return NULL;
}

GList glist_agregar_inicio(GList lista, void *dato) {
  GNodo *nuevoNodo = malloc(sizeof(GNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

void glist_imprimir(GList lista) {
  for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig)
    printf("%s\n",(char *)nodo->dato);
}

GList archivo_a_glist( GList lista , char *nombre , int *cant ) {
	int i = 0 , longMax = 50;
	FILE *archivo;
	archivo = fopen( nombre , "r" );
	while( !feof( archivo ) ) {
		char *s = malloc( sizeof( char ) * longMax );
		fgets( s , longMax , archivo );
		if( strlen( s ) > 1 ) {
			s[ strlen( s ) - 1 ] = '\0';
			if( s[ strlen( s ) - 1 ] == '\r' ) {
				s[ strlen( s ) - 1 ] = '\0';
			}
		}
		lista = glist_agregar_inicio( lista , s );
		i++;
	}
	*cant = i;
	glist_imprimir(lista);
	return lista;
}

void* elegir( GList opciones , int cant ) {
	int i = 0 , pos = rand() % cant;
	GList l = opciones;
	for(; i < pos ; i++ ) {
		assert(l != NULL);
		l = l->sig;
	}
	assert(l != NULL);
	return l->dato;
}

Persona* persona_random( GList nombres , int cantN , GList lugares , int cantL ) {
	Persona *persona = malloc( sizeof( Persona ) );
	persona->nombre = elegir( nombres , cantN );
	persona->edad = ( rand() % 100 ) + 1;
	persona->lugarDeNacimiento = elegir( lugares , cantL );
	return persona;
}
	

GList lista_personas( GList lista , GList nombres , int cantN , GList lugares , int cantL ) {
	int i = 0 , cant = ( rand() %  8001 ) + 2000;
	for(; i < cant ; i++ ) {
		lista = glist_agregar_inicio( lista , (void *)persona_random( nombres , cantN , lugares , cantL ) );
	}
	return lista;
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
		persona_a_archivo( i->dato , salida );
	}
	persona_a_archivo( i->dato , salida );
}

int main() {
	GList nombres = glist_crear() , lugares = glist_crear() , personas = glist_crear();
	int cantN , cantL;
	nombres = archivo_a_glist( nombres , "nombres.txt" , &cantN );
	lugares = archivo_a_glist( lugares , "paises.txt" , &cantL );
	printf("%d, %d\n" , cantN , cantL );
	//assert(NULL);
	
	personas = lista_personas( personas , nombres , cantN , lugares , cantL );
	
	glist_a_archivo( personas );
	
	return 0;
}
