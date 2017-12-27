import javax.swing.JOptionPane;

public class TestArray {
	public static void main(String[] args) {
		final int TOTAL_NUMBER = 6;
		int[] number = new int[TOTAL_NUMBER];

		for (int i = 0; i < number.length; i++) {
			String numString = JOptionPane.showInputDialog("Enter a number:");
			number[i] = Integer.parseInt(numString);
		}
		int max = number[0];
		for (int i = 1; i < number.length; i++)
			if (number[i] > max)
				max = number[i];
		int count = 0;
		for (int i = 0; i < number.length; i++)
			if (number[i] == max)
				count++;
		String output = "The array is ";
		for (int i = 0; i < number.length; i++)
			output += number[i] + " ";
		output += "\nThe largest number is " + max;
		output += "\nThe occurrence count of the largest number is " + count;
		JOptionPane.showMessageDialog(null, output);
	}
}
