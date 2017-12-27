public class StackOfIntegers {
	public static final int DEFAULT_CAPACITY = 16;
	private int[] elements;
	private int size;

	public StackOfIntegers() {
		this(DEFAULT_CAPACITY);
	}

	public StackOfIntegers(int capacity) {
		elements = new int[capacity];
	}

	public boolean empty() {
		return size == 0;
	}

	public int getSize() {
		return size;
	}

	public int peek() {
		return elements[size - 1];
	}

	public int pop() {
		return elements[--size];
	}

	public int push(int value) {
		if (size > elements.length) {
			int[] temp = new int[elements.length * 2];
			System.arraycopy(elements, 0, temp, 0, elements.length);
			elements = temp;
		}
		return elements[size++] = value;
	}
}
