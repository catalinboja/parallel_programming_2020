#include <thread>
#include <stdio.h>
#include <chrono>
#include <omp.h>
#include <vector>

using namespace std;

vector<int> getFibonacci(int limita) {
	vector<int> valori;
	int a = 0;
	int b = 1;
	valori.push_back(a);
	valori.push_back(b);

	for (; b <= limita;) {
		int c = b;
		b = a + b;
		a = c;
		valori.push_back(b);
	}

	return valori;
}

void afisareMesaj(int id, int& rezultat) {
	rezultat = 0;
	for (int i = 0; i < 10; i++) {
		printf("\n Salut de la thread %d", id);
		rezultat += 1;
	}
}

int main() {

	std::chrono::system_clock::time_point tStart =
		std::chrono::system_clock::now();

	int rezultat1;
	int rezultat2;
	int rezultat3;

	double momentStart = omp_get_wtime();

	std::thread t1(afisareMesaj, 1, ref(rezultat1));
	std::thread t2(afisareMesaj, 2, ref(rezultat2));
	std::thread t3(afisareMesaj, 3, ref(rezultat3));

	t1.join();
	t2.join();
	t3.join();

	double momentFinal = omp_get_wtime();

	printf("\n Rezultat obtinut %d", rezultat1);

	std::chrono::system_clock::time_point tFinal =
		std::chrono::system_clock::now();
	long durata =
		std::chrono::duration_cast<std::chrono::milliseconds>(tFinal - tStart).count();

	printf("\n Final in %d ms", durata);
	printf("\n Final in %f s", momentFinal - momentStart);


	vector<int> serieFibo = getFibonacci(1000);
	printf("\n Seria Fibonacci este: ");

	for (int i = 0; i < serieFibo.size(); i++) {
		printf(" %d", serieFibo[i]);
	}

}
