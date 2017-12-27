import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

class CancelListenerClass implements ActionListener {
	public void actionPerformed(ActionEvent e) {
		System.out.println("Cancel button clicked");
	}
}

@SuppressWarnings("serial")
public class HandleEvent extends JFrame {
	public static void main(String[] args) {
		JFrame frame = new HandleEvent();
		frame.setTitle("Handle Event");
		frame.setSize(200, 150);
		frame.setLocation(200, 100);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public HandleEvent() {
		FlowLayout layout = new FlowLayout();
		setLayout(layout);

		JButton jbtOK = new JButton("OK");
		JButton jbtCancel = new JButton("Cancel");
		add(jbtOK);
		add(jbtCancel);

		OKListenerClass listener1 = new OKListenerClass();
		CancelListenerClass listener2 = new CancelListenerClass();
		jbtOK.addActionListener(listener1);
		jbtCancel.addActionListener(listener2);
	}
}

class OKListenerClass implements ActionListener {
	public void actionPerformed(ActionEvent e) {
		System.out.println("OK button clicked");
	}
}
