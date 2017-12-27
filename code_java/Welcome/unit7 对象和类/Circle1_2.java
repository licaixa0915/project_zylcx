public class Circle1_2 {
	public static void main(String[] args) {
		Circle1_2 myCircle = new Circle1_2(5.0);
		System.out.println("The area of the circle of radius "
				+ myCircle.radius + " is " + myCircle.getArea());
		Circle1_2 yourCircle = new Circle1_2();
		System.out.println("The area of the circle of radius "
				+ yourCircle.radius + " is " + yourCircle.getArea());
		yourCircle.radius = 100;
		System.out.println("The area of the circle of radius "
				+ yourCircle.radius + " is " + yourCircle.getArea());
	}

	double radius;

	Circle1_2() {
		radius = 1.0;
	}

	Circle1_2(double newRadius) {
		radius = newRadius;
	}

	double getArea() {
		return radius * radius * Math.PI;
	}
}
