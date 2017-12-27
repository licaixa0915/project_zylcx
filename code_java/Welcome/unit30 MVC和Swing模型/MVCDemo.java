import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
public class MVCDemo extends JApplet {
	private JButton jbtController = new JButton("Show Controller");
	private JButton jbtView = new JButton("Show View");
	private CircleModel model = new CircleModel();

	public MVCDemo() {
		setLayout(new FlowLayout());
		add(jbtController);
		add(jbtView);

		jbtController.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFrame frame = new JFrame("Controller");
				CircleController controller = new CircleController();
				controller.setModel(model);
				frame.add(controller);
				frame.setSize(200, 200);
				frame.setLocation(200, 200);
				frame.setVisible(true);
			}
		});

		jbtView.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFrame frame = new JFrame("View");
				ViewController view = new ViewController();
				view.setModel(model);
				frame.add(view);
				frame.setSize(500, 200);
				frame.setLocation(200, 200);
				frame.setVisible(true);
			}
		});
	}
}
