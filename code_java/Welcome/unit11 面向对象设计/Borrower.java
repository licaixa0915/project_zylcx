public class Borrower extends Person {
	private Loan loan; // Loan����unit7�ж���

	public Borrower() {
		super(); // ���ø��๹�캯��
	}

	public Borrower(Name name, Address address) {
		super(name, address); // ���ø��๹�캯��
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
