#include <iostream>
#include <string>
#include <vector>
#include "omp.h"

using namespace std;

int cautareParalelaVector(vector<int> valori, int valoare) {
	int index = -1;

	int nrProcesoare = omp_get_num_procs();
	omp_set_num_threads(4);

#pragma omp parallel num_threads(8)
	{		
		cout << endl << "Hello de 8 ori";
#pragma omp for
		for (int i = 0; i < valori.size(); i++) {
			if (valori[i] == valoare) {
#pragma omp critical
				{
					index = i;
				}
			}
		}
	}

#pragma omp parallel
	{
		cout << endl << "Hello de 4 ori";
	}

	omp_set_num_threads(16);

#pragma omp parallel
	{
		cout << endl << "Hello de 16 ori";
	}

	return index;
}

//intoarce numarul de aparitii al lui p in t
int search(string t, string p)
{
	int count = 0;
	int m = p.size();

	if (m > t.size())
		return count;

	for (int i = 0; i < t.size() - m; i++)
	{
		int j = 0;
		for (j = 0; j < m; j++)
			if (t[i + j] != p[j]) break;

		if (j == m)
			count++;
	}
	return count;
}

string* generareTexte(int N, int dimMaxText) {
	string* texte = new string[N];
	srand(1000);
	for (int i = 0; i < N; i++) {
		int dimText = (rand() % dimMaxText) + 1;
		for (int j = 0; j < dimText; j++) {
			texte[i] += ('a' + rand() % 26);
		}
	}
	return texte;
}

# define NO_OF_CHARS 256  

// The preprocessing function for Boyer Moore's  
// bad character heuristic  
void badCharHeuristic(string str, int size, int badchar[NO_OF_CHARS])
{
	int i;

	// Initialize all occurrences as -1  
	for (i = 0; i < NO_OF_CHARS; i++)
		badchar[i] = -1;

	// Fill the actual value of last occurrence  
	// of a character  
	for (i = 0; i < size; i++)
		badchar[(int)str[i]] = i;
}

int max(int a, int b) {
	return a >= b ? a : b;
}

/* A pattern searching function that uses Bad
Character Heuristic of Boyer Moore Algorithm */
int searchBoyerMoore(string txt, string pat, int badchar[])
{
	int m = pat.size();
	int n = txt.size();

	if (m > n)
		return -1;

	//int badchar[NO_OF_CHARS];

	/* Fill the bad character array by calling
	the preprocessing function badCharHeuristic()
	for given pattern */
	//badCharHeuristic(pat, m, badchar);

	int s = 0; // s is shift of the pattern with  
				// respect to text  
	while (s <= (n - m))
	{
		int j = m - 1;

		/* Keep reducing index j of pattern while
		characters of pattern and text are
		matching at this shift s */
		while (j >= 0 && pat[j] == txt[s + j])
			j--;

		/* If the pattern is present at current
		shift, then index j will become -1 after
		the above loop */
		if (j < 0)
		{
			//cout << "pattern occurs at shift = " << s << endl;
			return s;

			/* Shift the pattern so that the next
			character in text aligns with the last
			occurrence of it in pattern.
			The condition s+m < n is necessary for
			the case when pattern occurs at the end
			of text */
			//s += (s + m < n) ? m - badchar[txt[s + m]] : 1;

		}

		else
			/* Shift the pattern so that the bad character
			in text aligns with the last occurrence of
			it in pattern. The max function is used to
			make sure that we get a positive shift.
			We may get a negative shift if the last
			occurrence of bad character in pattern
			is on the right side of the current
			character. */
			s += max(1, j - badchar[txt[s + j]]);
	}

	return -1;
}


int cautaInVectorStringuri(string* texte, int N, string text) {
	int contor = 0;
	for (int i = 0; i < N; i++) {
		if (search(texte[i], text) > 0) {
			contor += 1;
		}
	}

	return contor;
}

int cautaParalelInVectorStringuri(string* texte, int N, string text) {
	int contor = 0;
#pragma omp parallel for reduction(+: contor)
	for (int i = 0; i < N; i++) {
		if (search(texte[i], text) > 0) {
			contor += 1;
		}
	}
	return contor;
}

int cautaInVectorStringuriBoyer(string* texte, int N, string text) {

	int badchar[NO_OF_CHARS];
	badCharHeuristic(text, text.size(), badchar);


	int contor = 0;
	for (int i = 0; i < N; i++) {
		if (searchBoyerMoore(texte[i], text, badchar) >= 0) {
			contor += 1;
		}
	}

	return contor;
}


int cautaParalelInVectorStringuriBoyer(string* texte, int N, string text) {

	int badchar[NO_OF_CHARS];
	badCharHeuristic(text, text.size(), badchar);
	int contor = 0;

#pragma omp parallel for reduction(+: contor)
	for (int i = 0; i < N; i++) {
		if (searchBoyerMoore(texte[i], text, badchar) >= 0) {
			contor += 1;
		}
	}

	return contor;
}
int main() {

	const int N = 1e6;

	//generare date de test
	string* texte = generareTexte(N, 50);
	cout << endl << "Ultima valoare este " << texte[N - 1];

	string p = "aah";

	double tStart = omp_get_wtime();
	int rezultat = cautaInVectorStringuri(texte, N, p);
	double tFinal = omp_get_wtime();
	printf("\n Textul cautat %s se gaseste de %d ori in texte. Durata (naiva) %f sec", 
		p.c_str(), rezultat, (tFinal - tStart));

	tStart = omp_get_wtime();
	rezultat = cautaParalelInVectorStringuri(texte, N, p);
	tFinal = omp_get_wtime();
	printf("\n Textul cautat %s se gaseste de %d ori in texte. Durata (paralel) %f sec",
		p.c_str(), rezultat, (tFinal - tStart));

	tStart = omp_get_wtime();
	rezultat = cautaInVectorStringuriBoyer(texte, N, p); 
	tFinal = omp_get_wtime();
	printf("\n Textul cautat %s se gaseste de %d ori in texte. Durata (Boyer-Moore) %f sec",
		p.c_str(), rezultat, (tFinal - tStart));

	tStart = omp_get_wtime();
	rezultat = cautaParalelInVectorStringuriBoyer(texte, N, p);
	tFinal = omp_get_wtime();
	printf("\n Textul cautat %s se gaseste de %d ori in texte. Durata (Boyer-Moore paralel) %f sec",
		p.c_str(), rezultat, (tFinal - tStart));
}
