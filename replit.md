# Hospital Management System (Tarea 1)

## Overview
A C-based educational project implementing a Hospital Management System using custom Data Structure abstractions (TDAs - Tipos de Datos Abstractos). This is a terminal/console application with a menu-driven interface.

## Tech Stack
- **Language:** C (compiled with gcc)
- **Data Structures:** Custom linked list, queue, and CSV parsing utilities

## Project Structure
```
.
├── tarea1.c          # Main entry point - Hospital Management System
├── tdas/             # TDA (Data Structure) implementations
│   ├── list.h / list.c   # Linked List implementation
│   ├── queue.h           # Queue TDA (wraps List)
│   ├── extra.h / extra.c # Utility functions (CSV parsing, etc.)
```

## Build & Run
Compile:
```bash
gcc tdas/*.c tarea1.c -Wno-unused-result -o tarea1
```
Run:
```bash
./tarea1
```

## Workflow
- **Start application**: Compiles and runs the terminal app (console output type)

## Notes
- The `tdas/` directory contains the TDA implementations that the main program depends on
- The project is a template/starting point for students to implement hospital system logic
