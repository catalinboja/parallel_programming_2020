package ro.ase.csie.paralel;

public class Contor {
	
	int valoare = 0;
	
	synchronized public void increment() {
		this.valoare += 1;
	}
	
	public int getValoare() {
		return this.valoare;
	}
	
}
