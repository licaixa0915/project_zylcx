import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;

import java.awt.*;
import java.awt.event.*;
import java.sql.*;
import java.util.*;

@SuppressWarnings("serial")
public class TableEditor extends JPanel {
	private NewRecordDialog newRecordDialog = new NewRecordDialog();
	private Connection connection;
	private String tableName;
	private Statement statement;
	private ResultSet resultSet;
	private DefaultTableModel tableModel = new DefaultTableModel();
	private DefaultListSelectionModel listSelectionModel = new DefaultListSelectionModel();
	private Vector<Vector<Object>> rowVectors = new Vector<Vector<Object>>();
	private Vector<String> columnHeaderVector = new Vector<String>();
	private int columnCount;

	private JButton jbtFirst = new JButton("First");
	private JButton jbtNext = new JButton("Next");
	private JButton jbtPrior = new JButton("Prior");
	private JButton jbtLast = new JButton("Last");
	private JButton jbtInsert = new JButton("Insert");
	private JButton jbtDelete = new JButton("Delete");
	private JButton jbtUpdate = new JButton("Update");
	private JLabel jlblStatus = new JLabel();
	private JTable jTable1 = new JTable();

	/** Create a TableEditor */
	public TableEditor() {
		JPanel jPanel1 = new JPanel();
		jPanel1.add(jbtFirst);
		jPanel1.add(jbtNext);
		jPanel1.add(jbtPrior);
		jPanel1.add(jbtLast);
		jPanel1.add(jbtInsert);
		jPanel1.add(jbtDelete);
		jPanel1.add(jbtUpdate);

		setLayout(new BorderLayout());
		add(jPanel1, BorderLayout.NORTH);
		add(new JScrollPane(jTable1), BorderLayout.CENTER);
		add(jlblStatus, BorderLayout.SOUTH);

		// Set selection model for the table
		jTable1.setModel(tableModel);
		jTable1.setSelectionModel(listSelectionModel);

		// Register listeners
		jbtFirst.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				moveCursor("first");
			}
		});
		jbtNext.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				moveCursor("next");
			}
		});
		jbtPrior.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				moveCursor("previous");
			}
		});
		jbtLast.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				moveCursor("last");
			}
		});
		jbtInsert.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				insert();
			}
		});
		jbtDelete.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				delete();
			}
		});
		jbtUpdate.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				update();
			}
		});
		listSelectionModel
				.addListSelectionListener(new ListSelectionListener() {
					public void valueChanged(ListSelectionEvent e) {
						listSelectionModel_valueChanged(e);
					}
				});
	}

	/* Delete a row */
	private void delete() {
		try {
			resultSet.deleteRow();
			refreshResultSet();
			tableModel.removeRow(listSelectionModel.getLeadSelectionIndex());
		} catch (Exception ex) {
			jlblStatus.setText(ex.toString());
		}
	}

	private void insert() {
		newRecordDialog.displayTable(columnHeaderVector);
		Vector<?> newRecord = newRecordDialog.getNewRecord();
		if (newRecord == null)
			return;
		tableModel.addRow(newRecord);
		try {
			for (int i = 1; i < columnCount; i++) {
				resultSet.updateObject(i, newRecord.elementAt(i - 1));
			}
			resultSet.insertRow();
			refreshResultSet();
		} catch (Exception ex) {
			jlblStatus.setText(ex.toString());
		}
	}

	/** Set cursor in the table and set the row number in the status */
	private void setTableCursor() throws Exception {
		int row = resultSet.getRow();
		listSelectionModel.setSelectionInterval(row - 1, row - 1);
		jlblStatus.setText("Current row number: " + row);
	}

	private void update() {
		try {
			int row = jTable1.getSelectedRow();
			for (int i = 1; i <= resultSet.getMetaData().getColumnCount(); i++) {
				resultSet.updateObject(i, tableModel.getValueAt(row, i - 1));
			}
			resultSet.updateRow();
			refreshResultSet();
		} catch (Exception ex) {
			jlblStatus.setText(ex.toString());
		}
	}

	/** Move cursor to the specified location */
	private void moveCursor(String whereToMove) {
		try {
			if (whereToMove.equals("first"))
				resultSet.first();
			else if (whereToMove.equals("next") && !resultSet.isLast())
				resultSet.next();
			else if (whereToMove.equals("previous") && !resultSet.isFirst())
				resultSet.previous();
			else if (whereToMove.equals("last"))
				resultSet.last();
			setTableCursor();
		} catch (Exception ex) {
			jlblStatus.setText(ex.toString());
		}
	}

	/** Refresh the result set */
	private void refreshResultSet() {
		try {
			resultSet = statement.executeQuery("SELECT * FROM " + tableName);
			moveCursor("first");
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
	}

	/** Set database connection and table name in the TableEditor */
	public void setConnectionAndTable(Connection newConnection,
			String newTableName) {
		connection = newConnection;
		tableName = newTableName;
		try {
			statement = connection
					.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE,
							ResultSet.CONCUR_UPDATABLE);
			showTable();
			moveCursor("first");
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
	}

	/** Display database table to a Swing table */
	private void showTable() throws SQLException {
		rowVectors.clear();
		columnHeaderVector.clear();
		resultSet = statement.executeQuery("select * from " + tableName + ";");
		columnCount = resultSet.getMetaData().getColumnCount();
		while (resultSet.next()) {
			Vector<Object> singleRow = new Vector<Object>();
			for (int i = 0; i < columnCount; i++)
				singleRow.addElement(resultSet.getObject(i + 1));
			rowVectors.addElement(singleRow);
		}
		for (int i = 1; i <= columnCount; i++)
			columnHeaderVector.addElement(resultSet.getMetaData()
					.getColumnName(i));
		tableModel.setDataVector(rowVectors, columnHeaderVector);
	}

	/** Handle the selection in the table */
	void listSelectionModel_valueChanged(ListSelectionEvent e) {
		int selectedRow = jTable1.getSelectedRow();
		try {
			resultSet.absolute(selectedRow + 1);
			setTableCursor();
		} catch (Exception ex) {
			jlblStatus.setText(ex.toString());
		}
	}

}
