import java.awt.*;
import javax.swing.*;

@SuppressWarnings("serial")
// 弹簧布局管理器
public class ShowSpringLayout extends JApplet {
	public ShowSpringLayout() {
		SpringLayout springLayout = new SpringLayout();
		JPanel p1 = new JPanel(springLayout);
		JButton jbt1 = new JButton("Button 1");
		p1.add(jbt1);

		Spring spring = Spring.constant(0, 1000, 2000);
		springLayout.putConstraint(SpringLayout.WEST, jbt1, spring,
				SpringLayout.WEST, p1);
		springLayout.putConstraint(SpringLayout.EAST, p1, spring,
				SpringLayout.EAST, jbt1);
		springLayout.putConstraint(SpringLayout.NORTH, jbt1, spring,
				SpringLayout.NORTH, p1);
		springLayout.putConstraint(SpringLayout.SOUTH, p1, spring,
				SpringLayout.SOUTH, jbt1);
		add(p1, BorderLayout.CENTER);
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("ShowSpringLayout");
		ShowSpringLayout applet = new ShowSpringLayout();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.setSize(200, 300);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
