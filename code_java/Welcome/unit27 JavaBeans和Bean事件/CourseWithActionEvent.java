import java.util.*;
import java.awt.event.*;

public class CourseWithActionEvent {
	@SuppressWarnings("unused")
	private String name = "default name";
	private ArrayList<String> students = new ArrayList<String>();
	private int enrollmentCap = 10;
	private ArrayList<ActionListener> actionListenerList;

	public CourseWithActionEvent() {
	}

	public CourseWithActionEvent(String name) {
		this.name = name;
	}

	public void addStudent(String student) {
		students.add(student);
		if (students.size() > enrollmentCap) {
			processEvent(new ActionEvent(this, ActionEvent.ACTION_PERFORMED,
					null));
		}
	}

	public String[] getStudents() {
		return (String[]) students.toArray();
	}

	public int getEnrollmentCap() {
		return enrollmentCap;
	}

	public void setEnrollmentCap(int enrollmentCap) {
		this.enrollmentCap = enrollmentCap;
	}

	public synchronized void addActionListener(ActionListener listener) {
		if (actionListenerList == null) {
			actionListenerList = new ArrayList<ActionListener>(2);
		}
		if (!actionListenerList.contains(listener)) {
			actionListenerList.add(listener);
		}
	}

	public synchronized void removeActionListener(ActionListener listener) {
		if (actionListenerList != null && actionListenerList.contains(listener)) {
			actionListenerList.remove(listener);
		}
	}

	@SuppressWarnings({ "rawtypes" })
	private void processEvent(ActionEvent e) {
		ArrayList list;
		synchronized (this) {
			if (actionListenerList == null)
				return;
			list = (ArrayList) actionListenerList.clone();
		}
		for (int i = 0; i < list.size(); i++) {
			ActionListener listener = (ActionListener) list.get(i);
			listener.actionPerformed(e);
		}
	}
}
