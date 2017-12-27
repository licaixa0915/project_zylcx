import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class SierpinskiTriangle extends JApplet {
	static class SierpinskiTrianglePanel extends JPanel {
		private static void displayTriangles(Graphics g, int order, Point p1,
				Point p2, Point p3) {
			if (order >= 0) {
				g.drawLine(p1.x, p1.y, p2.x, p2.y);
				g.drawLine(p1.x, p1.y, p3.x, p3.y);
				g.drawLine(p2.x, p2.y, p3.x, p3.y);
				Point midBetweenP1P2 = midpoint(p1, p2);
				Point midBetweenP2P3 = midpoint(p2, p3);
				Point midBetweenP3P1 = midpoint(p3, p1);
				displayTriangles(g, order - 1, p1, midBetweenP1P2,
						midBetweenP3P1);
				displayTriangles(g, order - 1, midBetweenP1P2, p2,
						midBetweenP2P3);
				displayTriangles(g, order - 1, midBetweenP3P1, midBetweenP2P3,
						p3);
			}
		}

		private static Point midpoint(Point p1, Point p2) {
			return new Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
		}

		private int order = 0;

		public void paintComponent(Graphics g) {
			super.paintComponent(g);
			Point p1 = new Point(getWidth() / 2, 10);
			Point p2 = new Point(10, getHeight() - 10);
			Point p3 = new Point(getWidth() - 10, getHeight() - 10);
			displayTriangles(g, order, p1, p2, p3);
		}

		public void setOrder(int order) {
			this.order = order;
			repaint();
		}
	}
	private JTextField jtfOrder = new JTextField(5);

	private SierpinskiTrianglePanel trianglePanel = new SierpinskiTrianglePanel();

	public SierpinskiTriangle() {
		JPanel panel = new JPanel();
		panel.add(new JLabel("Enter an order: "));
		panel.add(jtfOrder);
		jtfOrder.setHorizontalAlignment(SwingConstants.RIGHT);
		getContentPane().add(trianglePanel);
		getContentPane().add(panel, BorderLayout.SOUTH);

		jtfOrder.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				trianglePanel.setOrder(Integer.parseInt(jtfOrder.getText()));
			}
		});
	}
}
