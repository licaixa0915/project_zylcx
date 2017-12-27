import javax.swing.JOptionPane;

public class ComputeFibonacci {
	public static long fib(long index) {
		if (index == 0)
			return 0;
		else if (index == 1)
			return 1;
		else
			return fib(index - 1) + fib(index - 2);
	}

	public static void main(String[] args) {
		String intString = JOptionPane
				.showInputDialog("Enter an index for the fibonacci number:");
		int index = Integer.parseInt(intString);
		JOptionPane.showMessageDialog(null, "Fibonacci number at index "
				+ index + " is " + fib(index));
	}
}
