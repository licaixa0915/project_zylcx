import javax.swing.*;
import java.awt.Graphics;

@SuppressWarnings("serial")
public class TestGetGraphics extends JFrame {
	public static void main(String[] args) {
		TestGetGraphics frame = new TestGetGraphics();
		frame.setTitle("TestGetGraphics");
		frame.setSize(200, 100);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
		JOptionPane.showMessageDialog(null,
				"Delay on purpose\nClick OK to dismiss the dialog");
		Graphics graphics = frame.jlblBanner.getGraphics();
		graphics.drawLine(0, 0, 50, 50);
	}

	private JLabel jlblBanner = new JLabel("Banner");

	public TestGetGraphics() {
		add(jlblBanner);
		System.out.println(jlblBanner.getGraphics());
	}
}
