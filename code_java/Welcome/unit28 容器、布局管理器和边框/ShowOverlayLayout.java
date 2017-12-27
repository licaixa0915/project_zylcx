import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
// 叠加布局管理器
public class ShowOverlayLayout extends JApplet {
	private JButton jbt1 = new JButton("Button 1");
	private JButton jbt2 = new JButton("Button 2");
	private JTextField jtfButton1AlignmentX = new JTextField(4);
	private JTextField jtfButton1AlignmentY = new JTextField(4);
	private JTextField jtfButton2AlignmentX = new JTextField(4);
	private JTextField jtfButton2AlignmentY = new JTextField(4);
	private JComboBox<?> jcboButton1Opaque = new JComboBox<Object>(new Object[] {
			new Boolean(true), new Boolean(false) });
	private JPanel p1 = new JPanel();

	public ShowOverlayLayout() {
		p1.setLayout(new OverlayLayout(p1));
		p1.add(jbt1);
		p1.add(jbt2);

		JPanel p2 = new JPanel();
		p2.setLayout(new GridLayout(5, 1));
		p2.add(new JLabel("Button 1's alignmentX"));
		p2.add(new JLabel("Button 1's alignmentY"));
		p2.add(new JLabel("Button 2's alignmentX"));
		p2.add(new JLabel("Button 2's alignmentY"));
		p2.add(new JLabel("Button 1's opaque"));

		JPanel p3 = new JPanel();
		p3.setLayout(new GridLayout(5, 1));
		p3.add(jtfButton1AlignmentX);
		p3.add(jtfButton1AlignmentY);
		p3.add(jtfButton2AlignmentX);
		p3.add(jtfButton2AlignmentY);
		p3.add(jcboButton1Opaque);

		JPanel p4 = new JPanel();
		p4.setLayout(new BorderLayout(4, 4));
		p4.add(p2, BorderLayout.WEST);
		p4.add(p3, BorderLayout.CENTER);

		add(p1, BorderLayout.CENTER);
		add(p4, BorderLayout.WEST);

		jtfButton1AlignmentX.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jbt1.setAlignmentX(Float.parseFloat(jtfButton1AlignmentX
						.getText()));
				p1.revalidate();
				p1.repaint();
			}
		});

		jtfButton1AlignmentY.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jbt1.setAlignmentY(Float.parseFloat(jtfButton1AlignmentY
						.getText()));
				p1.revalidate();
				p1.repaint();
			}
		});

		jtfButton2AlignmentX.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jbt2.setAlignmentX(Float.parseFloat(jtfButton2AlignmentX
						.getText()));
				p1.revalidate();
				p1.repaint();
			}
		});

		jtfButton2AlignmentY.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jbt2.setAlignmentY(Float.parseFloat(jtfButton2AlignmentY
						.getText()));
				p1.revalidate();
				p1.repaint();
			}
		});

		jcboButton1Opaque.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jbt1
						.setOpaque(((Boolean) (jcboButton1Opaque
								.getSelectedItem())).booleanValue());
				p1.revalidate();
				p1.repaint();
			}
		});
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ShowOverlayLayout");
		ShowOverlayLayout applet = new ShowOverlayLayout();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
