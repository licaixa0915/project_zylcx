import javax.swing.*;
import javax.swing.table.*;

@SuppressWarnings("serial")
public class TestTableModel extends JApplet {
	private String[] columnNames = { "Country", "Capital",
			"Population in Millions", "Democracy" };
	Object[][] data = { { "USA", "Washington DC", 280, true },
			{ "Canada", "Ottawa", 32, true } };
	DefaultTableModel tableModel = new DefaultTableModel(data, columnNames);
	JTable jTable1 = new JTable(tableModel);

	public TestTableModel() {
		add(new JScrollPane(jTable1));

		tableModel.addRow(new Object[] { "France", "Paris", 60, true });
		tableModel.insertRow(0,
				new Object[] { "India", "New Deli", 1046, true });
		tableModel.removeRow(1);
		tableModel.addColumn("Area");
		tableModel.setValueAt(10, 0, 4);
		tableModel.setValueAt(20, 1, 4);
		tableModel.setValueAt(30, 2, 4);
	}
}
