import javax.swing.JOptionPane;

public class ShowCurrentTime {
	public static void main(String[] args) {
		long totalMilliseconds = System.currentTimeMillis();

		long totalSeconds = totalMilliseconds / 1000;
		int currentSecond = (int) (totalSeconds % 60);
		long totalMinutes = totalSeconds / 60;
		int currentMinute = (int) (totalMinutes % 60);
		long totalHours = totalMinutes / 60;
		int currentHour = (int) (totalHours % 24);
		String output = "Current time is " + currentHour + ":" + currentMinute
				+ ":" + currentSecond + " GMT";
		JOptionPane.showMessageDialog(null, output);

	}
}