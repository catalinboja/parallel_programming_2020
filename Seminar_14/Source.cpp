#include <iostream>
#include "omp.h"

void testMasterThread(){
	printf("\n Parte secventiala");
	//....

	int idThread = omp_get_thread_num();
	int nrThreaduri = omp_get_num_threads();

	printf("\n Nr thread-uri = %d", nrThreaduri);
	printf("\n Id master thread = %d", idThread);

	//control la nivel de functie a numarului implicit de thread-uri 
	omp_set_num_threads(4);

	//regiune paralela - 2 thread-uri
#pragma omp parallel num_threads(2)
	printf("\n Parte paralela");
	printf("\n Nu este in zona paralela");
	

#pragma omp parallel 
	{
		printf("\n Parte paralela 2");
		int idThread = omp_get_thread_num();
		int nrThreaduri = omp_get_num_threads();

		printf("\n Nr thread-uri = %d", nrThreaduri);
		printf("\n Id master thread = %d", idThread);

		//se executa o singura data pe thread-ul principal
		if (idThread == 0) {
			printf("\n Salut de la thread-ul principal");
		}
	}
}

void testControlSecventaParalela(bool esteParalel) {

#pragma omp parallel if(esteParalel)
	{
		printf("Salut !!!");
	}
}

void testTipuriVariabile() {
	int global = 0;
	int local = 20;
	int local2 = 10;
#pragma omp parallel shared(global) private(local) firstprivate(local2)
	{
		printf("\n Valoare locala 2 = %d", local2);
		local2 += 100;
		local = 0;
		local += 1;
#pragma omp critical
		{
			global += 1;
		}
		int localaBlocului = 0;
	}

	printf("\n Valoare globala = %d", global);
	printf("\n Valoare locala = %d", local);
	printf("\n Valoare locala 2 = %d", local2);
}

void testOrchestrare() {

#pragma omp parallel
	{
		printf("\n Salut de la thread cu id  = %d", omp_get_thread_num());
		//alte prelucrari

#pragma omp barrier
		printf("\n Iesire zona paralela");
	}
}

void testForParalel() {

	omp_set_num_threads(4);

	int rezultat = 10;

#pragma omp parallel
	{
		
#pragma omp for nowait schedule(dynamic, 1) reduction(+: rezultat)
		for (int i = 0; i < 20; i++) {
			printf("\n Thread id %d proceseaza iteratia %d", omp_get_thread_num(), i);
			rezultat += 1;
		}

		printf("\n Zona dupa for");
	}

	printf("\n Valoare rezultat = %d", rezultat);

}

int main() {
	//testMasterThread();
	//testControlSecventaParalela(false);
	//testControlSecventaParalela(true);

	//testTipuriVariabile();

	//testOrchestrare();

	testForParalel();
}