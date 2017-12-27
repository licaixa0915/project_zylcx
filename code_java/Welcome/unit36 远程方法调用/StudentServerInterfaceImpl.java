import java.rmi.*;
import java.rmi.server.*;
import java.util.*;

@SuppressWarnings("serial")
public class StudentServerInterfaceImpl extends UnicastRemoteObject implements
		StudentServerInterface {
	@SuppressWarnings({ "rawtypes" })
	private HashMap scores = new HashMap();

	public StudentServerInterfaceImpl() throws RemoteException {
		initializeStudent();
	}

	@SuppressWarnings("unchecked")
	protected void initializeStudent() {
		scores.put("John", new Double(90.5));
		scores.put("Michael", new Double(100));
		scores.put("Michelle", new Double(98.5));
	}

	public double findScore(String name) throws RemoteException {
		Double d = (Double) scores.get(name);

		if (d == null) {
			System.out.println("Student " + name + " is not found ");
			return -1;
		} else {
			System.out.println("Student " + name + "\'s score is "
					+ d.doubleValue());
			return d.doubleValue();
		}
	}
}
