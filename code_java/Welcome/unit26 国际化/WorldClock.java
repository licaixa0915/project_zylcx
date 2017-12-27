import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Calendar;
import java.util.TimeZone;
import java.util.GregorianCalendar;
import java.text.*;

@SuppressWarnings("serial")
public class WorldClock extends JPanel {
	private class TimerListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			Calendar calendar = new GregorianCalendar(timeZone, getLocale());
			clock.setHour(calendar.get(Calendar.HOUR));
			clock.setMinute(calendar.get(Calendar.MINUTE));
			clock.setSecond(calendar.get(Calendar.SECOND));
			DateFormat formatter = DateFormat.getDateTimeInstance(
					DateFormat.MEDIUM, DateFormat.LONG, getLocale());
			formatter.setTimeZone(timeZone);
			jlblDigitTime.setText(formatter.format(calendar.getTime()));
		}
	}
	private StaticClock clock = new StaticClock();
	private JLabel jlblDigitTime = new JLabel("", JLabel.CENTER);
	private Timer timer = new Timer(1000, new TimerListener());

	private TimeZone timeZone = TimeZone.getTimeZone("EST");

	public WorldClock() {
		setLayout(new BorderLayout());
		add(clock, BorderLayout.CENTER);
		add(jlblDigitTime, BorderLayout.SOUTH);
		timer.start();
	}

	public void setTimeZone(TimeZone timeZone) {
		this.timeZone = timeZone;
	}
}
