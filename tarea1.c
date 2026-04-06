#include "tdas/list.h"
#include "tdas/extra.h"
#include "tdas/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
  char nombre[50];
}CATEGORIAS;

typedef struct{
  char categoria[50];
  char objetivo[100];
  char fecha[20];
}TAREAS;


// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Nueva Categoría");
  puts("2) Eliminar Categoría");
  puts("3) Mostrar Categorías");
  puts("4) Registrar Pendiente");
  puts("5) Atender Siguiente");
  puts("6) Visualización del Tablero General");
  puts("7) Filtrado por Categoría");
  puts("8) Salir");
}

void registrar_categorias(List *categorias) {
  printf("Registrar nueva categoría\n");
  //nomre categoria nueva
  char nombreNuevo[50];
  scanf(" %s", nombreNuevo);

  //variable auxiliar tipo CATEGORIAS para revisar si la categoria no se repite
  CATEGORIAS *revisarDuplicados = list_first(categorias);
  while(revisarDuplicados != NULL){
    //se revisa la lista completa de categorias para ver si se repite alguna
    if (strcmp(revisarDuplicados->nombre, nombreNuevo) == 0){
      printf("Ya existe esa categoria\n");
      return;
    }
    revisarDuplicados = list_next(categorias);
  }
  CATEGORIAS *nueva = (CATEGORIAS*)malloc(sizeof(CATEGORIAS));
  //se copia el nuevo nombre en la lista
  strcpy(nueva->nombre, nombreNuevo);
  list_pushBack(categorias, nueva);
  printf("Categoria registrada correctamente\n");
}

void mostrar_categorias(List *categorias) {
  // Mostrar categorías
  printf("Categorías:\n");
  if(categorias == NULL)return;
  CATEGORIAS *actual = list_first(categorias);
  //si no hay categorias en la lista
  if (actual == NULL){
    printf("Debe registrar una categoria antes (opcion 1)\n");
    return;
  }
  //mientras la lista no este vacia se muestra las categorias
  while(actual != NULL){
    printf("%s\n", actual->nombre);
    actual = list_next(categorias);
  }
}

void eliminar_categoria(List *categorias, Queue *cola){
  char aEliminar[50];
  //se revisa si la lista esta vacia o es nula
  if (list_first(categorias) == NULL || categorias == NULL){
    printf("No existen categorias, porfavor ingrese una (opcion 1), o elija otra opcion\n");
    return;
  }
  
  printf("Tarea a eliminar\n");
  scanf(" %s", aEliminar);

  CATEGORIAS *catAeliminar = list_first(categorias);

  //si se encuentra la categoria a eliminar
  int encontrado = 0;

  //buscar categoria a eliminar en la lista de categorias
  while (catAeliminar != NULL){
    if (strcmp(catAeliminar->nombre, aEliminar) == 0){
      list_popCurrent(categorias);
      encontrado = 1;
      break;
    }
    catAeliminar = list_next(categorias);
  }

  //si es que no existe la categoria
  if (encontrado == 0){
    printf("No existe esa categoria, ingrese una nueva (opcion 1) o elige otra opcion\n");
    return;
  }

  //se crea cola para pasar las tareas que no pertenecen a la categoria buscada
  Queue *colaAux = queue_create(NULL);
  //el ciclo busca todas las tareas coincidentes con la categoria, si coniciden la borra de la cola, sino la insertta al final de la cola auxiliar
  while(queue_front(cola) != NULL){
    TAREAS *t = queue_remove(cola);
    if (strcmp(t->categoria, aEliminar) == 0)free(t);
    else queue_insert(colaAux, t);
  }
  //ciclo para devolver a la cola original en orden original
  while(queue_front(colaAux) != NULL){
    queue_insert(cola, queue_remove(colaAux));
  }
  free(catAeliminar);
  free(colaAux);
  printf("Lista y cola elimininada\n");
}


void registrar_pendiente(List *categorias, Queue *cola){
  char nombreCategoria[50];
  printf("A que categoria pertenece la tarea?\n");
  scanf(" %s", nombreCategoria);

  //se revisa si la lista esta vacia
  if (list_first(categorias) == NULL || categorias == NULL){
    printf("No hay categorias, ingresar (opcion 1)\n");
    return;
  }
  
  CATEGORIAS *categoriaActual = list_first(categorias);

  int POSNULA = 0;

  //si existe en la lista de categorias la categoria ingresada por el usuario, se inserta una tarea en esa categoria.
  while(categoriaActual != NULL){
    if (strcmp(categoriaActual->nombre, nombreCategoria) == 0){
      POSNULA = 1;
      break;
    }
    categoriaActual = list_next(categorias);
  }
  
  //si no se encuentra entonces pide elegir otra opcion
  if (POSNULA == 0){
    printf("La categoria no existe, primero crear (opcion 1)\n");
    return;
  }

  //si existia la categoria entonces se crea variable de tipo tareas
  TAREAS *nuevaTarea = (TAREAS*)malloc(sizeof(TAREAS));

  if (nuevaTarea == NULL)return;

  //se le inserta la categoria seleccionada en la variable tipo tareas, despues se pide una tarea/objetivo que se requiere y se inserta en el objetivo de la variable tareas
  strcpy(nuevaTarea->categoria, nombreCategoria);
  printf("Cual es el objetivo de tu tarea?\n");
  scanf(" %[^\n]", nuevaTarea->objetivo);

  //se pide la hora actual y se inserta en la nueva tarea
  time_t hora = time(NULL);
  struct tm *tm = localtime(&hora);
  strftime(nuevaTarea->fecha, 20, "%H:%M", tm);
  //se inserta la nueva tarea en la cola de tareas
  queue_insert(cola, nuevaTarea);
}


void atender_siguiente(Queue *cola){
  TAREAS *tareaActual = queue_remove(cola);
  if (tareaActual == NULL){
    printf("No hay pendientes\n");
    return;
  }
  printf("objetivo: %s\n", tareaActual->objetivo);
  printf("categoria: %s | hora: %s\n", tareaActual->categoria, tareaActual->fecha);
  free(tareaActual);
  
}

int main() {
  char opcion;
  List *categorias = list_create(); // Lista para almacenar categorías
  Queue *colaTareas = queue_create(NULL);
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      registrar_categorias(categorias);
      break;
    case '2':
      eliminar_categoria(categorias, colaTareas);
      // Lógica para eliminar una categoría
      break;
    case '3':
      mostrar_categorias(categorias);
      break;
    case '4':
      // Lógica para registrar un paciente
      registrar_pendiente(categorias, colaTareas);
      break;
    case '5':
      atender_siguiente(colaTareas);
      // Lógica para atender al siguiente paciente
      break;
    case '6':
      // Lógica para mostrar el tablero general
      break;
    case '7':
      // Lógica para filtrar por categoría
      break;
    case '8':
      puts("Saliendo del sistema de gestión hospitalaria...\n");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.\n");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  // Liberar recursos, si es necesario
  list_clean(categorias);

  return 0;
}
