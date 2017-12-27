import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;

@SuppressWarnings("serial")
public class NewRecordDialog extends JDialog {
	private JButton jbtOK = new JButton("OK");
	private JButton jbtCancel = new JButton("Cancel");
	private DefaultTableModel tableModel = new DefaultTableModel();
	private JTable jTable = new JTable(tableModel);
	private Vector<?> newRecord;

	public NewRecordDialog(Frame parent, boolean modal) {
		super(parent, modal);
		setTitle("Insert a New Record");
		setModal(true);

		JPanel jPanel1 = new JPanel();
		jPanel1.add(jbtOK);
		jPanel1.add(jbtCancel);

		jbtOK.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setVisible(false);
			}
		});

		jbtCancel.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setVisible(false);
			}
		});

		add(jPanel1, BorderLayout.SOUTH);
		add(new JScrollPane(jTable), BorderLayout.CENTER);
	}

	public NewRecordDialog() {
		this(null, true);
	}

	public Vector<?> getNewRecord() {
		return newRecord;
	}

	void displayTable(Vector<?> columnHeaderVector) {
		this.setSize(new Dimension(400, 100));
		tableModel.setColumnIdentifiers(columnHeaderVector);
		tableModel.addRow(newRecord = new Vector<Object>());
		setVisible(true);
	}
}
