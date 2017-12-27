package chapter9;

public class Rectangle extends GeometricObject { // ≈……˙¿‡
	private double height;
	private double width;

	public Rectangle() {
	}

	public Rectangle(double width, double height) {
		this.width = width;
		this.height = height;
	}

	public double getArea() {
		return width * height;
	}

	public double getHeight() {
		return height;
	}

	public double getPerimeter() {
		return 2 * (width + height);
	}

	public double getWidth() {
		return width;
	}

	public void setHeight(double height) {
		this.height = height;
	}

	public void setWidth(double width) {
		this.width = width;
	}
}
