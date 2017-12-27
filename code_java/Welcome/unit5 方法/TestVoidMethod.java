import java.util.Scanner;

public class TestVoidMethod {
	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		System.out.print("Enter the score:");
		double score = scanner.nextDouble();
		printGrade(score);
		scanner.close();
	}

	public static void printGrade(double score) {
		if (score < 0 || score > 100) {
			System.out.println("Invalid score!");
			return;
		}
		if (score >= 90)
			System.out.println('A');
		else if (score >= 80)
			System.out.println('B');
		else if (score >= 70)
			System.out.println('C');
		else if (score >= 60)
			System.out.println('D');
		else
			System.out.println('F');
	}
}
