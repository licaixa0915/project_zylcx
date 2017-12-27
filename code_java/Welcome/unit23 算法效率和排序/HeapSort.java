public class HeapSort {
	public static void heapSort(int[] list) {
		for (int i = 1; i < list.length; i++) {
			makeHeap(list, i);
		}
		for (int last = list.length - 1; last > 0;) {
			int temp = list[last];
			list[last] = list[0];
			list[0] = temp;
			rebuildHeap(list, --last);
		}
	}

	// 创建初始堆
	private static void makeHeap(int[] list, int k) {
		int currentIndex = k;

		while (currentIndex > 0
				&& list[currentIndex] > list[(currentIndex - 1) / 2]) {
			int temp = list[currentIndex];
			list[currentIndex] = list[(currentIndex - 1) / 2];
			list[(currentIndex - 1) / 2] = temp;
			currentIndex = (currentIndex - 1) / 2;
		}
	}

	// 重建堆
	private static void rebuildHeap(int[] list, int last) {
		int currentIndex = 0;
		boolean isHeap = false;

		while (!isHeap) {
			int leftChildIndex = 2 * currentIndex + 1;
			int rightChildIndex = 2 * currentIndex + 2;
			int maxIndex = currentIndex;
			if (leftChildIndex <= last && list[maxIndex] < list[leftChildIndex]) {
				maxIndex = leftChildIndex;
			}
			if (rightChildIndex <= last
					&& list[maxIndex] < list[rightChildIndex]) {
				maxIndex = rightChildIndex;
			}
			if (maxIndex != currentIndex) {
				int temp = list[currentIndex];
				list[currentIndex] = list[maxIndex];
				list[maxIndex] = temp;
				currentIndex = maxIndex;
			} else {
				isHeap = true;
			}
		}
	}
}
