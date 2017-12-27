import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class AnimationDemo extends JFrame {
	static class MovingMessagePanel extends JPanel {
		class TimerListener implements ActionListener {
			public void actionPerformed(ActionEvent e) {
				repaint();
			}
		}
		private String message = "Welcome to Java";
		private int xCoordinate = 0;

		private int yCoordinate = 20;

		public MovingMessagePanel(String message) {
			this.message = message;
			Timer timer = new Timer(1000, new TimerListener());
			timer.start();
		}

		public void paintComponent(Graphics g) {
			super.paintComponent(g);
			if (xCoordinate > getWidth())
				xCoordinate = -20;
			xCoordinate += 5;
			g.drawString(message, xCoordinate, yCoordinate);
		}
	}

	public static void main(String[] args) {
		AnimationDemo frame = new AnimationDemo();
		frame.setTitle("AnimationDemo");
		frame.setSize(280, 100);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public AnimationDemo() {
		add(new MovingMessagePanel("message moving?"));
	}
}
