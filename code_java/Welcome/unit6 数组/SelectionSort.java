public class SelectionSort {
	public static void selectionSort(double[] list) {
		for (int i = list.length - 1; i >= 1; i--) {
			double currentMax = list[0];
			int currentMaxIndex = 0;
			for (int j = 1; j <= i; j++)
				if (currentMax < list[j]) {
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
