import java.awt.event.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class ClockAnimation extends StillClock {
	private class TimerListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			setCurrentTime();
			repaint();
		}
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ClockAnimation");
		ClockAnimation clock = new ClockAnimation();
		frame.add(clock);
		frame.setSize(200, 200);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public ClockAnimation() {
		Timer timer = new Timer(1000, new TimerListener());
		timer.start();
	}
}
