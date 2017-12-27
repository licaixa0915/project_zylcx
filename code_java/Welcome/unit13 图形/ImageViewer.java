import java.awt.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class ImageViewer extends JPanel {
	private java.awt.Image image;
	private boolean stretched = true;
	private int xCoordinate;
	private int yCoordinate;

	public ImageViewer() {
	}

	public ImageViewer(Image image) {
		this.image = image;
	}

	public java.awt.Image getImage() {
		return image;
	}

	public int getxCoordinate() {
		return xCoordinate;
	}

	public int getyCoordinate() {
		return yCoordinate;
	}

	public boolean isStretched() {
		return stretched;
	}

	protected void paintComponent(Graphics g) {
		super.paintComponents(g);
		if (image != null)
			if (isStretched())
				g.drawImage(image, xCoordinate, yCoordinate, getSize().width,
						getSize().height, this);
			else
				g.drawImage(image, xCoordinate, yCoordinate, this);
	}

	public void setImage(java.awt.Image image) {
		this.image = image;
		repaint();
	}

	public void setStretched(boolean stretched) {
		this.stretched = stretched;
		repaint();
	}

	public void setxCoordinate(int xCoordinate) {
		this.xCoordinate = xCoordinate;
		repaint();
	}

	public void setyCoordinate(int yCoordinate) {
		this.yCoordinate = yCoordinate;
		repaint();
	}
}
