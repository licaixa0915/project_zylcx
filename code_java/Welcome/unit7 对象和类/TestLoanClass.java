import javax.swing.JOptionPane;

public class TestLoanClass {
	public static void main(String[] args) {
		String annualInterestRateString = JOptionPane
				.showInputDialog("Enter yearly interest rate"
						+ ", for example 8.25:");
		double annualInterestRate = Double
				.parseDouble(annualInterestRateString);
		String numberOfYearString = JOptionPane
				.showInputDialog("Enter number of years as an integer, "
						+ "\nfor example 5:");
		int numberOfYear = Integer.parseInt(numberOfYearString);
		String loanString = JOptionPane
				.showInputDialog("Enter loan amount, for example 120000.95");
		double loanAmount = Double.parseDouble(loanString);

		Loan loan = new Loan(annualInterestRate, numberOfYear, loanAmount);
		double monthlyPayment = (int) (loan.getMonthlyPayment() * 100) / 100.0;
		double totalPayment = (int) (loan.getTotalPayment() * 100) / 100.0;

		String output = "the loan was created on "
				+ loan.getLoanDate().toString() + "\nThe monthly payment is "
				+ monthlyPayment + "\nThe total payment is " + totalPayment;
		JOptionPane.showMessageDialog(null, output);
	}
}
