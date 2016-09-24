/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////IMPORTACION DE LIBRERIAS////////////////////////////////////////////*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFFSIZE 256
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////PROTOTIPOS///////////////////////////////////////////////////*/
void MostrarParteC();
void CantDeTiempoDeCpu();
void CantDeCambDeContxt();
void FechaYHoraDeArranDeSist();
void NumDeProcIniDeSist();
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////IMPLEMENTACION DE FUNCIONES////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 1//////////////////////////////////////////////////*/
void MostrarParteC()
{
	printf("\n/////////////////////////PARTE C////////////////////////////\n");
	CantDeTiempoDeCpu();
	CantDeCambDeContxt();
	FechaYHoraDeArranDeSist();
	NumDeProcIniDeSist();
	printf("////////////////////////////////////////////////////////////\n");
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 2//////////////////////////////////////////////////*/
void CantDeTiempoDeCpu()
{
	FILE *f = NULL;
	char dato1[BUFFSIZE+1];
	char dato2[BUFFSIZE+1];
	char dato3[BUFFSIZE+1];
	int t_i;
	int tiempDeUsuario;
	int tiempDeSistema;
	int tiempDeIdle;

	f = fopen("/proc/stat","r");

	if(f == NULL)
	{
		printf("NO SE HA PODIDIO ENCONTRAR EL ARCHIVO \n");
	}
	else
	{
		fscanf(f,"cpu %s %s %s",dato1,dato2,dato3);

		t_i = atoi(dato1);
		tiempDeUsuario = t_i/100;
		t_i = atoi(dato2);
		tiempDeSistema = t_i/100;
		t_i = atoi(dato3);
		tiempDeIdle = t_i/100;

		printf("CANTIDAD DE TIEMPO DE CPU EN SEGUNDOS UTILIZADO POR:\nUSUARIO: %i\nSISTEMA: %i\nPROCESO IDLE: %i\n",tiempDeUsuario,tiempDeSistema,tiempDeIdle);
	}

	fclose(f);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 3//////////////////////////////////////////////////*/
void CantDeCambDeContxt()
{
	FILE *f = NULL;
	char buffer[BUFFSIZE+1];
	char *match = NULL;
	size_t bytesLeidos;
	char dato[BUFFSIZE+1];
	int i;

	f = fopen("/proc/stat","r");

	if(f == NULL)
	{
		printf("NO SE HA PODIDIO ENCONTRAR EL ARCHIVO \n");
	}
	else
	{
		for(i=0; i<40; i++)
		{
			bytesLeidos = fread(buffer,1,sizeof(buffer),f);

			match = strstr(buffer, "ctxt");

			if(match != NULL)
			{
				i=40;
			}
		}

		if(bytesLeidos == 0)
		{
			printf("FALLO LECTURA \n");
		}
		else
		{
			buffer[bytesLeidos] = '\0'; 

			if(match==NULL)
			{
				printf("FALLO BUSQUEDA DE LINEA \n");
			}
			else
			{
				sscanf(match, "ctxt %s",dato);
				printf("CANTIDAD DE CAMBIOS DE CONTEXTO: %s\n",dato);
			}
		}
	}

	fclose(f);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 4//////////////////////////////////////////////////*/
void FechaYHoraDeArranDeSist()
{
	FILE *f = NULL;
	char buffer[BUFFSIZE+1];
	char *match = NULL;
	size_t bytesLeidos;
	char dato1[BUFFSIZE+1];
	char dato2[BUFFSIZE+1];

	f = popen("who -b","r");

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

			match = strstr(buffer, "arranque del sistema");

			if(match==NULL)
			{
				printf("FALLO BUSQUEDA DE LINEA \n");
			}
			else
			{
				sscanf(match, "arranque del sistema %s %s",dato1,dato2);
				printf("FECHA Y HORA CUANDO EL SISTEMA FUE INICIADO: %s %s\n",dato1,dato2);
			}
		}
	}
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 5//////////////////////////////////////////////////*/
void NumDeProcIniDeSist()
{
	FILE *f = NULL;
	char buffer[BUFFSIZE+1];
	char *match = NULL;
	size_t bytesLeidos;
	char dato[BUFFSIZE+1];
	int i;

	f = fopen("/proc/stat","r");

	if(f == NULL)
	{
		printf("NO SE HA PODIDIO ENCONTRAR EL ARCHIVO \n");
	}
	else
	{
		for(i=0; i<40; i++)
		{
			bytesLeidos = fread(buffer,1,sizeof(buffer),f);

			match = strstr(buffer, "processes");

			if(match != NULL)
			{
				i=40;
			}
		}

		if(bytesLeidos == 0)
		{
			printf("FALLO LECTURA \n");
		}
		else
		{
			buffer[bytesLeidos] = '\0'; 

			if(match==NULL)
			{
				printf("FALLO BUSQUEDA DE LINEA \n");
			}
			else
			{
				sscanf(match, "processes %s",dato);
				printf("NUMERO DE PROCESOS CREADOS DESDE EL INICIO DEL SISTEMA: %s\n",dato);
			}
		}
	}

	fclose(f);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/