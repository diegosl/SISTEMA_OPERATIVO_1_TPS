/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////IMPORTACION DE LIBRERIAS////////////////////////////////////////////*/
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
//#include <stdlib.h>
#define BUFFERSIZE 256
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////IMPLEMENTACION DE FUNCIONES////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 1//////////////////////////////////////////////////*/
void Cabecera()
{
  //MOSTRAMOS NOMBRE DEL EQUIPO
  //MOSTRAMOS FECHA Y HORA DEL SISTEMA
  FILE *f = NULL;        
  char buffer[BUFFERSIZE+1];   
  char *match = NULL;       
  size_t bytesLeidos;       
  char dato1[BUFFERSIZE+1];   
  char dato2[BUFFERSIZE+1];   
  char dato3[BUFFERSIZE+1];   

  //NOMBRE DEL EQUIPO
  f = fopen("/proc/sys/kernel/hostname","r");

  if(f == NULL)
  {
    printf("NO SE HA PODIDIO ENCONTRAR EL ARCHIVO \n");
  }
  else
  {
    bytesLeidos = fread(buffer,1,sizeof(buffer),f);

    if(bytesLeidos == 0)
    {
      printf("FALLO LECTURA \n");
    }
    else
    {
      buffer[bytesLeidos] = '\0'; 

      sscanf(buffer, "%s",dato1);
      printf("NOMBRE DE LA MAQUINA: %s\n",dato1);
    }
  }

  fclose(f);

  f = NULL;          

  //FECHA Y HORA ACTUAL DEL SISTEMA
  f = fopen("/proc/driver/rtc","r");

  if(f == NULL)
  {
    printf("NO SE HA PODIDIO ENCONTRAR EL ARCHIVO \n");
  }
  else
  {
    bytesLeidos = fread(buffer,1,sizeof(buffer),f);

    if(bytesLeidos == 0)
    {
      printf("FALLO LECTURA \n");
    }
    else
    {
      buffer[bytesLeidos] = '\0'; 

      match = strstr(buffer, "rtc_time");

      if(match==NULL)
      {
        printf("FALLO BUSQUEDA DE LINEA \n");
      }
      else
      {
        sscanf(match, "rtc_time : %s",dato2);
      }

      match = strstr(buffer, "rtc_date");

      if(match==NULL)
      {
        printf("FALLO BUSQUEDA DE LINEA \n");
      }
      else
      {
        sscanf(match, "rtc_date : %s",dato3);
      }

      printf("FECHA Y HORA ACTUAL: %s %s\n",dato3,dato2);
    }
  }

  fclose(f);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 2//////////////////////////////////////////////////*/
void MostrarInfoAutor()
{
  //MOSTRAMOS ENCABEZADO PRINCIPAL
  printf("\n///////////////////SISTEMA OPERATIVO 1//////////////////////\n");
  printf("TRABAJO PRACTICO N°4: GESTION DE MEMORIA \n");              
  printf("ALUMNO: SOSA LUDUEÑA DIEGO MATIAS \n");            
  printf("ALUMNO: CHOQUEVILCA ZOTAR GUSTAVO BRAIAN \n");             
  Cabecera();                                  
  printf("////////////////////////////////////////////////////////////\n");  
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////ESTRUCTURA 1////////////////////////////////////////////////*/
//BLOQUE QUE CONTIENE UN TAMAÑO (size), UN APUNTADOR AL SIGUEINTE BLOQUE (next), 
//UN INDICADOR flag PARA SABER SI EL BLOQUE ESTA LIBRE O ESTA OCUPADO (free) 
//LA VARIABLE (magic) ES SOLO PARA DEBUGEAR
//POR LO TANTO BASICAMENTE SE APLICA EL CONCEPTO DE LISTA ENLAZADA (LISTA LIBRE) QUE SE TENDRA UNA ESTRUCTURA DE BLOQUES 
//DONDE CADA BLOQUE LIBRE APUNTARA AL SIGUIENTE BLOQUE LIBRE 
//Y EL ULTIMO BLOQUE LIBRE (DE DIRECCION DE MEMORIA MAS ALTA) DE LA LISTA APUNTARA AL PRIMER BLOQUE LIBRE DE LA LISTA
struct block_meta 
{
  size_t size;
  struct block_meta *next;
  int free;
  int magic;    
};
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

#define META_SIZE sizeof(struct block_meta)

void *global_base = NULL;

/*////////////////////////////////////////////////FUNCION 3//////////////////////////////////////////////////*/
//ENCONTRAR BLOQUE LIBRE
//CUANDO SE HACE UNA SOLICITUD, SE RASTREA  LA LISTA LIBRE HASTA QUE SE ENCUENTRA UN BLOQUE SUFICIENTEMENTE GRANDE.
struct block_meta *find_free_block(struct block_meta **last, size_t size) 
{
  struct block_meta *current = global_base;
  while (current && !(current->free && current->size >= size))
  {
    *last = current;
    current = current->next;
  }
  
  return current;
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 4//////////////////////////////////////////////////*/
//SOLICITAR ESPACIO
struct block_meta *request_space(struct block_meta* last, size_t size)
{
  struct block_meta *block;
  block = sbrk(0);
  void *request = sbrk(size + META_SIZE);
  assert((void*)block == request); 
  
  if (request == (void*) -1) 
  {
    return NULL;
  }
  
  // NULL EN LA PRIMERA SOLICITUD.
  if (last) 
  { 
    last->next = block;
  }

  block->size = size;
  block->next = NULL;
  block->free = 0;
  block->magic = 0x12345678;
  return block;
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 5//////////////////////////////////////////////////*/
// En la primera llamada, cuando, global_base == NULL, request_space y global_base se inicializan.
// De lo contrario, si encontramos un bloque libre es utilizado.
// Sino, request_space.
void *malloc(size_t size) 
{
  struct block_meta *block;

  if (size <= 0) 
  {
    return NULL;
  }

  //INGRESA EN LA PRIMERA LLAMADA
  if (!global_base) 
  { 
    block = request_space(NULL, size);
    if (!block) 
    {
      return NULL;
    }
    
    global_base = block;
  } 
  else 
  {
    struct block_meta *last = global_base;
    block = find_free_block(&last, size);

    //INGRESA CUANDO NO SE PUEDE ENCONTRAR BLOQUE LIBRE
    if (!block) 
    { 
      block = request_space(last, size);
      if (!block) 
      {
          return NULL;
      }
    } 
    //INGRESA CUANDO ENCUENTRA BLOQUE LIBRE
    else 
    {
      //AQUI SE DETERMINA BLOQUE DE DIVISION
      block->free = 0;
      block->magic = 0x77777777;
    }
  }
  
  return(block+1);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 6//////////////////////////////////////////////////*/
struct block_meta *get_block_ptr(void *ptr) 
{
  return (struct block_meta*)ptr - 1;
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 7//////////////////////////////////////////////////*/
void free(void *ptr) 
{
  if (!ptr) 
  {
    return;
  }
  //CONSIDERA LA FUNCION DE LOS BLOQUES UNA VEZ QUE EL FRACCIONAMIENTO DE LOS BLOQUES FUE IMPLEMENTADO
  struct block_meta* block_ptr = get_block_ptr(ptr);
  assert(block_ptr->free == 0);
  assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
  block_ptr->free = 1;
  block_ptr->magic = 0x55555555;  
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////FUNCION PRINCIPAL//////////////////////////////////////////////*/
int main(int argc, char *argv[])
{
  //MOSTRAMOS ENCABEZADO PRINCIPAL
	MostrarInfoAutor();

	int* p1;
	int* p2;
	int* p3;
	
  printf("\n////////////LLAMAMOS FUNCIONES malloc() Y free()////////////\n");

	p1 = malloc(sizeof(int));
	p2 = malloc(sizeof(int));
	p3 = malloc(sizeof(int));
	
	*p1 =2;
	*p2 =200;
	*p3 =3000;
	
	printf("\nPUNTERO 1 VALOR: %d \nPUNTERO 1 DIRECCION: %p \n", *p1, &p1);
	printf("\nPUNTERO 2 VALOR: %d \nPUNTERO 2 DIRECCION: %p \n", *p2, &p2);
	printf("\nPUNTERO 3 VALOR: %d \nPUNTERO 3 DIRECCION: %p \n", *p3, &p3);
	
	free(p1);
	free(p2);
	free(p3);
	
	// Aqui vemos que se vuelve a utilizar el espacio de memoria liberado por free
	// realizando nuevamente un malloc para el puntero *p3 entonces veremos que dos punteros
	// apuntaran a un misma espacio ya que se reutiliza el espacio de alojamiento segun la implementacion
  printf("\n////////////LLAMAMOS NUEVAMENTE FUNCION malloc()////////////\n");
	p3 = malloc(sizeof(int));
	*p3 =3000;
	printf("\nPUNTERO 1 VALOR: %d \nPUNTERO 1 DIRECCION: %p \n", *p1, &p1);
  printf("\nPUNTERO 2 VALOR: %d \nPUNTERO 2 DIRECCION: %p \n", *p2, &p2);
  printf("\nPUNTERO 3 VALOR: %d \nPUNTERO 3 DIRECCION: %p \n", *p3, &p3);
	
	return 0;
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/