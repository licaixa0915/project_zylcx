import java.util.*;

public class CourseWithEnrollmentEvent {
	@SuppressWarnings("unused")
	private String name = "default name";
	private ArrayList<String> students = new ArrayList<String>();
	private int enrollmentCap = 10;

	private ArrayList<EnrollmentListener> enrollmentListenerList;

	public CourseWithEnrollmentEvent() {
	}

	public CourseWithEnrollmentEvent(String name) {
		this.name = name;
	}

	public void addStudent(String student) {
		students.add(student);
		if (students.size() > enrollmentCap) {
			processEvent(new EnrollmentEvent(this, getNumberOfStudents(),
					getEnrollmentCap()));
		}
	}

	public String[] getStudents() {
		return (String[]) students.toArray();
	}

	public int getNumberOfStudents() {
		return students.size();
	}

	public int getEnrollmentCap() {
		return enrollmentCap;
	}

	public void setEnrollmentCap(int enrollmentCap) {
		this.enrollmentCap = enrollmentCap;
	}

	public synchronized void addEnrollmentListener(EnrollmentListener listener) {
		if (enrollmentListenerList == null) {
			enrollmentListenerList = new ArrayList<EnrollmentListener>(2);
		}
		if (!enrollmentListenerList.contains(listener)) {
			enrollmentListenerList.add(listener);
		}
	}

	public synchronized void removeEnrollmentListener(
			EnrollmentListener listener) {
		if (enrollmentListenerList != null
				&& enrollmentListenerList.contains(listener)) {
			enrollmentListenerList.remove(listener);
		}
	}

	@SuppressWarnings({ "rawtypes" })
	private void processEvent(EnrollmentEvent e) {
		ArrayList list;
		synchronized (this) {
			if (enrollmentListenerList == null)
				return;
			list = (ArrayList) enrollmentListenerList.clone();
		}
		for (int i = 0; i < list.size(); i++) {
			EnrollmentListener listener = (EnrollmentListener) list.get(i);
			listener.enrollmentExceeded(e);
		}
	}
}
