/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////IMPORTACION DE LIBRERIAS////////////////////////////////////////////*/
#include <stdio.h>											//
#include <string.h>											//
#include <stdlib.h>											//
#include <pwd.h>											//	
#include <unistd.h>											//
#include <sys/wait.h>										//
#include <sys/stat.h>										//		
#include <dirent.h>											//
#include <fcntl.h>											//	
#define BUFFERSIZE 256										//
#define MAX 51												//
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////PROTOTIPOS///////////////////////////////////////////////////*/
void MostrarInfoAutor();									//
void Cabecera();											//
int leeComando(char *argv[], char *cadena);					//
void buscaPaths(char *paths[]);								//
void buscaArchivo(char *arch,char *paths[],char *execPath);	//
int verificaPipe(char *argv[],char *argv1[],char *argv2[]);	//
void ejecutaPipe(char *argv1[],char *argv2[],char *paths[]);//
int redireccion(char *argv[],char fileName[]);				//
void salida(char fileName[]);								//
void entrada(char fileName[]);								//
void mostrarParametro(char *argv[]);						//
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////FUNCION PRINCIPAL//////////////////////////////////////////////*/
int main(int arg, char *argv)
{
	//VARIABLE DE CONTROL DE HILO Y TUBERIA
	int pid;												//
	int banderaPid;											//
	int banderaPipe;										//
	int procesosBG=0; 										//
	
	//VARIABLES DE CONTROL DE PARSEO
	int argC;												//
	char *argV[20];											//
	char *argv1[20];										//
	char *argv2[20];										//
	char comando [BUFFERSIZE];								//

	//GUARDAMOS EL HOSTNAME DE LA MAQUINA (hostname)
	char hostname[BUFFERSIZE];								//
	gethostname(hostname,sizeof(hostname));					//

	//GUARDAMOS EL NOMBRE DE USUARIO (user)
	char *user; 											//
	struct passwd *pass; 									//
	pass = getpwuid(getuid()); 								//
	user = pass->pw_name;									//

	//GUARDAMOS RUTA COMPLETA DEL DIRECTORIO ACTUAL (dirActual)
	char dirActual[BUFFERSIZE];								//
	getcwd(dirActual,sizeof(dirActual));					//
	
	//MOSTRAMOS ENCABEZADO PRINCIPAL
	MostrarInfoAutor();										//	
	
	//MOSTRAMOS DIRECTORIOS DEL PATH Y GUARDAMOS LOS DIRECTORIOS DEL PATH EN (paths[])
	char executePath[BUFFERSIZE]="";						//
	char *paths[BUFFERSIZE];								//
	buscaPaths(paths);										//

	printf("\nbaash: SE INGRESO A UN NUEVO BAASH\n\n");		//
	
	while(1) 												//CON (CTRL+D) O (CTRL+C) SALIMOS DEL CICLO WHILE Y FINALIZA BAASH (SERIA UN exit)
	{	
		getcwd(dirActual,sizeof(dirActual));				//ACTUALIZAMOS RUTA ACTUAL
		strcpy(comando,"\n");								//COPIAMOS EN (comando) UNA NUEVA LINEA (\n)

		printf("%s@%s:%s$ ",user,hostname,dirActual);		//MOSTRAMOS PROMPT
		
		fgets(comando,BUFFERSIZE,stdin);					//ESPERAMOS ENTRADA POR TECLADO (stdin) MAXIMO DE 1 BYTE (BUFFERSIZE) Y LA GUARDAMOS EN (comando)
		
		if((feof(stdin)) || strcmp("exit\n",comando)==0)	//SI LA ENTRADA POR TECLADO (stdin) ES EOF (end of file) o (comando=exit\n) FINALIZA BAASH
		{
			printf("\nbaash: FINALIZACION DE EJECUCION DE BAASH\n\n");
			exit(0);
		}
		
		if(!strcmp(comando,"\n")) 							//SI (comando=\n) VOLVEMOS A MOSTRAR EL PROMPT 
		{													//PARA UNA NUEVA INTERPRETACION DE COMANDOS
			continue;
		}

		else												//SI NO ES NINGUNAS DE LAS ANTERIRES
		{
			argC = leeComando(argV,comando); 			    //GUARDAMOS LA CANTIDAD DE ARGUMENTOS Y GUARDAMOS EN (argv[0]) EL NOMBRE DEL COMANDO
			argv1[0]=argV[1];
			argv2[0]=argV[2];

			int status = 0;									//EL ESTADO DEL PROCESO HIJO SE GUARDA EN (status)

			if(!strcmp(argV[0],"cd"))						//SI LA ENTRADA POR TECLADO (stdin) ES (cd)
			{
				chdir(argV[1]);								//CAMBIAMOS DIRECTORIO
				continue;
			}
			
			//VARIABLE DE CONTROL DE HILO Y TUBERIA
			int banderaRedir = 0;							//
			int hacerPipe = 0;								//
			char fileName[50];								//

			//VERIFICAMOS TUBERIA Y REALIZAMOS REDIRECCION
			hacerPipe = verificaPipe(argV,argv1,argv2);		//
			banderaRedir = redireccion(argV,fileName);		//


			//BUSCAMOS ARCHIVO EN LOS DIRECTORIOS DEL PATH
			buscaArchivo(argV[0], paths, executePath);		//

			if(executePath[0] == 'X')
			{
				printf("baash: NO SE ENCONTRO DIRECTORIO O ARCHIVO SOLICITADO\n");
			}
			else
			{
				if (procesosBG == 0)
				{				
					pid = fork();							//CREAMOS PROCESO HIJO PARA REALIZAR EJECUCION DE OTRO ARCHIVO
															//PID: NOS PROPORCIONA EL ID DEL PROCESO, SI ES 0 ES UN HIJO, SINO ES EL PADRE
				}
				
				else
				{				
					pid = vfork();							//CREAMOS PROCESO HIJO PARA REALIZAR EJECUCION DE OTRO ARCHIVO
															//PID: NOS PROPORCIONA EL ID DEL PROCESO, SI ES 0 ES UN HIJO, SINO ES EL PADRE
				}	
	
				if (pid<0) {
					perror("Creando el hijo");
					exit(1);
				}
				else if (pid == 0) 							//HIJO
				{					
					if(banderaRedir == 2)
					{
						salida(fileName);
					}
					else if(banderaRedir == 1)
					{						
						freopen(fileName,"r",stdin);
					}
					if(hacerPipe == 1)
					{
						ejecutaPipe(argv1, argv2, paths);
						banderaPipe = 1;
					}
					if(!banderaPipe){
						execv(executePath, argV);
						perror(executePath);
						exit(1);
					}
					return 0;
				}
				
				else										//PADRE
				{	
					banderaPid = -1;
				}
				pid = wait(&status); 						//PROCESO HIJO TERMINADO
			}
		}

	}
	return 0;												//FINALIZACION DE PROGRAMA
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////IMPLEMENTACION DE FUNCIONES////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 1//////////////////////////////////////////////////*/
void MostrarInfoAutor()
{
	//MOSTRAMOS ENCABEZADO PRINCIPAL
	printf("\n///////////////////SISTEMA OPERATIVO 1//////////////////////\n");		//
	printf("TRABAJO PRACTICO N°2: BAASH \n");										//
	printf("ALUMNO: SOSA LUDUEÑA DIEGO MATIAS \n");									//
	printf("ALUMNO: CHOQUEVILCA ZOTAR GUSTAVO BRAIAN \n");							//
	Cabecera();																		//
	printf("////////////////////////////////////////////////////////////\n");		//
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 2//////////////////////////////////////////////////*/
void Cabecera()
{
	//MOSTRAMOS NOMBRE DEL EQUIPO
	//MOSTRAMOS FECHA Y HORA DEL SISTEMA
	FILE *f = NULL;					//
	char buffer[BUFFERSIZE+1];		//
	char *match = NULL;				//
	size_t bytesLeidos;				//
	char dato1[BUFFERSIZE+1];		//
	char dato2[BUFFERSIZE+1];		//
	char dato3[BUFFERSIZE+1];		//

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

	f = NULL;						//

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
void buscaPaths(char *paths[])
{
	//GUARDAMOS EN (ptahs[]) Y MOSTRAMOS LOS DIRECTORIOS DEL PATH
	char *pathVar = getenv("PATH");		// APUNTAMOS AL PRIMER DIRECTORIO DEL PATH (pathVar)
	paths[0] = strtok(pathVar, ":");	// GUARDAMOS EN PRIMERA POSICION DEL ARREGLO DE PUNTEROS (paths[]) EL PRIMER DIRECTORIO DEL PATH

	//MOSTRAMOS LISTA DE DIRECTORIOS DEL PATH
	printf("\n/////////////LISTA DE DIRECTORIOS EN EL PATH////////////////\n");
	printf("PATH[%i] = %s \n",0,paths[0] );
	
	for(int contadorPaths = 1; contadorPaths<BUFFERSIZE; contadorPaths++)//GUARDAMOS LOS SIGUINETES DIRECTORIOS DEL PATH (path[])
	{
		paths[contadorPaths] = strtok(NULL,":");	

		if (paths[contadorPaths] == NULL)								//SI NO SE ENCUENTRAN MAS DIRECTORIOS DEL PATH SALIMOS DEL CICLO FOR
		{
			break;
		}
		printf("PATH[%i] = %s \n",contadorPaths,paths[contadorPaths]);
	}
	strtok(NULL,":");
	printf("////////////////////////////////////////////////////////////\n");
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 4//////////////////////////////////////////////////*/
int leeComando(char *argv[],char *cadena)
{
	//GUARDAMOS COMANDO EN (argv[0]) Y GUARDAMOS LOS DEMAS ARGUMENTOS DEL COMANDO
	int argumentos = 0;							//INICIALIZAMOS VARIABLE (argumentos)
	argv[0] = strtok(cadena, " \n");			//GUARDAMOS EN (argv[0]) EL NOMBRE DEL COMANDO (archivo)

	for(argumentos = 1; argumentos<BUFFERSIZE; argumentos++)
	{
		argv[argumentos] = strtok(NULL, " \n");	//GUARDAMOS LOS SIGUIENTES ARGUMENTOS DEL COMANDO
		
		if (argv[argumentos] == NULL)			//SI NO SE ENCUENTRAN MAS ARGUMENTOS DEL COMANDO SALIMOS DEL CICLO FOR
		{
			break;
		}
	}
	return argumentos-1;						//RETORNAMOS Y NO CONTAMOS EL NOMBRE DEL COMANDO
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 5//////////////////////////////////////////////////*/
void buscaArchivo(char *arch, char *paths[], char *execPath)
{
	//BUSCAMOS EL EJECUTABLE Y SE MUESTRA EN QUE CAMINO SE ENCUENTRA
	char returnPath[50];
	int resultado;
	char searchDir[50] = "";
	char* archivo="";
	strcpy(returnPath, arch);
	
	//PATH ABSOLUTO:
	if(arch[0] == '/' || (arch[0] == '.' && arch[1] == '.' && arch[2] == '/'))
	{
		char *dir;
		char *nextDir;
		int pathCompleto = 0;
		
		if(arch[0] == '/')
			searchDir[0] = '/';
		dir = strtok(arch,"/");
		nextDir = strtok(NULL,"/");
		
		if(nextDir != NULL) //Verifica si es archivo
			strcat(searchDir,dir);
		else
		{
			nextDir = dir;
			pathCompleto = 1;
		}

		while((nextDir != NULL) && !pathCompleto)
		{
			dir = nextDir;
			nextDir = strtok(NULL,"/");
			strcat(searchDir,"/");
			if(nextDir != NULL)
				strcat(searchDir,dir);
		}
		archivo = dir;
	}

	//PATH RELATIVO:	
	else if(arch[0] == '.' && arch[1] == '/'){
		getcwd(searchDir, 50);
		strcat(searchDir,"/");
		archivo = strtok(arch, "/");
		archivo = strtok(NULL,"/");
	}
	
	else{
		//Busca en todos los directorios
		int i;
		char aux[50];
		char aux2[50];
		char cadenaAux[256];
		char* m[1];
		m[0]= "";
		int bandera = 0;
		m[0] = getcwd(NULL,50);				//camino o direccion actual
		
		if(m[0] != NULL) 					//primero nos fijamos en el directorio actual
		{
			strcpy(aux2,m[0]);
			strcat(aux2,"/");				//agregamos / en aux2
			strcat(aux2, arch);
			
			resultado = access(aux2, F_OK);//Verifico si archivo existe en el path actual (camino o direccion actual)
			if(resultado==0)			   //si existe archivo guardo camino con archivo en execPath
			{
				strcpy(execPath, aux2);
				return;
			}
		}
		
		for(i = 0; i < 20; i++) 			// luego ejecutamos la busqueda en la secuencia PATH
		{
			strcpy(cadenaAux, arch);
			
			if(paths[i] == NULL)
				break;
			
			while(1)
			{
				strcpy(aux,paths[i]);
				strcat(aux,"/");
				if(bandera == 0)
				{
					m[0] = strtok(cadenaAux, "/");
					if(m[0] == NULL){ break; }
					strcat(aux, m[0]);
					
					resultado = access(aux, F_OK);
					if(resultado==0)
					{
						strcpy(execPath, aux);
						//printf("paso1\n");
						return;
					}
					strcat(aux,"/");
					bandera = 1;
				}
				else
				{
					m[0] = strtok(NULL, "/");
					if(m[0] == NULL){ break; }
					strcat(aux, m[0]);
				
					resultado = access(aux, F_OK);
					if(resultado==0)
					{
						strcpy(execPath, aux);
						return;
					}
					strcat(aux,"/");
				}
				
			}
			bandera=0;
		}
		execPath[0] = 'X';
		
		return;
	}
	
	strcat(searchDir, archivo);
	resultado = access(searchDir, F_OK);//verifico en Path serchDir si existe archivo
	if(resultado==0)					//si existe, paso
		strcpy(execPath, searchDir);	//guardo path en execPath
	else
	{									//sino
		execPath[0] = 'X';
	}									//guardo X
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 6//////////////////////////////////////////////////*/
void mostrarParametro(char* argv[])
{
	//VERIFICACION DE LOS DATOS DEL (argv[])
	static int i = 0;					//
	printf("arg%u: %s \n", i, *argv);	//
	i++;								//
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 7//////////////////////////////////////////////////*/
int redireccion(char* argv[], char fileName[])
{										
	//VERIFICA SI HAY QUE REDIRECCIONAR LA ENTRADA O SALIDA ESTANDAR
	//PARAMETRO (argv[]) CONTIENE EL COMANDO Y LOS ARGUMENTOS INGRESADOS
	//PARAMETRO (fileName[]) ALMACENA EL NOMBRE DEL ARCHIVO DEL QUE SE LEE, O EN EL QUE SE ESCRIBE LA SALIDA 
	int i = 0;							//INICIALIZAMOS LA VARIABLE (i)
	for (i = 0; i < 20; i++)
	{
			
		if(argv[i] == NULL)
		{
			fileName = NULL;
			return 0;					//NO HAY QUE REDIRECCIONAR
		}
		else if (!strcmp(argv[i], "<"))
		{
			strcpy(fileName, argv[i+1]);
			argv[i] = NULL;		
			argv[i+1] = NULL;
			return 1;				    //HAY QUE REDIRECCIONAR LA ENTRADA
		}
		else if (!strcmp(argv[i], ">"))
		{
			strcpy(fileName, argv[i+1]);
			argv[i] = NULL;	
			argv[i+1] = NULL;
			return 2;					//HAY QUE REDIRECCIONAR LA SALIDA
		}
	}
	return 0;							//NO HAY QUE REDIRECCIONAR
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 8//////////////////////////////////////////////////*/
int verificaPipe(char *argv[],char *argv1[],char *argv2[])
{
	//VERIFICA SI DEBE REALIZAR UN PIPELINE
	//PARAMETRO (argv[]) CONTIENE EL COMANDO INGRESADO
	//PARAMETRO (argv1[]) GUARDAMOS LOS ARGUMENTOS DEL COMANDO 1
	//PARAMETRO (argv2[]) GUARDAMOS LOS ARGUMENTOS DEL COMANDO 2
	int indexArg, aux, indexArg2;
	
	for(indexArg = 0; argv[indexArg] != NULL; indexArg++)
	{
		aux = strcmp("|", argv[indexArg]);
		if(aux == 0)
		{
			break;
		}
		argv1[indexArg] = (char*) malloc ( strlen(argv[indexArg] + 1));
		strcpy(argv1[indexArg], argv[indexArg]);
	}
	argv1[indexArg] = '\0';
	
	if(argv[indexArg] == NULL)
		return 0;						//NO HAY QUE EJECUTAR PIPELINE
	
	indexArg++;
	
	for(indexArg2 = 0; argv[indexArg] != NULL; indexArg2++)
	{
		
		if(argv[indexArg] == NULL)
		{
			break;
		}
		argv2[indexArg2] = (char*) malloc ( strlen(argv[indexArg] + 1) ) ;
		strcpy(argv2[indexArg2], argv[indexArg]);
		indexArg++;
	}
	argv2[indexArg2] = '\0';
	return 1;							//HAY QUE EJECUTAR PIPELINE
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 9//////////////////////////////////////////////////*/
void ejecutaPipe(char* argv1[], char* argv2[], char* paths[])
{
	//EJECUTA EL PIPE
	//PARAMETRO (argv1[]) ARGUMENTO DEL COMANDO 1
	//PARAMETRO (argv2[]) ARGUMENTO DEL COMANDO 2
	//PARAMETRO (paths[]) DONDE PUEDE BUSCAR LOS ARCHIVOS
	char executePath[256];
	
	int fd[2];
	pipe(fd);
	if (fork()==0)   	//HIJO
	{
		close(fd[0]);
		dup2(fd[1],1); //REDIRECCION DE LA SALIDA AL PIPE
		close(fd[1]);
		buscaArchivo(argv1[0], paths, executePath);
		execv(executePath, argv1);
		perror(executePath);
		exit(1);
	} 
	else 				//PADRE
	{
		close(fd[1]);
		dup2(fd[0],0);
		close(fd[0]);
		buscaArchivo(argv2[0], paths, executePath);
		execv(executePath, argv2);
		perror(executePath);
		exit(1);
	}
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 10/////////////////////////////////////////////////*/
void salida(char fileName[])
{
	//MODIFICA LA SALIDA ESTANDAR
	FILE *fichero;
	char* filename = fileName;

	fichero = fopen(filename, "w");
	fclose(fichero);

	int fid;
	int flags,perm;
	flags = O_WRONLY|O_CREAT|O_TRUNC;
	perm = S_IWUSR|S_IRUSR;
	
	fid = open(fileName, flags, perm);	
	if (fid<0) {
		perror("open");
		exit(1);
	}
	close(STDOUT_FILENO);
	if (dup(fid)<0) {
		perror("dup");
		exit(1);
	}
	close(fid);
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 11/////////////////////////////////////////////////*/
void entrada(char fileName[])
{
	//MODIFICA LA ENTRADA ESTANDAR
	int fid;
	int flags,perm;
	flags = O_RDONLY;
	perm = S_IWUSR|S_IRUSR;
	
	close(STDIN_FILENO);
	fid = open(fileName, flags, perm);	
	if (fid<0) {
		perror("open");
		exit(1);
	}	
	if (dup(fid)<0) {
		perror("dup");
		exit(1);
	}
	close(fid);	
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/