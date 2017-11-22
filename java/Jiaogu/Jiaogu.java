class Jiaogu {
	long steps(long number) {
		long step = 0;
		while (number != 1) {
			if (number % 2 == 0) {
				number = number / 2;
			} else {
				number = number * 3 + 1;
			}
			step++;
		}
		return step;
	}

	long steps(long number, boolean print) {
		long step = 0;
		if (print) System.out.println("-----------");
		while (number != 1) {
			step++;
			if (number % 2 == 0) {
				if (print) System.out.println("|->("+step+")  " + number + " / 2 = " + number / 2);
				number = number / 2;
			} else {
				if (print) System.out.println("|->("+step+")  " + number + " * 3 + 1 = " + (number * 3 + 1));
				number = number * 3 + 1;
			}
		}
		if (print) System.out.println("============");
		return step;
	}


	public static void main(String argv[]) {
		long max_s = 0, max_i = 0;
		Jiaogu jg = new Jiaogu();
		for (long i = 1; i <= 100_0000; i++) {
			long s = jg.steps(i);
//			System.out.println(i + " : " + s);
			if (s > max_s) {
				max_i = i;
				max_s = s;	
			}
		}
		System.out.println(max_i + " : " + max_s);
		jg.steps(max_i, true);
	}
}
