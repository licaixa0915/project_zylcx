public class IntegerMatrix extends GenericMatrix<Integer> {
	protected Integer add(Integer o1, Integer o2) {
		return new Integer(o1.intValue() + o2.intValue());
	}

	protected Integer multiply(Integer o1, Integer o2) {
		return new Integer(o1.intValue() * o2.intValue());
	}

	protected Integer zero() {
		return new Integer(0);
	}
}
