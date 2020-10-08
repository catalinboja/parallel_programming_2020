package ro.ase.csie.parallel;

public class TestApp {

	public static void main(String[] args) {
		
		System.out.println("Hello World");
		
		//definire thread-uri
		Thread t1 = new MyThread();
		Thread t2 = new Thread(new OtherThread());
		
		// nu cu run
		//t1.run();
		//t2.run();
		
		t1.start();
		t2.start();
		
		//sincronizare thread-uri cu main thread
		try {
			
			t1.join();
			t2.join();
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		//test race condition pe ContBanca
		
		ContBancar contBancar = new ContBancar(1000);
		Thread tSot = new Thread(new ClientBanca("Gigel", contBancar));
		Thread tSotie = new Thread(new ClientBanca("Ana", contBancar));
		
		System.out.println("Start shopping");
		
		tSotie.start();
		tSot.start();
		
		try {
			
			tSotie.join();
			tSot.join();
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
				
		System.out.println("The end");
	}
}

// definire Thread
// 1. prin extindere Thread

class MyThread extends Thread {
	
	@Override
	public void run() {
		System.out.println("Start MyThread");
		try {
			sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("End MyThread");
	}	
}

// 2. prin interfata Runnable

class OtherThread implements Runnable {

	@Override
	public void run() {
		System.out.println("Start OtherThread");
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("End OtherThread");
	}
}
