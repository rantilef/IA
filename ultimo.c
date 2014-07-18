#include <stdio.h>

#define numBotes 42
#define numPeriodos 8
#define numHosts 13
#define numVisitas 29

//int asignados = 0; 											
int asignados[numPeriodos];												// Cantidad de botes asignados en periodo
int C[numPeriodos][numHosts],T[numVisitas],A[numPeriodos][numVisitas]; 	// Arreglo de capacidades de los botes host, tripulaciones de los visitantes y asignaciones de estos 
int matrizResultado[numPeriodos][numBotes]; 							// Matriz con los resultados de la asignación de barcos en cada periodo
int hosts[numHosts][2], visitas[numVisitas][2]; 						// Arreglo que guarda el numero del bote host y su capacidad real ( Capacidad - tripulacion)
																		// Arreglo que guarda el numero del bote visitante y su tripulación
int iteracion=0;


void ordenar(int matriz[][2],int n); 			//funcion para ordenar botes por capacidad y/o tripulación
int BT(int vis,int host, int periodo);			// Función que realiza el Backtracking + cbj
int comprobar();								// Función que comprueba la asignación con las restricciones
void imprimir();								// Función para imprimir en la matriz resultado las asignaciones del periodo
void mostrarMatriz();							// Función para mostrar por pantalla las asignaciones

int main(int argc, char *argv[]){
	int linea[numBotes+1][2]; 			// Arreglo para guardar la información de capacidad y tripulación de cada bote
	int botes[numBotes+1];				// Arreglo que guarda si un bote es host o no
	
	int i=0, j=0, v=0,h=0, capacidad=0, demanda=0; // capacidad va guardando la capacidad total efectiva de los botes host, demanda hace lo mismo con su demanda

	FILE *filecapTrip;
	FILE *fileHost;

	//Si no se ingresa archivo de entrada
	if (argc!=3)
	{
		printf("Se deben ingresar mas parametros (archivos de entrada)\n");
	}

	// Abrir archivo de capacidades y tripulaciones de barcos.
	filecapTrip = fopen(argv[1],"r");
	fileHost = fopen(argv[2],"r");
	// cargar capacidades y trip
	do{
		fscanf(filecapTrip,"%d", &linea[i][0]);
		fscanf(filecapTrip,"%d", &linea[i][1]);
		i++;
	} while(!feof(filecapTrip));

	// cargar clasificación de botes	
	i=0;
	do{
		fscanf(fileHost,"%d", &botes[i]);
		i++;
	} while(!feof(fileHost));

	//calculo capacidad real botes hosts y tripulacion (else) de botes visita
	for (i = 0; i < numBotes; i++)
	{
		if(botes[i]==1){
			hosts[h][0] = i+1;
			hosts[h][1]= linea[i][0]-linea[i][1];
			capacidad = capacidad + hosts[h][1];
			h++;
		}
		else{
			visitas[v][0]= i+1;
			visitas[v][1]=linea[i][1];
			demanda = demanda + visitas[v][1];
			v++;
		}
	}

	// Si la demanda supera la capacidad se determina inmediatamente que no hay solución
	if (capacidad < demanda)
	{
		printf("La demanda supera la capacidad. No hay solución\n");
		return 0;
	}

/*******************************
	// Heurística de ordenamiento
********************************/
	ordenar(hosts,numHosts);
	ordenar(visitas,numVisitas);

	// se llena el arreglo con los datos de capacidades y tripulaciones de cada bote en el orden de mayor a menor
	for (j = 0; j < numPeriodos; j++)
	{
		for (i = 0; i < numHosts; i++)
		{
			C[j][i]=hosts[i][1];
		}
	}
	for (j=0; j < numPeriodos; j++)
	{
		for (i = 0; i < numVisitas; i++)
		{
			T[i]=visitas[i][1];
			A[j][i]=0;
		}
	}
	
/****************************+
***********************
*************+
			INICIO BT + CBJ
***************
********************+
**********************/
/*	for (i = 0; i < 2; i++)
	{
			printf("probando periodo %d\n", i);
			BT(0,0,i);	
	}
*/ 

	while(iteracion<7){
		iteracion = BT(0,0,iteracion);
	}
		

	mostrarMatriz();
//Cierre archivos
	fclose(filecapTrip);
	fclose(fileHost);

	
return 0;
}


// Función recibe la matriz a ordenar y el número de datos (botes) que tiene 
void ordenar(int matriz[][2],int n){
	int i,j,bote,aux;
	for ( i = 0; i < n; i++)
	{
		for ( j = 0; j < n-1; j++)
		{
			if (matriz[j][1]<matriz[j+1][1])
			{
				aux = matriz[j][1];
				bote= matriz[j][0];
				matriz[j][1]=matriz[j+1][1];
				matriz[j][0]=matriz[j+1][0];
				matriz[j+1][1]=aux;
				matriz[j+1][0]=bote;
			}
		}
	}
	return;
}


int BT(vis,host,periodo){
	int problema,aBarco,i,pos, encontrado=0, x;	
	
	// primer periodo
	if (periodo == 0){
			//Si el visitante no ha sido asignado
		if (A[periodo][vis]==0){
			//asignar si el host tiene capacidad para el visitante, si no cambiar de host
			if (C[periodo][host]>=T[vis])
			{
				printf("%d asignado en bote %d\n",vis ,host+1);
				C[periodo][host] = C[periodo][host]-T[vis];
				A[periodo][vis]=host+1;
				asignados[periodo] = asignados[periodo] +1;

				// Si no se han asignado todos, 
				// seguir con el siguiente visitante
				if (asignados[periodo]<numVisitas)
				{
					BT(vis+1,0,periodo);
				}
				else
				{ //si se asignaron todos pasar al siguiente periodo
					imprimir(periodo);
					iteracion=periodo+1;
					return iteracion;
				}
			}
			else{
				
				// si no ha probado en todos los hosts, pasa al siguiente host
				if (host < numHosts-1) ///////////////////////////////////////////////////////////
				{	
					BT(vis,host+1,periodo);
				}
				else{
					//si se acabaron los host, saltar a la instanciacion anterior. Volver a enterar capacidades
					asignados[periodo]--;
					aBarco= A[periodo][vis-1]-1;
					A[periodo][vis-1]=0;
					C[periodo][aBarco]= C[periodo][aBarco] + T[vis-1];
					BT(vis-1,aBarco+1,periodo);
				}
				
			}
		}
		//si está asignado
		else{
			BT(vis+1,host,periodo);
		}
	}


////////////////////// otros periodos

	else{
		// comprueba si el visitante ya fue asignado
		if (A[periodo][vis]==0){
			// comprueba si ya estuvo en el host
			printf("comprobar vis= %d    host= %d en periodo %d\n", vis,host+1,periodo);
			x = Comprobar(vis,host+1,periodo);
			printf("%d\n", x);
			if(x != 66){
				// ya estuve en el host pero quedan hosts por instanciar
				if (host < numHosts)
				{
					printf("paso al siguiente host, vis= %d, host= %d, per= %d\n",vis,host+1,periodo );
					BT(vis,host+1,periodo);
				}
				// si no puedo pasar a un siguiente host tengo que saltar al tiempo problema
				else{
					//sumar tripulaciones a las capacidades respectivas
					for (i = numVisitas-1; i > vis ; i--)
					{
						asignados[x]--;
						aBarco= A[x][i]-1;
						A[x][i]=0;
						C[x][aBarco]= C[x][aBarco] + T[i];
					}
					asignados[x]--;
					aBarco= A[x][vis]-1;
					A[x][vis]=0;
					C[x][aBarco]= C[x][aBarco] + T[vis];

					printf("salto al vis= %d, host= %d, periodo= %d \n\n\n", vis, aBarco+1,x);
					BT(vis,aBarco+1,x);
				}
			}
			else{
				if (C[periodo][host]>=T[vis])
				{
					printf("trato de asignar\n");
					C[periodo][host] = C[periodo][host]-T[vis];
					A[periodo][vis]=host+1;
					asignados[periodo] = asignados[periodo] +1;

					// Si no se han asignado todos, 
					// seguir con el siguiente visitante
					if (asignados[periodo]<numVisitas)
					{
						printf("asigno\n");
						BT(vis+1,0,periodo);
					}
					else
					{

						imprimir(periodo);
						iteracion=periodo+1;
						return;
					}
				}
				else{
				
					// si no ha probado en todos los hosts, pasa al siguiente host
					if (host < numHosts-1
						)
					{	
						BT(vis,host+1,periodo);
					}
					else{
						asignados[periodo]--;
						aBarco= A[periodo][vis-1]-1;
						A[periodo][vis-1]=0;
						C[periodo][aBarco]= C[periodo][aBarco] + T[vis-1];
						BT(vis-1,aBarco+1,periodo);
					}
				
				}
			}
		}

		//si está asignado
		else{
			BT(vis+1,host,periodo);
		}	
	}
}

int Comprobar(vis,host,periodo){
	int i;
	for (i = 0; i < periodo; i++)
	{
		//si el visitante ya estuvo en algun periodo en ese barco llenar matriz de conflictos
		if (A[i][vis] == host)
		{

			// devolver periodo en conflicto
			return i;
		}
	}
	return 66 ;
}
void imprimir(periodo){
	int i, boteHost, boteVisitante, destino;
	for (i = 0; i < numVisitas; i++)
	{
		boteVisitante = visitas[i][0];
		boteHost = A[periodo][i];
		destino = hosts[boteHost][0];
		printf("\n PERIODO %d             el barco %d está en barco %d\n",periodo,boteVisitante,destino);
		matrizResultado[periodo][boteVisitante-1]=destino;

	}
}

void mostrarMatriz(){
	int i,j;
	for (j = 0; j < numPeriodos; j++)
	{
		printf("\nBotes en periodo ", j);
	
		for (i = 0; i < numBotes; i++)
		{
			printf("|%d|",matrizResultado[j][i] );
		}
	}
}