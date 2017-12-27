public class TestCircle3 {
	public static void main(String[] args) {
		Circle3 myCircle = new Circle3(5.0);
		System.out.println("The area of the circle of radius "
				+ myCircle.getRaidus() + " is " + myCircle.getArea());
		myCircle.setRadius(myCircle.getRaidus() * 1.1);
		System.out.println("The area of the circle of radius "
				+ myCircle.getRaidus() + " is " + myCircle.getArea());
	}
}