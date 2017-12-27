public class MyPriorityQueue {
	private Heap heap = new Heap();

	public Object dequeue() {
		return heap.remove();
	}

	public void enqueue(Object newObject) {
		heap.add(newObject);
	}

	public int getSize() {
		return heap.getSize();
	}
}
