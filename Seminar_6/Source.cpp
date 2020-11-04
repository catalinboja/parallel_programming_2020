#include <stdio.h>
#include <omp.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

const long N = 1e10;
const int NR_THREAD = 4;

mutex semafor;


//versiune secventiala calcul numar divizori pentru valoare primita
void calculSecventialNumarDivizori(long n, long& nrDivizori) {
	for (long valoare = 1; valoare <= n; valoare++) {
		if (valoare % 10 == 0) {
			nrDivizori += 1;
		}
	}
}

void calculSecventialNumarDivizoriInterval(long limitaInf, long limitaSup, long n, long& nrDivizori) {
	for (long valoare = limitaInf; valoare <= limitaSup; valoare++) {
		if (valoare % 10 == 0) {
			nrDivizori += 1;
		}
	}
}

void calculSecventialNumarDivizoriIntervalSync(long limitaInf, long limitaSup, long n, long& nrDivizori) {
	for (long valoare = limitaInf; valoare <= limitaSup; valoare++) {
		if (valoare % 10 == 0) {	
			semafor.lock();
			nrDivizori += 1;
			semafor.unlock();
		}

	}
}

//versiune paralela pe un numar de thread-uri dat prin NR_THREAD cu race condition
void calculParalelNumarDivizoriRaceCond(long n, long& nrDivizori) {
	long dimensiuneInterval = n / NR_THREAD;
	vector<thread> fireExecutie;
	for (int i = 0; i < NR_THREAD; i++) {
		long limitaInf = (i == 0) ? 1 : (i * dimensiuneInterval);
		long limitaSup = (i == NR_THREAD-1) ? n : (i + 1) * dimensiuneInterval - 1;
		fireExecutie.push_back(
			thread(calculSecventialNumarDivizoriInterval, 
				limitaInf, 
				limitaSup, 
				n, 
				ref(nrDivizori)));
	}
	for (int i = 0; i < NR_THREAD; i++) {
		fireExecutie[i].join();
	}
}

//versiune paralela pe un numar de thread-uri dat prin NR_THREAD cu mutex
void calculParalelNumarDivizoriMutex(long n, long& nrDivizori) {
	long rezultat = 0;
	long dimensiuneInterval = n / NR_THREAD;
	vector<thread> fireExecutie;
	for (int i = 0; i < NR_THREAD; i++) {
		long limitaInf = (i == 0) ? 1 : (i * dimensiuneInterval);
		long limitaSup = (i == NR_THREAD - 1) ? n : (i + 1) * dimensiuneInterval - 1;
		fireExecutie.push_back(
			thread(calculSecventialNumarDivizoriIntervalSync,
				limitaInf,
				limitaSup,
				n,
				ref(rezultat)));
	}
	for (int i = 0; i < NR_THREAD; i++) {
		fireExecutie[i].join();
	}
	nrDivizori = rezultat;
}

//versiune paralela pe un numar de thread-uri dat prin NR_THREAD cu rezultate partiale
void calculParalelNumarDivizoriRezultatePartiale(long n, long& nrDivizori) {
	long dimensiuneInterval = n / NR_THREAD;
	long rezultate[NR_THREAD];
	for (int i = 0; i < NR_THREAD; i++)
		rezultate[i] = 0;
	vector<thread> fireExecutie;
	for (int i = 0; i < NR_THREAD; i++) {
		long limitaInf = (i == 0) ? 1 : (i * dimensiuneInterval);
		long limitaSup = (i == NR_THREAD - 1) ? n : (i + 1) * dimensiuneInterval - 1;
		fireExecutie.push_back(
			thread(calculSecventialNumarDivizoriInterval,
				limitaInf,
				limitaSup,
				n,
				ref(rezultate[i])));
	}
	for (int i = 0; i < NR_THREAD; i++) {
		fireExecutie[i].join();
	}
	for (int i = 0; i < NR_THREAD; i++) {
		nrDivizori += rezultate[i];
	}
}

//functie benchmarking
void benchmark(void (*pf)(long, long&), long n, string info) {
	printf("\n Test pentru %s ", info.c_str());
	
	double tStart = omp_get_wtime();
	long rezultat = 0;
	pf(n, rezultat);
	double tFinal = omp_get_wtime();

	printf("\n Rezultat = %d", rezultat);
	printf("\n Durata = %f secunde", (tFinal - tStart));
}

int main() {
	benchmark(calculSecventialNumarDivizori, N, "Test secvential ");
	benchmark(calculParalelNumarDivizoriRaceCond, N, "Test paralel cu race condition ");
	benchmark(calculParalelNumarDivizoriMutex, N, "Test paralel cu mutex ");
	benchmark(calculParalelNumarDivizoriRezultatePartiale, N, "Test paralel cu rezultate partiale ");
}