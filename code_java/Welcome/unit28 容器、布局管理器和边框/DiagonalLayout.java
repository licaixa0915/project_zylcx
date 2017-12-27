import java.awt.*;
import java.io.Serializable;

@SuppressWarnings("serial")
public class DiagonalLayout implements LayoutManager, Serializable {
	private int gap = 10;
	private boolean majorDiagonal = true;
	private boolean lastFill = false;

	public DiagonalLayout() {
	}

	public void addLayoutComponent(String name, Component comp) {
	}

	public void removeLayoutComponent(Component comp) {
	}

	public Dimension preferredLayoutSize(Container parent) {
		return minimumLayoutSize(parent);
	}

	public Dimension minimumLayoutSize(Container parent) {
		return new Dimension(0, 0);
	}

	public int getGap() {
		return gap;
	}

	public void setGap(int gap) {
		this.gap = gap;
	}

	public boolean isMajorDiagonal() {
		return majorDiagonal;
	}

	public void setMajorDiagonal(boolean newMajorDiagonal) {
		majorDiagonal = newMajorDiagonal;
	}

	public boolean isLastFill() {
		return lastFill;
	}

	public void setLastFill(boolean newLastFill) {
		lastFill = newLastFill;
	}

	public void layoutContainer(Container parent) {
		int numberOfComponents = parent.getComponentCount();
		Insets insets = parent.getInsets();
		int w = parent.getSize().width - insets.left - insets.right;
		int h = parent.getSize().height - insets.bottom - insets.top;

		if (majorDiagonal) {
			int x = 10, y = 10;
			for (int j = 0; j < numberOfComponents; j++) {
				Component c = parent.getComponent(j);
				Dimension d = c.getPreferredSize();
				if (c.isVisible())
					if (lastFill && (j == numberOfComponents - 1))
						c.setBounds(x, y, w - x, h - y);
					else
						c.setBounds(x, y, d.width, d.height);
				x += d.width + gap;
				y += d.height + gap;
			}
		}
	}
}
