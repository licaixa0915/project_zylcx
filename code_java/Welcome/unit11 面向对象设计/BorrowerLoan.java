import javax.swing.JOptionPane;

public class BorrowerLoan {
	public static void main(String[] args) {
		Name name = new Name("John", 'D', "Smith");
		Address address = new Address("100 Main Street", "Savannah", "GA",
				"31419");
		Loan loan = new Loan(5.5, 15, 250000);
		Borrower borrower = new Borrower(name, address);

		borrower.setLoan(loan);

		JOptionPane.showMessageDialog(null, borrower.toString());
	}
}
