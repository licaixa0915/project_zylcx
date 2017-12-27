import javax.swing.*;
import javax.swing.event.*;
import java.awt.BorderLayout;

@SuppressWarnings("serial")
// Î¢µ÷ÎÄ±¾Óò
public class SimpleSpinner extends JApplet {
	private JSpinner spinner = new JSpinner();
	private JLabel label = new JLabel(" ", JLabel.CENTER);

	public SimpleSpinner() {
		add(spinner, BorderLayout.NORTH);
		add(label, BorderLayout.CENTER);

		spinner.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				label.setText("Previous value: " + spinner.getPreviousValue()
						+ " current valus: " + spinner.getValue()
						+ " Next value: " + spinner.getNextValue());
			}
		});
	}
}
