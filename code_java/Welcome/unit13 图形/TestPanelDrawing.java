import javax.swing.*;
import java.awt.Graphics;

@SuppressWarnings("serial")
class NewPanel extends JPanel {
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawLine(0, 0, 50, 50);
		g.drawString("Banner", 0, 40);
	}
}

@SuppressWarnings("serial")
public class TestPanelDrawing extends JFrame {
	public static void main(String[] args) {
		TestPanelDrawing frame = new TestPanelDrawing();
		frame.setTitle("TestPanelDrawing");
		frame.setSize(200, 100);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public TestPanelDrawing() {
		add(new NewPanel());
	}
}
