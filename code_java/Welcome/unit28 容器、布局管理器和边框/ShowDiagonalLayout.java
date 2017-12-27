import javax.swing.*;
import javax.swing.border.*;

import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
// 自定义布局管理器
public class ShowDiagonalLayout extends JApplet {
	private FlowLayout flowLayout = new FlowLayout();
	private GridLayout gridLayout = new GridLayout(2, 2);
	private DiagonalLayout diagonalLayout = new DiagonalLayout();
	private JButton jbt1 = new JButton("Button 1");
	private JButton jbt2 = new JButton("Button 2");
	private JButton jbt3 = new JButton("Button 3");
	private JButton jbt4 = new JButton("Button 4");
	private JRadioButton jrbFlowLayout = new JRadioButton("FlowLayout");
	private JRadioButton jrbGridLayout = new JRadioButton("GridLayout");
	private JRadioButton jrbDiagonaLayout = new JRadioButton("DiagonaLayout",
			true);
	private JPanel jPanel2 = new JPanel();

	public ShowDiagonalLayout() {
		jPanel2.setLayout(diagonalLayout);
		jPanel2.add(jbt1);
		jPanel2.add(jbt2);
		jPanel2.add(jbt3);
		jPanel2.add(jbt4);
		jPanel2.setBorder(new LineBorder(Color.black));

		JPanel jPanel1 = new JPanel();
		jPanel1.setBorder(new TitledBorder("Sleect a Layout manager"));
		jPanel1.add(jrbFlowLayout);
		jPanel1.add(jrbGridLayout);
		jPanel1.add(jrbDiagonaLayout);

		ButtonGroup group = new ButtonGroup(); // "排它"行为,适用于单选按钮
		group.add(jrbFlowLayout);
		group.add(jrbGridLayout);
		group.add(jrbDiagonaLayout);

		add(jPanel1, BorderLayout.SOUTH);
		add(jPanel2, BorderLayout.CENTER);

		jrbFlowLayout.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jPanel2.setLayout(flowLayout);

				// 验证此容器及其所有子组件
				// （在容器中添加或移除组件，或者更改与布局相关的信息），应该调用上述方法。
				jPanel2.validate();
			}
		});

		jrbGridLayout.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jPanel2.setLayout(gridLayout);
				jPanel2.validate();
			}
		});

		jrbDiagonaLayout.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jPanel2.setLayout(diagonalLayout);
				jPanel2.validate();
			}
		});
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ShowDiagonalLayout");
		ShowDiagonalLayout applet = new ShowDiagonalLayout();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.setSize(400, 300);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
