public class TestCircle2 {
	public static void main(String[] args) {
		Circle2 c1 = new Circle2();
		System.out.println("Before creating c2");
		System.out.println("c1 is : radius (" + c1.radius
				+ ") and number of Circle objects (" + Circle2.numberOfObjects
				+ ")"); // ʹ��c1.numberOfObjeces �ᱨ�������!
		Circle2 c2 = new Circle2(5);
		c1.radius = 9;
		System.out.println("\nAfter creating c2 and modifying "
				+ "c1's radius to 9");
		System.out.println("c1 is : radius (" + c1.radius
				+ ") and number of Circle objects (" + Circle2.numberOfObjects
				+ ")"); // ʹ��c1.numberOfObjeces �ᱨ�������!
		System.out.println("c2 is : radius (" + c2.radius
				+ ") and number of Circle objects (" + Circle2.numberOfObjects
				+ ")"); // ʹ��c2.numberOfObjeces �ᱨ�������!
	}
}
