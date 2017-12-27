import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class SimpleEventDemoInnerClass extends JFrame {
	private class OKListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			System.out.println("Is is OK");
		}
	}

	public static void main(String[] args) {
		JFrame frame = new SimpleEventDemoInnerClass();
		frame.setTitle("SimpleEventDemoInnerClass");
		frame.setSize(220, 80);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public SimpleEventDemoInnerClass() {
		JButton jbtOK = new JButton("OK");
		setLayout(new FlowLayout());
		add(jbtOK);

		ActionListener listener = new OKListener();
		jbtOK.addActionListener(listener);
	}
}
