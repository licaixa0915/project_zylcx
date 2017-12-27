public class MyStack {
	@SuppressWarnings({ "rawtypes" })
	private java.util.ArrayList list = new java.util.ArrayList();

	public int getSize() {
		return list.size();
	}

	public boolean isEmpty() {
		return list.isEmpty();
	}

	public Object peek() {
		return list.get(getSize() - 1);
	}

	public Object pop() {
		Object o = list.get(getSize() - 1);
		list.remove(getSize() - 1);
		return o;
	}

	@SuppressWarnings("unchecked")
	public Object push(Object o) {
		list.add(o);
		return o;
	}

	public int search(Object o) {
		return list.lastIndexOf(o);
	}

	public String toString() {
		return "Stack: " + list.toString();
	}
}
