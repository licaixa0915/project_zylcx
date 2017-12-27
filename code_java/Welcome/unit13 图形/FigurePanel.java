import javax.swing.JPanel;
import java.awt.*;

@SuppressWarnings("serial")
public class FigurePanel extends JPanel {
	public static final int LINE = 1;
	public static final int OVAL = 4;
	public static final int RECTANGLE = 2;
	public static final int ROUND_RECTANGLE = 3;
	private boolean filled;
	private int type = 1;

	public FigurePanel() {
	}

	public FigurePanel(int type) {
		this.type = type;
	}

	public FigurePanel(int type, boolean filled) {
		this.type = type;
		this.filled = filled;
	}

	public Dimension getPreferredSize() {
		return new Dimension(80, 80);
	}

	public int getType() {
		return type;
	}

	public boolean isfilled() {
		return filled;
	}

	public void paintComponent(Graphics g) {
		super.paintComponents(g);
		int width = getSize().width;
		int height = getSize().height;

		switch (type) {
		case LINE:
			g.setColor(Color.BLACK);
			g.drawLine(10, 10, width - 10, height - 10);
			g.drawLine(width - 10, 10, 10, height - 10);
			break;
		case RECTANGLE:
			g.setColor(Color.BLUE);
			if (filled)
				g.fillRect((int) (0.1 * width), (int) (0.1 * height),
						(int) (0.8 * width), (int) (0.8 * height));
			else
				g.drawRect((int) (0.1 * width), (int) (0.1 * height),
						(int) (0.8 * width), (int) (0.8 * height));
			break;
		case ROUND_RECTANGLE:
			g.setColor(Color.RED);
			if (filled)
				g.fillRoundRect((int) (0.1 * width), (int) (0.1 * height),
						(int) (0.8 * width), (int) (0.8 * height), 20, 20);
			else
				g.drawRoundRect((int) (0.1 * width), (int) (0.1 * height),
						(int) (0.8 * width), (int) (0.8 * height), 20, 20);
			break;
		case OVAL:
			g.setColor(Color.BLACK);
			if (filled)
				g.fillOval((int) (0.1 * width), (int) (0.1 * height),
						(int) (0.8 * width), (int) (0.8 * height));
			else
				g.drawOval((int) (0.1 * width), (int) (0.1 * height),
						(int) (0.8 * width), (int) (0.8 * height));
		}
	}

	public void setFilled(boolean filled) {
		this.filled = filled;
		repaint();
	}

	public void setType(int type) {
		this.type = type;
		repaint();
	}
}
