import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.LineBorder;

@SuppressWarnings("serial")
// 盒子布局管理器
public class ShowBoxLayout extends JApplet {
	private Box box1 = Box.createHorizontalBox();
	private Box box2 = Box.createVerticalBox();
	private JLabel jlblFlag = new JLabel();
	private ImageIcon imageIconUS = new ImageIcon(getClass().getResource(
			"/image/us.gif"));
	private ImageIcon imageIconCanada = new ImageIcon(getClass().getResource(
			"/image/ca.gif"));
	private ImageIcon imageIconNorway = new ImageIcon(getClass().getResource(
			"/image/norway.gif"));
	private ImageIcon imageIconGermany = new ImageIcon(getClass().getResource(
			"/image/germany.gif"));
	private ImageIcon imageIconPrint = new ImageIcon(getClass().getResource(
			"/image/print.gif"));
	private ImageIcon imageIconSave = new ImageIcon(getClass().getResource(
			"/image/save.gif"));
	private JButton jbtUS = new JButton("US");
	private JButton jbtCanada = new JButton("Canada");
	private JButton jbtNorway = new JButton("Norway");
	private JButton jbtGermany = new JButton("Germany");

	public ShowBoxLayout() {
		box1.add(new JButton(imageIconPrint));
		box1.add(Box.createHorizontalStrut(20));
		box1.add(new JButton(imageIconSave));

		box2.add(jbtUS);
		box2.add(Box.createVerticalStrut(8));
		box2.add(jbtCanada);
		box2.add(Box.createGlue());
		box2.add(jbtNorway);
		box2.add(Box.createRigidArea(new Dimension(10, 8)));
		box2.add(jbtGermany);

		box1.setBorder(new LineBorder(Color.red));
		box2.setBorder(new LineBorder(Color.black));

		add(box1, BorderLayout.NORTH);
		add(box2, BorderLayout.EAST);
		add(jlblFlag, BorderLayout.CENTER);

		jbtUS.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlblFlag.setIcon(imageIconUS);
			}
		});

		jbtCanada.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlblFlag.setIcon(imageIconCanada);
			}
		});

		jbtNorway.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlblFlag.setIcon(imageIconNorway);
			}
		});

		jbtGermany.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlblFlag.setIcon(imageIconGermany);
			}
		});
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ShowBoxLayout");
		ShowBoxLayout applet = new ShowBoxLayout();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.setSize(300, 200);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
