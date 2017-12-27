import javax.swing.*;
import java.awt.Graphics;

@SuppressWarnings("serial")
class NewLabel extends JLabel {
	public NewLabel(String text) {
		super(text);
	}

	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawLine(0, 0, 50, 50);
	}
}

@SuppressWarnings("serial")
public class TestPaintComponent extends JFrame {
	public static void main(String[] args) {
		TestPaintComponent frame = new TestPaintComponent();
		frame.setTitle("TestPaintComponent");
		frame.setSize(200, 100);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public TestPaintComponent() {
		add(new NewLabel("Banner"));
	}
}
