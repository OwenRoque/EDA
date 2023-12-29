# Visualizador Octree

Visualizador de Octree con C++ y VTK

## Requisitos

Asegúrate de tener instalados los siguientes elementos antes de comenzar:

1. **CMake:** [Descargar CMake](https://cmake.org/download/)
2. **Visual Studio Community:** [Descargar Visual Studio Community](https://visualstudio.microsoft.com/visual-cpp-build-tools/)

## Instrucciones de Instalación

Sigue estos pasos para configurar y compilar el proyecto:

1. **Instalación de CMake y Visual Studio Community:**
   - Descarga e instala CMake desde [cmake.org](https://cmake.org/download/).
   - Descarga e instala Visual Studio Community desde [visualstudio.microsoft.com](https://visualstudio.microsoft.com/visual-cpp-build-tools/).

2. **Instalación de VTK:**
   - Mira el [video tutorial](https://www.youtube.com/watch?v=vseB1arVF14) para obtener instrucciones detalladas sobre la configuración del proyecto ().

3. **Generación de Proyecto con CMake (si no se utiliza plugins):**
   - Ejecuta el siguiente comando en la terminal (si no estás utilizando plugins como CMake Tools en VSCode):
     ```bash
     cmake -S . -B build
     ```
   - Nota: Si estás usando VSCode con la extensión CMake Tools, este paso puede ser omitido, ya que la extensión maneja la configuración de CMake de manera automática.

4. **Compilación del Proyecto:**
   - Abre la solución del proyecto (.sln) con Visual Studio.
   - Compila el proyecto dentro de Visual Studio.

¡Listo! Tu proyecto debería estar configurado y compilado correctamente.

## Problemas y Sugerencias

Si encuentras algún problema o tienes sugerencias para mejorar el proyecto, por favor crea un nuevo *issue* en este repositorio.
