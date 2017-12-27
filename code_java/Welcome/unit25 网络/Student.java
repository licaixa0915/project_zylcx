@SuppressWarnings("serial")
public class Student implements java.io.Serializable {
	private String city;
	private String name;
	private String state;
	private String street;
	private String zip;
	
	public Student(String name, String street, String city, String state, String zip) {
		this.name = name;
		this.street = street;
		this.city = city;
		this.state = state;
		this.zip = zip;
	}

	public String getCity() {
		return city;
	}

	public String getName() {
		return name;
	}

	public String getState() {
		return state;
	}

	public String getStreet() {
		return street;
	}

	public String getZip() {
		return zip;
	}
}
	