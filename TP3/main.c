/**
 * @nombre   mkmodulo.c
 * @autores  Choquevilca & Sosa Ludueña
 * @Descripcion: Encripta y Desencripta un mensaje mediante dos modulos respectivamente.
  */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER 1000               
static char receive[BUFFER];     


int main(){
   int fd,fp;
   int ret,ret2;
   int j,i;
   char Caracter[0];
   int num_cod=0;
   char stringToSend[BUFFER];
   char alfabeto[27] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm','n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' ,' '};
   printf("-----------------------------------------------------------------------\n");
   printf("                    Modulo de encriptado\n");
   printf("-----------------------------------------------------------------------\n");
   fd = open("/dev/mkmodulo",O_RDWR);             
   fp= open ("/dev/mkmodulo2",O_RDWR); 
   if (fd < 0 && fp < 0){
      perror("Error al abrir el dispositivo...");
      return errno;
   }
   printf("Escriba una cadena de carateres para enviar al modulo kernel: \n");
   scanf("%[^\n]%*c", stringToSend); 
   
   //Encriptado
   int len = strlen(stringToSend);
   char num_arreglo[len];
   for(i = 0 ; i < len; i++) 
    {
    strncpy(Caracter, stringToSend+i,1); 
    for(j = 0; j < 26; j++) {
    if(*Caracter == alfabeto[j]) break;
                            }        
    num_cod=j+1;
    num_arreglo[i]=num_cod;
    }

    ret = write(fd, num_arreglo, strlen(num_arreglo));
    ret2 = write(fp, num_arreglo, strlen(num_arreglo));  
   if (ret < 0 && ret2 < 0)
   {
      perror("Error al escribir el mensaje en el dispositivo.");
      return errno;
   }

   printf("Presione ENTER para ver el mensaje encriptado");
   getchar();
   printf("\nEl mensaje encriptado es:\n");
   int m,g;
   char Cadena3[BUFFER];
  for(m=0;m<len;m++)
   {
    printf("%d ",num_arreglo[m]);
   }

   printf("\n\nPresione ENTER para desencriptar\n");
   getchar();
   printf("-----------------------------------------------------------------------\n");
   printf("                    Modulo de desencriptado\n");
   printf("-----------------------------------------------------------------------\n");
   ret2 = read(fp, receive, BUFFER);        
   if (ret2 < 0){
      perror("Error al leer el mensaje desde el dispositivo.");
      return errno;
   }
   
	printf("El mensaje desencriptado es: \n");
    //Desencriptado
    for(i = 0 ; i < len; i++) 
    {
    strncpy(Caracter, receive+i,1); 
    char Caracter2[0];
    Caracter2[0]=*Caracter-1;
    
    for(g=0;g<26;g++){
    if(*Caracter2==g) break;
                     }
    Cadena3[i]= alfabeto[g];
    }

    for( m= 0;m < len;m++)
    {
    printf("%c", Cadena3[m]);
    }

  printf("\n");

   return 0;
}
