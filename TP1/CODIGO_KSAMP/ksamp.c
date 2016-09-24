/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////IMPORTACION DE LIBRERIAS////////////////////////////////////////////*/
#include "ParteB.h"
#include "ParteC.h"
#include "ParteD.h"
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////PROTOTIPOS///////////////////////////////////////////////////*/
void MostrarInfoAutor();
void OpcionIncorrecta();
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////FUNCION PRINCIPAL//////////////////////////////////////////////*/
int main(int argc, char *argv[])
{
	int a,b;

	if(argc == 1)
	{
		MostrarInfoAutor();
		MostrarParteB();
		return 0;
	}
	if(argc==2 && argv[1][0]=='-' && argv[1][1]=='s')
	{
		MostrarInfoAutor();
		MostrarParteB();
		MostrarParteC();
		return 0;
	}
	if(argc==4 && argv[1][0]=='-' && argv[1][1]=='l')
	{
		a = atoi(argv[2]);
		b = atoi(argv[3]);
		MostrarInfoAutor();
		MostrarParteB();
		MostrarParteC();
		MostrarParteD(a,b);
		return 0;
	}
	else
	{
		OpcionIncorrecta();
		return -1;
	}
	
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////IMPLEMENTACION DE FUNCIONES////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 1//////////////////////////////////////////////////*/
void MostrarInfoAutor()
{
	printf("\n///////////////////SISTEMA OPERATIVO 1//////////////////////\n");
	printf("TRABAJO PRACTICO N°1: KSAMP \n");
	printf("ALUMNO: SOSA LUDUEÑA DIEGO MATIAS \n");
	printf("ALUMNO: CHOQUEVILCA ZOTAR GUSTAVO BRIAN \n");
	printf("////////////////////////////////////////////////////////////\n");
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////FUNCION 2//////////////////////////////////////////////////*/
void OpcionIncorrecta()
{
	printf("\n////////////////////////////////////////////////////////////\n");
	printf("OPCION INGRESADA INCORRECTA. VUELVA A INTENTARLO \n");
	printf("////////////////////////////////////////////////////////////\n");
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////*/
