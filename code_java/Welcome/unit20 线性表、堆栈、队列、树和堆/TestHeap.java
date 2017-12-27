public class TestHeap {
	public static void main(String[] args) {
		Heap heap = new Heap(new Integer[] { 8, 9, 2, 3, 4, 1, 5, 6, 7 });
		Integer[] i = new Integer[9];
		int k = 0;
		while (heap.getSize() > 0) {
			// System.out.print(heap.remove() + " "); // ½µĞòÅÅÁĞ
			i[i.length - 1 - k] = (Integer) heap.remove(); // ÉıĞòÅÅÁĞ
			k++;
		}
		System.out.println();
		for (int j = 0; j < i.length; j++)
			System.out.print(i[j] + " ");
	}
}
