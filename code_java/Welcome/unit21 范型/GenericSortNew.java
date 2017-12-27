public class GenericSortNew {
	public static void main(String[] args) {
		String[] stringArray = { "Tom", "John", "Fred" };

		sort(stringArray);
		for (int i = 0; i < stringArray.length; i++)
			System.out.print(stringArray[i] + " ");
	}

	public static <E extends Comparable<E>> void sort(E[] list) {
		E currentMax;
		int currentMaxIndex;

		for (int i = list.length - 1; i >= 1; i--) {
			currentMax = list[i];
			currentMaxIndex = i;
			for (int j = i - 1; j >= 0; j--)
				if (currentMax.compareTo(list[j]) < 0) {
					currentMax = list[j];
					currentMaxIndex = j;
				}
			if (currentMaxIndex != i) {
				list[currentMaxIndex] = list[i];
				list[i] = currentMax;
			}
		}
	}
}
