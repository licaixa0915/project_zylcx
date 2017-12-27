public class CountLettersInArray {
	public static int[] countLetters(char[] chars) {
		int[] counts = new int[26];
		for (int i = 0; i < chars.length; i++)
			counts[chars[i] - 'a']++;
		return counts;
	}

	public static char[] creatArray() {
		char[] chars = new char[100];
		for (int i = 0; i < chars.length; i++)
			chars[i] = RandomCharacter.getRandomLowerCaseLetter();
		return chars;
	}

	public static void displayArray(char[] chars) {
		for (int i = 0; i < chars.length; i++)
			if ((i + 1) % 20 == 0)
				System.out.println(chars[i] + " ");
			else
				System.out.print(chars[i] + " ");
	}

	public static void displayCounts(int[] counts) {
		for (int i = 0; i < counts.length; i++)
			if ((i + 1) % 10 == 0)
				System.out.println(counts[i] + "" + (char) (i + 'a')); // 如果没有+"",则显示int
			else
				System.out.print(counts[i] + "" + (char) (i + 'a') + " "); // 注意
		// count[i]
		// +
		// ""
		// +
		// (char)(i+'a')
	}

	public static void main(String[] args) {
		char[] chars = creatArray();
		System.out.println("The lowercase letters are:");
		displayArray(chars);
		int[] counts = countLetters(chars);
		System.out.println();
		System.out.println("The occurrences of each letter are:");
		displayCounts(counts);
	}
}
