import javax.swing.JFrame;
import java.awt.event.*;

@SuppressWarnings("serial")
public class TestWindowEvent extends JFrame {
	public static void main(String[] args) {
		TestWindowEvent frame = new TestWindowEvent();
		frame.setTitle("TestWindowEvent");
		frame.setSize(220, 80);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public TestWindowEvent() {
		addWindowListener(new WindowListener() {
			public void windowActivated(WindowEvent e) {
				System.out.println("Window activated");
			}

			public void windowClosed(WindowEvent e) {
				System.out.println("Window closed");
			}

			public void windowClosing(WindowEvent e) {
				System.out.println("Window closing");
			}

			public void windowDeactivated(WindowEvent e) {
				System.out.println("Window deactivated");
			}

			public void windowDeiconified(WindowEvent e) {
				System.out.println("Window deiconified");
			}

			public void windowIconified(WindowEvent e) {
				System.out.println("Window inconified");
			}

			public void windowOpened(WindowEvent e) {
				System.out.println("Window opened");
			}
		});
	}
}
