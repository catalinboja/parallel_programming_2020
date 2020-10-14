package ro.ase.csie.paralel;

public class TestPrime {

	static long contorNumerePrime(long limitaI, long limitaS) {
		long contor = 0;
		
		for(long i = limitaI; i <= limitaS; i++) {
			
			boolean estePrim = true;
			
			for(long j = 2; j <= Math.sqrt(i); j+=1 ) {
				if(i % j == 0) {
					estePrim = false;
					break;
				}
			}
			
			if(estePrim)
				contor += 1;
		}
			
		return contor;
	}
	

	
	public static void main(String[] args) throws InterruptedException {
		
		final long LIMITA_SUP = (long) 1e7;
		int nrPrime = 0;
		
		long tStart = System.currentTimeMillis();
		nrPrime = (int) contorNumerePrime(0, LIMITA_SUP);
		long tFinal = System.currentTimeMillis();
		
		System.out.println(
				String.format("Rezultat secvential = %d in %d secunde", 
						nrPrime, (tFinal- tStart)/1000));
		
		//solutia 1
		//rezultat partajat
		//impartim intervalul in 4 parti egale = 4 core-uri
		
		//faza de orchestare
		Contor contor2 = new Contor();
		int nrCoreuri = 8;			//peste 10000 timpul creste - prea multe thread-uri pe acelasi core
		long interval = LIMITA_SUP / nrCoreuri;
		
		Thread[] threaduri = new Thread[nrCoreuri];
		for(int i = 0 ;i < nrCoreuri; i+=1) {
			
			long limitaInferioara = i * interval;
			long limitaSuperioara = (i+1) * interval;
			if(i == nrCoreuri-1)
				limitaSuperioara = LIMITA_SUP;
			
			threaduri[i] = new ThreadPrime(limitaInferioara, 
					limitaSuperioara, contor2);
			
		}
		
		tStart = System.currentTimeMillis();
		for(Thread t : threaduri) {
			t.start();
		}
		
		for(Thread t : threaduri) {
			try {
				t.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		tFinal = System.currentTimeMillis();
		
		System.out.println(
				String.format("Rezultat paralel = %d in %d secunde", 
						contor2.getValoare(), (tFinal- tStart)/1000));
		
		// solutia 2
		// distribuim verificarea fiecarui numar pe cate un thread
		
		//dureaza mult prea mult deoarece costul generarii thread-urilor este prea mare
		/*
		contor2.valoare = 0;
		tStart = System.currentTimeMillis();
		for(long numar = 0; numar < LIMITA_SUP; numar += 4) {
			for(int i = 0; i < nrCoreuri; i+=1) {
				threaduri[i] = new TestPrim(numar + i, contor2); //costa prea mult
			}
			for(int i = 0; i< nrCoreuri; i++) {
				threaduri[i].start();
			}
			for(int i = 0; i< nrCoreuri; i++) {
				threaduri[i].join();
			}
		}
		tFinal = System.currentTimeMillis();
		System.out.println(
				String.format("Rezultat paralel = %d in %d secunde", 
						contor2.getValoare(), (tFinal- tStart)/1000));
		 */
		
		
		//solutia 3
		//rezultat partajat
		//toate thread-urile trec prin acelasi interval cu pas separat
		
		//faza de orchestare
		contor2.valoare = 0;
		
		threaduri = new Thread[nrCoreuri];
		for(int i = 0 ;i < nrCoreuri; i+=1) {
			threaduri[i] = new ThreadPrime2(i, LIMITA_SUP, contor2, nrCoreuri);
		}
		
		tStart = System.currentTimeMillis();
		for(Thread t : threaduri) {
			t.start();
		}
		
		for(Thread t : threaduri) {
			try {
				t.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		tFinal = System.currentTimeMillis();
		
		System.out.println(
				String.format("Rezultat paralel 3 = %d in %d secunde", 
						contor2.getValoare(), (tFinal- tStart)/1000));
		
	}

}
