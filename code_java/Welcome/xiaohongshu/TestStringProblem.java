import java.util.Scanner;

/* ���������ַ���A��B, ɾ��A�а�����B�Ӵ� 
 * ���磺����"abcdabcaba","ab", �����"cdca" */

public class TestStringProblem {

	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		System.out.println("Please Enter String A:");
		String strA = sc.nextLine();
		System.out.println("Please Enter String B:");
		String strB = sc.nextLine();
		sc.close();
		
		String result = strA.replace(strB, "");
		
		System.out.printf("StringA is %s, StringB is %s, ", strA, strB);
		System.out.printf("result is %s", result);
	}

}
