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

//copiar: void * -> void *
//Recibe un puntero a una persona (pero lo recibe como puntero a void) y copia los datos guardados en la estructura de persona
//en una nueva direccion
//Luego devuelve esta nueva direccion a persona como puntero a void
void * copiar( void * persona ) {
	int maxN = 15, maxL = 45, edad;
	Persona *aux = (Persona *) persona , *copia = malloc( sizeof( Persona ) );
	char *nombre = malloc( sizeof( char ) * maxN ), *lugar = malloc( sizeof( char ) * maxL );
	strcpy(nombre,aux->nombre);
	edad = aux->edad;
	strcpy(lugar,aux->lugarDeNacimiento);
	copia->nombre = nombre;
	copia->edad = edad;
	copia->lugarDeNacimiento = lugar;
	return (void *) copia;
}

//destruirP: void * -> void
//Recibe un puntero a una persona (como puntero a void) y libera primero los datos que guarda y luego la persona en si
void destruirP ( void *persona ) {
	Persona * aux = (Persona *) persona;
	free( aux->nombre );
	free( aux->lugarDeNacimiento );
	free( aux );
}

//destruir: GList-> void
//Recibe una lista de personas y llama a un destruirP para cada una para liberarlas
//Luego libera la lista
void destruir( GList listaPersonas ) {
	for (GNodo *nodo = listaPersonas; nodo != NULL;) {
		GNodo *temp = nodo;
		destruirP( (void *) nodo->dato );
		nodo = nodo->sig;
		free(temp);
	}
	return;
}

//glist_imprimir: GList -> void
//Recibe una lista de personas e imprime los datos que contiene cada una de ellas
void glist_imprimir(GList lista) {
	for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig) {
		Persona *aux = (Persona *) nodo->dato;
		printf("%s\n",aux->nombre);
		printf("%d\n",aux->edad);
		printf("%s\n",aux->lugarDeNacimiento);
  }
}

//imprimir_persona: Persona * -> void
//Recibe un puntero a persona e imprime cada dato contenido en la estructura
//void imprimir_persona(Persona *persona) {
	//printf("%s\n",persona->nombre);
	//printf("%d\n",persona->edad);
	//printf("%s\n",persona->lugarDeNacimiento);
//}


//contarLineas: FILE * -> int
//Recibe un puntero a un archivo abierto y recorre el archivo contando la cantidad de lineas
//Luego devuelve el resultado
//int contarLineas( FILE *archivo ){
	//char* l;
	//int lineas = 0;
	//while( !feof( archivo ) ){
		//fscanf( archivo , "%s" , l );
		//lineas++;
	//}
	//rewind( archivo );
	//return lineas;
//}

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
	int i = 0 ,j=0, maxN = 15 , maxE = 4 , maxL = 45;
	char *nombre = malloc( sizeof( char ) * maxN ) , *edad = malloc( sizeof( char ) * maxE ) , *lugar = malloc( sizeof( char ) * maxL );
	
	for(; string[i] != ',' ; i++,j++) {
		nombre[j] = string[i];
	}
	nombre[j] = '\0';
	p->nombre = nombre;
	//printf("%s\n",p->nombre);
	i++;
	
	for(j=0; string[i] != ',' ; i++,j++) {
		edad[j] = string[i];
	}
	edad[j] = '\0';
	p->edad = atoi(edad);
	//printf("%d\n",p->edad);
	i++;
	
	for(j=0; string[i] != '\0' ; i++,j++) {
		lugar[j] = string[i];
	}
	lugar[j] = '\0';
	p->lugarDeNacimiento = lugar;
	//printf("%s\n",p->lugarDeNacimiento);
	
	free(edad);
	
	return p;
}

GList archivo_a_glist( GList lista , FILE *archivo ) {
	int longMax = 65;
	while( !feof( archivo ) ) {
		char* persona = malloc( sizeof( char ) * longMax );
		if (fgets( persona , longMax , archivo) != NULL) {
			if( persona[ strlen( persona ) - 1 ] == '\n' ) {
				persona[ strlen( persona ) - 1 ] = '\0';
			}
			lista = glist_agregar_inicio( lista , (void *) string_a_persona( persona ) );
		}
		free(persona);
	}
	fclose( archivo );
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

void glist_a_archivo( GList lista , char* tipo ) {
	FILE *salida ;
    char *nombre = malloc( sizeof( char ) * 20 ) ;
    printf( "Ingrese el nombre del archivo donde guardar la lista %s (max 20 caracteres): " , tipo ) ;
    scanf( "%s" , nombre ) ;
    if( (salida = fopen(strcat(nombre , ".txt") , "w")) ) {
		for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig) {
			persona_a_archivo(nodo->dato , salida);
		}
	}
	free(nombre);
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
	GList filtrada = glist_crear();
	for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig) {
		if( (p(nodo->dato)) ) {
			filtrada = glist_agregar_inicio( filtrada , c((void *) nodo->dato));
		}
	}
    return  filtrada ;
}

GList map( GList lista, Funcion f , Copia c ) {
    GList nueva = glist_crear();
    for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig){
        nueva = glist_agregar_inicio(nueva , f( c((void *) nodo->dato)) );
    }
    return nueva ;
}

int menor( void * persona ) {
	return ((Persona *) persona)->edad < 18;
}

int inicial_A( void * persona ) {
	return ((Persona *) persona)->nombre[0] == 'A';
}

void * doble_edad( void * persona ) {
	Persona * aux = (Persona *) persona;
	int i = aux->edad;
	aux->edad = i * 2;
	return aux;
}

void * mitad_edad( void * persona ) {
	Persona * aux = (Persona *) persona;
	int i = aux->edad;
	aux->edad = i / 2;
	return aux;
} 

int main() {
	FILE* archivo;
	char* nombre = malloc( sizeof( char ) * 20 );

	printf( "Ingrese el nombre del archivo que contiene la lista de personas (max 20 caracteres): " );
	scanf( "%s" , nombre );

	if( (archivo = fopen( strcat( nombre , ".txt" )  ,  "r"  )) ) {
		GList listaP = glist_crear() , filtrada1 = glist_crear() , filtrada2 = glist_crear() , map1 = glist_crear() , map2 = glist_crear();
		
		listaP = archivo_a_glist( listaP , archivo ) ;
		//glist_imprimir(listaP);
		
		filtrada1 = filter( listaP , menor , copiar );
		//printf("filtrada1:\n");
		//glist_imprimir(filtrada1);
		glist_a_archivo( filtrada1 , "filtrada 1" );
		destruir( filtrada1 );

		filtrada2 = filter( listaP , inicial_A , copiar );
		//printf("filtrada2:\n");
		//glist_imprimir(filtrada2);
		glist_a_archivo( filtrada2 , "filtrada 2" );
		destruir( filtrada2 );

		map1 = map( listaP , doble_edad , copiar );
		//printf("map1:\n");
		//glist_imprimir(map1);
		glist_a_archivo( map1 , "map 1" );
		destruir( map1 );

		map2 = map( listaP , mitad_edad , copiar );
		//printf("map2:\n");
		//glist_imprimir(map2);
		glist_a_archivo( map2 , "map 2" );
		destruir( map2 );

		destruir( listaP );
	}
	else{
		printf( "Error: el archivo no existe.\n" );
	}
	free(nombre);
	return 0;
}
