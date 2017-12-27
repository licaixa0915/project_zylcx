import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
// 组合框模型
public class ComboBoxCellRendererDemo extends JApplet {
	private final static int NUMBER_OF_NATIONS = 7;
	private String[] nations = new String[] { "Denmark", "Germany", "China",
			"India", "Norway", "UK", "US" };
	private ImageIcon[] icons = new ImageIcon[NUMBER_OF_NATIONS];
	private ImageIcon[] bigIcons = new ImageIcon[NUMBER_OF_NATIONS];
	private DefaultComboBoxModel<Object[]> model = new DefaultComboBoxModel<Object[]>();
	private JComboBox<Object[]> jcboCountries = new JComboBox<Object[]>(model);
	private MyListCellRenderer renderer = new MyListCellRenderer();
	private JLabel jlblImage = new JLabel("", JLabel.CENTER);

	public ComboBoxCellRendererDemo() {
		for (int i = 0; i < NUMBER_OF_NATIONS; i++) {
			icons[i] = new ImageIcon(getClass().getResource("image/flagIcon" + i + ".gif"));
			model.addElement(new Object[] { icons[i], nations[i] });
			bigIcons[i] = new ImageIcon(getClass().getResource(
					"image/flag" + i + ".gif"));
		}

		jcboCountries.setRenderer(renderer);
		jlblImage.setIcon(bigIcons[0]);
		add(jcboCountries, BorderLayout.NORTH);
		add(jlblImage, BorderLayout.CENTER);

		jcboCountries.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlblImage.setIcon(bigIcons[jcboCountries.getSelectedIndex()]);
			}
		});
	}
}
