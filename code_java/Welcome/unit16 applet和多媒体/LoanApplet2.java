import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.TitledBorder;

@SuppressWarnings("serial")
public class LoanApplet2 extends JApplet {
	private class ButtonListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			double interest = Double.parseDouble(jtfAnnualInterestRate
					.getText());
			int year = Integer.parseInt(jtfNumberOfYears.getText());
			double loanAmount = Double.parseDouble(jtfLoanAmount.getText());
			Loan loan = new Loan(interest, year, loanAmount);

			jtfMonthlyPayment.setText(String.format("%.2f", loan
					.getMonthlyPayment()));
			jtfTotalPayment.setText(String.format("%.2f", loan
					.getTotalPayment()));
		}
	}
	public static void main(String[] args) {
		JFrame frame = new JFrame("Applet is in the frame");
		LoanApplet2 applet = new LoanApplet2();

		frame.add(applet, BorderLayout.CENTER);
		applet.init();
		frame.setSize(300, 300);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	private JButton jbtComputeLoan = new JButton("Compute Payment");
	private JTextField jtfAnnualInterestRate = new JTextField();
	private JTextField jtfLoanAmount = new JTextField();
	private JTextField jtfMonthlyPayment = new JTextField();

	private JTextField jtfNumberOfYears = new JTextField();

	private JTextField jtfTotalPayment = new JTextField();

	public void init() {
		jtfMonthlyPayment.setEditable(false);
		jtfTotalPayment.setEditable(false);
		jtfAnnualInterestRate.setHorizontalAlignment(JTextField.RIGHT);
		jtfNumberOfYears.setHorizontalAlignment(JTextField.RIGHT);
		jtfLoanAmount.setHorizontalAlignment(JTextField.RIGHT);
		jtfMonthlyPayment.setHorizontalAlignment(JTextField.RIGHT);
		jtfTotalPayment.setHorizontalAlignment(JTextField.RIGHT);
		JPanel p1 = new JPanel(new GridLayout(5, 2));
		p1.add(new JLabel("Annual Interest Rate"));
		p1.add(jtfAnnualInterestRate);
		p1.add(new JLabel("Number Of Years"));
		p1.add(jtfNumberOfYears);
		p1.add(new JLabel("Loan Amount"));
		p1.add(jtfLoanAmount);
		p1.add(new JLabel("Monthly Payment"));
		p1.add(jtfMonthlyPayment);
		p1.add(new JLabel("Total Payment"));
		p1.add(jtfTotalPayment);
		p1.setBorder(new TitledBorder(
				"Enter interest rate, year and loan amount"));
		JPanel p2 = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		p2.add(jbtComputeLoan);
		add(p1, BorderLayout.CENTER);
		add(p2, BorderLayout.SOUTH);
		jbtComputeLoan.addActionListener(new ButtonListener());
	}
}
