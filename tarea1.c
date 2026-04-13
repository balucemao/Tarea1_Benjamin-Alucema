#include "tdas/list.h"
#include "tdas/extra.h"
#include "tdas/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


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
  puts("     Sistema de Gestión SMART_TODO");
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

//funcion auxiliar, convierte cadena (plabra) a mayusculas y elimina espacios al final
void aMayus(char *categoria){
  for (int i = 0; categoria[i] != '\0'; i++){
    categoria[i] = toupper((unsigned char)categoria[i]);
  }

  //con esto elimino todos los espacios al final para ahorrar problemas de categorias repetidas
  int n = strlen(categoria);
  while (n > 0 && isspace((unsigned char)categoria[n-1])){
    categoria[n-1] = '\0';
    n -= 1;
  }
}

//funciones requeridas

// 1- REGISTRAR NUEVA CATEGORIA
void registrar_categorias(List *categorias) {
  printf("Registrar nueva categoría (sin tildes o ñ)\n");
  
  //nomre categoria nueva
  char nuevaCategoria[50];
  scanf(" %49[^\n]", nuevaCategoria);

  //convertir a mayusculas para asegurar que no se repita categoria.
  aMayus(nuevaCategoria);
  
  //variable auxiliar tipo CATEGORIAS, recibe la primera categoria en lista.
  CATEGORIAS *revisarDuplicados = list_first(categorias);
  //se compara la nueva categoria con cada categoria en lista, si no existe se agrega.
  while(revisarDuplicados != NULL){
    //si se repite entonces se descarta.
    if (strcmp(revisarDuplicados->nombre, nuevaCategoria) == 0){
      printf("Ya existe esa categoria\n");
      return;
    }
    revisarDuplicados = list_next(categorias);
  }

  //se crea una nueva categoria.
  CATEGORIAS *nueva = (CATEGORIAS*)malloc(sizeof(CATEGORIAS));
  //se copia el nuevo nombre en la categoria creada recien.
  strcpy(nueva->nombre, nuevaCategoria);
  //se inserta la nueva categoria en la lista.
  list_pushBack(categorias, nueva);
  printf("Categoria registrada correctamente\n");
}


// 2- eliminar categoria
void eliminar_categoria(List *categorias, Queue *cola){
  
  //se revisa si existe categorias en la lista
  if (list_first(categorias) == NULL || categorias == NULL){
    printf("No existen categorias, porfavor elija otra opcion\n");
    return;
  }

  //aqui estara el nombre de la categoria a eliminar
  char aEliminar[50];
  printf("Categoria a eliminar\n");
  //el usuario ingresa la categoria a eliminar
  scanf(" %49[^\n]", aEliminar);

  //convertir a mayusculas para asegurar que no se repita categoria.
  aMayus(aEliminar);

  //aqui se almacena la primera categoria de la lista.
  CATEGORIAS *catActual = list_first(categorias);

  //aqui se verifica si existe la categoria.
  int encontrado = 0;

  //buscar categoria a eliminar en la lista de categorias.
  while (catActual != NULL){
    //se cmpara la categoria a eliminar con todas las categorias en lista.
    if (strcmp(catActual->nombre, aEliminar) == 0){
      CATEGORIAS *categoriaAux = list_popCurrent(categorias); //aqui accedo por completo a la lista de categoria, categoria actual
      free(categoriaAux); //se elimina la categoria actual de mi lista de categorias
      encontrado = 1; //aqui verifico que se encontro la categoria
      break;
    }
    //con esto recorro la lista hasta encontrar categoria buscada
    catActual = list_next(categorias);
  }

  //si es que no existe la categoria en la lista de categorias
  if (encontrado == 0){
    printf("No existe esa categoria, otra opcion\n");
    return;
  }

  //se crea cola para pasar las tareas que no pertenecen a la categoria buscada
  Queue *colaAux = queue_create(NULL);
  //el ciclo busca todas las tareas coincidentes con la categoria, si coniciden la borra de la cola, sino la insertta al final de la cola auxiliar

  int borradas = 0; //verifica si se borraron tareas para esa categoria
  
  while(queue_front(cola) != NULL){
    TAREAS *tareaActual = queue_remove(cola); //aqui obtengo la primera tarea de la cola y esta se elimina de la cola
    
    if (strcmp(tareaActual->categoria, aEliminar) == 0){
      free(tareaActual); //si categoria de tarea actual coincide con categoria a eliminar, se elimina la tarea
      borradas += 1;
    }
    else queue_insert(colaAux, tareaActual); //de lo contrario la tarea se inserta en la cola auxiliar 

  }
  //ciclo para devolver a la cola original en orden original
  while(queue_front(colaAux) != NULL){
    //queue_remove elimina elemento de cola y devuelve el primero de la cola
    queue_insert(cola, queue_remove(colaAux));
  }
  
  //se libera la memoria de la cola auxiliar
  free(colaAux);
  if (borradas > 0) printf("Categoria %s y sus tareas eliminadas.\n", aEliminar, borradas);
  else printf("Categoria eliminada, no tenia tareas asociadas.\n");
}

// 3- mostrar categorias
void mostrar_categorias(List *categorias) {
  // Mostrar categorías
  printf("Categorías:\n");
  
  if(categorias == NULL)return; //aqui se comprueba si es que existen categorias en la lista
  
  CATEGORIAS *actual = list_first(categorias); //aqui se obtiene la primera categoria
  //si no hay categorias en la lista
  if (actual == NULL){
    printf("Debe registrar una categoria antes\n");
    return;
  }
  //mientras la lista no este vacia se muestra las categorias
  while(actual != NULL){
    printf("%s\n", actual->nombre);
    actual = list_next(categorias); //aqui se recorre la lista de categorias
  }
}

// 4-registrar tarea pendiente
void registrar_pendiente(List *categorias, Queue *cola){
  
  //se revisa si la lista esta vacia
  if (list_first(categorias) == NULL || categorias == NULL){
    printf("No hay categorias, ingresar otra opcion\n");
    return;
  }

  char nombreCategoria[50]; //aqui se almacena el nombre de la categoria 
  printf("A que categoria pertenece la tarea?\n");
  scanf(" %49[^\n]", nombreCategoria);
  //convertir a mayusculas para asegurar que no se repitan categorias
  aMayus(nombreCategoria);
  
  CATEGORIAS *categoriaActual = list_first(categorias); //aqui se guarda la primera categoria de la lista de categorias

  int POSNULA = 0; //variable que verifica si existe categoria

  //aqui se recorrer la lista de categorias para saber si alguna coincide con la ingresada
  while(categoriaActual != NULL){
    if (strcmp(categoriaActual->nombre, nombreCategoria) == 0){
      POSNULA = 1;
      break;
    }
    categoriaActual = list_next(categorias); //se recorre la lista de categorias
  }
  
  //si no se encuentra entonces pide elegir otra opcion.
  if (POSNULA == 0){
    printf("La categoria no existe, ingresar otra opcion.\n");
    return;
  }

  //si existia la categoria entonces se crea variable de tipo tareas.
  TAREAS *nuevaTarea = (TAREAS*)malloc(sizeof(TAREAS));
  
  //se verifica si no hubo problemas al reservar memoria
  if (nuevaTarea == NULL)return;

  //se le inserta la categoria seleccionada en la variable tipo tareas, despues se pide una tarea/objetivo que se requiere y se inserta en el objetivo de la variable tareas
  strcpy(nuevaTarea->categoria, nombreCategoria);
  printf("Cual es el objetivo de tu tarea?\n");
  //leer hasta el salto de linea
  scanf(" %[^\n]", nuevaTarea->objetivo);

  //se pide la hora actual y se inserta en la nueva tarea
  time_t hora = time(NULL);
  hora -= 14400; //esto lo hice para que coincida con la hora en chile
  struct tm *tm = localtime(&hora);
  strftime(nuevaTarea->fecha, 20, "%H:%M:%S", tm);
  //se inserta la nueva tarea en la cola de tareas
  queue_insert(cola, nuevaTarea);
  printf("Tarea registrada a las %s.\n\n", nuevaTarea->fecha);
}

// 5- atender siguiente tarea, eliminarla y dejar la siguiente en cola
void atender_siguiente(Queue *cola){
  TAREAS *tareaActual = queue_remove(cola);
  if (tareaActual == NULL){
    printf("No hay pendientes.\n\n");
    return;
  }
  printf("Objetivo: %s.\n", tareaActual->objetivo);
  printf("Categoria: %s | hora: %s.\n", tareaActual->categoria, tareaActual->fecha);
  free(tareaActual);
  
}

// 6- mostrar tareas pendientes
void mostrar_tablero(Queue *cola){
  //se revisa si existen tareas pendiente.
  if (queue_front(cola) == NULL){
    printf("No existen tareas pendientes en el sistema\n\n");
    return;
  }

  //cola auxiliar
  Queue *colaAux = queue_create(NULL);
  printf("TABLERO GENERAL (tareas en orden de llegada)\n\n");

  //recorrer cola para enviar tareas a la auxiliar, mientras existan tareas
  while(queue_front(cola) != NULL){
    TAREAS *tareaActual = queue_remove(cola);
    //mostrar tarea actual
    printf("| %s | %s | registrada a las %s |\n", tareaActual->categoria, tareaActual->objetivo, tareaActual->fecha);
    //insertar en cola axuliar la tarea actual
    queue_insert(colaAux, tareaActual);
  }

  //bucle para obtener orden original
  while(queue_front(colaAux) != NULL){
    queue_insert(cola, queue_remove(colaAux));
  }
  //liberar la memoria de la cola auxiliar
  free(colaAux);
}

// 7- filtrar tareas por una categoria
void filtrar_por_categoria(List *categorias, Queue *colaTareas){
  //se revisa si existen categorias en la lista de categorias
  if (list_first(categorias) == NULL || categorias == NULL){
    printf("No hay categorias, ingresar otra opcion\n\n");
    return;
  }

  //categoria para filtrar
  char catFiltrar[50];
  printf("Ingrese la categoria para filtrar sus tarea derivadas\n\n");
  scanf(" %49[^\n]", catFiltrar);
  aMayus(catFiltrar); //categoria para filtrar a mayusculas

  
  CATEGORIAS *categoriaActual = list_first(categorias); //aqui se guarda la primera categoria de la lista de categorias
  int POSNULA = 0; //variable que verifica si existe categoria
  //aqui se recorrer la lista de categorias para saber si alguna coincide con la ingresada
  while(categoriaActual != NULL){
    if (strcmp(categoriaActual->nombre, catFiltrar) == 0){
      POSNULA = 1;
      break;
    }
    categoriaActual = list_next(categorias); //se recorre la lista de categorias
  }

  //si no se encuentra entonces pide elegir otra opcion.
  if (POSNULA == 0){
    printf("La categoria no existe, ingresar otra opcion.\n");
    return;
  }
  
  Queue *colaAux = queue_create(NULL); //se crea cola auxiliar
  int hayTarea = 0;
  printf("Tareas en la categoria %s:\n\n", catFiltrar);
  printf("---------------------------------------------\n\n");

  //mostrar tareas de esa categoria
  while(queue_front(colaTareas) != NULL){
    //se crea variable tipo TAREAS que resive el primer elemento de la cola
    //ese elemento tambien se elimina de la cola de tareas
    TAREAS *tareaActual = queue_remove(colaTareas);
    if (strcmp(tareaActual->categoria, catFiltrar) == 0){
      printf("- %s (Registrada: %s)\n", tareaActual->objetivo, tareaActual->fecha);
      hayTarea = 1;
    }
    //insertar tarea en la cola de tareas auxiliar
    queue_insert(colaAux, tareaActual);
  }

  //se verifica si existen tareas en la categoria
  if(hayTarea == 0)printf("No existen tareas de esa categoria.\n\n");

  //devolver cola original a su orden corrector FIFO
  while(queue_front(colaAux) != NULL){
    queue_insert(colaTareas, queue_remove(colaAux));
  }
  //se libera la memoria de la cola auxiliar
  free(colaAux);
}

int main() {
  char opcion;
  List *categorias = list_create(); // Lista para almacenar categorías
  Queue *colaTareas = queue_create(NULL); //cola para almacenar tareas
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      //logica para registrar categorias
      registrar_categorias(categorias);
      break;
    case '2':
      // Lógica para eliminar una categoría
      eliminar_categoria(categorias, colaTareas);
      break;
    case '3':
      //logica para mostrar categorias
      mostrar_categorias(categorias);
      break;
    case '4':
      // Lógica para registrar un paciente
      registrar_pendiente(categorias, colaTareas);
      break;
    case '5':
      // Lógica para atender al siguiente paciente
      atender_siguiente(colaTareas);
      break;
    case '6':
      mostrar_tablero(colaTareas);
      // Lógica para mostrar el tablero general
      break;
    case '7':
      filtrar_por_categoria(categorias, colaTareas);
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

  //bucle para eliminar todas las categorias individualmente
  while(list_size(categorias) > 0){
    free(list_popFront(categorias));
  }
  free(categorias); //eliminar lista de categorias

  //bucle para eliminar todas las tareas individuales en la cola
  while(queue_front(colaTareas) != NULL){
    free(queue_remove(colaTareas));
  }
  free(colaTareas); //eliminar cola
  
  
  return 0;
}
