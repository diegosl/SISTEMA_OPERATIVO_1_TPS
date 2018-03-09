# TRABAJO PRACTICO 1: KSAMP

Este trabajo practico se lo utilizo en SO ubuntu.

El proyecto ksamp esta compuesto por los siguientes archivos:

* [ksamp.c](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP1/CODIGO_KSAMP/ksamp.c)
* [ParteB.h](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP1/CODIGO_KSAMP/ParteB.h)
* [ParteC.h](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP1/CODIGO_KSAMP/ParteC.h)
* [ParteD.h](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP1/CODIGO_KSAMP/ParteD.h)
* [makefile](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP1/CODIGO_KSAMP/makefile)

Luego a modo de documentacion se realizo en un informe, el cual es:

* [INFORME KSAMP.pdf](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP1/INFORME_KSAMP/INFORME%20KSAMP.pdf)

Se describira como es el uso del proyecto ksamp:

1. **Primero se debe ir al directorio donde se encuentra el proyecto:**

  *por ejemplo:* **$ cd Documentos/ProyectoKsamp**

2. **Para compilar el proyecto ksamp debe ingresar en la consola:**

  **$ make**

3. **Para ejecutar la primera version del proyecto ksamp (PARTE B) debe ingresar en la consola:**

  *por ejemplo:* **$ ./ksamp.out**

4. **Para ejecutar la segunda version del proyecto ksamp (PARTE C) debe ingresar en la consola:**

  *por ejemplo:* **$ ./ksamp.out -s**

5. **Para ejecutar la tercera version del proyecto ksamp (PARTE D) debe ingresar en la consola:**

  *por ejemplo:* **$ ./ksamp.out -l 5 15**

6. **Para finalizar debemos eliminar el archivo ejecutable donde se debe colocar en la consola:**

  **$ make clean**
  
# TRABAJO PRACTICO 2: BAASH

Este trabajo practico se lo utilizo en SO ubuntu.

El proyecto baash esta compuesto por los siguientes archivos:
* [baash.c](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP2/CODIGO_BAASH/baash.c)
* [makefile](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP2/CODIGO_BAASH/makefile)

Luego a modo de documentacion se realizo en un informe, el cual es:

* [INFORME BAASH.pdf](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP2/INFORME_BAASH/INFORME%20BAASH.pdf)

Se describira como es el uso del proyecto baash:

1. **Primero se debe ir al directorio donde se encuentra el proyecto:**

  *por ejemplo:* **$ cd Documentos/ProyectoBaash**

2. **Para compilar el proyecto baash debe ingresar en la consola:**

  **$ make**

3. **Para ejecutar el proyecto baash, debe ingresar en la consola:**

  *por ejemplo:* **$ ./baash.out**

4. **Para realizar la primera parte del proyecto baash (PARTE A) debe ingresar en la consola:**

  *por ejemplo:* **$ cd /home/usuario/Documentos/ProyectoKsamp**

5. **Para realizar la segunda parte del proyecto baash (PARTE B) debe ingresar en la consola:**

  *por ejemplo:* **$ ./ksamp.out -l 3 120 &**
  
6. **Para realizar la tercera parte del proyecto baash (PARTE C) debe ingresar en la consola:**

  *por ejemplo:* **$ ls | sort**

7. **Para finalizar debemos eliminar el archivo ejecutable donde se debe colocar en la consola:**

  **$ make clean**
  
# TRABAJO PRACTICO 3: MODULOS PARA KERNEL DE LINUX

Este trabajo practico se lo utilizo en SO ubuntu.

El proyecto modulos para kernel de linux esta compuesto por los siguientes archivos:
* [main.c](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP3/main.c)
* [mkmodulo.c](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP3/mkmodulo.c)
* [mkmodulo2.c](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP3/mkmodulo2.c)
* [makefile](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP3/Makefile)

Descripcion del proyecto:

El objetivo de este trabajo practico es el desarrollo de dos simples módulos para insertar en el kernel de linux. Uno de los dispositivos realizara la encriptacion simple de los caracteres que se escriben en el. El otro de los módulos realizara la desencriptacion de los caracteres que se escriben en el. Esta encriptacion y desencriptacon consta de la conversión de cada carácter a numero y luego sumar y restar un valor constante a cada carácter. En este caso la constante es uno. Con respecto a los caracteres solo se tiene en cuenta las letras de abecedario en minúscula.

Pasos que se deben realizar para poder ejecutar y compilar el proyecto:

1. **Primero se debe ir al directorio donde se encuentra el proyecto:**

  *por ejemplo:* **$ cd Documentos/ProyectoModulosParaKernelDeLinux**

2. **Para compilar el proyecto modulos para kernel de linux debe ingresar en la consola:**

  **$ make**

3. **Se debe cargar los modulos en el kernel:**

  * *En modo usuario (para encriptar):*    **$ sudo insmod mkmodulo.ko**
  * *En modo usuario (para desencriptar):* **$ sudo insmod mkmodulo2.ko**

4. **Se debe verificar que los modulos estan cargados en el kernel:**

  **$ lsmod | head**

5. **Para ejecutar el proyecto:**

  **$ sudo ./lkm**

6. **Para finalizar debemos eliminar el archivo ejecutable donde se debe colocar en la consola:**

  **$ make clean**

7. **Para eliminar los modulos cargados en el kernel, se debe colocar:**

  * **$ sudo rmmod mkmodulo**
  * **$ sudo rmmod mkmodulo2**
  
# TRABAJO PRACTICO 4: GESTION DE MEMORIA

Este trabajo practico se lo utilizo en SO ubuntu.

El proyecto gestion de memoria esta compuesto por los siguientes archivos:
* [GestionDeMemoria.c](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP4/GESTIONDEMEMORIA/GestionDeMemoria.c)
* [makefile](https://github.com/diegosl/SISTEMA_OPERATIVO_1_TPS/blob/master/TP4/GESTIONDEMEMORIA/makefile)

Descripcion del proyecto:

En este trabajo practico se implemento la funcion *malloc* y la funcion *free* para la asignacion y liberacion de la memoria (memoria dinamica). La memoria dinamica es la memoria que se reserva en tiempo de ejecucion. Por lo tanto nosotros como programadores tendremos la tarea de reservar y liberar memoria. 

La funcion *malloc* reserva un bloque de memoria y devuelve un puntero void al inicio de la misma, el prototipo de dicha funcion es:

**void *malloc(size_t size);**

Donde el parametro *size* especifica el numero de bytes a reservar. En caso de que no se pueda realizar la asignacion, devuelve el valor nulo.

La funcion *free* sirve para liberar memoria que se asigno dinamicamente. Si el puntero es nulo, la funcion no hace nada, el prototipo de dicha funcion es:

**void *free(void *ptr);**

Donde el parametro *ptr* es el puntero a la memoria que se desea liberar.

Una vez liberada la memoria, si se quiere volver a utilizar el puntero, primero se debe reservar nueva memoria con la funcion *malloc*

Pasos que se deben realizar para poder ejecutar y compilar el proyecto:

1. **Primero se debe ir al directorio donde se encuentra el proyecto:**

  *por ejemplo:* **$ cd Documentos/ProyectoGestionDeMemoria**

2. **Para compilar el proyecto gestion de memoria debe ingresar en la consola:**

  **$ make**

3. **Para ejecutar el proyecto:**

  **$ ./GestionDeMemoria.out**

4. **Para finalizar debemos eliminar el archivo ejecutable donde se debe colocar en la consola:**

  **$ make clean**
