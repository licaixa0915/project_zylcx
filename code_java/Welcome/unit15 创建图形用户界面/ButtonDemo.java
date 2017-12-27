import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
public class ButtonDemo extends JFrame {
	public static void main(String[] args) {
		ButtonDemo frame = new ButtonDemo();
		frame.setTitle("ButtonDemo");
		frame.setSize(250, 100);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	private JButton jbtLeft = new JButton("Left");
	private JButton jbtRight = new JButton("Right");

	protected MessagePanel messagePanel = new MessagePanel("Welcome to Java");

	public ButtonDemo() {
		messagePanel.setBackground(Color.WHITE);
		JPanel jpButtons = new JPanel();
		jpButtons.setLayout(new FlowLayout());
		jpButtons.add(jbtLeft);
		jpButtons.add(jbtRight);
		jbtLeft.setMnemonic('L'); // …Ë÷√»»º¸
		jbtRight.setMnemonic('R');

		// jbtLeft.setIcon(new ImageIcon("image/left.gif"));
		// jbtRight.setIcon(new ImageIcon("image/right.gif"));
		// jbtLeft.setText("Left");
		// jbtRight.setText(null);

		jbtLeft.setToolTipText("Move message to left");
		jbtRight.setToolTipText("Move message to right");

		setLayout(new BorderLayout());
		add(messagePanel, BorderLayout.CENTER);
		add(jpButtons, BorderLayout.SOUTH);

		jbtLeft.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				messagePanel.moveLeft();
			}
		});

		jbtRight.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				messagePanel.moveRight();
			}
		});
	}
}
