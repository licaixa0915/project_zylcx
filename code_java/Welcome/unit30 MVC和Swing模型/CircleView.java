import java.awt.*;
import java.awt.event.*;
import javax.swing.JPanel;

@SuppressWarnings("serial")
public class CircleView extends JPanel implements ActionListener {
	private CircleModel model;

	public void actionPerformed(ActionEvent actionEvent) {
		repaint();
	}

	public void setModel(CircleModel newModel) {
		model = newModel;
		if (model != null)
			model.addActionListener(this);
		repaint();
	}

	public CircleModel getModel() {
		return model;
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);

		if (model == null)
			return;
		g.setColor(model.getColor());
		int xCenter = getWidth() / 2;
		int yCenter = getHeight() / 2;
		int radius = (int) model.getRadius();
		if (model.isFilled()) {
			g.fillOval(xCenter - radius, yCenter - radius, 2 * radius,
					2 * radius);
		} else {
			g.drawOval(xCenter - radius, yCenter - radius, 2 * radius,
					2 * radius);
		}

	}
}
