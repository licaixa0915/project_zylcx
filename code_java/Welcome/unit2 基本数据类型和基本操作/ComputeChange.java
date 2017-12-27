import javax.swing.JOptionPane;

public class ComputeChange {
	public static void main(String[] args) {
		String amountString = JOptionPane
				.showInputDialog("Enter an amount in double, for "
						+ "example 11.56");
		double amount = Double.parseDouble(amountString);

		int remainingAmount = (int) Math.round(amount * 100);

		int numberOfOneDollars = remainingAmount / 100;
		remainingAmount = remainingAmount % 100;

		int numberOfQuarters = remainingAmount / 25;
		remainingAmount = remainingAmount % 25;

		int numberOfDimes = remainingAmount / 10;
		remainingAmount = remainingAmount % 10;

		int numberOfNickels = remainingAmount / 5;
		remainingAmount = remainingAmount % 5;

		int numberOfPennies = remainingAmount;

		String output = "Your amount " + amount + " consists of \n"
				+ numberOfOneDollars + " dollars\n" + numberOfQuarters
				+ " quarters\n" + numberOfDimes + " dimes\n" + numberOfNickels
				+ " nickels\n" + numberOfPennies + " pennies\n";
		JOptionPane.showMessageDialog(null, output);
	}
}
