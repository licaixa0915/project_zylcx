import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;
import java.util.*;

@SuppressWarnings("serial")
public class SimpleTreeDemo extends JApplet {
	private JTree jTree1 = new JTree();
	private JTree jTree2 = new JTree(new String[] { "dog", "cow", "cat", "pig",
			"rabbit" });
	private Vector<Object> vector = new Vector<Object>(Arrays.asList(new Object[] { "red",
			"green", "black", "white", "purple" }));
	private JTree jTree3 = new JTree(vector);
	private Hashtable<Integer, String> hashtable = new Hashtable<Integer, String>();
	private JTree jTree4;
	private JComboBox<?> jcboRootVisible = new JComboBox<Object>(new String[] { "false",
			"true" });
	private JComboBox<?> jcboShowRootHandles = new JComboBox<Object>(new String[] {
			"false", "true" });
	private JSpinner jSpinnerRowHeight = new JSpinner(new SpinnerNumberModel(
			16, 1, 50, 1));

	public SimpleTreeDemo() {
		jTree1.setRootVisible(false);

		hashtable.put(1, "red");
		hashtable.put(2, "green");
		hashtable.put(3, "blue");
		hashtable.put(4, "yellow");
		jTree4 = new JTree(hashtable);

		JPanel panel1 = new JPanel(new GridLayout(1, 4));
		panel1.add(new JScrollPane(jTree1));
		panel1.add(new JScrollPane(jTree2));
		panel1.add(new JScrollPane(jTree3));
		panel1.add(new JScrollPane(jTree4));

		JPanel panel2 = new JPanel();
		panel2.add(new JLabel("rootVisible"));
		panel2.add(jcboRootVisible);
		panel2.add(new JLabel("rowHeight"));
		panel2.add(jSpinnerRowHeight);
		panel2.add(new JLabel("showsRootHandles"));
		panel2.add(jcboShowRootHandles);

		add(panel1, BorderLayout.CENTER);
		add(panel2, BorderLayout.SOUTH);

		jcboRootVisible.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				boolean rootVisible = jcboRootVisible.getSelectedItem().equals(
						"true");
				jTree1.setRootVisible(rootVisible);
				jTree2.setRootVisible(rootVisible);
				jTree3.setRootVisible(rootVisible);
				jTree4.setRootVisible(rootVisible);
			}
		});

		jcboShowRootHandles.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				boolean showsRootHandles = jcboShowRootHandles
						.getSelectedItem().equals("true");
				jTree1.setShowsRootHandles(showsRootHandles);
				jTree2.setShowsRootHandles(showsRootHandles);
				jTree3.setShowsRootHandles(showsRootHandles);
				jTree4.setShowsRootHandles(showsRootHandles);
			}
		});

		jSpinnerRowHeight.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				int height = ((Integer) (jSpinnerRowHeight.getValue()))
						.intValue();
				jTree1.setRowHeight(height);
				jTree2.setRowHeight(height);
				jTree3.setRowHeight(height);
				jTree4.setRowHeight(height);
			}
		});
	}
}
