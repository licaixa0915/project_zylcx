public class TotalArea {
	public static Circle3[] creatCircleArray() {
		Circle3[] circleArray = new Circle3[10];
		for (int i = 0; i < circleArray.length; i++)
			circleArray[i] = new Circle3(Math.random() * 100);
		return circleArray;
	}

	public static void main(String[] args) {
		Circle3[] circleArray;
		circleArray = creatCircleArray();
		printCircleArray(circleArray);
	}

	public static void printCircleArray(Circle3[] circleArray) {
		System.out.println("Radius\t\t\t\t" + "Area");
		for (int i = 0; i < circleArray.length; i++)
			System.out.print(circleArray[i].getRaidus() + "\t\t"
					+ circleArray[i].getArea() + "\n");
		System.out
				.println("----------------------------------------------------");
		System.out.println("The total areas of circles is \t"
				+ sum(circleArray));
	}

	public static double sum(Circle3[] circleArray) {
		double sum = 0;
		for (int i = 0; i < circleArray.length; i++)
			sum += circleArray[i].getArea();
		return sum;
	}
}
