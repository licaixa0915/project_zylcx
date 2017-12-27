public class Sort {
	public static void sort(double[] list) {
		sort(list, list.length - 1);
	}

	public static void sort(double[] list, int high) {
		if (high > 1) {
			int indexOfMax = 0;
			double max = list[0];
			for (int i = 1; i <= high; i++) {
				if (list[i] > max) {
					max = list[i];
					indexOfMax = i;
				}
			}
			list[indexOfMax] = list[high];
			list[high] = max;
			sort(list, high - 1);
		}
	}
}
