public class Circle2 {
	static int numberOfObjects = 0;
	static int getNumberOfObjects() {
		return numberOfObjects;
	}

	double radius;

	Circle2() {
		radius = 1.0;
		numberOfObjects++;
	}

	Circle2(double newRadius) {
		radius = newRadius;
		numberOfObjects++;
	}

	double getArea() {
		return radius * radius * Math.PI;
	}
}
