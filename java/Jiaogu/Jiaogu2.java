class Jiaogu {
	int steps(int number, boolean print) {
		int step = 0;
		if (print) System.out.println("-----------");
		while (number != 1) {
			if (number % 2 == 0) {
				if (print) System.out.println("|->" + number + " / 2 = " + number / 2);
				number = number / 2;
			} else {
				if (print) System.out.println("|->" + number + " * 3 + 1 = " + (number * 3 + 1));
				number = number * 3 + 1;
			}
			step++;
		}
		if (print) System.out.println("============");
		return step;
	}

	public static void main(String argv[]) {
		int sum = 0;
		Jiaogu jg = new Jiaogu();
		for (int i = 1; i <= 1024; i++) {
			int s = jg.steps(i, false);
			if (s == 10) {
				System.out.println(i + " : " + s);
				jg.steps(i, true);
				sum += i;
			}
		}
		System.out.println("Sum = " + sum);
	}
}
