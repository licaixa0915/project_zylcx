import java.awt.*;
import javax.swing.*;

@SuppressWarnings("serial")
// 不使用布局管理器
public class ShowNoLayout extends JApplet {
	private JLabel jlbl = new JLabel("Resize the Window and Study No Layout",
			JLabel.CENTER);
	private JTextArea jta1 = new JTextArea("Text Area 1", 5, 10);
	private JTextArea jta2 = new JTextArea("Text Area 2", 5, 10);
	private JTextField jtf = new JTextField("JtextField");
	private JPanel jp = new JPanel();
	private JButton jbt1 = new JButton("Cancel");
	private JButton jbt2 = new JButton("OK");

	public ShowNoLayout() {
		jp.setBackground(Color.red);
		setLayout(null);

		add(jlbl);
		add(jp);
		add(jta1);
		add(jta2);
		add(jtf);
		add(jbt1);
		add(jbt2);

		jlbl.setBounds(0, 10, 400, 40);
		jta1.setBounds(0, 50, 100, 100);
		jp.setBounds(100, 50, 200, 100);
		jta2.setBounds(300, 50, 100, 50);
		jtf.setBounds(300, 100, 100, 50);
		jbt1.setBounds(100, 150, 100, 50);
		jbt2.setBounds(200, 150, 100, 50);
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ShowNoLayout");
		ShowNoLayout applet = new ShowNoLayout();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.setSize(640, 480);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
