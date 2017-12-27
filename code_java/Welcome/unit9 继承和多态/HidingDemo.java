class A {
	public static int j = 11;
	public static String m1() {
		return "A's static m1";
	}

	public int i = 1;

	public String m2() {
		return "A's instance m2";
	}

	public String m3() {
		return "A's instance m3";
	}
}

class B extends A {
	public static int j = 12;
	public static String m1() {
		return "B's static m1";
	}

	public int i = 2;

	public String m2() {
		return "B's instance m2";
	}
}

public class HidingDemo {
	@SuppressWarnings("static-access")
	public static void main(String[] args) {
		A x = new B();
		System.out.println("(1) x.i is " + x.i);
		System.out.println("(2) (B)x.i is " + ((B) x).i);
		System.out.println("(3) x.j is " + x.j);
		System.out.println("(4) ((B)x).j is " + ((B) x).j);
		System.out.println("(5) x.m1() is " + x.m1());
		System.out.println("(6) ((B)x).m1() is " + ((B) x).m1());
		System.out.println("(7) x.m2() is " + x.m2());
		System.out.println("(8) x.m3() is " + x.m3());
	}
}
