import javax.swing.*;

@SuppressWarnings("serial")
public class WorldClockApp extends JApplet {
	public WorldClockApp() {
		add(new WorldClockControl());
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame("WorldClockApp");
		WorldClockApp applet = new WorldClockApp();
		frame.add(applet);
		
//		applet.init();
//		applet.start();
		
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
