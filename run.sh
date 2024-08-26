#!/bin/sh

# Crear el directorio build si no existe
if [ ! -d "build" ]; then
  mkdir build
fi

# Navegar al directorio build
cd build

# Ejecutar CMake para generar los archivos de construcción
cmake ..

# Compilar el proyecto
make

# Ejecutar el programa con los argumentos pasados al script
./MySDLProject "$1" "$2"