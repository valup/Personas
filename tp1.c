#include <stdio.h>
#include <stdlib.h>

typedef struct _GNodo {
	Persona * persona ;
	struct _GNodo * sig ;
} GNodo ;

typedef GNodo * GList ;

typedef int (* Predicado ) ( void * dato ) ;

typedef void * (* Funcion ) ( void * dato ) ;

typedef struct {
char * nombre ;
int edad ;
char * lugarDeNacimiento ; // pais o capital
} Persona ;

GList glist_crear() {
  return NULL;
}

GList glist_agregar_final(GList lista, int dato) {
  GNodo *nuevoNodo = malloc(sizeof(GNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;

  if (lista == NULL)
    return nuevoNodo;

  GList nodo = lista;
  for (;nodo->sig != NULL;nodo = nodo->sig);

  nodo->sig = nuevoNodo;
  return lista;
}

//GList archivo_a_glist(GList lista){
  //FILE * archivo;
  //char * nombreArchivo = malloc(sizeof(char) * );
  //printf("Ingrese el nombre del archivo con .txt al final:\n");
  //scanf("%s",nombreArchivo);
  //archivo=fopen(nombreArchivo, "r");
  //int i=0;
  //char c=fgetc(archivo);
  //while(i<100&&c!=EOF){
	  //ungetc(c,archivo);
	  //fgets(listPalabras[i],100,(FILE*)archivo);
    //listPalabras[i][strlen(listPalabras[i])-1]='\0';
	  //i++;
	  //c=fgetc(archivo);
  //}
  //fclose(archivo);
  //return i;
//}

int glist_len ( GList lista ) {
	GList i = lista ;
	int j = 0 ;
	for ( ; i->sig != NULL ; i = i->sig ) {
		j++ ;
	}
	return j ;
}

GList filter ( GList lista , Predicado p ) {
	GList filtrada = glist_crear() , i = lista ;
	for ( ; i->sig != NULL ; i = i->sig ) {
		if ( p(i->dato) ) {
			glist_agregar_final( filtrada , i->dato ) ;
		}
	}
    if ( p(i->dato) ) {
        glist_agregar_final( filtrada , i->dato ) ;
    }
    return  filtrada ;
}

GList map ( GList lista, Funcion f ) {
    GList nueva = glist_crear() , i = lista ;
    for (; i->sig != NULL , i = i->sig ){
        glist_agregar_final( nueva , f( i->dato ) ) ;
    }
    glist_agregar_final( nueva , f( i->dato ) ) ;
    return nueva ;
}

void glist_a_archivo( GList lista ) {
    FILE * salida ;
    char * nombre ;
    printf( "Ingrese el nombre del archivo donde guardar la lista: " ) ;
    scanf( "%s" , &nombre ) ;
    //comprobar que existe
    salida = fopen( strcat( nombre , ".txt" ) , "w" ) ;
    int i = lista ;
    for(; i->sig != NULL ; i = i->sig ) {
        

int main() {
    Persona * listaP = malloc ( sizeof ( Persona ) * 3000 ) ;
    archivo_a_glist( listaP ) ;
    Persona * filtrada1 = malloc ( sizeof ( Persona ) * 3000 ) , filtrada2 = malloc ( sizeof ( Persona ) * 3000 ) ;
    Persona * map1 = malloc ( sizeof ( Persona ) * 3000 ) , map2 = malloc ( sizeof ( Persona ) * 3000 ) ;
    
}

