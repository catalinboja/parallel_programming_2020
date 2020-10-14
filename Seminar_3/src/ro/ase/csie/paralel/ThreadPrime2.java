package ro.ase.csie.paralel;

public class ThreadPrime2 extends Thread{
	long limitaInferioara;
	long limitaSuperioara;
	Contor contor;
	int nextI;
	
	public ThreadPrime2(long limitaInferioara, long limitaSuperioara, 
			Contor contor, int nextI) {
		this.limitaInferioara = limitaInferioara;
		this.limitaSuperioara = limitaSuperioara;
		this.contor = contor;
		this.nextI = nextI;
	}

	@Override
	public void run() {
		
		for(long i = limitaInferioara; i <= limitaSuperioara; i+= nextI) {
			
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
