import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

// 前三秒不显示,有缺陷
@SuppressWarnings("serial")
public class ClockWithAudio extends JApplet {
	private class TimerListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			clock.setCurrentTime();
			clock.repaint();
			jlblDigitTime.setText(clock.getHour() + ":" + clock.getMinute()
					+ ":" + clock.getSecond());
			if (clock.getSecond() == 0)
				announceTime(clock.getHour(), clock.getMinute());
		}
	}
	private AudioClip amAudio = Applet.newAudioClip(this.getClass()
			.getResource("audio/am.au"));
	private StillClock clock = new StillClock();
	private AudioClip[] hourAudio = new AudioClip[12];
	private JLabel jlblDigitTime = new JLabel("", JLabel.CENTER);
	private AudioClip[] minuteAudio = new AudioClip[60];
	private AudioClip pmAudio = Applet.newAudioClip(this.getClass()
			.getResource("audio/pm.au"));

	private Timer timer = new Timer(1000, new TimerListener());

	public void announceTime(int hour, int minute) {
		hourAudio[hour % 12].play();
		try {
			Thread.sleep(1500);
			minuteAudio[minute].play();
			Thread.sleep(1500);
		} catch (InterruptedException e) {
		}
		if (hour < 12)
			amAudio.play();
		else
			pmAudio.play();
	}

	public void init() {
		for (int i = 0; i < 12; i++)
			hourAudio[i] = Applet.newAudioClip(this.getClass().getResource(
					"audio/hour" + i + ".au"));
		for (int i = 0; i < 60; i++)
			minuteAudio[i] = Applet.newAudioClip(this.getClass().getResource(
					"audio/minute" + i + ".au"));
		add(clock, BorderLayout.CENTER);
		add(jlblDigitTime, BorderLayout.SOUTH);
	}

	public void start() {
		timer.start();
	}

	public void stop() {
		timer.stop();
	}
}
