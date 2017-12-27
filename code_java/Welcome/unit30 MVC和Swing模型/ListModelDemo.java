import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
// 列表模型
public class ListModelDemo extends JApplet {
	@SuppressWarnings("rawtypes")
	private DefaultListModel listModel = new DefaultListModel();
	@SuppressWarnings({ "unchecked", "rawtypes" })
	private JList jlst = new JList(listModel);
	private JButton jbtAdd = new JButton("Add new item");
	private JButton jbtRemove = new JButton("Remove selected item");

	@SuppressWarnings("unchecked")
	public ListModelDemo() {
		listModel.addElement("item1");
		listModel.addElement("item2");
		listModel.addElement("item3");
		listModel.addElement("item4");
		listModel.addElement("item5");
		listModel.addElement("item6");

		JPanel panel = new JPanel();
		panel.add(jbtAdd);
		panel.add(jbtRemove);
		add(panel, BorderLayout.NORTH);
		add(new JScrollPane(jlst), BorderLayout.CENTER);

		jbtAdd.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String newItem = JOptionPane.showInputDialog("Enter a item");
				if (newItem != null)
					if (jlst.getSelectedIndex() == -1)
						listModel.addElement(newItem);
					else
						listModel.add(jlst.getSelectedIndex(), newItem);
			}
		});

		jbtRemove.addActionListener(new ActionListener() {
			@SuppressWarnings("deprecation")
			public void actionPerformed(ActionEvent e) {
				// 删除一个元素
				// listModel.remove(jlst.getSelectedIndex());

				// 删除选定的元素
				Object[] n = jlst.getSelectedValues();
				for (Object i : n)
					listModel.removeElementAt(listModel.indexOf(i));
			}
		});
	}
}
