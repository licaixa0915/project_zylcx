public class TestCircleWithException {
	public static void main(String[] args) {
		try {
			@SuppressWarnings("unused")
			CircleWithException c1 = new CircleWithException(5);
			@SuppressWarnings("unused")
			CircleWithException c2 = new CircleWithException(-5);
			@SuppressWarnings("unused")
			CircleWithException c3 = new CircleWithException(0);
		} catch (IllegalArgumentException ex) {
			System.out.println(ex);
		}
		System.out.println("Number of objects created: "
				+ CircleWithException.getNumberOfObjects());
	}
}
