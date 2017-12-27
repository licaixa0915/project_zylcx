public class IsPalindrome2 {
	public static boolean isPalindrome2(String s) {
		return isPalindrome2(s, 0, s.length() - 1);
	}

	public static boolean isPalindrome2(String s, int low, int high) {
		if (high <= low)
			return true;
		else if (s.charAt(low) != s.charAt(high))
			return false;
		else
			return isPalindrome2(s, low + 1, high - 1);
	}
}
