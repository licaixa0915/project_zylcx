import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

@SuppressWarnings("serial")
// 边框
public class BorderDemo extends JApplet {
	private JLabel jLabel1 = new JLabel("Display the border type",
			JLabel.CENTER);
	private JCheckBox jchkTitled;
	private JRadioButton jrbLoweredBevel, jrbRaisedBevel, jrbEthched, jrbLine,
			jrbMatte, jrbEmpty;
	private JRadioButton jrbAboveBottom, jrbBottom, jrbBelowBottom,
			jrbAboveTop, jrbTop, jrbBelowTop, jrbLeft, jrbCenter, jrbRight;
	private TitledBorder jLabelBoder;

	public BorderDemo() {
		jLabel1.setBackground(Color.yellow);
		jLabel1.setBorder(jLabelBoder);

		JPanel jpPostition = new JPanel();
		jpPostition.setLayout(new GridLayout(3, 2));
		jpPostition.add(jrbAboveBottom = new JRadioButton("ABOVE_BOTTOM"));
		jpPostition.add(jrbAboveTop = new JRadioButton("ABOVE_TOP"));
		jpPostition.add(jrbBottom = new JRadioButton("BOTTOM"));
		jpPostition.add(jrbTop = new JRadioButton("TOP"));
		jpPostition.add(jrbBelowBottom = new JRadioButton("BELOW_BOTTOM"));
		jpPostition.add(jrbBelowTop = new JRadioButton("BELOW_TOP"));
		jpPostition.setBorder(new TitledBorder("Position"));

		JPanel jpJustification = new JPanel();
		jpJustification.setLayout(new GridLayout(3, 1));
		jpJustification.add(jrbLeft = new JRadioButton("LEFT"));
		jpJustification.add(jrbCenter = new JRadioButton("CENTER"));
		jpJustification.add(jrbRight = new JRadioButton("RIGHT"));
		jpJustification.setBorder(new TitledBorder("Justification"));

		JPanel jpTitleOptions = new JPanel();
		jpTitleOptions.setLayout(new BorderLayout());
		jpTitleOptions.add(jpPostition, BorderLayout.CENTER);
		jpTitleOptions.add(jpJustification, BorderLayout.EAST);

		JPanel jpTitle = new JPanel();
		jpTitle.setBorder(new TitledBorder("Border Title"));
		jpTitle.setLayout(new BorderLayout());
		jpTitle.add(jchkTitled = new JCheckBox("Titled"), BorderLayout.NORTH);
		jpTitle.add(jpTitleOptions, BorderLayout.CENTER);

		ButtonGroup btgTitlePosition = new ButtonGroup();
		btgTitlePosition.add(jrbAboveBottom);
		btgTitlePosition.add(jrbBottom);
		btgTitlePosition.add(jrbBelowBottom);
		btgTitlePosition.add(jrbAboveTop);
		btgTitlePosition.add(jrbTop);
		btgTitlePosition.add(jrbBelowTop);

		ButtonGroup btgTitleJustification = new ButtonGroup();
		btgTitleJustification.add(jrbLeft);
		btgTitleJustification.add(jrbCenter);
		btgTitleJustification.add(jrbRight);

		JPanel jpBorderStyle = new JPanel();
		jpBorderStyle.setBorder(new TitledBorder("Border Style"));
		jpBorderStyle.setLayout(new GridLayout(6, 1));
		jpBorderStyle.add(jrbLoweredBevel = new JRadioButton("Lowered Bevel"));
		jpBorderStyle.add(jrbRaisedBevel = new JRadioButton("Raised bevel"));
		jpBorderStyle.add(jrbEthched = new JRadioButton("Ethched"));
		jpBorderStyle.add(jrbLine = new JRadioButton("Line"));
		jpBorderStyle.add(jrbMatte = new JRadioButton("Matte"));
		jpBorderStyle.add(jrbEmpty = new JRadioButton("Empty"));

		ButtonGroup btgBorderStyle = new ButtonGroup();
		btgBorderStyle.add(jrbLoweredBevel);
		btgBorderStyle.add(jrbRaisedBevel);
		btgBorderStyle.add(jrbEthched);
		btgBorderStyle.add(jrbLine);
		btgBorderStyle.add(jrbMatte);
		btgBorderStyle.add(jrbEmpty);

		JPanel jpAllChoices = new JPanel();
		jpAllChoices.setLayout(new BorderLayout());
		jpAllChoices.add(jpTitle, BorderLayout.CENTER);
		jpAllChoices.add(jpBorderStyle, BorderLayout.EAST);

		setLayout(new BorderLayout());
		add(jLabel1, BorderLayout.CENTER);
		add(jpAllChoices, BorderLayout.SOUTH);

		ActionListener listener = new EventListener();
		jchkTitled.addActionListener(listener);
		jrbAboveBottom.addActionListener(listener);
		jrbBottom.addActionListener(listener);
		jrbBelowBottom.addActionListener(listener);
		jrbAboveTop.addActionListener(listener);
		jrbTop.addActionListener(listener);
		jrbBelowTop.addActionListener(listener);
		jrbLeft.addActionListener(listener);
		jrbCenter.addActionListener(listener);
		jrbRight.addActionListener(listener);
		jrbLoweredBevel.addActionListener(listener);
		jrbRaisedBevel.addActionListener(listener);
		jrbLine.addActionListener(listener);
		jrbEthched.addActionListener(listener);
		jrbMatte.addActionListener(listener);
		jrbEmpty.addActionListener(listener);
	}

	private class EventListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			Border border = new EmptyBorder(2, 2, 2, 2);
			if (jrbLoweredBevel.isSelected()) {
				border = new BevelBorder(BevelBorder.LOWERED);
				jLabel1.setText("Lowered Bevel Style");
			} else if (jrbRaisedBevel.isSelected()) {
				border = new BevelBorder(BevelBorder.RAISED);
				jLabel1.setText("Raised Bevel Style");
			} else if (jrbEthched.isSelected()) {
				border = new EtchedBorder();
				jLabel1.setText("Etched Style");
			} else if (jrbLine.isSelected()) {
				border = new LineBorder(Color.black, 5);
				jLabel1.setText("Line Style");
			} else if (jrbMatte.isSelected()) {
				border = new MatteBorder(15, 15, 15, 15, new ImageIcon(
						getClass().getResource("image/caicon.gif")));
				jLabel1.setText("Matte Style");
			} else if (jrbEmpty.isSelected()) {
				border = new EmptyBorder(2, 2, 2, 2);
				jLabel1.setText("Empty Style");
			}

			if (jchkTitled.isSelected()) {
				int titlePosition = TitledBorder.DEFAULT_POSITION; // 位置
				int titleJustification = TitledBorder.DEFAULT_JUSTIFICATION; // 对齐方式

				if (jrbAboveBottom.isSelected())
					titlePosition = TitledBorder.ABOVE_BOTTOM;
				else if (jrbBottom.isSelected())
					titlePosition = TitledBorder.BOTTOM;
				else if (jrbBelowBottom.isSelected())
					titlePosition = TitledBorder.BELOW_BOTTOM;
				else if (jrbAboveTop.isSelected())
					titlePosition = TitledBorder.ABOVE_TOP;
				else if (jrbTop.isSelected())
					titlePosition = TitledBorder.TOP;
				else if (jrbBelowTop.isSelected())
					titlePosition = TitledBorder.BELOW_TOP;

				if (jrbLeft.isSelected())
					titleJustification = TitledBorder.LEFT;
				else if (jrbCenter.isSelected())
					titleJustification = TitledBorder.CENTER;
				else if (jrbRight.isSelected())
					titleJustification = TitledBorder.RIGHT;

				jLabelBoder = new TitledBorder("A Title");
				jLabelBoder.setBorder(border);
				jLabelBoder.setTitlePosition(titlePosition);
				jLabelBoder.setTitleJustification(titleJustification);
				jLabel1.setBorder(jLabelBoder);
			} else {
				jLabel1.setBorder(border);
			}
		}
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame("BorderDemo");
		BorderDemo applet = new BorderDemo();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.setSize(500, 300);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
