import javax.swing.*;

@SuppressWarnings("serial")
public class EventDispatcherThreadDemo extends JApplet {
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				JFrame frame = new JFrame("EventDispatcherThreadDemo");
				frame.add(new EventDispatcherThreadDemo());
				frame.setSize(200, 200);
				frame.setLocationRelativeTo(null);
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
			}
		});
	}

	public EventDispatcherThreadDemo() {
		add(new JLabel("Hi, it runs from an event dispatcher thread"));
	}
}
