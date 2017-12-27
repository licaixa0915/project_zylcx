public class MyQueue {
	private MyLinkedList list = new MyLinkedList();

	public Object dequeue() {
		return list.removeFirst();
	}

	public void enqueue(Object o) {
		list.addLast(o);
	}

	public int getsize() {
		return list.size();
	}

	public String toString() {
		return "Queue: " + list.toString();
	}
}
