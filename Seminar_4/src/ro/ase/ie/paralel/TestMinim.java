
package ro.ase.ie.paralel;

import java.util.Random;

public class TestMinim {

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
		
		
	}

}
