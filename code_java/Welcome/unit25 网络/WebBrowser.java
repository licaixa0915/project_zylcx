import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import java.net.URL;
import javax.swing.event.*;

import java.io.*;

@SuppressWarnings("serial")
public class WebBrowser extends JApplet {
	public static boolean isSubstring(String first, String second) {
		int remainingLength = second.length();
		int startingIndex = 0;

		// Note toWhile is a label. You can use break with a label
		// attached.
		toWhile: while (first.length() <= remainingLength) {
			// What is wrong if the following line is used
			// for (int i=startingIndex; i<=first.length(); i++)
			for (int i = 0; i < first.length(); i++) {
				if (first.charAt(i) != second.charAt(startingIndex + i)) {
					startingIndex++;
					remainingLength--;
					continue toWhile;
				}
			}

			return true;
		}

		return false;
	}
	public static void main(String[] args) {
		// Create a frame
		JFrame frame = new JFrame("WebBrowser");

		// Create an instance of the applet
		WebBrowser applet = new WebBrowser();

		// Add the applet instance to the frame
		frame.getContentPane().add(applet, BorderLayout.CENTER);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// Invoke init() and start()
		applet.init();
		applet.start();

		// Display the frame
		frame.setSize(600, 600);
		// frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	private JEditorPane jep = new JEditorPane();

	private JLabel jlblURL = new JLabel("URL");

	private JTextField jtfURL = new JTextField();

	public void init() {
		JPanel jpURL = new JPanel();
		jpURL.setLayout(new BorderLayout());
		jpURL.add(jlblURL, BorderLayout.WEST);
		jpURL.add(jtfURL, BorderLayout.CENTER);
		JScrollPane jspViewer = new JScrollPane();
		jspViewer.getViewport().add(jep, null);
		add(jspViewer, BorderLayout.CENTER);
		add(jpURL, BorderLayout.NORTH);
		jep.setEditable(false);

		jep.addHyperlinkListener(new HyperlinkListener() {
			public void hyperlinkUpdate(HyperlinkEvent e) {
				try {
					jep.setPage(e.getURL());
				} catch (IOException ex) {
					System.out.println(ex);
				}
			}
		});

		jtfURL.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String urlString = jtfURL.getText().trim();
				if (isSubstring("www", urlString)) {
					urlString = "http://" + urlString;
				}

				URL url;
				try {
					if (isSubstring("http", urlString)) {
						// Get the URL from remote Web server
						url = new URL(urlString);
					} else {
						// Get the URL from local host
						url = this.getClass().getResource(urlString);
					}

					System.out.println("urlString " + urlString);

					// Display the HTML file
					jep.setPage(url);
				} catch (IOException ex) {
					System.out.println(ex);
				}
			}
		});
	}
}
