@SuppressWarnings({ "rawtypes" })
public final class Name implements Comparable {
	private String firstName;
	private String lastName;
	private char mi;

	public Name(String firstname, char mi, String lastName) {
		this.firstName = firstname;
		this.mi = mi;
		this.lastName = lastName;
	}

	public int compareTo(Object o) {
		if (!lastName.equals(((Name) o).lastName))
			return lastName.compareTo(((Name) o).lastName);
		else if (!firstName.equals(((Name) o).firstName))
			return firstName.compareTo(((Name) o).firstName);
		else
			return mi - ((Name) o).mi;
	}

	public String getFirstName() {
		return firstName;
	}

	public String getFullName() {
		return firstName + ' ' + mi + ' ' + lastName;
	}

	public String getLastName() {
		return lastName;
	}

	public char getMi() {
		return mi;
	}
}
