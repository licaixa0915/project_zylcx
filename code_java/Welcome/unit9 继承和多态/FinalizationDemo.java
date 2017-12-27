class Cake extends Object {
	private int id;

	public Cake(int id) {
		this.id = id;
		System.out.println("Cake object " + id + " is created");
	}

	protected void finalize() throws java.lang.Throwable {
		super.finalize();
		System.out.println("Cake object " + id + " is disposed");
	}
}

public class FinalizationDemo {
	public static void main(String[] args) {
		@SuppressWarnings("unused")
		Cake a1 = new Cake(1);
		@SuppressWarnings("unused")
		Cake a2 = new Cake(2);
		@SuppressWarnings("unused")
		Cake a3 = new Cake(3);

		a2 = a3 = null;
		System.gc();
	}
}