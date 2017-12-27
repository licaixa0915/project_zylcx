import java.io.*;
import java.net.*;
import java.awt.BorderLayout;
import javax.swing.*;

@SuppressWarnings("serial")
public class AppletClient extends JApplet {
	public static void main(String[] args) {
		JFrame frame = new JFrame("Applet client");
		AppletClient applet = new AppletClient();
		applet.isStandAlone = true;
		if (args.length == 1)
			applet.host = args[0];
		frame.getContentPane().add(applet, BorderLayout.CENTER);
		applet.init();
		applet.start();
		frame.pack();
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	private String host = "localhost";
	private boolean isStandAlone = false;

	private JLabel jlblCount = new JLabel();

	@SuppressWarnings("resource")
	public void init() {
		add(jlblCount);

		try {
			Socket socket;
			if (isStandAlone)
				socket = new Socket(host, 8000);
			else
				socket = new Socket(getCodeBase().getHost(), 8000);
			DataInputStream inputFromServer = new DataInputStream(socket
					.getInputStream());
			int count = inputFromServer.readInt();
			jlblCount.setText("You are visitor number " + count);
			inputFromServer.close();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}
}
