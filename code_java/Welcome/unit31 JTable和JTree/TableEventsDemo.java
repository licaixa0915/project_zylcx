import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

import java.util.*;

@SuppressWarnings("serial")
// 表格模型事件
public class TableEventsDemo extends JApplet {
	private String[] columnNames = { "Title", "Copies Needed", "Publisher",
			"Date Published", "In-stock", "Book Photo" };
	private ImageIcon intro1eImageIcon = new ImageIcon(getClass().getResource(
			"image/intro1e.gif"));
	private ImageIcon intro2eImageIcon = new ImageIcon(getClass().getResource(
			"image/intro2e.gif"));
	private ImageIcon intro3eImageIcon = new ImageIcon(getClass().getResource(
			"image/intro3e.jpg"));
	private Object[][] rowData = {
			{ "Introduction to Java Programming", 120,
					"Que Education & Training",
					new GregorianCalendar(1998, 1 - 1, 6).getTime(), false,
					intro1eImageIcon },
			{ "Introduction to Java Programming, 2E", 220,
					"Que Education & Training",
					new GregorianCalendar(1999, 1 - 1, 6).getTime(), false,
					intro2eImageIcon },
			{ "Introduction to Java Programming, 3E", 220, "Prentice Hall",
					new GregorianCalendar(2000, 12 - 1, 0).getTime(), true,
					intro3eImageIcon } };
	private MyTableModel tableModel = new MyTableModel(rowData, columnNames);
	private JTable jTable1 = new JTable(tableModel);
	private TableColumnModel tableColumnModel = jTable1.getColumnModel();
	private ListSelectionModel selectionModel = jTable1.getSelectionModel();
	private JTextArea jtaMessage = new JTextArea();
	private JButton jbtDeleteColumn = new JButton("Delete Selected Column");

	public TableEventsDemo() {
		TableColumn bookCover = jTable1.getColumn("Book Photo");
		bookCover.setCellRenderer(new MyImageCellRenderer());

		JComboBox<String> jcboPublishers = new JComboBox<String>();
		jcboPublishers.addItem("Prentice Hall");
		jcboPublishers.addItem("Que Education & Training");
		jcboPublishers.addItem("McGraw-Hill");

		TableColumn publisherColumn = jTable1.getColumn("Publisher");
		publisherColumn.setCellEditor(new DefaultCellEditor(jcboPublishers));

		jTable1.setRowHeight(160);
		jTable1.setColumnSelectionAllowed(true);
		JSplitPane jSplitPane1 = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		jSplitPane1.add(new JScrollPane(jTable1), JSplitPane.LEFT);
		jSplitPane1.add(new JScrollPane(jtaMessage), JSplitPane.RIGHT);
		add(jbtDeleteColumn, BorderLayout.NORTH);
		add(jSplitPane1, BorderLayout.CENTER);

		tableModel.addTableModelListener(new TableModelListener() {
			public void tableChanged(TableModelEvent e) {
				jtaMessage.append("Table changed at row " + e.getFirstRow()
						+ " and column " + e.getColumn() + "\n");
			}
		});

		tableColumnModel.addColumnModelListener(new TableColumnModelListener() {
			public void columnSelectionChanged(ListSelectionEvent e) {
			}

			public void columnRemoved(TableColumnModelEvent e) {
				jtaMessage.append("Column indexed at " + e.getFromIndex()
						+ " is deleted \n");
			}

			public void columnMoved(TableColumnModelEvent e) {
			}

			public void columnMarginChanged(ChangeEvent e) {
			}

			public void columnAdded(TableColumnModelEvent e) {
			}
		});

		jbtDeleteColumn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (jTable1.getSelectedColumn() >= 0) {
					TableColumnModel columnModel = jTable1.getColumnModel();
					TableColumn tableColumn = columnModel.getColumn(jTable1
							.getSelectedColumn());
					columnModel.removeColumn(tableColumn);
				}
			}
		});

		selectionModel.addListSelectionListener(new ListSelectionListener() {
			public void valueChanged(ListSelectionEvent e) {
				if (e.getValueIsAdjusting()) {     // *****如果不判断，会显示两条记录信息
					jtaMessage.append("Row " + jTable1.getSelectedRow()
							+ " and column " + jTable1.getSelectedColumn()
							+ " selected\n");
				}
			}
		});
	}
}
