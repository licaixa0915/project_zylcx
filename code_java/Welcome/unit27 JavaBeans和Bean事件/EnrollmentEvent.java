import java.util.EventObject;

@SuppressWarnings("serial")
public class EnrollmentEvent extends EventObject {
	private int numberOfStudents;
	private int enrollmentCap;

	public EnrollmentEvent(Object source, int numberOfStudents,
			int enrollmentCap) {
		super(source);
		this.numberOfStudents = numberOfStudents;
		this.enrollmentCap = enrollmentCap;
	}

	public long getNumberOfStudents() {
		return numberOfStudents;
	}

	public long getEnrollmentCap() {
		return enrollmentCap;
	}
}
