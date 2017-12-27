public class TestPassObject {
	public static void main(String[] args) {
		Circle3 myCircle = new Circle3(1);
		int n = 5;
		printAreas(myCircle, n);
		System.out.println("\nRadius is " + myCircle.getRaidus());
		System.out.println("n is " + n);
	}

	public static void printAreas(Circle3 c, int times) {
		System.out.println("Radius \t\tArea");
		while (times >= 1) {
			System.out.println(c.getRaidus() + "\t\t" + c.getArea());
			c.setRadius(c.getRaidus() + 1);
			times--;
		}
	}
}
