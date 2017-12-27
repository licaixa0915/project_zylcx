import javax.swing.JOptionPane;

public class A7_6RePrintCalendar {
	@SuppressWarnings("static-access")
	public static void main(String[] args) {
		// ����
		String yearString = JOptionPane
				.showInputDialog("Enter full year (e.g., 2001):");
		int year = Integer.parseInt(yearString);
		String monthString = JOptionPane
				.showInputDialog("Enter month in number between 1 and 12:");
		int month = Integer.parseInt(monthString);
		// ����������
		RePrintCalendar calendar = new RePrintCalendar(year, month);
		JOptionPane.showMessageDialog(null, calendar.printMonth());
	}
}

// ������
class RePrintCalendar {
	static int month;
	static String output = ""; // �������
	static int year;

	static String getMonthName() { // �����·�����
		String monthName = null;
		switch (month) {
		case 1:
			monthName = "January";
			break;
		case 2:
			monthName = "February";
			break;
		case 3:
			monthName = "March";
			break;
		case 4:
			monthName = "April";
			break;
		case 5:
			monthName = "May";
			break;
		case 6:
			monthName = "June";
			break;
		case 7:
			monthName = "July";
			break;
		case 8:
			monthName = "August";
			break;
		case 9:
			monthName = "September";
			break;
		case 10:
			monthName = "October";
			break;
		case 11:
			monthName = "November";
			break;
		case 12:
			monthName = "December";
			break;
		}
		return monthName;
	}

	static int getNumberOfDaysInMonth(int year, int month) { // ����year��month�µ�����
		if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
				|| month == 10 || month == 12)
			return 31;
		if (month == 4 || month == 6 || month == 9 || month == 11)
			return 30;
		if (month == 2)
			return isLeapYear(year) ? 29 : 28;
		return 0;
	}

	static int getStartDay() { // ���ؿ�ʼ����
		int startDay1800 = 3;
		int totalnumberOfDays = getTotalNumberOfDays();
		return (totalnumberOfDays + startDay1800) % 7;
	}

	static int getTotalNumberOfDays() { // ����������(��1800��1��1�������)
		int total = 0;
		for (int i = 1800; i < year; i++)
			if (isLeapYear(i))
				total += 366;
			else
				total += 365;
		for (int i = 1; i < month; i++)
			total += getNumberOfDaysInMonth(year, i);
		return total;
	}

	static boolean isLeapYear(int year) { // �ж��Ƿ�����
		return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
	}

	static String printMonth() { // ��ӡ����
		printMonthTitle();
		printMonthBody();
		return output;
	}

	static void printMonthBody() { // ��ӡ�������ڲ���
		int startday = getStartDay();
		int numberOfDaysInMonth = getNumberOfDaysInMonth(year, month);
		int i = 0;
		for (i = 0; i < startday; i++)
			output += "       ";
		for (i = 1; i <= numberOfDaysInMonth; i++) {
			if (i < 10)
				output += "     " + i;
			else
				output += "   " + i;
			if ((i + startday) % 7 == 0)
				output += "\n";
		}
		output += "\n";
	}

	static void printMonthTitle() { // ��ӡ�������ⲿ��
		output += "          " + getMonthName() + "  " + year + "\n";
		output += "-------------------------------------------\n";
		output += " Sun Mon Tus Wed Thu Fri Sat\n";
	}

	public RePrintCalendar() {
	}

	@SuppressWarnings("static-access")
	public RePrintCalendar(int year, int month) {
		this.year = year;
		this.month = month;
	}

	public int getMonth() {
		return month;
	}

	public int getYear() {
		return year;
	}

	@SuppressWarnings("static-access")
	public void setMonth(int month) {
		this.month = month;
	}

	@SuppressWarnings("static-access")
	public void setYear(int year) {
		this.year = year;
	}
}
