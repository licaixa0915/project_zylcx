public class Loan {
	private double annualInterestRate;
	private double loanAmount;
	private java.util.Date loanDate;
	private int numberOfYears;

	public Loan() {
		this(7.5, 30, 100000);
	}

	public Loan(double annualInterestRate, int numberOfYears, double loanAmount) {
		this.annualInterestRate = annualInterestRate;
		this.numberOfYears = numberOfYears;
		this.loanAmount = loanAmount;
		loanDate = new java.util.Date();
	}

	public double getAnnualInterestRate() {
		return annualInterestRate;
	}

	public double getLoanAmount() {
		return loanAmount;
	}

	public java.util.Date getLoanDate() {
		return loanDate;
	}

	public double getMonthlyPayment() {
		double monthlyInterestRate = annualInterestRate / 1200;
		return loanAmount
				* monthlyInterestRate
				/ (1 - (Math.pow(1 / (1 + monthlyInterestRate),
						numberOfYears * 12)));
	}

	public int getNumberOfYears() {
		return numberOfYears;
	}

	public double getTotalPayment() {
		return getMonthlyPayment() * numberOfYears * 12;
	}

	public void setAnnualInterestRate(double annualInterestRate) {
		this.annualInterestRate = annualInterestRate;
	}

	public void setLoanAmount(double loanAmount) {
		this.loanAmount = loanAmount;
	}

	public void setNumberOfYears(int numberOfYears) {
		this.numberOfYears = numberOfYears;
	}
}
