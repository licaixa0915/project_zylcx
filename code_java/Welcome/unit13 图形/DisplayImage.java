import java.awt.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class DisplayImage extends JFrame {
	public static void main(String[] args) {
		JFrame frame = new DisplayImage();
		frame.setTitle("DisplayImage");
		frame.setSize(500, 300);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public DisplayImage() {
		add(new ImageCanvas());
	}
}

@SuppressWarnings("serial")
class ImageCanvas extends JPanel {
	ImageIcon imageIcon = new ImageIcon("image/us.gif");
	Image image = imageIcon.getImage();
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		if (image != null)
			g.drawImage(image, 0, 0, getWidth(), getHeight(), this);
	}
}