import javax.swing.JOptionPane;

public class SubtrationTutorLoop {
	public static void main(String[] args) {
		int correctCount = 0;
		int count = 0;
		long startTime = System.currentTimeMillis();
		String output = "";

		while (count < 10) {
			int number1 = (int) (Math.random() * 10);
			int number2 = (int) (Math.random() * 10);
			if (number1 < number2) {
				int temp = number1;
				number1 = number2;
				number2 = temp;
			}
			String answerString = JOptionPane.showInputDialog("What is "
					+ number1 + " - " + number2 + "?");
			int answer = Integer.parseInt(answerString);
			String replyString;
			if (number1 - number2 == answer) {
				replyString = "You are correct!";
				correctCount++;
			} else
				replyString = "your answer is wrong.\n" + number1 + " - "
						+ number2 + " should be " + (number1 - number2);
			JOptionPane.showMessageDialog(null, replyString);
			count++;
			output += "\n" + number1 + " - " + number2 + " = " + answerString
					+ ((number1 - number2 == answer) ? " correct" : " wrong");
		}
		long endTime = System.currentTimeMillis();
		long testTime = endTime - startTime;
		JOptionPane.showMessageDialog(null, "Correct count is " + correctCount
				+ "\nTest time is " + testTime / 1000 + " Sceonds\n" + output);

	}
}
