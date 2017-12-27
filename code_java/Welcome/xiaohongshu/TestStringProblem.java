import java.util.Scanner;

/* 输入两个字符串A、B, 删除A中包含的B子串 
 * 例如：输入"abcdabcaba","ab", 则输出"cdca" */

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
