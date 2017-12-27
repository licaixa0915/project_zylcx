import java.awt.*;

import javax.swing.*;

@SuppressWarnings("serial")
public class TestImageIcon extends JFrame {
	public static void main(String[] args) {
		TestImageIcon frame = new TestImageIcon();
		frame.setTitle("TestImageIcon");
		frame.setSize(500, 200);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	private ImageIcon frIcon = new ImageIcon("image/fr.gif");
	private ImageIcon myIcon = new ImageIcon("image/my.jpg");
	private ImageIcon ukIcon = new ImageIcon("image/uk.gif");

	private ImageIcon usIcon = new ImageIcon("image/us.gif");

	public TestImageIcon() {
		setLayout(new GridLayout(1, 4, 5, 5));
		add(new JLabel(usIcon));
		add(new JLabel(myIcon));
		add(new JButton(frIcon));
		add(new JButton(ukIcon));
	}
}
