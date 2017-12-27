import javax.swing.*;

@SuppressWarnings("serial")
public class TestTable extends JApplet {
	String[] columnNames = { "Country", "Capital", "Population in Millions",
			"Democracy" };
	Object[][] data = { { "USA", "Washington DC", 280, true },
			{ "Canada", "Ottawa", 32, true },
			{ "United Kingdom", "London", 60, true },
			{ "Germany", "Berlin", 83, true }, { "France", "Paris", 60, true },
			{ "Norway", "Oslo", 4.5, true },
			{ "India", "New Deli", 1046, true } };
	JTable jTable1 = new JTable(data, columnNames);

	public TestTable() {
		add(new JScrollPane(jTable1));
	}
}
