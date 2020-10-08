package ro.ase.csie.parallel;

public class ContBancar {
	
	private double balanta;

	public ContBancar(double balanta) {
		this.balanta = balanta;
	}
	
	public double getBalanta() {
		return this.balanta;
	}
	
	public synchronized void plateste(double suma) {
		if(this.balanta >= suma) {
			System.out.println("Din cont se plateste suma " + suma);
			System.out.println("Sold disponibil " + this.balanta);
			
			this.balanta -= suma;
		}
		else
			System.out.println("S-a incercat plata in cuantum de " + suma);
	}
	
	
}
