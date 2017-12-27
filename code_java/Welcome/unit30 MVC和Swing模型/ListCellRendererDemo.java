import javax.swing.*;
import javax.swing.event.*;

import java.awt.*;

@SuppressWarnings("serial")
// 列表框单元格绘制器
public class ListCellRendererDemo extends JApplet {
	private final static int NUMBER_OF_NATIONS = 7;
	private String[] nations = new String[] { "Denmark", "Germany", "China",
			"India", "Norway", "UK", "US" };
	private ImageIcon[] icons = new ImageIcon[NUMBER_OF_NATIONS];
	private ImageIcon[] bigIcons = new ImageIcon[NUMBER_OF_NATIONS];
	@SuppressWarnings("rawtypes")
	private DefaultListModel listModel = new DefaultListModel();
	@SuppressWarnings({ "unchecked", "rawtypes" })
	private JList jlstNations = new JList(listModel);
	@SuppressWarnings("rawtypes")
	private ListCellRenderer renderer = new MyListCellRenderer();
	private JSplitPane jSplitPane1 = new JSplitPane(); // 分隔窗格
	private JLabel jlblImage = new JLabel("", JLabel.CENTER);

	@SuppressWarnings("unchecked")
	public ListCellRendererDemo() {
		for (int i = 0; i < NUMBER_OF_NATIONS; i++) {
			icons[i] = new ImageIcon(getClass().getResource(
					"image/flagIcon" + i + ".gif"));
			listModel.addElement(new Object[] { icons[i], nations[i] });
			bigIcons[i] = new ImageIcon(getClass().getResource(
					"image/flag" + i + ".gif"));
		}

		jlstNations.setCellRenderer(renderer);
		jlstNations.setPreferredSize(new Dimension(200, 200));
		jSplitPane1.setLeftComponent(new JScrollPane(jlstNations));
		jSplitPane1.setRightComponent(jlblImage);
		jlstNations.setSelectedIndex(0);
		jlblImage.setIcon(bigIcons[0]);
		add(jSplitPane1, BorderLayout.CENTER);

		jlstNations.addListSelectionListener(new ListSelectionListener() {
			public void valueChanged(ListSelectionEvent e) {
				jlblImage.setIcon(bigIcons[jlstNations.getSelectedIndex()]);
			}
		});
	}
}
