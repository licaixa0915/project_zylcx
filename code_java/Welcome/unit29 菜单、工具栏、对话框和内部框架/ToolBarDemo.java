import javax.swing.*;

import java.awt.*;

@SuppressWarnings("serial")
public class ToolBarDemo extends JApplet {
	private JButton jbtNew = new JButton(new ImageIcon(getClass().getResource(
			"image/new.gif")));
	private JButton jbtOpen = new JButton(new ImageIcon(getClass().getResource(
			"image/open.gif")));
	private JButton jbtPrint = new JButton(new ImageIcon(getClass()
			.getResource("image/print.gif")));

	public ToolBarDemo() {
		JToolBar jToolBar1 = new JToolBar("My Tool Bar");
		jToolBar1.setFloatable(true);
		jToolBar1.add(jbtNew);
		jToolBar1.add(jbtOpen);
		jToolBar1.add(jbtPrint);

		jbtNew.setToolTipText("New");
		jbtOpen.setToolTipText("Open");
		jbtPrint.setToolTipText("Print");

		jbtNew.setBorderPainted(false); // ²»»æÖÆ±ß¿ò
		jbtOpen.setBorderPainted(false);
		jbtPrint.setBorderPainted(false);

		add(jToolBar1, BorderLayout.CENTER);
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ToolBarDemo");
		ToolBarDemo applet = new ToolBarDemo();
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
