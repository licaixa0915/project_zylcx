import javax.swing.*;
import java.awt.*;

@SuppressWarnings("serial")
public class CustomFrame extends JFrame {
	public static void main(String[] args) {
		JFrame frame = new CustomFrame();
		frame.setTitle("Window 1");
		frame.setSize(200, 150);
		frame.setLocation(200, 100);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public CustomFrame() {
		FlowLayout layout = new FlowLayout();
		setLayout(layout);
		JButton jbtOK = new JButton("OK");
		JButton jbtCancel = new JButton("Cancel");
		add(jbtOK);
		add(jbtCancel);
	}
}
