import javax.swing.JOptionPane;

public class TestDo {
	public static void main(String[] args) {
		int data;
		int sum = 0;

		do {
			String dataString = JOptionPane
					.showInputDialog("Enter an int value:\n"
							+ "(the program exits if the input is 0)");
			data = Integer.parseInt(dataString);
			sum += data;
		} while (data != 0);

		JOptionPane.showMessageDialog(null, "The sum is " + sum, "TestDo",
				JOptionPane.INFORMATION_MESSAGE);

	}
}
