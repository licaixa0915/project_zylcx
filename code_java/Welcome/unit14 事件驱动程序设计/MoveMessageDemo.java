import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class MoveMessageDemo extends JFrame {
	static class MovableMessagePanel extends JPanel {
		private String message = "Welcome to java";
		private int x = 20;
		private int y = 20;

		public MovableMessagePanel(String s) {
			message = s;
			addMouseMotionListener(new MouseMotionAdapter() {
				public void mouseDragged(MouseEvent e) { // 鼠标按住拖动
					x = e.getX();
					y = e.getY();
					// System.out.println("x = " + x + " " + "y = " + y);
					repaint();
				}
			});
		}

		protected void paintComponent(Graphics g) {
			super.paintComponent(g);
			g.drawString(message, x, y);
		}
	}

	public static void main(String[] args) {
		MoveMessageDemo frame = new MoveMessageDemo();
		frame.setTitle("MoveMessageDemo");
		frame.setSize(200, 100);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public MoveMessageDemo() {
		MovableMessagePanel p = new MovableMessagePanel("Welcome to Java");
		getContentPane().setLayout(new BorderLayout());
		getContentPane().add(p);
	}
}
