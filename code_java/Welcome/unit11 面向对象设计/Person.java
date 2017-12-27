public class Person {
	private Address address;
	private Name name;

	public Person() {
		this(new Name("Jill", 'S', "Barr"), new Address("100 Main", "Savannah",
				"GA", "31411"));
	}

	public Person(Name name, Address address) {
		this.name = name;
		this.address = address;
	}

	public int compareTo(Object o) {
		return name.compareTo(((Person) o).name);
	}

	public Address getAddress() {
		return address;
	}

	public Name getName() {
		return name;
	}

	public void setAddress(Address address) {
		this.address = address;
	}

	public void setName(Name name) {
		this.name = name;
	}

	public String toString() {
		return '\n' + name.getFullName() + '\n' + address.getFullAddress()
				+ '\n';
	}
}
