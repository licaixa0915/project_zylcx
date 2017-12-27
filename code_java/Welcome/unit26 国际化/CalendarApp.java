import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.*;
import java.util.*;

@SuppressWarnings("serial")
public class CalendarApp extends JApplet {
	private CalendarPanel calendarPanel = new CalendarPanel();
	private JButton jbtNext = new JButton("Next");
	private JButton jbtPrior = new JButton("Prior");
	@SuppressWarnings("rawtypes")
	private JComboBox jcboLocale = new JComboBox();
	private Locale locales[] = Calendar.getAvailableLocales();

	@SuppressWarnings("unchecked")
	public void init() {
		JPanel jpLocale = new JPanel();
		jpLocale.setBorder(new TitledBorder("Choose a locale"));
		jpLocale.setLayout(new FlowLayout());
		jpLocale.add(jcboLocale);
		for (int i = 0; i < locales.length; i++)
			jcboLocale.addItem(locales[i].getDisplayName());
		JPanel jpButtons = new JPanel();
		jpButtons.setLayout(new FlowLayout());
		jpButtons.add(jbtPrior);
		jpButtons.add(jbtNext);

		JPanel jpCalendar = new JPanel();
		jpCalendar.setLayout(new BorderLayout());
		jpCalendar.add(calendarPanel, BorderLayout.CENTER);
		jpCalendar.add(jpButtons, BorderLayout.SOUTH);

		setLayout(new BorderLayout());
		add(jpCalendar, BorderLayout.CENTER);
		add(jpLocale, BorderLayout.SOUTH);

		jcboLocale.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (e.getSource() == jcboLocale)
					calendarPanel.changeLocale(locales[jcboLocale
							.getSelectedIndex()]);
			}
		});

		jbtPrior.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				int currentMonth = calendarPanel.getMonth();
				if (currentMonth == 1) {
					calendarPanel.setMonth(12);
					calendarPanel.setYear(calendarPanel.getYear() - 1);
				} else
					calendarPanel.setMonth(currentMonth - 1);
			}
		});

		jbtNext.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				int currentMonth = calendarPanel.getMonth();
				if (currentMonth == 12) {
					calendarPanel.setMonth(1);
					calendarPanel.setYear(calendarPanel.getYear() + 1);
				} else
					calendarPanel.setMonth(currentMonth + 1);
			}
		});

		calendarPanel.changeLocale(locales[jcboLocale.getSelectedIndex()]);
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame("CalendarApp");
		CalendarApp applet = new CalendarApp();
		frame.add(applet);
		
		applet.init();
//		applet.start();
		
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
		
//		try {
//			Thread.sleep(5000);
//		} catch (InterruptedException e) {
//			// 
//			e.printStackTrace();
//		}
//		System.out.println("5000");
	}
}
