@SuppressWarnings({ "rawtypes" })
public class House implements Cloneable, Comparable {
	private double area;
	private int id;
	private java.util.Date whenBuilt;

	public House(int id, double area) {
		this.id = id;
		this.area = area;
		whenBuilt = new java.util.Date();
	}

	public Object clone() {
		try {
			return super.clone();
		} catch (CloneNotSupportedException ex) {
			return null;
		}
	}

	public int compareTo(Object o) {
		if (area > ((House) o).area)
			return 1;
		else if (area < ((House) o).area)
			return -1;
		else
			return 0;
	}

	public double getArea() {
		return area;
	}

	public double getId() {
		return id;
	}

	public java.util.Date getWhenBuilt() {
		return whenBuilt;

	}
}
