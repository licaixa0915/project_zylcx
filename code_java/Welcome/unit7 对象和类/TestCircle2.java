public class TestCircle2 {
	public static void main(String[] args) {
		Circle2 c1 = new Circle2();
		System.out.println("Before creating c2");
		System.out.println("c1 is : radius (" + c1.radius
				+ ") and number of Circle objects (" + Circle2.numberOfObjects
				+ ")"); // 使用c1.numberOfObjeces 会报警告错误!
		Circle2 c2 = new Circle2(5);
		c1.radius = 9;
		System.out.println("\nAfter creating c2 and modifying "
				+ "c1's radius to 9");
		System.out.println("c1 is : radius (" + c1.radius
				+ ") and number of Circle objects (" + Circle2.numberOfObjects
				+ ")"); // 使用c1.numberOfObjeces 会报警告错误!
		System.out.println("c2 is : radius (" + c2.radius
				+ ") and number of Circle objects (" + Circle2.numberOfObjects
				+ ")"); // 使用c2.numberOfObjeces 会报警告错误!
	}
}
