import javax.swing.JOptionPane;

public class ComputeFactorial {
	public static long factorial(int n) {
		if (n == 0)
			return 1;
		else
			return n * factorial(n - 1);
	}

	public static void main(String[] args) {
		String intString = JOptionPane
				.showInputDialog("Please enter a non-negative integer: ");
		int n = Integer.parseInt(intString);
		JOptionPane.showMessageDialog(null, "Factorial of " + n + " is "
				+ factorial(n));
	}
}
