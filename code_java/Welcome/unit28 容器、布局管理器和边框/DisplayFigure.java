import java.awt.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class DisplayFigure extends JApplet {
	private JTabbedPane jtpFigure = new JTabbedPane(); // ѡ�
	private FigurePanel squarePanel = new FigurePanel();
	private FigurePanel rectanglePanel = new FigurePanel();
	private FigurePanel circlepanel = new FigurePanel();
	private FigurePanel ovalPanel = new FigurePanel();

	public DisplayFigure() {
		squarePanel.setType(FigurePanel.LINE);
		rectanglePanel.setType(FigurePanel.RECTANGLE);
		circlepanel.setType(FigurePanel.ROUND_RECTANGLE);
		ovalPanel.setType(FigurePanel.OVAL);

		add(jtpFigure, BorderLayout.CENTER);
		jtpFigure.add(squarePanel, "Line");
		jtpFigure.add(rectanglePanel, "Rectangle");
		jtpFigure.add(circlepanel, "Round Rectangle");
		jtpFigure.add(ovalPanel, "Oval");

		jtpFigure.setToolTipTextAt(0, "Line");
		jtpFigure.setToolTipTextAt(1, "Rectangle");
		jtpFigure.setToolTipTextAt(2, "Round Rectangle");
		jtpFigure.setToolTipTextAt(3, "Oval");
	}

	public static void main(String[] args) {
		JFrame frame = new JFrame("DisplayFigure");
		DisplayFigure applet = new DisplayFigure();
		frame.add(applet);

		applet.init();
		applet.start();

		frame.setSize(400, 300);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
