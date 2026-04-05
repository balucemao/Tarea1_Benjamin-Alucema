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
  char nombreNuevo[50];
  scanf(" %s", nombreNuevo);

  CATEGORIAS *revisarDuplicados = list_first(categorias);
  while(revisarDuplicados != NULL){
    if (strcmp(revisarDuplicados->nombre, nombreNuevo) == 0){
      printf("ya existe esa categoria");
      return;
    }
    revisarDuplicados = list_next(categorias);
  }
  CATEGORIAS *nueva = (CATEGORIAS*)malloc(sizeof(CATEGORIAS));
  strcpy(nueva->nombre, nombreNuevo);
  list_pushBack(categorias, nueva);
  printf("categoria registrada");
}

void mostrar_categorias(List *categorias) {
  // Mostrar categorías
  printf("Categorías:\n");
  if(categorias == NULL)return;
  CATEGORIAS *actual = list_first(categorias);
  if (actual == NULL){
    printf("debe registrar una categoria antes (opcion 1)\n");
    return;
  }
  while(actual != NULL){
    printf("%s\n", actual->nombre);
    actual = list_next(categorias);
  }
}

void eliminar_categoria(List *categorias, Queue *cola){
  char aEliminar[50];
  printf("Tarea a eliminar\n");
  scanf(" %s", aEliminar);

  CATEGORIAS *catAeliminar = list_first(categorias);

  int encontrado = 0;

  while (catAeliminar != NULL){
    if (strcmp(catAeliminar->nombre, aEliminar) == 0){
      list_popCurrent(categorias);
      encontrado = 1;
      break;
    }
    catAeliminar = list_next(categorias);
  }
  
  if (encontrado == 0){
    printf("no existe categoria, ingre una nueva (opcion 1) o elige otra opcion\n");
    return;
  }

  Queue *colaAux = queue_create(NULL);
  while(queue_front(cola) != NULL){
    TAREAS *t = queue_remove(cola);
    if (strcmp(t->categoria, aEliminar) == 0)free(t);
    else queue_insert(colaAux, t);
  }
  while(queue_front(colaAux) != NULL){
    queue_insert(cola, queue_remove(colaAux));
  }
  free(catAeliminar);
  free(colaAux);
  printf("lista y cola elimininada\n");
}


void registrar_pendiente(List *categorias, Queue *cola){
  char nombreCategoria[50];
  printf("A que categoria pertenece la tarea?\n");
  scanf(" %s", nombreCategoria);

  if (list_first(categorias) == NULL){
    printf("no hay categorias, ingresar (opcion 1)\n");
  }
  
  CATEGORIAS *categoriaActual = list_first(categorias);

  int POSNULA = 0;
  
  while(categoriaActual != NULL){
    if (strcmp(categoriaActual->nombre, nombreCategoria) == 0){
      POSNULA = 1;
      break;
    }
    categoriaActual = list_next(categorias);
  }

  if (POSNULA == 0){
    printf("La categoria no existe, primero crear (opcion 1)\n");
    return;
  }
  
  TAREAS *nuevaTarea = (TAREAS*)malloc(sizeof(TAREAS));

  if (nuevaTarea == NULL)return;
  
  strcpy(nuevaTarea->categoria, nombreCategoria);
  printf("Cual es el objetivo de tu tarea?\n");
  scanf(" %[^\n]", nuevaTarea->objetivo);

  time_t hora = time(NULL);
  struct tm *tm = localtime(&hora);
  strftime(nuevaTarea->fecha, 20, "%H:%M", tm);

  queue_insert(cola, nuevaTarea);
}


void atender_siguiente(Queue *cola){
  TAREAS *tareaActual = queue_remove(cola);
  if (tareaActual == NULL){
    printf("No hay pendientes\n");
    return;
  }
  printf("objetivo: %s\n", tareaActual->objetivo);
  printf("categoria: %s | hora: %s", tareaActual->categoria, tareaActual->fecha);
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
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  // Liberar recursos, si es necesario
  list_clean(categorias);

  return 0;
}
