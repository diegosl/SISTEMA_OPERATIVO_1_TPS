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
void MostrarParteB();
void Cabecera();
void TipoDeCpu();
void ModeloDeCpu();
void VersionDeKernel();
void TiempoActivo();
void CantDeSistDeArchKernel();
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////IMPLEMENTACION DE FUNCIONES////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 1//////////////////////////////////////////////////*/
void MostrarParteB()
{
	printf("\n////////////////////////CABECERA////////////////////////////\n");
	Cabecera();
	printf("////////////////////////////////////////////////////////////\n");

	printf("\n/////////////////////////PARTE B////////////////////////////\n");
	TipoDeCpu();
	ModeloDeCpu();
	VersionDeKernel();
	TiempoActivo();
	CantDeSistDeArchKernel();
	printf("////////////////////////////////////////////////////////////\n");
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 2//////////////////////////////////////////////////*/
void Cabecera()
{
	FILE *f = NULL;
	char buffer[BUFFSIZE+1];
	char *match = NULL;
	size_t bytesLeidos;
	char dato1[BUFFSIZE+1];
	char dato2[BUFFSIZE+1];
	char dato3[BUFFSIZE+1];

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
				sscanf(match, "rtc_time	: %s",dato2);
			}

			match = strstr(buffer, "rtc_date");

			if(match==NULL)
			{
				printf("FALLO BUSQUEDA DE LINEA \n");
			}
			else
			{
				sscanf(match, "rtc_date	: %s",dato3);
			}

			printf("FECHA Y HORA ACTUAL: %s %s\n",dato3,dato2);
		}
	}

	fclose(f);

}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 3//////////////////////////////////////////////////*/
void TipoDeCpu()
{
	FILE *f = NULL;
	char buffer[BUFFSIZE+1];
	char *match = NULL;
	size_t bytesLeidos;
	char dato[BUFFSIZE+1];

	f = fopen("/proc/cpuinfo","r");

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

			match = strstr(buffer, "vendor_id");

			if(match==NULL)
			{
				printf("FALLO BUSQUEDA DE LINEA \n");
			}
			else
			{
				sscanf(match, "vendor_id : %s",dato);
				printf("TIPO DE CPU: %s\n",dato);
			}
		}
	}

	fclose(f);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 4//////////////////////////////////////////////////*/
void ModeloDeCpu()
{
	FILE *f = NULL;
	char buffer[BUFFSIZE+1];
	char *match = NULL;
	size_t bytesLeidos;
	char dato1[BUFFSIZE+1];
	char dato2[BUFFSIZE+1];
	char dato3[BUFFSIZE+1];
	char dato4[BUFFSIZE+1];
	char dato5[BUFFSIZE+1];
	char dato6[BUFFSIZE+1];

	f = fopen("/proc/cpuinfo","r");

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

			match = strstr(buffer, "model name");

			if(match==NULL)
			{
				printf("FALLO BUSQUEDA DE LINEA \n");
			}
			else
			{
				sscanf(match, "model name : %s %s %s %s %s %s",dato1,dato2,dato3,dato4,dato5,dato6);
				printf("MODELO DE CPU: %s %s %s %s %s %s\n",dato1,dato2,dato3,dato4,dato5,dato6);
			}
		}
	}

	fclose(f);
	
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 5//////////////////////////////////////////////////*/
void VersionDeKernel()
{
	FILE *f = NULL;
	char buffer[BUFFSIZE+1];
	char *match = NULL;
	size_t bytesLeidos;
	char dato[BUFFSIZE+1];

	f = fopen("/proc/version","r");

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

			match = strstr(buffer, "Linux version");

			if(match==NULL)
			{
				printf("FALLO BUSQUEDA DE LINEA \n");
			}
			else
			{
				sscanf(match, "Linux version %s",dato);
				printf("VERSION DE KERNEL: %s\n",dato);
			}
		}
	}

	fclose(f);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 6//////////////////////////////////////////////////*/
void TiempoActivo()
{
	FILE *f = NULL;
	char buffer[BUFFSIZE+1];
	float t_f;
	int dias;
	int horas;
	int minutos;
	int segundos;

	f = fopen("/proc/uptime","r");

	if(f == NULL)
	{
		printf("NO SE HA PODIDIO ENCONTRAR EL ARCHIVO \n");
	}
	else
	{
		fscanf(f,"%s",buffer);

		t_f = atof(buffer);

		dias = t_f/86400;
		horas = (t_f)/3600;
		minutos = ((t_f/3600)-horas)*60;
		segundos = ((((t_f/3600)-horas)*60)-minutos)*60;

		printf("TIEMPO ACTIVO DE SO: %iD:%i:%i:%i \n",dias,horas,minutos,segundos);
	}

	fclose(f);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 1//////////////////////////////////////////////////*/
void CantDeSistDeArchKernel()
{
	FILE *f = NULL;
	char buffer = '1';
	int CantSistArch = 0;

	f = fopen("/proc/filesystems","r");

	if(f == NULL)
	{
		printf("NO SE HA PODIDIO ENCONTRAR EL ARCHIVO \n");
	}
	else
	{
		while (buffer != EOF)
		{
			buffer = fgetc(f);

			if(buffer == '\n')
			{
				CantSistArch++;
			}
		}

		printf("CANTIDAD DE SISTEMA DE ARCHIVOS SOPORTADOS POR EL KERNEL: %i\n",CantSistArch);
	}

	fclose(f);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/