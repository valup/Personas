#include <stdio.h>
#include <stdlib.h>

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

Copia copiar( GList lista , Persona persona ) {
	
}

Destruir destruir( GList listaPersonas ) {
	GList i = listaPersonas;
	for(; i->sig != NULL ; i = i->sig ) {
		free( i->dato->nombre );
		free( i->dato->lugarDeNacimiento );
		free( i->dato );
	}
	free( i->dato->nombre );
	free( i->dato->lugarDeNacimiento );
	free( i->dato );
	
	free( listaPersonas );
}

int contarLineas( FILE *archivo ){
	char* c;
	int lineas = 0;
	while( !feof( archivo ) ){ 
		fscanf( archivo , "%s" , c );
		lineas++;
	}
	rewind( archivo );
	return lineas;
}

GList glist_crear() {
	return NULL;
}

GList glist_agregar_final( GList lista , Persona persona ) {
	GNodo *nuevoNodo = malloc( sizeof( GNodo ) );
	nuevoNodo->dato = persona;
	nuevoNodo->sig = NULL;
	if( lista == NULL ) {
		return nuevoNodo;
	}
	GList nodo = lista;
	for(; nodo->sig != NULL ; nodo = nodo->sig);
	nodo->sig = nuevoNodo;
	return lista;
}

Persona string_a_persona( char* string ) {
	Persona *p = malloc( sizeof( Persona ) );
	char *nombre , *edad , *lugar;
	int i = 0;
	for(; string[i] != "," ; i++ ) {
		nombre[i] = string[i];
	}
	nombre[i] = '\0';
	p->nombre = nombre;
	i++;
	for(; string[i] != "," ; i++ ) {
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

void archivo_a_glist( GList lista , FILE *archivo ){
	int i=0;
	for( !feof( archivo ) ) {
		char* persona;
		fscanf( archivo , "%s" , persona );
		glist_agregar_final( lista , string_a_persona( persona ) );
	}
	fclose( archivo );
}

void persona_a_archivo( Persona persona , FILE *archivo ) {
	fputs( persona->nombre , archivo );
	fputs( "," , archivo );
	fputs( persona->edad , archivo );
	fputs( "," , archivo );
	fputs( persona->lugarDeNacimiento , archivo );
	fputs( "\n" , archivo );
}

void glist_a_archivo( GList lista , char* tipo ) {
    FILE *salida ;
    char *nombre ;
    printf( "Ingrese el nombre del archivo donde guardar la lista %s: " , tipo ) ;
    scanf( "%s" , &nombre ) ;
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
			c( filtrada , i->dato ) ;
		}
	}
    if( p(i->dato) ) {
        c( filtrada , i->dato ) ;
    }
    return  filtrada ;
}

GList map( GList lista, Funcion f , Copia c ) {
    GList nueva = glist_crear() , i = lista ;
    for(; i->sig != NULL , i = i->sig ){
        c( nueva , f( i->dato ) ) ;
    }
    c( nueva , f( i->dato ) ) ;
    return nueva ;
}

int main() {
	FILE* archivo;
	char* nombre;

	printf( "Ingrese el nombre del archivo que contiene la lista de personas: " );
	scanf( "%s" , &nombre );
	printf( "Nombre: %s" , nombre );

	if( archivo = fopen( strcat( nombre , ".txt" )  ,  "r"  ) ) {
		int lineas = contarLineas( archivo );
		Persona * listaP = malloc( sizeof( Persona ) * lineas ) ;

		archivo_a_glist( listaP , archivo ) ;

		Persona * filtrada1 = malloc( sizeof( Persona ) * lineas ) , filtrada2 = malloc( sizeof( Persona ) * lineas ) ;
		Persona * map1 = malloc( sizeof( Persona ) * lineas ) , map2 = malloc( sizeof( Persona ) * lineas ) ;

		filtrada1 = filter( listaP , predicado1 , copiar );
		glist_a_archivo( filtrada1 , "filtrada 1" );
		destruir( fitrada1 );

		filtrada2 = filter( listaP , predicado2 , copiar );
		glist_a_archivo( filtrada2 , "filtrada 2" );
		destruir( fitrada2 );

		map1 = map( listaP , funcion1 , copiar );
		glist_a_archivo( map1 , "map 1" );
		destruir( map1 );

		map2 = map( listaP , funcion2 , copiar );
		glist_a_archivo( map2 , "map 2" );
		destruir( map2 );

		destruir( listaP );
	}
	else{
		printf( "Error: el archivo no existe.\n" );
	}
    return 0;
}
