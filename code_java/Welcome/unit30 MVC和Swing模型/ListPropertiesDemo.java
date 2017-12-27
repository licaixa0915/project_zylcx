import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;

@SuppressWarnings("serial")
// ап╠М©Р
public class ListPropertiesDemo extends JApplet {
	private JList<?> jlst = new JList<Object>(new String[] { "Item1", "Item2", "Item3",
			"Item4", "Item5", "Item6" });
	private JSpinner jspinnerVisibleRowCount = new JSpinner(
			new SpinnerNumberModel(8, -1, 20, 1));
	private JComboBox<?> jcboLayoutOrientation = new JComboBox<Object>(new String[] {
			"VERTICAL (1)", "VERTICAL_WRAP (2)", "HORIZONTAL_WRAP (2)" });
	private JComboBox<?> jcboSelectionMode = new JComboBox<Object>(new String[] {
			"SINGLE_SELECTION (0)", "SINGLE_INTERVAL_SELECTION (1)",
			"MULTIPLE_INTERVAL_SELECTION (2)" });
	private JLabel jlblStatues = new JLabel();

	public ListPropertiesDemo() {
		JPanel panel1 = new JPanel();
		panel1.setLayout(new GridLayout(3, 1));
		panel1.add(new JLabel("visibleRowCount"));
		panel1.add(new JLabel("layoutOrientation"));
		panel1.add(new JLabel("selectionMode"));

		JPanel panel2 = new JPanel();
		panel2.setLayout(new GridLayout(3, 1));
		panel2.add(jspinnerVisibleRowCount);
		panel2.add(jcboLayoutOrientation);
		panel2.add(jcboSelectionMode);

		JPanel panel3 = new JPanel();
		panel3.setLayout(new BorderLayout(5, 5));
		panel3.add(panel1, BorderLayout.WEST);
		panel3.add(panel2, BorderLayout.CENTER);

		add(panel3, BorderLayout.NORTH);
		add(new JScrollPane(jlst), BorderLayout.CENTER);
		add(jlblStatues, BorderLayout.SOUTH);

		jlst.setFixedCellWidth(50);
		jlst.setFixedCellHeight(20);
		jlst.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		jspinnerVisibleRowCount.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				jlst.setVisibleRowCount(((Integer) jspinnerVisibleRowCount
						.getValue()).intValue());
			}
		});

		jcboLayoutOrientation.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlst.setLayoutOrientation(jcboLayoutOrientation
						.getSelectedIndex());
			}
		});

		jcboSelectionMode.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlst.setSelectionMode(jcboSelectionMode.getSelectedIndex());
			}
		});

		jlst.addListSelectionListener(new ListSelectionListener() {
			@SuppressWarnings("deprecation")
			public void valueChanged(ListSelectionEvent e) {
				Object[] values = jlst.getSelectedValues();
				String display = " ";
				for (int i = 0; i < values.length; i++) {
					display += (String) values[i] + " ";
				}
				jlblStatues.setText(display);
			}
		});
	}
}
