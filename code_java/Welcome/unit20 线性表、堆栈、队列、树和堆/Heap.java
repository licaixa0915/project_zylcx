public class Heap {
	@SuppressWarnings({ "rawtypes" })
	private java.util.ArrayList list = new java.util.ArrayList();

	public Heap() {
	}

	public Heap(Object[] objects) {
		for (int i = 0; i < objects.length; i++)
			add(objects[i]);
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public void add(Object newObject) {
		list.add(newObject);
		int currentIndex = list.size() - 1;
		while (currentIndex > 0) {
			int parentIndex = (currentIndex - 1) / 2;
			if (((Comparable) (list.get(currentIndex))).compareTo(list
					.get(parentIndex)) > 0) {
				Object temp = list.get(currentIndex);
				list.set(currentIndex, list.get(parentIndex));
				list.set(parentIndex, temp);
			} else
				break;
			currentIndex = parentIndex;
		}
	}

	public int getSize() {
		return list.size();
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public Object remove() {
		if (list.size() == 0)
			return null;
		Object removeObject = list.get(0);
		list.set(0, list.get(list.size() - 1));
		list.remove(list.size() - 1);
		int currentIndex = 0;
		while (currentIndex < list.size()) {
			int leftChildIndex = 2 * currentIndex + 1;
			int rightChildIndex = 2 * currentIndex + 2;
			if (leftChildIndex >= list.size())
				break;
			int maxIndex = leftChildIndex;
			if (rightChildIndex < list.size()) {
				if (((Comparable) (list.get(maxIndex))).compareTo(list
						.get(rightChildIndex)) < 0) {
					maxIndex = rightChildIndex;
				}
			}
			if (((Comparable) (list.get(currentIndex))).compareTo(list
					.get(maxIndex)) < 0) {
				Object temp = list.get(maxIndex);
				list.set(maxIndex, list.get(currentIndex));
				list.set(currentIndex, temp);
				currentIndex = maxIndex;
			} else
				break;
		}
		return removeObject;
	}
}
