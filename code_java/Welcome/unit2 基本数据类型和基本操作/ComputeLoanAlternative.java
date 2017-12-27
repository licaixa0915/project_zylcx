import java.util.Scanner;

public class ComputeLoanAlternative {
	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);

		System.out.print("Enter yearly interest rate, for example 8.25:");
		double annualInterestRate = input.nextDouble();
		System.out.print("Enter number of years as an interger"
				+ ", \nfor example 5:");
		int numberOfYears = input.nextInt();
		System.out.print("Enter loan amount," + "for example 120000.95:");
		double loanAmount = input.nextDouble();

		double monthlyInterestRate = annualInterestRate / 1200;
		double monthlyPayment = loanAmount
				* monthlyInterestRate
				/ (1 - 1 / Math
						.pow(1 + monthlyInterestRate, numberOfYears * 12));
		double totalpayment = monthlyPayment * numberOfYears * 12;
		monthlyPayment = (int) Math.round(monthlyPayment * 100) / 100.0; // 注意四舍五入
		totalpayment = (int) Math.round(totalpayment * 100) / 100.0; // 注意四舍五入

		System.out.println("The monthly payment is " + monthlyPayment
				+ "\nThe total payment is " + totalpayment);
		input.close();
	}
}
