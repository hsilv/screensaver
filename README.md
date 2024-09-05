# Spectrum Analyzer

Spectrum Analyzer es un analizador de espectro de audio en tiempo real que utiliza SDL2, PortAudio y FFTW3 para capturar y visualizar datos de audio. Este proyecto está diseñado para mostrar un espectro de audio en una ventana SDL, con barras que representan diferentes bandas de frecuencia.

## Requisitos

- CMake 3.10 o superior
- SDL2
- PortAudio
- FFTW3
- SDL2_ttf
- OpenMP

## Instalación

1. Clona el repositorio:

    ```sh
    git clone https://github.com/hsilv/screensaver.git
    cd screensaver
    git checkout equalizer
    ```

2. Ejecuta el script `run.sh` para compilar y ejecutar el proyecto:

    ```sh
    ./run.sh
    ```

## Uso

Para ejecutar el programa, simplemente ejecuta el script 
```
./run.sh <numBars>
```
donde numBars es el número de barras visibles del espectro

Esto capturará el audio de tu micrófono, si quieres escuchar el audio de tu escritorio debes configurar un loopback desde tu dispositivo de salida de audio al de tu entrada.

## Ejemplo
Una vez ejecutado deberías ver algo así

| ![image](https://github.com/user-attachments/assets/358a6b3f-b4cd-41f6-b596-defc207e9bc0) | ![image](https://github.com/user-attachments/assets/0767a2a9-6b69-45a3-8a12-0276c5e48579) |
| ---- | ---- |


