public class Ch2 {
	public static void main(String argv[]) {
		long   d = 12345;
		int    i = 67890;
		float  f = 123.456f;
		double dd = 456.789;
		int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		System.out.println("d = " + d);
		System.out.println("i = " + i);
		System.out.println("f = " + f);
		System.out.println("dd= " + dd);
		for (i = 0; i < 12; i++) {
			System.out.println("month[" + i + "] = " + month_days[i]);
		}
		for (int x: month_days) {
			System.out.println("month[" + x + "]");
		}
	}
}
