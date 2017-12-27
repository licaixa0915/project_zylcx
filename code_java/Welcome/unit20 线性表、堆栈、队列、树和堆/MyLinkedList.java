public class MyLinkedList extends MyAbstractList {
	private static class Node {
		Object element;
		Node next;

		public Node(Object o) {
			element = o;
		}
	}

	private Node first, last;

	public MyLinkedList() {
	}

	public MyLinkedList(Object[] objects) {
		super(objects);
	}

	public void add(int index, Object o) {
		if (index == 0)
			addFirst(o);
		else if (index >= size)
			addLast(o);
		else {
			Node current = first;
			for (int i = 1; i < index; i++)
				current = current.next;
			Node temp = current.next;
			current.next = new Node(o);
			(current.next).next = temp;
			size++;
		}
	}

	public void addFirst(Object o) {
		Node newNode = new Node(o);
		newNode.next = first;
		first = newNode;
		size++;
		if (last == null)
			last = first;
	}

	public void addLast(Object o) {
		if (last == null) {
			first = last = new Node(o);
		} else {
			last.next = new Node(o);
			last = last.next;
		}
		size++;
	}

	public void clear() {
		first = last = null;
	}

	public boolean contains(Object o) {
		Node current = first;
		for (int i = 0; i < size; i++) {
			if (current.element.equals(o))
				return true;
			current = current.next;
		}
		return false;
	}

	public Object get(int index) {
		if (index < 0 || index > size - 1)
			return null;
		Node current = first;
		for (int i = 0; i < index; i++)
			current = current.next;
		return current.element;
	}

	public Object getFirst() {
		if (size == 0)
			return null;
		else
			return first.element;
	}

	public Object getLast() {
		if (size == 0)
			return null;
		else
			return last.element;
	}

	public int indexOf(Object o) {
		Node current = first;
		for (int i = 0; i < size; i++) {
			if (current.element.equals(o))
				return i;
			current = current.next;
		}
		return -1;
	}

	public int lastIndexOf(Object o) {
		int lastIndex = -1;
		Node current = first;
		for (int i = 0; i < size; i++) {
			if (current.element.equals(o))
				lastIndex = i;
			current = current.next;
		}
		return lastIndex;
	}

	public Object remove(int index) {
		if ((index < 0) || (index >= size))
			return null;
		else if (index == 0)
			return removeFirst();
		else if (index == size - 1)
			return removeLast();
		else {
			Node previous = first;
			for (int i = 1; i < index; i++) {
				previous = previous.next;
			}
			Node current = previous.next;
			previous.next = current.next;
			size--;
			return current.element;
		}
	}

	public Object removeFirst() {
		if (size == 0)
			return null;
		else {
			Node temp = first;
			first = first.next;
			size--;
			if (first == null)
				last = null;
			return temp.element;
		}
	}

	public Object removeLast() {
		if (size == 0)
			return null;
		else if (size == 1) {
			Node temp = first;
			first = last = null;
			size = 0;
			return temp;
		} else {
			Node current = first;
			for (int i = 0; i < size - 2; i++) {
				current = current.next;
			}
			Node temp = last;
			last = current;
			last.next = null;
			size--;
			return temp.element;
		}
	}

	public Object set(int index, Object o) {
		return null;
	}

	public String toString() {
		StringBuffer result = new StringBuffer("[");
		Node current = first;
		for (int i = 0; i < size; i++) {
			result.append(current.element);
			current = current.next;
			if (current != null)
				result.append(", ");
			else
				result.append("]");
		}
		return result.toString();
	}
}
