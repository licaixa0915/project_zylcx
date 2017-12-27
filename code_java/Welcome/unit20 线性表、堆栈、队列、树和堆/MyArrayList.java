public class MyArrayList extends MyAbstractList {
	public static final int INITIAL_CAPACITY = 16;
	private Object[] data = new Object[INITIAL_CAPACITY];

	public MyArrayList() {
	}

	public MyArrayList(Object[] objects) {
		data = objects;
		size = objects.length;
	}

	public void add(int index, Object o) {
		ensureCapacity();
		for (int i = size - 1; i >= index; i--)
			data[i + 1] = data[i];
		data[index] = o;
		size++;
	}

	public void clear() {
		data = new Object[INITIAL_CAPACITY];
	}

	public boolean contains(Object o) {
		for (int i = 0; i < size; i++)
			if (o.equals(data[i]))
				return true;
		return false;
	}

	private void ensureCapacity() {
		if (size >= data.length) {
			Object[] newData = new Object[data.length * 2];
			System.arraycopy(data, 0, newData, 0, data.length);
			data = newData;
		}
	}

	public Object get(int index) {
		return data[index];
	}

	public int indexOf(Object o) {
		for (int i = 0; i < size; i++)
			if (o.equals(data[i]))
				return i;
		return -1;
	}

	public int lastIndexOf(Object o) {
		for (int i = size - 1; i >= 0; i--)
			if (o.equals(data[i]))
				return i;
		return -1;
	}

	public Object remove(int index) {
		Object o = data[index];
		for (int j = index; j < size - 1; j++)
			data[j] = data[j + 1];
		size--;
		return o;
	}

	public Object set(int index, Object o) {
		Object old = data[index];
		data[index] = o;
		return old;
	}

	public String toString() {
		StringBuffer result = new StringBuffer("[");
		for (int i = 0; i < size; i++) {
			result.append(data[i]);
			if (i < size - 1)
				result.append(", ");
		}
		return result.toString() + "]";
	}
}
