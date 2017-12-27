import javax.swing.*;
import javax.swing.event.*;

import java.util.*;
import java.text.*;
import java.awt.*;

@SuppressWarnings("serial")
// 微调文本域模型和编辑器
public class SpinnerModelEditorDemo extends JApplet {
	private JSpinner spinnerDate = new JSpinner(new SpinnerDateModel());
	private JSpinner spinnerDay = new JSpinner(new SpinnerNumberModel(29, 1,
			31, 1));
	private String[] monthNames = new DateFormatSymbols().getMonths();
	private JSpinner spinnerMonth = new JSpinner(new SpinnerListModel(Arrays
			.asList(monthNames).subList(0, 12)));
	private JSpinner spinnerYear = new JSpinner(new SpinnerNumberModel(2011, 1,
			3000, 1));

	public SpinnerModelEditorDemo() {
		JPanel panel1 = new JPanel();
		panel1.setLayout(new GridLayout(4, 1));
		panel1.add(new JLabel("Date"));
		panel1.add(new JLabel("Day"));
		panel1.add(new JLabel("Month"));
		panel1.add(new JLabel("Year"));

		JPanel panel2 = new JPanel();
		panel2.setLayout(new GridLayout(4, 1));
		panel2.add(spinnerDate);
		panel2.add(spinnerDay);
		panel2.add(spinnerMonth);
		panel2.add(spinnerYear);

		add(panel1, BorderLayout.WEST);
		add(panel2, BorderLayout.CENTER);

		JSpinner.DateEditor dateEditor = new JSpinner.DateEditor(spinnerDate,
				"MMM dd, yyyy");
		spinnerDate.setEditor(dateEditor);
		JSpinner.NumberEditor yearEditor = new JSpinner.NumberEditor(
				spinnerYear, "####");
		spinnerYear.setEditor(yearEditor);

		spinnerMonth.getModel().setValue("三月");

		updateDate();

		spinnerDay.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				updateDate();
			}
		});

		spinnerMonth.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				updateDate();
			}
		});

		spinnerYear.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				updateDate();
			}
		});
	}

	private void updateDate() {
		int month = ((SpinnerListModel) spinnerMonth.getModel()).getList()
				.indexOf(spinnerMonth.getValue());
		int year = ((Integer) spinnerYear.getValue()).intValue();

		SpinnerNumberModel numberModel = (SpinnerNumberModel) spinnerDay
				.getModel();
		numberModel.setMaximum(new Integer(maxDaysInMonth(year, month)));
		if (((Integer) (numberModel.getValue())).intValue() > maxDaysInMonth(
				year, month))
			numberModel.setValue(new Integer(maxDaysInMonth(year, month)));

		int day = ((Integer) spinnerDay.getValue()).intValue();

		spinnerDate.setValue(new GregorianCalendar(year, month, day).getTime());
	}

	private int maxDaysInMonth(int year, int month) {
		Calendar calendar = new GregorianCalendar(year, month, 1);
		return calendar.getActualMaximum(Calendar.DAY_OF_MONTH);
	}
}
