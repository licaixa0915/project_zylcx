import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

class OKListener implements ActionListener {
	public void actionPerformed(ActionEvent e) {
		System.out.println("Is is OK");
	}
}

@SuppressWarnings("serial")
public class SimpleEventDemo extends JFrame {
	public static void main(String[] args) {
		JFrame frame = new SimpleEventDemo();
		frame.setTitle("SimpleEventDemo");
		frame.setSize(220, 80);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public SimpleEventDemo() {
		JButton jbtOK = new JButton("OK");
		setLayout(new FlowLayout());
		add(jbtOK);

		ActionListener listener = new OKListener();
		jbtOK.addActionListener(listener);
	}
}
