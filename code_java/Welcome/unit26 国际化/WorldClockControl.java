import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.util.*;

@SuppressWarnings("serial")
public class WorldClockControl extends JPanel {
	private Locale[] availableLocales = Locale.getAvailableLocales();
	private String[] availableTimeZones = TimeZone.getAvailableIDs();
	private WorldClock clock = new WorldClock();
	@SuppressWarnings("rawtypes")
	private JComboBox jcbLocales = new JComboBox();
	@SuppressWarnings("rawtypes")
	private JComboBox jcbTimeZones = new JComboBox();

	public WorldClockControl() {
		setAvailableLocales();
		setAvailableTimeZones();

		clock.setLocale(availableLocales[jcbLocales.getSelectedIndex()]);
		clock.setTimeZone(TimeZone.getTimeZone(availableTimeZones[jcbTimeZones
				.getSelectedIndex()]));
		JPanel panel1 = new JPanel();
		panel1.setLayout(new GridLayout(2, 1));
		panel1.add(new JLabel("Locale"));
		panel1.add(new JLabel("Time Zone"));

		JPanel panel2 = new JPanel();
		panel2.setLayout(new GridLayout(2, 1));
		panel2.add(jcbLocales, BorderLayout.CENTER);
		panel2.add(jcbTimeZones, BorderLayout.CENTER);

		JPanel panel3 = new JPanel();
		panel3.setLayout(new BorderLayout());
		panel3.add(panel1, BorderLayout.WEST);
		panel3.add(panel2, BorderLayout.CENTER);

		setLayout(new BorderLayout());
		add(panel3, BorderLayout.NORTH);
		add(clock, BorderLayout.CENTER);

		jcbLocales.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				clock
						.setLocale(availableLocales[jcbLocales
								.getSelectedIndex()]);
			}
		});

		jcbTimeZones.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				clock.setTimeZone(TimeZone
						.getTimeZone(availableTimeZones[jcbTimeZones
								.getSelectedIndex()]));
			}
		});
	}

	@SuppressWarnings("unchecked")
	private void setAvailableLocales() {
		for (int i = 0; i < availableLocales.length; i++) {
			jcbLocales.addItem(availableLocales[i].getDisplayName() + " "
					+ availableLocales[i].toString());
		}
	}

	@SuppressWarnings("unchecked")
	private void setAvailableTimeZones() {
		for (int i = 0; i < availableTimeZones.length; i++) {
			jcbTimeZones.addItem(availableTimeZones[i]);
		}
	}
}
