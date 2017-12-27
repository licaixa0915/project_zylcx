import javax.swing.*;

import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class PopupMenuDemo extends JApplet {
	private JPopupMenu jPopupmenu1 = new JPopupMenu();
	private JMenuItem jmiNew = new JMenuItem("New", new ImageIcon(getClass()
			.getResource("image/new.gif")));
	private JMenuItem jmiOpen = new JMenuItem("Open", new ImageIcon(getClass()
			.getResource("image/open.gif")));
	private JMenuItem jmiPrint = new JMenuItem("Print", new ImageIcon(
			getClass().getResource("image/print.gif")));
	private JMenuItem jmiExit = new JMenuItem("Exit");
	private JTextArea jTextArea1 = new JTextArea();

	public PopupMenuDemo() {
		jPopupmenu1.add(jmiNew);
		jPopupmenu1.add(jmiOpen);
		jPopupmenu1.addSeparator(); // ·Ö¸ôÏß
		jPopupmenu1.add(jmiPrint);
		jPopupmenu1.addSeparator();
		jPopupmenu1.add(jmiExit);

		add(new JScrollPane(jTextArea1), BorderLayout.CENTER);

		jmiNew.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.out.println("Process New");
			}
		});

		jmiOpen.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.out.println("Process Open");
			}
		});

		jmiPrint.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.out.println("Process Print");
			}
		});

		jmiExit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});

		jTextArea1.addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent e) {
				showPopup(e);
			}

			public void mouseReleased(MouseEvent e) {
				showPopup(e);
			}
		});
	}

	private void showPopup(MouseEvent evt) {
		if (evt.isPopupTrigger())
			jPopupmenu1.show(evt.getComponent(), evt.getX(), evt.getY());
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("PopupMenuDemo");
		PopupMenuDemo applet = new PopupMenuDemo();
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
