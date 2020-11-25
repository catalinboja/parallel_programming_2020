#include <iostream>
#include <string>
#include <omp.h>
using namespace std;

bool estePrim(long valoare) {
	bool estePrim = true;
	for (long i = 2; i < valoare; i++) {
		if (valoare % i == 0)
		{
			estePrim = false;
			break;
		}
	}
	return estePrim;
}

void determinaNumarPrime(long start, long final, int& rezultat) {
	for (long i = start; i < final; i++) {
		if (estePrim(i))
			rezultat += 1;
	}
}

//nu este ok deoarece for-ul se replica pe toate thread-urile
void determinaNumarPrimeParalel_1(long start, long final, int& rezultat) {
	printf("\n Solutie paralele pe %d thread-uri", omp_get_num_procs());
#pragma omp parallel num_threads(omp_get_num_procs())
	{
		for (long i = start; i < final; i++) {
			if (estePrim(i))
				rezultat += 1;
		}
	}
}


//mai bine insa avem un race condition pe rezultat
void determinaNumarPrimeParalel_2(long start, long final, int& rezultat) {

	printf("\n Solutie paralele pe %d thread-uri", omp_get_num_procs());
#pragma omp parallel num_threads(omp_get_num_procs()) shared(rezultat)
	{
		int nrThreaduri = omp_get_num_threads();
		long dimensiuneInterval = (final - start) / nrThreaduri;

		int idThread = omp_get_thread_num();
		long startLocal = (idThread * dimensiuneInterval) + start;
		long finalLocal = (idThread + 1) * dimensiuneInterval + start;
		if (idThread == nrThreaduri - 1) {
			finalLocal = final;
		}

		for (long i = startLocal; i < finalLocal; i++) {
			if (estePrim(i))
				rezultat += 1;
		}
	}
}

void determinaNumarPrimeParalel_3(long start, long final, int& rezultat) {

	omp_lock_t* mutex = new omp_lock_t();
	omp_init_lock(mutex);

	printf("\n Solutie paralele pe %d thread-uri", omp_get_num_procs());
#pragma omp parallel num_threads(omp_get_num_procs())
	{
		int nrThreaduri = omp_get_num_threads();
		long dimensiuneInterval = (final - start) / nrThreaduri;

		int idThread = omp_get_thread_num();
		long startLocal = (idThread * dimensiuneInterval) + start;
		long finalLocal = (idThread + 1) * dimensiuneInterval + start;
		if (idThread == nrThreaduri - 1) {
			finalLocal = final;
		}

		for (long i = startLocal; i < finalLocal; i++) {
			if (estePrim(i)) {
				omp_set_lock(mutex);
				rezultat += 1;
				omp_unset_lock(mutex);
			}
		}
	}

	omp_destroy_lock(mutex);
}


//varianta fara race condition si fara lock
void determinaNumarPrimeParalel_4(long start, long final, int& rezultat) {

	printf("\n Solutie paralele pe %d thread-uri", omp_get_num_procs());

	int nrMaxThreaduri = omp_get_num_procs();
	int* rezultate = new int[nrMaxThreaduri];
	for (int i = 0; i < nrMaxThreaduri; i++) {
		rezultate[i] = 0;
	}

#pragma omp parallel num_threads(omp_get_num_procs())
	{
		int nrThreaduri = omp_get_num_threads();
		long dimensiuneInterval = (final - start) / nrThreaduri;

		int idThread = omp_get_thread_num();
		long startLocal = (idThread * dimensiuneInterval) + start;
		long finalLocal = (idThread + 1) * dimensiuneInterval + start;
		if (idThread == nrThreaduri - 1) {
			finalLocal = final;
		}

		for (long i = startLocal; i < finalLocal; i++) {
			if (estePrim(i)) {
				rezultate[idThread] += 1;
			}
		}
	}

	for (int i = 0; i < nrMaxThreaduri; i++) {
		rezultat += rezultate[i];
	}

}

//varianta fara race condition si fara vector
void determinaNumarPrimeParalel_5(long start, long final, int& rezultat) {
	omp_lock_t* mutex = new omp_lock_t();
	omp_init_lock(mutex);

	printf("\n Solutie paralele pe %d thread-uri", omp_get_num_procs());
	int rezultatLocal;

#pragma omp parallel num_threads(omp_get_num_procs()) \
	shared(rezultat) firstprivate(rezultatLocal)
	{
		rezultatLocal = 0;
		int nrThreaduri = omp_get_num_threads();
		long dimensiuneInterval = (final - start) / nrThreaduri;

		int idThread = omp_get_thread_num();
		long startLocal = (idThread * dimensiuneInterval) + start;
		long finalLocal = (idThread + 1) * dimensiuneInterval + start;
		if (idThread == nrThreaduri - 1) {
			finalLocal = final;
		}

		for (long i = startLocal; i < finalLocal; i++) {
			if (estePrim(i)) {
				rezultatLocal += 1;
			}
		}
		//atentie la race condition 
		omp_set_lock(mutex);
		rezultat += rezultatLocal;
		omp_unset_lock(mutex);
	}
	omp_destroy_lock(mutex);
}

//varianta fara race condition si fara vector si fara mutex explicit
void determinaNumarPrimeParalel_6(long start, long final, int& rezultat) {
	
	printf("\n Solutie paralele pe %d thread-uri", omp_get_num_procs());
	int rezultatLocal = 0;

#pragma omp parallel num_threads(omp_get_num_procs()) \
	shared(rezultat) firstprivate(rezultatLocal)
	{
		int nrThreaduri = omp_get_num_threads();
		long dimensiuneInterval = (final - start) / nrThreaduri;

		int idThread = omp_get_thread_num();
		long startLocal = (idThread * dimensiuneInterval) + start;
		long finalLocal = (idThread + 1) * dimensiuneInterval + start;
		if (idThread == nrThreaduri - 1) {
			finalLocal = final;
		}

		for (long i = startLocal; i < finalLocal; i++) {
			if (estePrim(i)) {
				rezultatLocal += 1;
			}
		}
		//atentie la race condition 
#pragma omp atomic
		rezultat += rezultatLocal;
	}
}


//varianta cu omp for
void determinaNumarPrimeParalel_7(long start, long final, int& rezultat) {

	printf("\n Solutie paralele pe %d thread-uri", omp_get_num_procs());
	int rezultatLocal = 0;

#pragma omp parallel num_threads(omp_get_num_procs()) \
	shared(rezultat) firstprivate(rezultatLocal)
	{
#pragma omp for
		for (long i = start; i < final; i++) {
			if (estePrim(i)) {
				rezultatLocal += 1;
			}
		}
		//atentie la race condition 
#pragma omp atomic
		rezultat += rezultatLocal;
	}
}

//varianta cu omp for
void determinaNumarPrimeParalel_8(long start, long final, int& rezultat) {
	
	printf("\n Solutie paralele pe %d thread-uri", omp_get_num_procs());

#pragma omp parallel for num_threads(omp_get_num_procs()) shared(rezultat) 
		for (long i = start; i < final; i++) {
			if (estePrim(i)) {
#pragma omp critical
				rezultat += 1;
			}
		}
}

//varianta cu omp for cu agregare
void determinaNumarPrimeParalel_9(long start, long final, int& rezultat) {

	int rezultatLocal = 0;
#pragma omp parallel for num_threads(omp_get_num_procs()) reduction(+: rezultatLocal)
	for (long i = start; i < final; i++) {
		if (estePrim(i)) {
			rezultatLocal += 1;
		}
	}

	rezultat = rezultatLocal;
}

void benchmark(void(*pf)(long, long, int&), long start, long final, string mesaj) {
	printf("\n Test pentru %s ", mesaj.c_str());

	int rezultat = 0;
	double tStart = omp_get_wtime();
	pf(start, final, rezultat);
	double tFinal = omp_get_wtime();

	printf("Rezultat = %d obtinut in %f secunde", rezultat, (tFinal - tStart));
}

int main() {

	printf("\n Numar core-uri existente %d", omp_get_num_procs());
	printf("\n Numar maxim thread-uri %d", omp_get_max_threads());
	if (omp_in_parallel()) {
		printf("\n Rulam in secventa paralela");
	}
	else {
		printf("\n Rulam in sectiune secventiala");
	}

	//solicitare numar thread-uri pentru secventele paralele
	printf("\n Solicitare 16 thread-uri pentru secventele paralele");
	omp_set_num_threads(16);
	printf("\n Numar core-uri existente %d", omp_get_num_procs());
	printf("\n Numar maxim thread-uri %d", omp_get_max_threads());

#pragma omp parallel num_threads(4)
	printf("\n Hello !");
	printf("\n from OMP !");


#pragma omp \
	parallel 
	{
		if (omp_get_thread_num() == 0) {
			if (omp_in_parallel()) {
				printf("\n Rulam in secventa paralela");
			}
			else {
				printf("\n Rulam in sectiune secventiala");
			}
				printf("\n Numar total thread-uri obtinute = %d", omp_get_num_threads());
		}

		printf("\n ID thread curent = %d", omp_get_thread_num());
	}


	//test metode
	long start = 1;
	long final = 2 * 1e5;
	benchmark(determinaNumarPrime, start, final, "Solutie secventiala");
	//benchmark(determinaNumarPrimeParalel_1, start, final, "Solutie paralela 1");
	benchmark(determinaNumarPrimeParalel_2, start, final, "Solutie paralela 2");
	benchmark(determinaNumarPrimeParalel_3, start, final, "Solutie paralela 3 cu omp lock");
	benchmark(determinaNumarPrimeParalel_4, start, final, "Solutie paralela 4 cu vector");
	benchmark(determinaNumarPrimeParalel_5, start, final, "Solutie paralela 5 cu variabile private");
	benchmark(determinaNumarPrimeParalel_6, start, final, "Solutie paralela 6 cu atomic");
	benchmark(determinaNumarPrimeParalel_7, start, final, "Solutie paralela 7 omp for");
	benchmark(determinaNumarPrimeParalel_8, start, final, "Solutie paralela 8 omp for");
	benchmark(determinaNumarPrimeParalel_9, start, final, "Solutie paralela 9 omp for cu agregare");
}
