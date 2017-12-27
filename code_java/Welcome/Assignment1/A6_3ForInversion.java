import javax.swing.JOptionPane;

public class A6_3ForInversion {
	public static void main(String[] args) {
		// 初始化
		final int TOTAL_NUMBER = 10;
		int[] number = new int[TOTAL_NUMBER];
		// 输入
		for (int i = 0; i < number.length; i++) {
			String numString = JOptionPane.showInputDialog("Enter a number:"
					+ "(" + (i + 1) + ")");
			number[i] = Integer.parseInt(numString);
		}
		// 输出倒置数组
		String output = "";
		for (int i = 0; i < number.length; i++)
			output += number[number.length - i - 1] + " ";
		JOptionPane.showMessageDialog(null, output);
	}
}
