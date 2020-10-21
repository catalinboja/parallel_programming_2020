package ro.ase.csie.paralel;

public class ThreadPrime extends Thread{

	long limitaInferioara;
	long limitaSuperioara;
	Contor contor;
	public long durata;
	public int id;
	
	public ThreadPrime(long limitaInferioara, long limitaSuperioara, 
			Contor contor, int id) {
		this.limitaInferioara = limitaInferioara;
		this.limitaSuperioara = limitaSuperioara;
		this.contor = contor;
		this.id = id;
	}

	@Override
	public void run() {
		
		long ts = System.currentTimeMillis();
		
		for(long i = limitaInferioara; i <= limitaSuperioara; i++) {
			
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
