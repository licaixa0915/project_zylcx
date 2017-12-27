import javax.swing.*;
import javax.swing.table.*;
import java.util.*;

@SuppressWarnings("serial")
public class MyTableModel extends DefaultTableModel {
	public MyTableModel() {
	}

	public MyTableModel(Object[][] data, Object[] columnNames) {
		super(data, columnNames);
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public Class getColumnClass(int column) {
		return getValueAt(0, column).getClass();
	}

	@SuppressWarnings("rawtypes")
	public boolean isCellEditable(int row, int Column) {
		Class columnClass = getColumnClass(Column);
		return columnClass != ImageIcon.class && columnClass != Date.class;
	}
}
