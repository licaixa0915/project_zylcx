public class Course {
	private String name;
	private int numberOfStudents;
	private String[] students = new String[100];

	public Course(String name) {
		this.name = name;
	}

	public void addStudent(String student) {
		students[numberOfStudents] = student;
		numberOfStudents++;
	}

	public String getName() {
		return name;
	}

	public int getNumberOfStudents() {
		return numberOfStudents;
	}

	public String[] getStudents() {
		return students;
	}
}
