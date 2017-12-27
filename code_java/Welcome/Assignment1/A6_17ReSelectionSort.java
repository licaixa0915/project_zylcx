//¸ÄÐ´Ñ¡ÔñÅÅÐò
public class A6_17ReSelectionSort {
	public static void selectionSort(double[] list) {
		for (int i = 0; i < list.length; i++) {
			double currentMin = list[0];
			int currentMinIndex = 0;
			for (int j = i + 1; j <= list.length; j++)
				if (currentMin > list[j]) {
					currentMin = list[j];
					currentMinIndex = j;
				}
			if (currentMinIndex != i) {
				list[currentMinIndex] = list[i];
				list[i] = currentMin;
			}
		}
	}
}
