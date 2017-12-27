import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
public class ActionInterfaceDemo extends JApplet {
	private JPanel buttonPanel = new JPanel();
	private FlowLayout flowLayout = new FlowLayout();

	public ActionInterfaceDemo() {
		ImageIcon leftImageIcon = new ImageIcon(getClass().getResource(
				"image/leftAlignment.png"));
		ImageIcon centerImageIcon = new ImageIcon(getClass().getResource(
				"image/centerAlignment.png"));
		ImageIcon rightImageIcon = new ImageIcon(getClass().getResource(
				"image/rightAlignment.png"));

		Action leftAction = new MyAction("Left", leftImageIcon,
				"Left alignment for the buttons in the panel", new Integer(
						KeyEvent.VK_L), KeyStroke.getKeyStroke(KeyEvent.VK_L,
						ActionEvent.CTRL_MASK));
		Action centerAction = new MyAction("Center", centerImageIcon,
				"Center alignment for the buttons in the panel", new Integer(
						KeyEvent.VK_C), KeyStroke.getKeyStroke(KeyEvent.VK_C,
						ActionEvent.CTRL_MASK));
		Action rightAction = new MyAction("Right", rightImageIcon,
				"Right alignment for the buttons in the panel", new Integer(
						KeyEvent.VK_R), KeyStroke.getKeyStroke(KeyEvent.VK_R,
						ActionEvent.CTRL_MASK));

		JMenuBar jMenuBar1 = new JMenuBar();
		JMenu jmenuAlignment = new JMenu("Alignment");
		setJMenuBar(jMenuBar1);
		jMenuBar1.add(jmenuAlignment);

		jmenuAlignment.add(leftAction);
		jmenuAlignment.add(centerAction);
		jmenuAlignment.add(rightAction);

		JToolBar jToolBar1 = new JToolBar(JToolBar.VERTICAL);
		jToolBar1.setBorder(BorderFactory.createLineBorder(Color.red));
		jToolBar1.add(leftAction);
		jToolBar1.add(centerAction);
		jToolBar1.add(rightAction);

		buttonPanel.setLayout(flowLayout);
		JButton jbtLeft = new JButton(leftAction);
		JButton jbtCenter = new JButton(centerAction);
		JButton jbtRight = new JButton(rightAction);
		buttonPanel.add(jbtLeft);
		buttonPanel.add(jbtCenter);
		buttonPanel.add(jbtRight);

		add(jToolBar1, BorderLayout.EAST);
		add(buttonPanel, BorderLayout.CENTER);
	}

	private class MyAction extends AbstractAction {
		String name;

		@SuppressWarnings("unused")
		MyAction(String name, Icon icon) {
			super(name, icon);
			this.name = name;
		}

		MyAction(String name, Icon icon, String desc, Integer mnemonic,
				KeyStroke accelerator) {
			super(name, icon);
			putValue(Action.SHORT_DESCRIPTION, desc); // 使用关联的键设置此对象的一个属性
			putValue(Action.MNEMONIC_KEY, mnemonic);
			putValue(Action.ACCELERATOR_KEY, accelerator);
			this.name = name;
		}

		public void actionPerformed(ActionEvent e) {
			if (name.equals("Left"))
				flowLayout.setAlignment(FlowLayout.LEFT);
			else if (name.equals("Center"))
				flowLayout.setAlignment(FlowLayout.CENTER);
			else if (name.equals("Right"))
				flowLayout.setAlignment(FlowLayout.RIGHT);
			
			buttonPanel.revalidate();
		}
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame("ActionInterfaceDemo");
		ActionInterfaceDemo applet = new ActionInterfaceDemo();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.setSize(400, 300);
		// frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
