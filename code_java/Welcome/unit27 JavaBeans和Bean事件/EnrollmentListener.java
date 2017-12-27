import java.util.EventListener;

public interface EnrollmentListener extends EventListener {
	public void enrollmentExceeded(EnrollmentEvent e);
}
