class Car implements Runnable {
	public int speed;
	public char mark;
	Car(int s, char c) {
		speed = s;
		mark = c;
	}

	public void run() {
		Thread self = Thread.currentThread();
		System.out.println("My name is " + self);
		for (;;) {
			System.out.print(mark);
			try {
				Thread.sleep(1000/speed);
			} catch (InterruptedException e) {
			}
		}
	}
}

class ThreadDemo {
	public static void main(String argv[]) {
		Car c1 = new Car(10, '*');
		Thread t1 = new Thread(c1, "The first Car!");
		Car c2 = new Car(20, '-');
		Thread t2 = new Thread(c2, "The second Car!");

		t1.start();
		t2.start();
		try {
			Thread.sleep(10 * 1000);
		} catch (InterruptedException e) {
		}
		System.out.println("Main Thread timeout ...");
		t1.stop();
		t2.stop();
		System.out.println("Child Thread stopped");
		try {
			Thread.sleep(2 * 1000);
		} catch (InterruptedException e) {
		}
		System.out.println("Main Thread exiting ...");
	}
}
