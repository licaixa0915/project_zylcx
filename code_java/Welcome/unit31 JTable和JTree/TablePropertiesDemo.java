import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;

@SuppressWarnings("serial")
public class TablePropertiesDemo extends JApplet {
	private String[] columnNames = { "Country", "Capital",
			"Population in Millions", "Democracy" };
	Object[][] rowData = { { "USA", "Washington DC", 280, true },
			{ "Canada", "Ottawa", 32, true },
			{ "United Kingdom", "London", 60, true },
			{ "Germany", "Berlin", 83, true }, { "France", "Paris", 60, true },
			{ "Norway", "Oslo", 4.5, true },
			{ "India", "New Deli", 1046, true } };
	private JTable jTable1 = new JTable(rowData, columnNames);
	private JSpinner jspiRowHeight = new JSpinner(new SpinnerNumberModel(16, 1,
			50, 1));
	private JSpinner jspiRowMargin = new JSpinner(new SpinnerNumberModel(1, 1,
			50, 1));
	private JCheckBox jchkShowGrid = new JCheckBox("showGrid", true);
	private JComboBox<?> jcboAutoResizeMode = new JComboBox<Object>(new String[] {
			"AUTO_RESIZE_OFF", "AUTO_RESIZE_LAST_COLUMN",
			"AUTO_RESIZE_SUBSEQUENT_COLUMNS", "AUTO_RESIZE_NEXT_COLUMN",
			"AUTO_RESIZE_ALL_COLUMNS" });

	public TablePropertiesDemo() {
		JPanel panel1 = new JPanel();
		panel1.add(new JLabel("rowHeight"));
		panel1.add(jspiRowHeight);
		panel1.add(new JLabel("roeMargin"));
		panel1.add(jspiRowMargin);
		panel1.add(jchkShowGrid);

		JPanel panel2 = new JPanel();
		panel2.add(new JLabel("autoResizeMode"));
		panel2.add(jcboAutoResizeMode);

		add(panel1, BorderLayout.SOUTH);
		add(panel2, BorderLayout.NORTH);
		add(new JScrollPane(jTable1));

		jTable1.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
		jTable1.setGridColor(Color.BLUE);
		jTable1.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		jTable1.setSelectionBackground(Color.RED);
		jTable1.setSelectionForeground(Color.WHITE);

		jspiRowHeight.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				jTable1.setRowHeight(((Integer) (jspiRowHeight.getValue()))
						.intValue());
			}
		});

		jspiRowMargin.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				jTable1.setRowMargin(((Integer) (jspiRowMargin.getValue()))
						.intValue());
			}
		});

		jchkShowGrid.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jTable1.setShowGrid(jchkShowGrid.isSelected());
			}
		});

		jcboAutoResizeMode.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String selectedItem = (String) jcboAutoResizeMode
						.getSelectedItem();
				if (selectedItem.equals("AUTO_RESIZE_OFF"))
					jTable1.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
				else if (selectedItem.equals("AUTO_RESIZE_LAST_COLUMN"))
					jTable1.setAutoResizeMode(JTable.AUTO_RESIZE_LAST_COLUMN);
				else if (selectedItem.equals("AUTO_RESIZE_SUBSEQUENT_COLUMNS"))
					jTable1
							.setAutoResizeMode(JTable.AUTO_RESIZE_SUBSEQUENT_COLUMNS);
				else if (selectedItem.equals("AUTO_RESIZE_LAST_COLUMN"))
					jTable1.setAutoResizeMode(JTable.AUTO_RESIZE_LAST_COLUMN);
				else if (selectedItem.equals("AUTO_RESIZE_NEXT_COLUMN"))
					jTable1.setAutoResizeMode(JTable.AUTO_RESIZE_NEXT_COLUMN);
				else if (selectedItem.equals("AUTO_RESIZE_ALL_COLUMNS"))
					jTable1.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
			}
		});
	}
}
