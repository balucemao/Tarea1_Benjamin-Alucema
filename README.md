- Benjamin Alucema.
- 22122629-1
- 12-4-2026

# SMART_TODO - PLANIFICADOR DINAMICO DE TAREAS

Este proyecto consiste en una aplicacion/programa en el lenguaje C.
Consiste en llevar un registro de tareas ordenadas en orden estricto de llegada, para lograr esto se utiliza el TDA cola cuya principal caracteristica es mantener su principio FIFO, el primero en entrar a la cola es el primero en salir.

Otro punto a tener en consideracion del modo de funcionamiento es que estas tareas tienen etiquetas denominadas CATEGORIAS (una lista tiene todas estas categorias), por cada tarea ademas, se debe asignar la hora de ingreso al sistema y tambien el objetivo/accion asignado a esta tarea. 

# Para poder compilar este programa se deben tener instalados los archivos TDAs dentro de la carpeta (tdas), utilizados para poder hacer funcionar este codigo (en el programa que estas usando para compilar). 
- Los archivos TDAs correspondientes son: extra.c, list.c, y queue.h
- Archivos para entender que hace cada funcion: extra.h y list.h. (En estos archivos viene explicado resumidamente como funciona la logica de los TDAs usados en mi programa).
- Archivo tarea1.c donde esta el codigo para que funcione la aplicacion.
- - ES IMPORTANTE QUE LOS ARCHIVOS TDAS ESTEN DENTRO DE LA CARPETA CON NOMBRE TDAS

# Luego de tener ya instalado estos archivos necesitas ejecutar el siguiente comando en la terminal:

```bash
gcc tarea1.c tdas/*.c -o smart_todo
```

# Finalmente debes ejecutar la aplicacion utilizando este codigo en la terminal:

```bash
./smart_todo
```

# FUNCIONES PRINCIPALES:

- registrar_categoria: Esta funcion permite al usuario registrar una categoria, si existe se lo informa al usuario, de lo contrario inserta la categoria en la lista de categorias. Idealmente la categoria sin tildes o ñ. Maximo 50 caracteres.

- eliminar_categoria: Esta funcion funciona como un "vaciado" o borron, pide al usuario una categoria y elimina sus derivados, primero la categoria y todas sus tareas ramificadas, luego muestra si se borro la categoria, y tambien tareas (si es que habian tareas en cola que formaban parte de esa categoria).

- mostrar_categorias: Muestra todas las categorias actuales, permite al usuario decidir si es posible encolar tareas en una categoria existente, o primero crear la categoria. Si no existen categoria pide registrar una antes.

- registrar_pendiente: Permite al usuario (si es que hay categorias) poner en la cola la tarea que desea el usuario, pide primero la categoria a la que pertenece, si existe luego pide ingresar la descripcion de la tarea, finalmente se ingresa en la cola la tarea con la hora actual, nombre de categoria y el objetivo de la tarea. Si no existen categorias pide ingresar una antes. Tarea maximo 100 caracteres por objetivo.
  
- atender_siguiente: Primero verifica se existen tareas pendientes, sino pide ingresar una antes. Si efectivamente existen tareas en cola entonces primero la muestra junto con todos sus datos: hora, objetivo y categoria de pertenencia. Luego la elimina y deja al inicio de la cola la siguiente tarea ingresada al sistema.
  
- mostrar_tablero: Muestra todas las tareas pendientes (mas antigua a mas reciente), su categoria, objetivo y fecha. Si no existen tareas informa al usuario.
  
- filtrar_por_categoria: Permite al usuario conocer todas las tareas en cola que pertenecen a una determinada categoria. Si no existen categorias lo informa, tambien informa si existe la categoria pero no tiene tareas en cola.

## FUNCION(ES) SECUNDARIA

- aMayus: convierte una cadena (palabra) en mayuscula, esto es util al ingresar las categorias, ya que quiza el usuario olvida como escribio la categoria, o tuvo un error, al convertir todo a mayuscula disminuye mucho el riesgo de un error de ese estilo. Por ejemplo hola = HOLA = HoLa = HOla.
  Ademas esta funcion elimina todos los espacios luego de la ultima letra o numero, por ejemplo sin este borrado de espacios al final, benjamin != benjamin(aqui iria un       espacio)

# EJEMPLO DE FUNCIONAMIENTO DEL PROGRAMA

  - Usuario ingresa opcion 1, primera categoria HOGAR.
  - Usuario ingresa opcion 1 (nuevamente), segunda categoria COCINA.
  - Usuario ingresa opcion 4, ingresa la categoria HOGAR y luego la tarea LAVAR.
  - Usuario ingresa opcion 4 (nuevamente), ingresa la categoria COCINA y luego la tarea COCINAR.
  - Usuario ingresa opcion 6, se visualiza el tablero general con la tarea LAVAR y luego COCINAR, ambas con hora exacta.
  - Usuario ingresa opcion 5, se muestra la tarea en cabeza LAVAR, esta se elimina y queda en cabeza de la cola COCINAR.
  - Usuario ingresa opcion 7, ingresa la categoria HOGAR, se informara que ya no existen tareas en esa categoria.
  - Usuario ingresa opcion 3, se mostrara la categoria HOGAR Y COCINA.
  - Usuario ingresa opcion 2, ingresa la categoria HOGAR, se elimina del sistema.
  - Usuario ingresa opcion 3, se mostrar la categoria COCINA.
  - Usuario ingresa opcion 8, se sale del programa.

