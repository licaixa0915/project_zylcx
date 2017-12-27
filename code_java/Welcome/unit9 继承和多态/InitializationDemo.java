public class InitializationDemo {
	static {
		System.out.println("(1) InitializationDemo's static block");
	}

	public static void main(String[] args) {
		new InitializationDemo();
	}

	{
		System.out.println("(2) InitializationDemo's instance block");
	}

	public InitializationDemo() {
		new M();
	}
}

class M extends N {
	static {
		System.out.println("(4) M's static initialization block");
	}

	{
		System.out.println("(7) M's instance initialization block");
	}

	M() {
		System.out.println("(8) M's constructor body");
	}
}

class N {
	static {
		System.out.println("(3) N's static initialization block");
	}

	{
		System.out.println("(5) N's instance initialization block");
	}

	N() {
		System.out.println("(6) N's constructor body");
	}
}