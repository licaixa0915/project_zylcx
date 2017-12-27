import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class SimpleEventDemoAnonymousInnerClass extends JFrame {
	public static void main(String[] args) {
		JFrame frame = new SimpleEventDemoAnonymousInnerClass();
		frame.setTitle("SimpleEventDemoInnerClass");
		frame.setSize(220, 80);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public SimpleEventDemoAnonymousInnerClass() {
		JButton jbtOK = new JButton("OK");
		setLayout(new FlowLayout());
		add(jbtOK);

		jbtOK.addActionListener(new ActionListener() { // 匿名内部类
					public void actionPerformed(ActionEvent e) {
						System.out.println("Is is OK");
					}
				}); // 匿名内部类
	}
}
