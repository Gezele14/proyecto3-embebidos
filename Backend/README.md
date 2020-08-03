# Server HTTP Ulfius como servicio en linux

## Requerimientos

1. Instalar la biblioteca Ulfius necesaria para el api rest.

   ```bash
   sudo apt install libulfius-dev
   ```

2. Instalar la biblioteca MagickWand necesaria para el trabajar con las imagenes.

   ```bash
   sudo apt install libmagickwand-dev
   ```

3. Tener instalados los paquetes de make y gcc para poder compilar el proyecto.

   ```bash
   sudo apt install gcc make
   ```

## Instalación

1. En la raiz del proyecto ejecutar el comando "make" para construir los binarios necesarios para poder ejecutar el proyecto.

   ```bash
   make
   ```
