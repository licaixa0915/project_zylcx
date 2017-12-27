public class Borrower extends Person {
	private Loan loan; // Loan类在unit7中定义

	public Borrower() {
		super(); // 调用父类构造函数
	}

	public Borrower(Name name, Address address) {
		super(name, address); // 调用父类构造函数
	}

	public Loan getLoan() {
		return loan;
	}

	public void setLoan(Loan loan) {
		this.loan = loan;
	}

	public String toString() {
		return super.toString() + "Monthly payment is "
				+ loan.getMonthlyPayment() + '\n' + "Total payment is "
				+ loan.getTotalPayment();
	}
}
