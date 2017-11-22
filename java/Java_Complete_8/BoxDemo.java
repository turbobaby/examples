class Box {
	int width;
	int height;
	int depth;
	void volume() {
		System.out.println("volume is " + width * height * depth);
	}
}

class BoxDemo {
	public static void main(String argv[]) {
		Box b = new Box();
		double vol;
		b.width = 12;
		b.height = 25;
		b.depth = 10;
		vol = b.width * b.height * b.depth;
		b.volume();
		System.out.println("Box's volume is " + vol);
		System.out.println("Box's " + b.toString());
	}
}
