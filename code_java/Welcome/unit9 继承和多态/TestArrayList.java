import chapter9.Circle;

public class TestArrayList {
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static void main(String[] args) {
		java.util.ArrayList<String> cityList = new java.util.ArrayList<String>();
		cityList.add("London");
		cityList.add("New York");
		cityList.add("Paris");
		cityList.add("Toronto");
		cityList.add("Hong Kong");
		cityList.add("Singapore");

		System.out.println("List size? " + cityList.size());
		System.out.println("Is Toronto in the  list? "
				+ cityList.contains("Toronto"));
		System.out.println("The location of New York in the list? "
				+ cityList.indexOf("New York"));
		System.out.println("Is the list empty? " + cityList.isEmpty());

		cityList.add(2, "Beijing");
		cityList.remove("Toronto");
		cityList.remove(1);
		for (int i = 0; i < cityList.size(); i++)
			System.out.print(cityList.get(i) + " ");
		System.out.println();

		java.util.ArrayList list = new java.util.ArrayList();
		list.add(new Circle(2));
		list.add(new Circle(3));
		System.out.println("The area of the circle? "
				+ ((Circle) list.get(0)).getArea());
	}
}
