#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef struct _GNodo {
	void * dato ;
	struct _GNodo * sig ;
} GNodo ;

typedef GNodo * GList ;

typedef int (* Predicado ) ( void * dato ) ;

typedef void * (* Funcion ) ( void * dato ) ;

typedef void * (* Copia) (void *) ;

typedef void (* Destruir) (void *) ;

typedef struct {
	char * nombre ;
	int edad ;
	char * lugarDeNacimiento ; // pais o capital
} Persona ;

void * copiar( void * persona ) {
	return persona;
}

void destruirP ( void *persona ) {
	Persona * aux = (Persona *) persona;
	free( aux->nombre );
	free( aux->lugarDeNacimiento );
	free( aux );
}
	
void destruir( GList listaPersonas ) {
	GList i = listaPersonas;
	for(; i->sig != NULL ; i = i->sig ) {
		destruirP( (void *) i->dato );
	}
	destruirP( (void *) i->dato );
	free( listaPersonas );
	return;
}

int contarLineas( FILE *archivo ){
	char* l;
	int lineas = 0;
	while( !feof( archivo ) ){
		fscanf( archivo , "%s" , l );
		lineas++;
	}
	rewind( archivo );
	return lineas;
}

GList glist_crear() {
	return NULL;
}

GList glist_agregar_inicio(GList lista, void *dato) {
  GNodo *nuevoNodo = malloc(sizeof(GNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

Persona * string_a_persona( char* string ) {
	Persona *p = malloc( sizeof( Persona ) );
	char *nombre , *edad , *lugar;
	int i = 0;
	
	for(; string[i] != ',' ; i++ ) {
		nombre[i] = string[i];
	}
	nombre[i] = '\0';
	p->nombre = nombre;
	i++;
	
	for(; string[i] != ',' ; i++ ) {
		edad[i] = string[i];
	}
	edad[i] = '\0';
	p->edad = atoi(edad);
	i++;
	
	for(; string[i] != '\0' ; i++ ) {
		lugar[i] = string[i];
	}
	lugar[i] = '\0';
	p->lugarDeNacimiento = lugar;
	
	return p;
}

void archivo_a_glist( GList lista , FILE *archivo ) {
	int longMax = 65;
	while( !feof( archivo ) ) {
		char* persona = "";
		fgets( persona , longMax , archivo );
		persona[ strlen( persona ) - 1 ] = '\0';
		glist_agregar_inicio( lista , (void *) string_a_persona( persona ) );
	}
	fclose( archivo );
}

void persona_a_archivo( Persona *persona , FILE *archivo ) {
	fputs( persona->nombre , archivo );
	fputc( ',' , archivo );
	fputs( itoa( persona->edad ) , archivo );
	fputc( ',' , archivo );
	fputs( persona->lugarDeNacimiento , archivo );
	fputc( '\n' , archivo );
}

void glist_a_archivo( GList lista , char* tipo ) {
    FILE *salida ;
    char *nombre = "" ;
    printf( "Ingrese el nombre del archivo donde guardar la lista %s: " , tipo ) ;
    scanf( "%s" , nombre ) ;
    salida = fopen( strcat( nombre , ".txt" ) , "w" ) ;
    GList i = lista ;
    for(; i->sig != NULL ; i = i->sig ) {
		persona_a_archivo( i->dato , salida );
	}
	persona_a_archivo( i->dato , salida );
}

int glist_len( GList lista ) {
	GList i = lista ;
	int j = 0 ;
	for( ; i->sig != NULL ; i = i->sig ) {
		j++ ;
	}
	return j ;
}

GList filter( GList lista , Predicado p , Copia c ) {
	GList filtrada = glist_crear() , i = lista ;
	for( ; i->sig != NULL ; i = i->sig ) {
		if( p( i->dato ) ) {
			c( i->dato ) ;
		}
	}
    if( p(i->dato) ) {
        c( i->dato ) ;
    }
    return  filtrada ;
}

GList map( GList lista, Funcion f , Copia c ) {
    GList nueva = glist_crear() , i = lista ;
    for(; i->sig != NULL ; i = i->sig ){
		f( (void *) i->dato );
        c( (void *) i->dato ) ;
    }
    f( i->dato );
    c( i->dato ) ;
    return nueva ;
}

int menor( void * persona ) {
	return ((Persona *) persona)->edad < 18;
}

int inicial_A( void * persona ) {
	return ((Persona *) persona)->nombre[0] = 'A';
}

void * doble( void * persona ) {
	Persona * aux = (Persona *) persona;
	int i = aux->edad;
	aux->edad = i * 2;
	return aux;
}

int main() {
	FILE* archivo;
	char* nombre;

	printf( "Ingrese el nombre del archivo que contiene la lista de personas: " );
	scanf( "%s" , nombre );
	printf( "Nombre: %s" , nombre );

	if( (archivo = fopen( strcat( nombre , ".txt" )  ,  "r"  )) ) {
		//int lineas = contarLineas( archivo );
		GList listaP = glist_crear() , filtrada1 = glist_crear() , filtrada2 = glist_crear() , map1 = glist_crear() , map2 = glist_crear();

		archivo_a_glist( listaP , archivo ) ;

		filtrada1 = filter( listaP , menor , copiar );
		glist_a_archivo( filtrada1 , "filtrada 1" );
		destruir( filtrada1 );

		filtrada2 = filter( listaP , inicial_A , copiar );
		glist_a_archivo( filtrada2 , "filtrada 2" );
		destruir( filtrada2 );

		map1 = map( listaP , doble , copiar );
		glist_a_archivo( map1 , "map 1" );
		destruir( map1 );

		map2 = map( listaP , doble , copiar );
		glist_a_archivo( map2 , "map 2" );
		destruir( map2 );

		destruir( listaP );
	}
	else{
		printf( "Error: el archivo no existe.\n" );
	}
	return 0;
}
