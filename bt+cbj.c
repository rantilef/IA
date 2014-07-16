void BT(vis,host,periodo){
	int problema,aBarco,i,pos, encontrado=0, x;	
	
	if (periodo == 0) // primer periodo
	{
			//Si el visitante no ha sido asignado
		if (A[periodo][vis]==0){
			//asignar si el host tiene capacidad para el visitante, si no cambiar de host
			if (C[periodo][host]>=T[vis])
			{
		
				C[periodo][host] = C[periodo][host]-T[vis];
				A[periodo][vis]=host+1;
				asignados = asignados +1;

				// Si no se han asignado todos, 
				// seguir con el siguiente visitante
				if (asignados<numVisitas)
				{
					BT(vis+1,0,periodo);
				}
				else
				{
					asignados = 0;
					imprimir(periodo);
					periodo++;

				}
			}
			else{
				
				// si no ha probado en todos los hosts, pasa al siguiente host
				if (host < numHosts-1)
				{	
					BT(vis,host+1,periodo);
				}
				else{
					asignados--;
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
	else{
		if (A[periodo][vis]==0){
			//Si comprueba que no hay problema asignar
			if((x = Comprobar(vis,host+1,periodo))!=0){
				//sumar cantidades a la capacidad
				for (i = numVisitas; i > vis ; i--)
				{
					asignados--;
					aBarco= A[periodo][vis]-1;
					A[periodo][vis]=0;
					C[periodo][aBarco]= C[periodo][aBarco] + T[vis];
				}
				asignados--;
				aBarco= A[periodo][vis]-1;
				A[periodo][vis]=0;
				C[periodo][aBarco]= C[periodo][aBarco] + T[vis];
				BT(vis-1,aBarco+1,x);
			}
			else{
				if (C[periodo][host]>=T[vis])
				{
					C[periodo][host] = C[periodo][host]-T[vis];
					A[periodo][vis]=host+1;
					asignados = asignados +1;

					// Si no se han asignado todos, 
					// seguir con el siguiente visitante
					if (asignados<numVisitas)
					{
						BT(vis+1,0,periodo);
					}
					else
					{
						imprimir(periodo);
						asignados=0;
						periodo++;
					}
				}
				else{
				
					// si no ha probado en todos los hosts, pasa al siguiente host
					if (host < numHosts-1)
					{	
						BT(vis,host+1,periodo);
					}
					else{
						asignados--;
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
		if (A[periodo][vis] == host)
		{
			// Conflicto
			return i;
		}
	}
	return 0 ;
}