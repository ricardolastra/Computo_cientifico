#include<stdio.h>
#include<mpi.h>
#include<math.h>
//compilamos:
//mpicc trapecio_compuesto_mpi.c -o trapecio_compuesto_mpi.out -lm
//ejecutamos de manera local:
//mpiexec -n 10 trapecio_compuesto_mpi.out
//ejecutamos de manera pseudodistribuida:
//mpirun --prefix /opt/openmpi-2.0.2/ -n 2 -H master,nodo1 trapecio_compuesto_mpi.out
//prototipo de Trap:
double Trap(double ext_izq, double ext_der, int num_trap, double longitud_base);
int main(void){
	int mi_rango;
	int comm_sz;
	int n=1e6;//número de trapecios
	int local_n;
	double a=-1.0, b=3.0,h;
	double local_a, local_b,local_int, total_int;
	//local_int estima por cada proceso con la regla del trapecio
	//la integral
	int num_proceso_contador;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);
	h=(b-a)/n; //el valor de h es el mismo para todos los procesos
	local_n = n/comm_sz;//número de subintervalos para cada proceso
	local_a = a + mi_rango*local_n*h;//calculamos el extremo izquierdo
	local_b = local_a + local_n*h;//calculamos el extremo derecho
	local_int = Trap(local_a, local_b, local_n, h);
	if(mi_rango!= 0)
	 MPI_Send(&local_int, 1, MPI_DOUBLE, 0,0,MPI_COMM_WORLD);
	else{
		total_int = local_int;
		for(num_proceso_contador=1;num_proceso_contador<comm_sz;num_proceso_contador++){
			MPI_Recv(&local_int, 1, MPI_DOUBLE, num_proceso_contador, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			total_int+=local_int;
		}//for
	}//else
	if(mi_rango == 0)
		printf("Valor de la integral de %f a %f es: %1.14e\n",a,b,total_int);
	MPI_Finalize();
	return 0;
}//main
//prototipo de f:
double f(double nodo);
double Trap(double ext_izq, double ext_der, int num_trap, double longitud_base){
	double estimacion;
	double x;
	int contador;
	estimacion = (f(ext_izq) + f(ext_der))/2.0;
	for(contador=1;contador<num_trap-1;contador++){
		x =ext_izq + contador*longitud_base;
		estimacion += f(x);
	}
	estimacion=estimacion*longitud_base;
	return estimacion;
}
//la regla de correspondencia f: integrando
double f(double nodo){
	double valor_f;
	valor_f = exp(nodo);
	return valor_f;
}


