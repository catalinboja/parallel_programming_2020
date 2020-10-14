package ro.ase.csie.paralel;

public class ThreadPrime extends Thread{

	long limitaInferioara;
	long limitaSuperioara;
	Contor contor;
	
	public ThreadPrime(long limitaInferioara, long limitaSuperioara, Contor contor) {
		this.limitaInferioara = limitaInferioara;
		this.limitaSuperioara = limitaSuperioara;
		this.contor = contor;
	}

	@Override
	public void run() {
		
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
	}
	
	
	
}
