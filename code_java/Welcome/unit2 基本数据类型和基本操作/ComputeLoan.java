import javax.swing.JOptionPane;

public class ComputeLoan {
	public static void main(String[] args) {
		String annualInterestRateString = JOptionPane
				.showInputDialog("Enter yearly interest rate,"
						+ " for example 8.25:");
		double annualInterestRate = Double
				.parseDouble(annualInterestRateString);
		String numberOfYearsString = JOptionPane
				.showInputDialog("Enter number of years as an interger"
						+ ", \nfor example 5:");
		int numberOfYears = Integer.parseInt(numberOfYearsString);
		String loanString = JOptionPane.showInputDialog("Enter loan amount,"
				+ "for example 120000.95:");
		double loanAmount = Double.parseDouble(loanString);

		double monthlyInterestRate = annualInterestRate / 1200;
		double monthlyPayment = loanAmount
				* monthlyInterestRate
				/ (1 - 1 / Math
						.pow(1 + monthlyInterestRate, numberOfYears * 12));
		double totalpayment = monthlyPayment * numberOfYears * 12;
		monthlyPayment = (int) Math.round(monthlyPayment * 100) / 100.0; // 注意四舍五入
		totalpayment = (int) Math.round(totalpayment * 100) / 100.0;

		JOptionPane.showMessageDialog(null, "The monthly payment is "
				+ monthlyPayment + "\nThe total payment is " + totalpayment);
	}
}
