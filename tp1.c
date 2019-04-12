#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef struct _GNodo {
  void * dato;
  struct _GNodo * sig;
} GNodo;

typedef GNodo *GList;

typedef int (*Predicado) (void *dato);

typedef void *(*Funcion) (void *dato);

typedef void *(*Copia) (void *);

typedef void (*Destruir) (void *);

typedef struct {
  char *nombre;
  int edad;
  char *lugarDeNacimiento; // pais o capital
} Persona;

//copiar: void* -> void*
//Recibe un puntero a una persona (pero lo recibe como puntero a void) y copia los datos guardados en la estructura de persona
//en una nueva direccion
//Luego devuelve esta nueva direccion a persona como puntero a void
void * copiar(void *persona) {
  int maxN = 15, maxL = 45, edad;//maxN y maxL son las longitudes maximas posibles de los nombres y los lugares de los archivos
  Persona *aux = (Persona *) persona, *copia = malloc(sizeof(Persona));
  char *nombre = malloc(sizeof(char) * maxN), *lugar = malloc(sizeof(char) * maxL);
  strcpy(nombre,aux->nombre);
  edad = aux->edad;
  strcpy(lugar,aux->lugarDeNacimiento);
  copia->nombre = nombre;
  copia->edad = edad;
  copia->lugarDeNacimiento = lugar;
  return (void *) copia;
}

//destruirP: void* -> void
//Recibe un puntero a una persona (como puntero a void) y libera primero los datos que guarda y luego la persona en si
void destruirP (void *persona) {
  Persona *aux = (Persona *) persona;
  free(aux->nombre);
  free(aux->lugarDeNacimiento);
  free(aux);
}

//destruir: GList-> void
//Recibe una lista de personas y llama a un destruirP para cada una para liberarlas
void destruir(GList listaPersonas) {
  for (GNodo *nodo = listaPersonas; nodo != NULL;) {
    GNodo *temp = nodo;
    destruirP((void *) nodo->dato);
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

//imprimir_persona: Persona* -> void
//Recibe un puntero a persona e imprime cada dato contenido en la estructura
//void imprimir_persona(Persona *persona) {
  //printf("%s\n",persona->nombre);
  //printf("%d\n",persona->edad);
  //printf("%s\n",persona->lugarDeNacimiento);
//}

//glist_crear: void -> GList
//Se utiliza para inicializar una lista retornando el valor NULL
//para asignarlo a la variable de tipo GList que se iguala a esta funcion
GList glist_crear() {
  return NULL;
}

//glist_agregar_inicio: Glist void* -> GList
//Recibe una lista y un puntero a void y devuelve un nuevo GNodo
//que guarda en dato el valor al que apunta el puntero void
GList glist_agregar_inicio(GList lista, void *dato) {
  GNodo *nuevoNodo = malloc(sizeof(GNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

//string_a_persona: char* -> Persona*
//Recibe un string que contiene los datos de una persona separados por coma
//y devuelve un puntero a una nueva persona de tipo Persona donde copia los datos del string
Persona * string_a_persona(char *string) {
  Persona *p = malloc(sizeof(Persona));
  int i = 0,j=0, maxN = 15, maxE = 4, maxL = 45;//los ultimos 3 indican las longitudes maximas para nombres, edades y lugares
  char *nombre = malloc(sizeof(char) * maxN);
  char *edad = malloc(sizeof(char) * maxE);
  char *lugar = malloc(sizeof(char) * maxL);
  
  for (; string[i] != ','; i++, j++) {
    nombre[j] = string[i];
  }
  nombre[j] = '\0';
  p->nombre = nombre;
  i++;
  
  for (j=0; string[i] != ','; i++,j++) {
    edad[j] = string[i];
  }
  edad[j] = '\0';
  p->edad = atoi(edad);
  i++;
  
  for (j=0; string[i] != '\0'; i++,j++) {
    lugar[j] = string[i];
  }
  lugar[j] = '\0';
  p->lugarDeNacimiento = lugar;
  
  free(edad);
  
  return p;
}


//archivo_a_glist: GList FILE* -> GList
//Recibe una lista y un puntero a un archivo abierto (que contiene datos de personas) y para cada linea del archivo
//llama a string_a_persona y agrega la persona creada a la lista
//y finalmente devuelve la lista modificada
GList archivo_a_glist(GList lista, FILE *archivo) {
  int longMax = 65;
  while(!feof(archivo)) {
    char* persona = malloc(sizeof(char) * longMax);
    if (fgets(persona, longMax, archivo) != NULL) {
      if (persona[strlen(persona) - 1] == '\n') {
        persona[strlen(persona) - 1] = '\0';
      }
      lista = glist_agregar_inicio(lista, (void *) string_a_persona(persona));
    }
    free(persona);
  }
  fclose(archivo);
  return lista;
}

//persona_a_archivo: Persona* FILE* -> void
//Recibe un puntero a una persona y un puntero a un archivo abierto
//y copia los datos de la persona en el archivo, separandolos con coma
void persona_a_archivo(Persona *persona, FILE *archivo) {
  fputs(persona->nombre, archivo);
  fputc(',', archivo);
  fprintf(archivo, "%d", persona->edad);
  fputc(',', archivo);
  fputs(persona->lugarDeNacimiento, archivo);
  fputc('\n', archivo);
}

//glist_a_archivo: GList char* -> void
//Recibe una lista de personas y el nombre del tipo de lista (segun si se creo a traves de filter o map)
//y para la persona a la que apunta cada nodo de la lista llama a persona_a_archivo
//para agregarla al archivo en una nueva linea
void glist_a_archivo(GList lista, char *tipo) {
  FILE *salida;
  char *nombre = malloc(sizeof(char) * 20);
  printf("Ingrese el nombre del archivo donde guardar la lista %s (max 20 caracteres): ", tipo);
  scanf("%s", nombre);
  salida = fopen(strcat(nombre, ".txt"), "w");
  for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig) {
    persona_a_archivo(nodo->dato, salida);
  }
  fclose(salida);
  free(nombre);
}

//filter: GList Predicado Copia -> GList
//Recibe una lista, una funcion de tipo Predicado y una funcion de tipo Copia
//y copia a una lista nueva los elementos de la lista que cumplan con el predicado
//luego devuelve la nueva lista creada
GList filter(GList lista, Predicado p, Copia c) {
  GList filtrada = glist_crear();
  for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig) {
    if ((p(nodo->dato))) {
      filtrada = glist_agregar_inicio(filtrada, c((void *) nodo->dato));
    }
  }
  return  filtrada;
}

//map: GList Funcion Copia -> GList
//Recibe una lista, una funcion de tipo Funcion y una funcion de tipo Copia
//y agrega a una lista nueva copias de los elementos de la lista con la funcion aplicada
//luego devuelve la nueva lista creada
GList map(GList lista, Funcion f, Copia c) {
  GList nueva = glist_crear();
  for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig) {
    nueva = glist_agregar_inicio(nueva, f(c((void *) nodo->dato)));
  }
  return nueva;
}

//menor: void* -> int
//Es una funcion de tipo Predicado que recibe un puntero a una persona (en forma de puntero a void)
//y devuelve 1 si la edad de la persona es menor a 18
//o 0 en caso contrario
int menor(void *persona) {
  return ((Persona *) persona)->edad < 18;
}

//inicial_A: void* -> int
//Es una funcion de tipo Predicado que recibe un puntero a una persona (en forma de puntero a void)
//y devuelve 1 si el nombre de la persona comienza con A
//o 0 en caso contrario
int inicial_A(void *persona) {
  return ((Persona *) persona)->nombre[0] == 'A';
}

//doble_edad: void* -> void*
//Recibe un puntero a una persona (en forma de puntero a void)
//y devuelve la persona con el doble de edad
void * doble_edad(void *persona) {
  Persona *aux = (Persona *) persona;
  int i = aux->edad;
  aux->edad = i * 2;
  return aux;
}

//mitad_edad: void* -> void*
//Recibe un puntero a una persona (en forma de puntero a void)
//y devuelve la persona con la mitad de edad
void * mitad_edad(void *persona) {
  Persona *aux = (Persona *) persona;
  int i = aux->edad;
  aux->edad = i / 2;
  return aux;
} 

int main() {
  FILE *archivo;
  char *nombre = malloc(sizeof(char) * 20);
  
  printf("Ingrese el nombre del archivo que contiene la lista de personas (max 20 caracteres): ");
  scanf("%s", nombre);

  if ((archivo = fopen(strcat(nombre, ".txt"), "r"))) {
    GList listaP = glist_crear(), filtrada1 = glist_crear(), filtrada2 = glist_crear();
    GList map1 = glist_crear(), map2 = glist_crear();
    //Se inicializan las listas
  	
    listaP = archivo_a_glist(listaP, archivo);//Se copian las personas del archivo a listaP
    //glist_imprimir(listaP);
    
    filtrada1 = filter(listaP, menor, copiar);//Se copian a filtrada1 las personas de listaP menores de edad
    //printf("filtrada1:\n");
    //glist_imprimir(filtrada1);
    glist_a_archivo(filtrada1, "filtrada 1");//Se copia filtrada1 a un archivo
    destruir(filtrada1);//Se liberan los espacios de memoria de filtrada1
    
    filtrada2 = filter(listaP, inicial_A, copiar);
    //Se copian a filtrada2 las personas de listaP cuyos nombres empiezan con A
    //printf("filtrada2:\n");
    //glist_imprimir(filtrada2);
    glist_a_archivo(filtrada2, "filtrada 2");//Se copia filtrada2 a un archivo
    destruir(filtrada2);//Se liberan los espacios de memoria de filtrada2
    
    map1 = map(listaP, doble_edad, copiar);//Se copian a map1 las personas de listaP con el doble de edad
    //printf("map1:\n");
    //glist_imprimir(map1);
    glist_a_archivo(map1, "map 1");//Se copia map1 a un archivo
    destruir(map1);//Se liberan los espacios de memoria de map1
    
    map2 = map(listaP, mitad_edad, copiar);//Se copian a map2 las personas de listaP con la mitad de edad
    //printf("map2:\n");
    //glist_imprimir(map2);
    glist_a_archivo(map2, "map 2");//Se copia map2 a un archivo
    destruir(map2);//Se liberan los espacios de memoria de map2
    
    destruir(listaP);//Se liberan los espacios de memoria de listaP
  }
  else {
    printf("Error: el archivo no existe.\n");
  }
  free(nombre);//Se libera el espacio de memoria del nombre del archivo
  return 0;
}
