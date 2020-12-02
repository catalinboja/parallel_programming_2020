#include "sha1.h"
#include "sha1.cpp"
#include <string>
#include <fstream>
using namespace std;

int main() {
	string parola = "test";
	string hash = sha1(parola);
	cout << endl << "sha1('test'):" << hash;

	string hashParolaDeGasit = "00c3cc7c9d684decd98721deaa0729d73faa9d9b";

	ifstream fisier("10-million-password-list-top-1000000.txt", ios::in);
	if (fisier.is_open()) {
		string parola;
		fisier >> parola;
		cout << endl << "Parola citita este " << parola;

		fisier.close();
	}
	else {
		cout << endl << "Fisierul nu a fost deschis";
	}

	ifstream fisier2("Date.txt", ios::in);
	string linie = "";
	while (!fisier2.eof()) {
		fisier2 >> linie;
		cout << endl << "Linie din Date.txt " << linie;
	}
	fisier2.close();
}

