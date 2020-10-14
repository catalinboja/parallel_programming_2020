package ro.ase.csie.paralel;

public class ThreadPrim extends Thread {

	long numar;
	Contor contor;
	
	
	public ThreadPrim(long numar, Contor contor) {
		this.numar = numar;
		this.contor = contor;
	}

	@Override
	public void run() {
		boolean estePrim = true;
		
		for(long j = 2; j <= Math.sqrt(numar); j+=1 ) {
			if(numar % j == 0) {
				estePrim = false;
				break;
			}
		}
		
		if(estePrim)
			contor.increment();
	}
	
	
}
