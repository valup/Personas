#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

//Parte 1: Creacion de la lista de personas
//Se crea una lista de entre 2000 y 10000 personas (estructuras formadas por nombre, edad y lugar de nacimiento)
//de edad entre 1 y 100 y nombre y lugar de nacimiento elegidos al azar de los archivos nombre.txt y paises.txt
//Luego se guarda esta lista en un nuevo archivo

typedef struct {
char *nombre;
int edad;
char *lugarDeNacimiento; // pais o capital
} Persona;

typedef struct _GNodo {
  void *dato;
  struct _GNodo *sig;
} GNodo;

typedef GNodo *GList;

typedef void (*Destruir) (void *);

//destruirP: void* -> void
//Recibe un puntero a una persona (como puntero a void) y libera primero los datos que guarda y luego la persona en si
void destruirP (void *persona) {
  Persona *aux = (Persona *) persona;
  free(aux->nombre);
  free(aux->lugarDeNacimiento);
  free(aux);
}

//destruirPersonas: GList-> void
//Recibe una lista de personas y llama a un destruirP para cada una para liberarlas
void destruirPersonas(GList listaPersonas) {
  for (GNodo *nodo = listaPersonas; nodo != NULL;) {
    GNodo *temp = nodo;
    destruirP((void *) nodo->dato);
    nodo = nodo->sig;
    free(temp);
  }
}

//destruirStrings: GList-> void
//Recibe una lista de strings y la libera
void destruirStrings(GList listaStrings) {
  for (GNodo *nodo = listaStrings; nodo != NULL;) {
    GNodo *temp = nodo;
    free(nodo->dato);
    nodo = nodo->sig;
    free(temp);
  }
}

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

//glist_imprimir: GList -> void
//Recibe una lista de strings e imprime cada uno
void glist_imprimir(GList lista) {
  for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig)
    printf("%s\n",(char *)nodo->dato);
}

//archivo_a_glist: GList char* int* -> GList
//Recibe una lista, un nombre de un archivo (que deberia estar en la misma carpeta que este codigo)
//y un puntero a int,
//y agrega las palabras del archivo a la lista (linea por linea) a la vez que cuenta las palabras;
//luego guarda la cantidad de palabras en el puntero a int y devuelve la lista
GList archivo_a_glist(GList lista, char *nombre, int *cant) {
  int i = 0, longMax = 50;//longitud maxima de los strings
  FILE *archivo;
  archivo = fopen(nombre, "r");
  while (!feof(archivo)) {
    char *s = malloc(sizeof(char) * longMax);
    fgets(s, longMax, archivo);
    if (strlen(s) > 1) {
      s[strlen(s) - 1] = '\0';
      if (s[strlen(s) - 1] == '\r') {
        s[strlen(s) - 1] = '\0';
      }
    }
    lista = glist_agregar_inicio(lista, (void *) s);
    i++;
  }
  *cant = i;
  //glist_imprimir(lista);
  fclose(archivo);
  return lista;
}

//elegir: GList int -> void*
//Recibe una lista de opciones de donde elegir y la cantidad de strings de la lista,
//luego elige una posicion random de la lista y devuelve un puntero al dato guardado
//en esa posicion
void* elegir(GList opciones, int cant) {
  int i = 0, pos = rand() % cant;
  GList l = opciones;
  for (; i < pos; i++) {
    assert(l != NULL);
    l = l->sig;
  }
  assert(l != NULL);
  return l->dato;
}

//persona_random: GList int GList int -> Persona*
//Recibe la lista de nombres, la cantidad de nombres que tiene,
//la lista de lugares y la cantidad de lugares que tiene
//y a partir de eso crea una nueva persona de edad entre 1 y 100
//con un nombre y un lugar de nacimiento al azar de las listas
//y devuelve un puntero a ella
Persona * persona_random(GList nombres, int cantN, GList lugares, int cantL) {
  Persona *persona = malloc(sizeof(Persona));
  persona->nombre = elegir(nombres, cantN);
  persona->edad = (rand() % 100) + 1;//una edad random entre 1 y 100
  persona->lugarDeNacimiento = elegir(lugares, cantL);
  return persona;
}
	
//lista_personas: GList GList int GList int -> GList
//Recibe una lista vacia, la lista de nombres, la cantidad de nombres que tiene,
//la lista de lugares y la cantidad de lugares que tiene
//luego elige una cantidad random de personas que agregar entre 2000 y 10000
//y agrega a la lista vacia personas random creadas a partir de las listas
//luego devuelve la lista modificada
GList lista_personas(GList lista, GList nombres, int cantN, GList lugares, int cantL) {
  int i = 0, cant = (rand() %  8001) + 2000;//cant es una cantidad de personas random entre 2000 y 10000
  for (; i < cant; i++) {
    lista = glist_agregar_inicio(lista, (void *) persona_random(nombres, cantN, lugares, cantL));
  }
  return lista;
}

//persona_a_archivo: Persona* FILE*
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
void glist_a_archivo(GList lista) {
  FILE *salida;
  char *nombre = malloc(sizeof(char) * 20);
  printf("Ingrese el nombre del archivo donde guardar la lista generada (max 20 caracteres): ");
  scanf("%s", nombre);
  salida = fopen(strcat(nombre, ".txt"), "w");
  for (GNodo *nodo = lista; nodo != NULL; nodo = nodo->sig) {
    persona_a_archivo(nodo->dato, salida);
  }
  fclose(salida);
  free(nombre);
}

int main() {
  GList nombres = glist_crear(), lugares = glist_crear(), personas = glist_crear();
  //Se inicializan las listas
  int cantN, cantL;
  nombres = archivo_a_glist(nombres, "nombres.txt", &cantN);//Se copian los nombres de nombres.txt en la lista nombres
  lugares = archivo_a_glist(lugares, "paises.txt", &cantL);//Se copian los paises de paises.txt en la lista lugares
  //Se asume que ambos archivos .txt estan en la carpeta donde se halla este codigo
  
  personas = lista_personas(personas, nombres, cantN, lugares, cantL);//Se crea la lista de personas random
  
  glist_a_archivo(personas);//Se guarda la lista de personas en un archivo
  
  destruirStrings(nombres);//Se liberan los espacios de memoria de nombres
  destruirStrings(lugares);//Se liberan los espacios de memoria de lugares
  destruirPersonas(personas);//Se liberan los espacios de memoria de personas
  
  return 0;
}
