import javax.swing.*;

import java.awt.*;

public class UseFlowLayout {
	public static void main(String[] args) {
		JFrame frame = new JFrame();
		FlowLayout layout = new FlowLayout();
		frame.setLayout(layout);
		JButton jbtOK = new JButton("OK");
		frame.add(jbtOK);

		frame.setTitle("Window 1");
		frame.setSize(200, 150);
		frame.setLocation(200, 100);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
