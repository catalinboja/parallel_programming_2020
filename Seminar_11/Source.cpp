#include <omp.h>
#include <iostream>
#include <vector>
using namespace std;

void testSectiuniOmp() {

	omp_set_num_threads(4);

#pragma omp parallel
	{
		printf("\n Hello de la thread-ul %d", omp_get_thread_num());

#pragma omp master 
		{
			printf("\n Hello de la master thread  - id %d", omp_get_thread_num());
		}

#pragma omp single 
		{
			printf("\n *single* Hello de la thread-ul cu id %d", omp_get_thread_num());
		}
#pragma omp sections 
		{
#pragma omp section
			{
				printf("\n *section 1* Hello de la thread - ul cu id % d", omp_get_thread_num());
			}

#pragma omp section
			{
				printf("\n *section 2* Hello de la thread - ul cu id % d", omp_get_thread_num());
			}

#pragma omp section
			{
				printf("\n *section 3* Hello de la thread - ul cu id % d", omp_get_thread_num());
			}

#pragma omp section
			{
				printf("\n *section 4* Hello de la thread - ul cu id % d", omp_get_thread_num());
			}

#pragma omp section
			{
				printf("\n *section 5* Hello de la thread - ul cu id % d", omp_get_thread_num());
			}
		}

		printf("\n Bye");
	}
}

vector<int> generareVector(long N) {
	vector<int> valori;
	srand(1000);
	for (int i = 0; i < N; i++) {
		valori.push_back(rand());
	}
	return valori;
}

void afisareVector(vector<int> v, int n) {
	printf("\n Primele %d valori din vector sunt ", n);
	for (int i = 0; i < n; i++) {
		printf(" %d", v[i]);
	}
}

vector<int> sortareCrescatoareBule(vector<int> v) {
	bool esteSortat = false;
	while (!esteSortat) {
		esteSortat = true;
		for (int i = 0; i < v.size() - 1; i++) {
			if (v[i] > v[i + 1]) {
				int temp = v[i];
				v[i] = v[i + 1];
				v[i + 1] = temp;
				esteSortat = false;
			}
		}
	}
	return v;
}

vector<int> sortareCrescatoareBuleOptimizata(vector<int> v) {
	bool esteSortat = false;
	int n = v.size() - 1;

	while (!esteSortat) {
		esteSortat = true;
		int n_nou = 0;
		for (int i = 0; i < n; i++) {
			if (v[i] > v[i + 1]) {
				int temp = v[i];
				v[i] = v[i + 1];
				v[i + 1] = temp;
				esteSortat = false;
				n_nou = i;
			}
		}
		n = n_nou;
	}
	return v;
}

vector<int> sortareCrescatoareParImpar(vector<int> v) {

	for (int i = 0; i < v.size(); i++) {
		if (i % 2 == 0) {
			for (int j = 1; j < v.size(); j += 2) {
				if (v[j-1] > v[j]) {
					int temp = v[j];
					v[j] = v[j - 1];
					v[j- 1] = temp;
				}
			}
		}
		else {
			for (int j = 2; j < v.size(); j += 2) {
				if (v[j - 1] > v[j]) {
					int temp = v[j];
					v[j] = v[j - 1];
					v[j - 1] = temp;
				}
			}
		}
	}
	return v;
}

vector<int> sortareCrescatoareParalela(vector<int> v) {

	int nrMaxThreaduri = omp_get_num_procs();
	omp_set_num_threads(nrMaxThreaduri);

	for (int i = 0; i < v.size(); i++) {
		if (i % 2 == 0) {
#pragma omp parallel for shared(v)
			for (int j = 1; j < v.size(); j += 2) {
				if (v[j - 1] > v[j]) {
					int temp = v[j];
					v[j] = v[j - 1];
					v[j - 1] = temp;
				}
			}
		}
		else {
#pragma omp parallel for shared(v)
			for (int j = 2; j < v.size(); j += 2) {
				if (v[j - 1] > v[j]) {
					int temp = v[j];
					v[j] = v[j - 1];
					v[j - 1] = temp;
				}
			}
		}
	}
	return v;
}


// idea from https://www.geeksforgeeks.org/merge-sort/
// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(vector<int>& arr, int l, int m, int r)
{
	int n1 = m - l + 1;
	int n2 = r - m;

	// Create temp arrays
	vector<int> L(n1);
	vector<int> R(n2);

	// Copy data to temp arrays L[] and R[]
	for (int i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (int j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	// Merge the temp arrays back into arr[l..r]

	// Initial index of first subarray
	int i = 0;

	// Initial index of second subarray
	int j = 0;

	// Initial index of merged subarray
	int k = l;

	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements of
	// L[], if there are any
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	// Copy the remaining elements of
	// R[], if there are any
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

// l is for left index and r is
// right index of the sub-array
// of arr to be sorted */
void mergeSort(vector<int>& arr, int l, int r) {
	if (l >= r) {
		return;//returns recursively
	}
	int m = (l + r - 1) / 2;
	mergeSort(arr, l, m);
	mergeSort(arr, m + 1, r);
	merge(arr, l, m, r);
}



int main() {
	//testSectiuniOmp();
	printf("\n Start. Generare vector ...");
	vector<int> valori = generareVector(1e4);
	afisareVector(valori, 20);

	double tStart = omp_get_wtime();
	//test sortare prin metoda bulelor
	vector<int> vsortat = sortareCrescatoareBule(valori);
	double tFinal = omp_get_wtime();
	printf("\n Vectorul a fost sortat prin metoda bulelor in %f sec", (tFinal - tStart));


	tStart = omp_get_wtime();
	//test sortare prin metoda bulelor
	vector<int> vsortat2 = sortareCrescatoareBuleOptimizata(valori);
	tFinal = omp_get_wtime();
	printf("\n Vectorul a fost sortat prin metoda bulelor optimizata in %f sec", (tFinal - tStart));


	tStart = omp_get_wtime();
	//test sortare prin metoda bulelor
	vector<int> vsortat3 = sortareCrescatoareParImpar(valori);
	tFinal = omp_get_wtime();
	printf("\n Vectorul a fost sortat prin metoda par-impar in %f sec", (tFinal - tStart));

	tStart = omp_get_wtime();
	//test sortare prin metoda bulelor
	vector<int> vsortat4 = sortareCrescatoareParalela(valori);
	tFinal = omp_get_wtime();
	printf("\n Vectorul a fost sortat prin metoda par-impar paralelal in %f sec", (tFinal - tStart));

	//afisareVector(valori, 100);

	tStart = omp_get_wtime();
	//test sortare prin metoda bulelor
	mergeSort(valori, 0, valori.size() - 1);
	tFinal = omp_get_wtime();
	printf("\n Vectorul a fost sortat prin metoda merge sort in %f sec", (tFinal - tStart));

	//afisareVector(valori, 100);

	//verificare vectori
	for (int i = 0; i < valori.size(); i++) {
		if (vsortat4[i] != valori[i]) {
			printf("\n Rezultatele nu sunt identice pe index %d!!!!!", i);
			break;
		}
	}


	//afisareVector(valori, 20);
	//afisareVector(vsortat, 20);

}
