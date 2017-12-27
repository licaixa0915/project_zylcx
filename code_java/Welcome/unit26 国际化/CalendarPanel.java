import java.awt.*;

import javax.swing.*;
import javax.swing.border.LineBorder;
import java.util.*;
import java.text.*;

@SuppressWarnings("serial")
public class CalendarPanel extends JPanel {
	private Calendar calendar;
	private JLabel[] jlblDay = new JLabel[49];
	private JLabel jlblHeader = new JLabel(" ", JLabel.CENTER);
	private int month;
	private int year;
	
	@SuppressWarnings("static-access")
	public CalendarPanel() {
		JPanel jpDays = new JPanel();
		jpDays.setLayout(new GridLayout(7, 1));
		for (int i = 0; i < 49; i++) {
			jpDays.add(jlblDay[i] = new JLabel());
			jlblDay[i].setBorder(new LineBorder(Color.BLACK, 1));
			jlblDay[i].setHorizontalAlignment(JLabel.RIGHT);
			jlblDay[i].setVerticalAlignment(JLabel.TOP);
		}
		
		this.setLayout(new BorderLayout());
		this.add(jlblHeader, BorderLayout.NORTH);
		this.add(jpDays, BorderLayout.CENTER);
		
		calendar = new GregorianCalendar();
		month = calendar.get(calendar.MONTH) + 1;
		year = calendar.get(calendar.YEAR);
		showHeader();
		showDayNames();
		showDays();
	}
	
	public void changeLocale(Locale newLocale) {
		setLocale(newLocale);
		showHeader();
		showDayNames();
	}
	
	public int getMonth() {
		return month;
	}
	
	public int getYear() {
		return year;
	}
	
	public void setMonth(int newMonth) {
		month = newMonth;
		showDays();
	}
	
	public void setYear(int newYear) {
		year = newYear;
		showDays();
	}
	
	private void showDayNames() {
		DateFormatSymbols dfs =  new DateFormatSymbols(getLocale());
		String dayNames[] = dfs.getWeekdays();
		for (int i = 0; i < 7; i++) {
			jlblDay[i].setText(dayNames[i + 1]);
			jlblDay[i].setHorizontalAlignment(JLabel.CENTER);
		}
	}
	
	@SuppressWarnings("static-access")
	private void showDays() {
		System.out.println("month=" + month);
		calendar.set(calendar.YEAR, year);
		calendar.set(calendar.MONTH, month - 1);
		calendar.set(calendar.DATE, 1);
		int startingDayOfMonth = calendar.get(Calendar.DAY_OF_WEEK);
		System.out.println("startingDayOfMonth=" + startingDayOfMonth);
//		Calendar cloneCalendar = (Calendar)calendar.clone();
//		cloneCalendar.add(Calendar.DATE, -1);
//		int daysInMonth = cloneCalendar.getActualMaximum(Calendar.DAY_OF_MONTH);
		
		int daysInMonth = calendar.getActualMaximum(Calendar.DAY_OF_MONTH);
		System.out.println("Calendar.DAY_OF_MONTH=" + Calendar.DAY_OF_MONTH);
		// Fill the calendar with the days before this month
		for (int i = 0; i < startingDayOfMonth - 1; i++) {
			jlblDay[i + 7].setForeground(Color.RED);
			jlblDay[i + 7].setText(daysInMonth - startingDayOfMonth + 2 + i + "");
		}
		
		for (int i = 1; i <= daysInMonth; i++) {
			jlblDay[i - 2 + startingDayOfMonth + 7].setForeground(Color.BLACK);
			jlblDay[i - 2 + startingDayOfMonth + 7].setText(i + "");
		}
		
		int j = 1;
		for (int i = daysInMonth - 1 + startingDayOfMonth + 7; i < 49; i++) {
			jlblDay[i].setForeground(Color.RED);
			jlblDay[i].setText(j++ + "");
		}
		
		showHeader();
	}
	
	private void showHeader() {
		SimpleDateFormat sdf = new SimpleDateFormat("MMMM yyyy", getLocale());
		String header = sdf.format(calendar.getTime());
		jlblHeader.setText(header);
	}
}
