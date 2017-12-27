import javax.swing.*;
import javax.swing.table.*;

@SuppressWarnings("serial")
public class TestTableColumnModel extends JApplet {
	String[] columnNames = { "Country", "Capital", "Population in Millions",
			"Democracy" };
	Object[][] data = { { "USA", "Washington DC", 280, true },
			{ "Canada", "Ottawa", 32, true } };
	JTable jTable1 = new JTable(data, columnNames);

	public TestTableColumnModel() {
		add(new JScrollPane(jTable1));
		TableColumnModel columnModel = jTable1.getColumnModel();
		columnModel.moveColumn(0, 1);
		columnModel.removeColumn(columnModel.getColumn(3));
	}
}
