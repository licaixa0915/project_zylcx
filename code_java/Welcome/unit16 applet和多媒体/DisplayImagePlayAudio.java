import javax.swing.*;
import java.net.URL;
import java.applet.*;

@SuppressWarnings("serial")
public class DisplayImagePlayAudio extends JApplet {
	private AudioClip audioClip;

	public DisplayImagePlayAudio() {
		URL urlForImage = getClass().getResource("image/denmark.gif");
		add(new JLabel(new ImageIcon(urlForImage)));
		URL urlForAudio = getClass().getResource("audio/denmark.mid");
		audioClip = Applet.newAudioClip(urlForAudio);
		audioClip.loop();
	}

	public void start() {
		if (audioClip != null)
			audioClip.loop();
	}

	public void stop() {
		if (audioClip != null)
			audioClip.stop();
	}
}
