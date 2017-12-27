import java.util.Scanner;

public class TestMax {
	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		System.out.print("Enter a number:");
		int i = scanner.nextInt();
		System.out.print("Enter other number:");
		int j = scanner.nextInt();
		int k = max(i, j);
		System.out.println("The maximum between " + i + " and " + j + " is "
				+ k);
		
		scanner.close();
	}

	public static int max(int num1, int num2) {
		int result;
		if (num1 > num2)
			result = num1;
		else
			result = num2;
		return result;

	}
}
