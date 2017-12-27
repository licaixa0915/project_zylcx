public abstract class GeometricObject {
	private String color = "white";
	private java.util.Date dateCreated;
	private boolean filled;

	protected GeometricObject() {
		dateCreated = new java.util.Date();
	}

	public abstract double getArea();

	public String getColor() {
		return color;
	}

	public java.util.Date getDateCreated() {
		return dateCreated;
	}

	public abstract double getPerimeter();

	public boolean isFilled() {
		return filled;
	}

	public void setColor(String color) {
		this.color = color;
	}

	public void setFilled(boolean filled) {
		this.filled = filled;
	}

	public String toString() {
		return "created on " + dateCreated + "\ncolor: " + color
				+ " and filled: " + filled;
	}
}
