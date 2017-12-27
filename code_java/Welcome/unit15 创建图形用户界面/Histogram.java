import java.awt.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class Histogram extends JPanel {
	private int[] count;

	public Dimension getPreferredSize() {
		return new Dimension(300, 300);
	}

	protected void paintComponent(Graphics g) {
		if (count == null)
			return;
		super.paintComponent(g);
		int width = getWidth();
		int height = getHeight();
		int interval = (width - 40) / count.length;
		int individualWidth = (int) (((width - 40) / 24) * 0.60);
		int maxCount = 0;
		for (int i = 0; i < count.length; i++)
			if (maxCount < count[i])
				maxCount = count[i];
		int x = 30;
		g.drawLine(10, height - 45, width - 10, height - 45);
		for (int i = 0; i < count.length; i++) {
			int barHeight = (int) (((double) count[i] / (double) maxCount) * (height - 55));
			g.drawRect(x, height - 45 - barHeight, individualWidth, barHeight);
			g.drawString((char) (65 + i) + " ", x, height - 30);
			x += interval;
		}
	}

	public void showHistogram(int[] count) {
		this.count = count;
		repaint();
	}
}
