#include <omp.h>
#include <stdio.h>


void hello(int idThread, const char* mesaj) {
	printf("\n %d - %s", idThread, mesaj);
}

int main() {
	int nrProcesoare = omp_get_num_procs();
	printf("\n Numarul de core-uri este %d", nrProcesoare);
	
	omp_set_num_threads(4);

#pragma omp parallel
	{

		int idThread = omp_get_thread_num();
		hello(idThread, "hello");

		if (idThread == 0) {
			int nrThreaduri = omp_get_num_threads();
			printf("\n Am obtinut %d thread-uri", nrThreaduri);
		}
	}
}