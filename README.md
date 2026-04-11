*Este proyecto ha sido creado como parte del currículo de 42 por <mvasquez>.*

# Descripción
El objetivo de este proyecto es programar una función que devuelva una línea leída desde un descriptor de archivo. A diferencia de la función `read`, que lee una cantidad arbitraria de bytes, `get_next_line` gestiona la lectura continua devolviendo el texto línea a línea hasta el final del archivo.

**Características principales:**
* **Variables Estáticas:** Uso de variables persistentes en C para mantener información entre llamadas a la función (el "buffer" sobrante).
* **Gestión de Memoria:** Control de `malloc` y `free` para evitar fugas de memoria (leaks).
* **Manipulación de Buffers:** Lectura de archivos en bloques de tamaño variable definidos por `BUFFER_SIZE`.

# Instrucciones
Prototipo de la función donde se procesa un file descriptor en donde la función se encargará de procesar dicho archivo para devolver una lectura secuencial dado un EOF `\0`
`char *get_next_line(int fd);`

## Pruebas

Pruebas con un fichero leido con la función open():
``` c 
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *linea;

    // Abre el archivo 'text.txt' en modo lectura
    fd = open("text.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error abriendo el archivo");
        return (1);
    }

    // Bucle para leer hasta que get_next_line devuelva NULL
    while (1)
    {
        linea = get_next_line(fd);
        if (linea == NULL)
            break;
        printf("%s", linea);
        free(line);
    }
    
    close(fd);
    return (0);
}
```
Pruebas con la salida estandar stdin:
``` c 
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    char *linea;
	linea = get_next_line(1);
	printf("%s", linea);
	free(linea);
	return (0);
}
```

## Compilación
Para compilar el proyecto, debes incluir los archivos fuente y definir el tamaño del buffer (opcional, por defecto es 42).
`cc -Wall -Wextra -Werror -D BUFFER_SIZE=4 *.c`
`cc -Wall -Wextra -Werror -D BUFFER_SIZE=100 *.c`
`cc -Wall -Wextra -Werror -D BUFFER_SIZE=1024 *.c`
`cc -Wall -Wextra -Werror -D BUFFER_SIZE=99999 *.c`

Una vez compilado procedemos a abrir el archivo ejecutable para poder ver el desarrollo de la función.
`./gnl_test.out`

# Recursos
Referencias


## Uso de IA
Este proyecto se ha realizado siguiendo las política sobre el uso de la IA de 42. Se ha utilizado un asistente de IA utilizado para comprender conceptos teoricos para facilitar el proceso de aprendizaje, sin generación directa de código final.

## Tareas y partes del proyecto donde se utilizó la IA:
- File Descriptors (fd): Cómo el sistema operativo maneja los archivos abiertos.
- Variables Estáticas: Diferencia entre variables locales, globales y estáticas en C.
- Gestión del Heap: Uso correcto de malloc y prevención de memory leaks.

# Diseño y Arquitectura:
Explicación del flujo de trabajo en tres pasos: leer en bucle, extraer la línea y guardar el resto estático para no perderlo en la siguiente ejecución.
Para ello las variables estáticas ayuda para entender por qué una variable normal perdía su valor entre llamadas y cómo static soluciona esto para mantener el buffer de lectura.