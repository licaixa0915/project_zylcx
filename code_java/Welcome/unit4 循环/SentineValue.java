import javax.swing.JOptionPane;

public class SentineValue {
	public static void main(String[] args) {
		String dataString = JOptionPane.showInputDialog("Enter an int value:\n"
				+ "(thr program exits if the input is 0)");
		int data = Integer.parseInt(dataString);
		int sum = 0;
		while (data != 0) {
			sum += data;
			dataString = JOptionPane.showInputDialog("Enter an int value:\n"
					+ "(thr program exits if the input is 0)");
			data = Integer.parseInt(dataString);
		}
		JOptionPane.showMessageDialog(null, "The sum is " + sum);

	}
}
