package chapter9;

class GraduateStudent extends Student {
	/*
	 * public String toString(){ return "Graduate Student"; }
	 */
}

class Person extends Object {
	public String toString() {
		return "Person";
	}
}

public class PolymorphismDemo {
	public static void m(Object x) {
		System.out.println(x.toString());
	}

	public static void main(String[] args) {
		m(new GraduateStudent());
		m(new Student());
		m(new Person());
		m(new Object());
	}
} // 类要在外面写...

class Student extends Person {
	public String toString() {
		return "Student";
	}
}
