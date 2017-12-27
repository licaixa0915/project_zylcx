import javax.swing.*;
import java.awt.*;

@SuppressWarnings("serial")
public class TestFigurePanel extends JFrame {
	public static void main(String[] args) {
		TestFigurePanel frame = new TestFigurePanel();
		frame.setTitle("TestFigurePanel");
		frame.setSize(400, 200);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	public TestFigurePanel() {
		setLayout(new GridLayout(2, 3, 5, 5));
		add(new FigurePanel(FigurePanel.LINE));
		add(new FigurePanel(FigurePanel.RECTANGLE));
		add(new FigurePanel(FigurePanel.ROUND_RECTANGLE));
		add(new FigurePanel(FigurePanel.OVAL));
		add(new FigurePanel(FigurePanel.RECTANGLE, true));
		add(new FigurePanel(FigurePanel.ROUND_RECTANGLE, true));
	}
}
