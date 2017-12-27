public abstract class MyAbstractList implements MyList {
	protected int size = 0;

	protected MyAbstractList() {
	}

	protected MyAbstractList(Object[] objects) {
		for (int i = 0; i < objects.length; i++)
			this.add(objects[i]);
	}

	public void add(Object o) {
		add(size, o);
	}

	public boolean isEmpty() {
		return size == 0;
	}

	public boolean remove(Object o) {
		if (indexOf(o) >= 0) {
			remove(indexOf(o));
			return true;
		} else
			return false;
	}

	public int size() {
		return size;
	}
}
