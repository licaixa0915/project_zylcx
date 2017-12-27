import javax.swing.*;
import java.awt.event.*;

public class TestSourceListener {
	public static void main(String[] args) {
		JFrame frame = new JFrame("TestSourceListener");
		JButton jbt = new JButton("OK");
		frame.add(jbt);
		frame.setSize(200, 200);
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLocationRelativeTo(null);
		MyListener listener = new MyListener();
		jbt.addActionListener(listener);
	}
}

class MyListener implements ActionListener {
	public void actionPerformed(ActionEvent e) {
		System.out.println("I will process it!");
	}
}