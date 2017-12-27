import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

@SuppressWarnings("serial")
// 分隔窗格
public class ShowLayout extends JApplet {
	private String flowLayoutDesc = "FlowLayout arranges components "
			+ "according to their preferredSize in "
			+ "a left-to-right flow, much like lines of text in a paragraph.";
	private String gridLayoutDesc = "Gridlayout arranges ...";
	private String boxLayoutDesc = "BoxLayout arranges ...";

	private JRadioButton jrbFlowLayout = new JRadioButton("FlowLayout", true);
	private JRadioButton jrbGridLayout = new JRadioButton("GridLayout");
	private JRadioButton jrbBoxLayout = new JRadioButton("BoxLayout");

	private JPanel jpComponents = new JPanel();
	private JTextArea jtfDescription = new JTextArea();

	private FlowLayout flowLayout = new FlowLayout();
	private GridLayout gridLayout = new GridLayout(2, 2, 3, 3);
	private BoxLayout boxLayout = new BoxLayout(jpComponents, BoxLayout.X_AXIS);

	public ShowLayout() {
		Box jpChooselayout = Box.createVerticalBox();
		jpChooselayout.add(jrbFlowLayout);
		jpChooselayout.add(jrbGridLayout);
		jpChooselayout.add(jrbBoxLayout);

		ButtonGroup btg = new ButtonGroup(); // "排它"行为,适用于单选按钮
		btg.add(jrbFlowLayout);
		btg.add(jrbGridLayout);
		btg.add(jrbBoxLayout);

		jtfDescription.setLineWrap(true);
		jtfDescription.setWrapStyleWord(true);

		jpComponents.add(new JButton("Button 1"));
		jpComponents.add(new JButton("Button 2"));
		jpComponents.add(new JButton("Button 3"));
		jpComponents.add(new JButton("Button 4"));

		// 分隔窗格
		JSplitPane jSplitPane2 = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
				jpComponents, new JScrollPane(jtfDescription));
		JSplitPane jSplitPane1 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
				jpChooselayout, jSplitPane2);

		jpComponents.setLayout(flowLayout);
		jpComponents.validate();
		jtfDescription.setText(flowLayoutDesc);

		add(jSplitPane1, BorderLayout.CENTER);

		jrbFlowLayout.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jpComponents.setLayout(flowLayout);
				jtfDescription.setText(flowLayoutDesc);
				jpComponents.revalidate();
			}
		});

		jrbGridLayout.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jpComponents.setLayout(gridLayout);
				jtfDescription.setText(gridLayoutDesc);
				jpComponents.revalidate();
			}
		});

		jrbBoxLayout.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jpComponents.setLayout(boxLayout);
				jtfDescription.setText(boxLayoutDesc);
				jpComponents.revalidate();
			}
		});
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ShowLayout");
		ShowLayout applet = new ShowLayout();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.setSize(400, 300);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
