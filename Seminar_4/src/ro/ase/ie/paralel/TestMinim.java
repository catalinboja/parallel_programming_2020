
package ro.ase.ie.paralel;

import java.util.Random;

public class TestMinim {
	
	//metoda ce trebuie folosita pentru a compara 2 valori
	static boolean esteMaiMic(int a, int b) {
		int rez = 0;

		for(int i = 0 ; i< 1500; i++) {
			float temp = (float)a/i;
			rez += (int) temp;
		}
		
		for(int i = 0 ; i< 1500; i++) {
			float temp = (float)a/i;
			rez += (int) temp;
		}
		
		return (a <= b);
	}
	
	static int getMinim(int[] valori) {
		int minim = valori[0];
		for(int valoare : valori) {
			if(esteMaiMic(valoare, minim)) {
				minim = valoare;
			}
		}
		return minim;
	}

	public static void main(String[] args) {
		
		final int LIMITA = 10000;
		int nrElemente = (int) 1e8;
		int[] valori = new int[nrElemente];
		int[] valoriSortateDesc = new int[nrElemente];
		
		//generam valorile random
		Random random = new Random();
		for(int i = 0; i < nrElemente; i++) {
			valori[i] = random.nextInt(LIMITA);
			valoriSortateDesc[i] = nrElemente - i;
		}
		
		System.out.println("Final");
		
		//pentru fiecare vector de valori
		//benchmark solutie secventiala determinare minim
		//benchmark solutie paralela cu minime locale pe intervale - 4 core-uri
		//benchmark solutie paralela cu o alta abordare
		
		long tStart = System.currentTimeMillis();
		
		getMinim(valori);
		
		long tFinal = System.currentTimeMillis();
		
		System.out.println("Durata " + (tFinal- tStart));
			
	}

}
