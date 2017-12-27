import javax.swing.JOptionPane;

public class PringPyramid {
	public static void main(String[] args) {
		String input = JOptionPane
				.showInputDialog("Enter the number of lines:");
		int numberOfLines = Integer.parseInt(input);
		if (numberOfLines < 1 || numberOfLines > 15) {
			System.out.println("You must enter a number from 1 to 15");
			System.exit(0);
		}

		for (int row = 1; row <= numberOfLines; row++) {
			for (int column = 1; column <= numberOfLines - row; column++)
				System.out.print("   ");
			for (int num = row; num >= 1; num--)
				System.out.print((num >= 10) ? " " + num : "  " + num);
			for (int num = 2; num <= row; num++)
				System.out.print((num >= 10) ? " " + num : "  " + num);
			System.out.println();
		}
	}
}
