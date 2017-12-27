import javax.swing.JOptionPane;

public class ComputeTax {
	public static double computeTax(int status, double income) {
		double[] rates = { 0.10, 0.15, 0.27, 0.30, 0.35, 0.386 };
		int[][] brackets = { { 6000, 27950, 67700, 141250, 307050 },
				{ 12000, 46700, 112850, 171950, 307050 },
				{ 6000, 23350, 56425, 85975, 153525 },
				{ 10000, 37450, 96700, 156600, 307050 } };
		double tax = 0;
		if (income <= brackets[status][0])
			return tax = income * rates[0];
		else
			tax = brackets[status][0] * rates[0];
		for (int i = 1; i < brackets[status].length; i++)
			if (income > brackets[status][i])
				tax += (brackets[status][i] - brackets[status][i - 1])
						* rates[i];
			else {
				tax += (income - brackets[status][i - 1]) * rates[i];
				return tax;
			}
		return tax += (income - brackets[status][4]) * rates[5];
	}

	public static void main(String[] args) {
		String statusString = JOptionPane
				.showInputDialog("Enter the filing statu:s\n"
						+ "(0-singer filer, 1-married jointly,\n"
						+ "2-married separately, 3-head of household)");
		int status = Integer.parseInt(statusString);
		String incomeString = JOptionPane
				.showInputDialog("Enter the taxable income:");
		double income = Double.parseDouble(incomeString);

		JOptionPane.showMessageDialog(null, "Tax is "
				+ (int) Math.round(computeTax(status, income) * 100) / 100.0);
	}
}
