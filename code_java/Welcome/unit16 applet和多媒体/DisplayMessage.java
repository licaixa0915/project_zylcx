import javax.swing.*;

@SuppressWarnings("serial")
public class DisplayMessage extends JApplet {
	public void init() {
		String message = getParameter("MESSAGE");
		int x = Integer.parseInt(getParameter("X"));
		int y = Integer.parseInt(getParameter("Y"));

		MessagePanel messagePanel = new MessagePanel(message);
		messagePanel.setXCoordinate(x);
		messagePanel.setYCoordinate(y);
	}
}
