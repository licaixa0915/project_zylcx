import java.awt.Dimension;

import javax.swing.*;

public class JOptionPaneDemo {
	public static void main(String[] args) {
		Object[] rateList = new Object[25];
		int i = 0;
		for (double rate = 5; rate <= 8; rate += 1.0 / 8) {
			rateList[i++] = new Double(rate);
		}
		Object annualInterestRateObject = JOptionPane.showInputDialog(null,
				"Select annual interest rate:", "JOptionPaneDemo",
				JOptionPane.QUESTION_MESSAGE, null, rateList, null);
		double annualInterestRate = ((Double) annualInterestRateObject)
				.doubleValue();

		Object[] yearList = { new Integer(7), new Integer(15), new Integer(30) };
		Object numberOfYearsObject = JOptionPane.showInputDialog(null,
				"Select number of years:", "JOptionPaneDemo",
				JOptionPane.QUESTION_MESSAGE, null, yearList, null);
		int numberOfYears = ((Integer) numberOfYearsObject).intValue();

		String loanAmountString = JOptionPane.showInputDialog(null,
				"Enter loan amount,\nfor example, 150000 for $150000",
				"JOptionPaneDemo", JOptionPane.QUESTION_MESSAGE);
		double loanAmount = Double.parseDouble(loanAmountString);

		Loan loan = new Loan(annualInterestRate, numberOfYears, loanAmount);
		double monthlyPayment = loan.getMonthlyPayment();
		double totalpayment = loan.getTotalPayment();

		String output = "Interest Rate: " + annualInterestRate + "%"
				+ " Number of Years: " + numberOfYears + " Loan Amount: $"
				+ loanAmount;
		output += "\nMonthly Payment: $" + (int) (monthlyPayment * 100) / 100.0;
		output += "\nTotal Payment: $"
				+ (int) (totalpayment * 100) / 100.0
				+ "\n";
		double monthlyInterestRate = annualInterestRate / 1200;
		double balance = loanAmount;
		double interest;
		double principal;
		output += "\nPayment#\tInterest\tPrincipal\tBalance\n";

		for (i = 1; i <= numberOfYears * 12; i++) {
			interest = (int) (monthlyInterestRate * balance * 100) / 100.0;
			principal = (int) ((monthlyPayment - interest) * 100) / 100.0;
			balance = (int) ((balance - principal) * 100) / 100.0;
			output += i + "\t" + interest + "\t" + principal + "\t" + balance
					+ "\n";
		}

		JScrollPane jsp = new JScrollPane(new JTextArea(output));
		jsp.setPreferredSize(new Dimension(400, 200));
		JOptionPane.showMessageDialog(null, jsp, "JOptionPaneDemo",
				JOptionPane.INFORMATION_MESSAGE, null);
	}
}
