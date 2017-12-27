import java.awt.FontMetrics;
import java.awt.Dimension;
import java.awt.Graphics;
import javax.swing.JPanel;

@SuppressWarnings("serial")
public class MessagePanel extends JPanel {
	private boolean centered;
	private int interval = 10;
	private String message = "Welcome to Java";
	private int xCoordinate = 20;
	private int yCoordinate = 20;

	public MessagePanel() {
	}

	public MessagePanel(String message) {
		this.message = message;
	}

	public int getInterval() {
		return interval;
	}

	public String getMessage() {
		return message;
	}

	public Dimension getPreferredSize() {
		return new Dimension(200, 30);
	}

	public int getXCoordinate() {
		return xCoordinate;
	}

	public int getYCoordinate() {
		return yCoordinate;
	}

	public boolean isCentered() {
		return centered;
	}

	public void moveDown() {
		yCoordinate += interval;
		repaint();
	}

	public void moveLeft() {
		xCoordinate -= interval;
		repaint();
	}

	public void moveRight() {
		xCoordinate += interval;
		repaint();
	}

	public void moveUp() {
		yCoordinate -= interval;
		repaint();
	}

	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		if (centered) {
			FontMetrics fm = g.getFontMetrics();
			int stringWidth = fm.stringWidth(message);
			int stringAscent = fm.getAscent();
			xCoordinate = getWidth() / 2 - stringWidth / 2;
			yCoordinate = getHeight() / 2 + stringAscent / 2;
		}
		g.drawString(message, xCoordinate, yCoordinate);
	}

	public void setCentered(boolean centered) {
		this.centered = centered;
		repaint();
	}

	public void setInterval(int interval) {
		this.interval = interval;
		repaint();
	}

	public void setMessage(String message) {
		this.message = message;
		repaint();
	}

	public void setXCoordinate(int xCoordinate) {
		this.xCoordinate = xCoordinate;
		repaint();
	}

	public void setYCoordinate(int yCoordinate) {
		this.yCoordinate = yCoordinate;
		repaint();
	}
}
