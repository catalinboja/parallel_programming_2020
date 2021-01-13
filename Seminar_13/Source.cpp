#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;


//simulare Game of Life
//secvential si paralel


class Celula {
public:
	int i;
	int j;

	Celula(int i, int j) {
		this->i = i;
		this->j = j;
	}
};

void creareSimulare(vector<vector<int>>& simulare, int N) {
	for (int i = 0; i < N; i++) {
		vector<int> linie;
		for (int j = 0; j < N; j++) {
			linie.push_back(0);
		}
		simulare.push_back(linie);
	}
}

void initSimulare(vector<vector<int>>& simulare, int N, vector<Celula>& stareInitiala) {
	creareSimulare(simulare, N);

	//initializare stare initiala
	for (int k = 0; k < stareInitiala.size(); k++) {
		simulare[stareInitiala[k].i][stareInitiala[k].j] = 1;
	}
}

void stergeDate(vector<vector<int>>& simulare, int N) {
#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		simulare[i].clear();
	}

	simulare.clear();
}

//implementare paralela pentru a numara cate celule mai exista in simulare
int getNrCelule(vector<vector<int>>& simulare, int N) {
	int total = 0;
#pragma omp parallel for reduction(+: total) 
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (simulare[i][j] == 1) {
				total += 1;
			}
		}
	}
	return total;
}

int getNrVecini(vector<vector<int>>& simulare, int i, int j, int N) {
	int nrVecini = 0;
	for (int k = (i==0 ? i : i-1); k <= (i == N-1 ? i : i+1); k++) {
		for (int l = (j == 0 ? j : j - 1); l <= (j == N - 1 ? j : j + 1); l++) {
			if (simulare[k][l] == 1) {
				nrVecini += 1;
			}
		}
	}
	if (simulare[i][j] == 1) {
		nrVecini -= 1;
	}
	return nrVecini;
}

//metoda care trece simularea la generatia urmatoare
void generatiaUrmatoareSecvential(
	vector<vector<int>>& simulare, 
	vector<vector<int>>& simulareTemporara, 
	int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int nrVecini = getNrVecini(simulare, i, j, N);
			if (simulare[i][j] == 0 && nrVecini == 3) {
				simulareTemporara[i][j] = 1;	//s-a nascut o celula
			}
			else
				simulareTemporara[i][j] = 0;

			if (simulare[i][j] == 1) {
				if (nrVecini < 2 || nrVecini > 3) {
					simulareTemporara[i][j] = 0;		//celula a murit
				}
				else
				{
					simulareTemporara[i][j] = 1;		//celula trece la generatia urmatoare
				}
			}
		}
	}

	for(int i =0; i<N ; i++)
		for (int j = 0; j < N; j++) {
			simulare[i][j] = simulareTemporara[i][j];
		}
	
}

void generatiaUrmatoareParalela(
	vector<vector<int>>& simulare, 
	vector<vector<int>>& simulareTemporara,
	int N) {

#pragma omp parallel for schedule(dynamic,15)
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int nrVecini = getNrVecini(simulare, i, j, N);
			if (simulare[i][j] == 0 && nrVecini == 3) {
				simulareTemporara[i][j] = 1;	//s-a nascut o celula
			}
			else
				simulareTemporara[i][j] = 0;

			if (simulare[i][j] == 1) {
				if (nrVecini < 2 || nrVecini > 3) {
					simulareTemporara[i][j] = 0;		//celula a murit
				}
				else
				{
					simulareTemporara[i][j] = 1;		//celula trece la generatia urmatoare
				}
			}
		}
	}

#pragma omp parallel for
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			simulare[i][j] = simulareTemporara[i][j];
		}
}

int main() {

	int N = 1000;	//dimensiunea mediului - matrice de NxN

	vector<vector<int>> simulareTemporara;
	creareSimulare(simulareTemporara, N);

	vector<Celula> stareInitiala;
	//generare Blinker
	stareInitiala.push_back(Celula(2, 1));
	stareInitiala.push_back(Celula(2, 2));
	stareInitiala.push_back(Celula(2, 3));

	stareInitiala.push_back(Celula(50, 60));
	stareInitiala.push_back(Celula(50, 59));
	stareInitiala.push_back(Celula(60, 70));
	stareInitiala.push_back(Celula(61, 70));

	vector<vector<int>> simulare;
	initSimulare(simulare, N, stareInitiala);


	cout << endl << "Numar celule " << getNrCelule(simulare, N);
	cout << endl << "Start test ";

	int NR_ITERATII = 30;
	//test dupa 10^6 iteratii
	double tStart = omp_get_wtime();
	//for (int i = 0; i < NR_ITERATII; i++) {
	//	generatiaUrmatoareSecvential(simulare, simulareTemporara, N);
	//}
	double tFinal = omp_get_wtime();

	cout << endl << "Durata simulare secventiala " << (tFinal - tStart) << " secunde";
	cout << endl << "Numar celule " << getNrCelule(simulare, N);


	NR_ITERATII = 50;

	//zona paralela
	initSimulare(simulare, N, stareInitiala);
	cout << endl << "Numar celule " << getNrCelule(simulare, N);
	cout << endl << "Start test paralel";

	tStart = omp_get_wtime();
	for (int i = 0; i < NR_ITERATII; i++) {
		generatiaUrmatoareParalela(simulare, simulareTemporara, N);
	}
	tFinal = omp_get_wtime();

	cout << endl << "Durata simulare paralela " << (tFinal - tStart) << " secunde";
	cout << endl << "Numar celule " << getNrCelule(simulare, N);


}