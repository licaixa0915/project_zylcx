public class A7_3TestAccount {
	public static void main(String[] args) {
		Account account = new Account(1122, 20000, 4.5);
		System.out.println(account.toString());
		// 取款并打印
		account.withDraw(2500);
		System.out.println("After load $2500 ");
		System.out.println(account.toString());
		// 存款并打印
		account.deposit(3000);
		System.out.println("After save $3000 ");
		System.out.println(account.toString());
	}
}

// 账户类
class Account {
	private double annualInterestRate;
	private double balance;
	private java.util.Date dateCreated;
	private int id;

	public Account() {
		this(0, 0, 0);
	}

	public Account(int id, double balance, double annualInterestRate) {
		this.id = id;
		this.balance = balance;
		this.annualInterestRate = annualInterestRate;
		dateCreated = new java.util.Date();
	}

	public void deposit(double saves) {
		balance = balance + saves;
	}

	public double getAnnualInterestRate() {
		return annualInterestRate;
	}

	public double getBalance() {
		return balance;
	}

	public java.util.Date getDateCreated() {
		return dateCreated;
	}

	public int getId() {
		return id;
	}

	public double getMonthlyInterestRate(double annualInterestRate) {
		return annualInterestRate / 1200;
	}

	public void setAnnualInterestRate(double annualInterestRate) {
		this.annualInterestRate = annualInterestRate;
	}

	public void setBalance(double balance) {
		this.balance = balance;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String toString() {
		return "The id " + id + "'s  balance is " + balance
				+ ", and monthytinterestrate is "
				+ getMonthlyInterestRate(annualInterestRate) + "\nThe date is "
				+ getDateCreated();
	}

	public void withDraw(double loads) {
		balance = balance - loads;
	}
}
