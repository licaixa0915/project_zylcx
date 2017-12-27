import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

@SuppressWarnings("serial")
public class ViewController extends CircleView {
	public ViewController() {
		addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent e) {
				CircleModel model = getModel();
				if (model == null)
					return;
				if (e.isMetaDown())
					model.setRadius(model.getRadius() - 5);
				else
					model.setRadius(model.getRadius() + 5);
			}
		});
	}
}
