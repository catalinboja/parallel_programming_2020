package ro.ase.csie.paralel;

public class ThreadPrime2 extends Thread{
	long limitaInferioara;
	long limitaSuperioara;
	Contor contor;
	int nextI;
	int id;
	long durata;
	int nrVerificate = 0;
	
	public ThreadPrime2(long limitaInferioara, long limitaSuperioara, 
			Contor contor, int nextI, int id) {
		this.limitaInferioara = limitaInferioara;
		this.limitaSuperioara = limitaSuperioara;
		this.contor = contor;
		this.nextI = nextI;
		this.id = id;
	}

	@Override
	public void run() {
		long ts = System.currentTimeMillis();
		for(long i = limitaInferioara; i <= limitaSuperioara; i+= nextI) {
			
			nrVerificate += 1;
			
			boolean estePrim = true;
			
			for(long j = 2; j <= Math.sqrt(i); j+=1 ) {
				if(i % j == 0) {
					estePrim = false;
					break;
				}
			}
			
			if(estePrim)
				contor.increment();
		}
		long tf = System.currentTimeMillis();
		this.durata = (tf - ts);
	}
	
}
