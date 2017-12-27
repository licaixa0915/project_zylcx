import java.awt.*;
import javax.swing.*;
import javax.swing.border.LineBorder;

@SuppressWarnings("serial")
// 动态网格布局管理器
public class ShowGridBagLayout extends JApplet {
	private JLabel jlbl = new JLabel(
			"Resize the Window and Study GridBagLayout", JLabel.CENTER);
	private JTextArea jta1 = new JTextArea("Text Area 1", 5, 15);
	private JTextArea jta2 = new JTextArea("Text Area 2", 5, 15);
	private JTextField jtf = new JTextField("JtextField");
	private JPanel jp = new JPanel();
	private JButton jbt1 = new JButton("Button 1");
	private JButton jbt2 = new JButton("Button 2");

	public ShowGridBagLayout() {
		setLayout(new GridBagLayout());
		GridBagConstraints gbConstraints = new GridBagConstraints();
		gbConstraints.fill = GridBagConstraints.BOTH;
		gbConstraints.anchor = GridBagConstraints.CENTER;

		Container container = getContentPane();
		addComp(jlbl, container, gbConstraints, 0, 0, 1, 4, 0, 0);
		addComp(jta1, container, gbConstraints, 1, 0, 2, 1, 5, 1);
		addComp(jta2, container, gbConstraints, 1, 3, 1, 1, 5, 1);
		addComp(jtf, container, gbConstraints, 2, 3, 1, 1, 5, 0);
		addComp(jbt1, container, gbConstraints, 3, 1, 1, 1, 5, 0);
		addComp(jbt2, container, gbConstraints, 3, 2, 1, 1, 5, 0);

		jp.setBackground(Color.red);
		jp.setBorder(new LineBorder(Color.black));
		gbConstraints.insets = new Insets(10, 10, 10, 10);
		addComp(jp, container, gbConstraints, 1, 1, 2, 2, 10, 1);
	}

	private void addComp(Component c, Container container,
			GridBagConstraints gbConstraints, int row, int column,
			int numberOfRows, int numberOfColumns, double weightx,
			double weighty) {
		gbConstraints.gridx = column;
		gbConstraints.gridy = row;
		gbConstraints.gridwidth = numberOfColumns;
		gbConstraints.gridheight = numberOfRows;
		gbConstraints.weightx = weightx;
		gbConstraints.weighty = weighty;

		container.add(c, gbConstraints);
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ShowGridBagLayout");
		ShowGridBagLayout applet = new ShowGridBagLayout();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
