public final class Address {
	private String city;
	private String state;
	private String street;
	private String zip;

	public Address(String street, String city, String state, String zip) {
		this.street = street;
		this.city = city;
		this.state = state;
		this.zip = zip;
	}

	public String getCity() {
		return city;
	}

	public String getFullAddress() {
		return street + '\n' + city + "," + state + ' ' + zip + '\n';
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
