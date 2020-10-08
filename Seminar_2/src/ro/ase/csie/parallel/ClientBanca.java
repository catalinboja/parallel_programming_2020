package ro.ase.csie.parallel;

import java.util.Random;

public class ClientBanca implements Runnable {
	
	private String numeClient;
	private ContBancar cont;
	
	public ClientBanca(String numeClient, ContBancar cont) {
		this.numeClient = numeClient;
		this.cont = cont;
	}

	@Override
	public void run() {
		while(this.cont.getBalanta() > 0) {
			Random random = new Random();
			int suma = random.nextInt(10);
			String mesaj = String.format("%s doreste sa cheltuie %d",
					this.numeClient, suma);
			System.out.println(mesaj);
			this.cont.plateste(suma);
			System.out.println("Sold disponibil " + this.cont.getBalanta());
		}
	}

}
