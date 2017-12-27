public class Circle extends GeometricObject { // ������
	private double radius;

	public Circle() {
	}

	public Circle(double radius) {
		this.radius = radius;
	}

	public double getArea() {
		return radius * radius * Math.PI;
	}

	public double getDiameter() {
		return 2 * radius;
	}

	public double getPerimeter() {
		return 2 * radius * Math.PI;
	}

	public double getRadius() {
		return radius;
	}

	public void printCircle() {
		System.out.println("The circle is created " + getDateCreated()
				+ " and the radius is " + radius);
	}

	public void setRadius(double radius) {
		this.radius = radius;
	}

	// ��������
	public String toString() {
		return super.toString() + "\nradius is " + radius;
	}
}