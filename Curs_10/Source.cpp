#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <omp.h>
using namespace std;

int minVector(vector<int> valori) {
	int min = valori[0];
	for (int i = 0; i < valori.size(); i++) {
		if (min > valori[i]) {
			min = valori[i];
		}
	}
	return min;
}

//costul platit pentru mutex este prea mare
int minVectorParalelCritic(vector<int> valori) {
	int min = valori[0];

#pragma omp parallel for num_threads(8) shared(min)
	for (int i = 0; i < valori.size(); i++) {
#pragma omp critical
			if (min > valori[i]) {
				min = valori[i];
			}
	}
	return min;
}

int minVectorParalel(vector<int> valori, bool esteParalel) {
	int min = valori[0];
	int minGlobal = valori[0];

	cout << endl << "Min global " << minGlobal;

#pragma omp parallel if(esteParalel) num_threads(8) firstprivate(min) shared(minGlobal)
	{
#pragma omp for
		for (int i = 0; i < valori.size(); i++) {
			if (min > valori[i]) {
				min = valori[i];
			}
		}

#pragma omp critical 
		{
			cout << endl << "Minim local " << min;
			if (min < minGlobal) {
				minGlobal = min;
				cout << endl << "Min global nou " << minGlobal;
			}
		}
	}


	cout << endl << "Min global final " << minGlobal;

	//if (omp_in_parallel) {
	//	cout << endl << "Suntem inca in zona paralela";
	//}
	//else {
	//	cout << endl << "Am iesit din zona paralela";
	//}

	return minGlobal;
}

vector<int> generareVector(int nrElemente) {
	vector<int> valori;
	srand(1000);
	for (int i = 0; i < nrElemente; i++) {
		valori.push_back(rand());
	}
	return valori;
}

//suma cu overflow
int suma(vector<int> valori) {
	int s = 0;
#pragma omp parallel for num_threads(8) reduction(+: s) schedule(guided, 1000)
	for (int i = 0; i < valori.size(); i++)
		s += valori[i];

	return s;
}

int main() {
	cout << endl << "Start generare ....";
	vector<int> valori = generareVector(5*1e7);
	valori[0] = 1000001;

	for (int i = 0; i < 100; i++) {
		cout << endl << "Valoare random " << valori[i];
	}

	cout << endl << "Start minim";

	double tStart = omp_get_wtime();
	int minim = minVector(valori);
	double tFinal = omp_get_wtime();
	cout << endl << "Minim = " << minim << " in " << (tFinal - tStart) << " secunde";

	tStart = omp_get_wtime();
	minim = minVectorParalel(valori, true);
	tFinal = omp_get_wtime();
	cout << endl << "Minim = " << minim << " in " << (tFinal - tStart) << " secunde";

	tStart = omp_get_wtime();
	minim = minVectorParalel(valori, false);
	tFinal = omp_get_wtime();
	cout << endl << "Minim = " << minim << " in " << (tFinal - tStart) << " secunde";

}
