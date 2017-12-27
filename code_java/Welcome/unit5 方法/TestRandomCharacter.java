public class TestRandomCharacter {
	public static void main(String[] args) {
		final int NUMBER_OF_CHAR = 175;
		final int CHAR_PER_LINE = 25;

		for (int i = 1; i <= NUMBER_OF_CHAR; i++) {
			char ch = RandomCharacter.getRandomLowerCaseLetter();
			if (i % CHAR_PER_LINE == 0)
				System.out.println(ch);
			else
				System.out.print(ch);
		}
	}
}
