public class A7_1TestRectangle {
	public static void main(String[] args) {
		Rectangle_2 rectangle1;
		Rectangle_2 rectangle2;
		// 设置矩形属性
		rectangle1 = new Rectangle_2(4, 40);
		rectangle1.setColor("red");
		rectangle2 = new Rectangle_2(3.5, 35.9);
		rectangle2.setColor("red");
		// 输出矩形属性
		System.out.println("The NO.1 rectangle:\n" + rectangle1.toString());
		System.out.println("The NO.2 rectangle:\n" + rectangle2.toString());
	}
}

// 矩形类
class Rectangle_2 {
	private String color = "white";
	private double height = 1;
	private double width = 1;

	public Rectangle_2() {
	}

	public Rectangle_2(double width, double height) {
		this.width = width;
		this.height = height;
	}

	public double getArea() {
		return (width * height * 100) / 100.0; // 保留两位小数并四舍五入
	}

	public String getColor() {
		return color;
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

	public void setColor(String color) {
		this.color = color;
	}

	public void setHeight(double height) {
		this.height = height;
	}

	public void setWidth(double width) {
		this.width = width;
	}

	public String toString() {
		return " The width of rectangle is " + width
				+ " and the heigth of rectangle is " + height
				+ "\n The color of rectnagle is " + color + "\n The Area is "
				+ getArea() + " and the " + "Perimeter is " + getPerimeter();
	}
}