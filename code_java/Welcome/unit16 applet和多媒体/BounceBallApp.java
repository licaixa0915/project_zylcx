import javax.swing.*;

@SuppressWarnings("serial")
public class BounceBallApp extends JApplet {
	public BounceBallApp() {
		add(new BallControl());
	}
}
