import java.awt.*;
import java.util.Calendar;
import java.util.GregorianCalendar;
import javax.swing.*;

/**
 * 静态时钟面板
 * 
 * @author zhangyin,dongyu,panziyao
 * 
 */
@SuppressWarnings("serial")
public class StaticClock extends JPanel {
	protected int clockRadius;
	// 定义变量
	private int hour;
	private int minute;
	private int second;
	protected int xCenter;
	protected int yCenter;

	// 构造方法
	public StaticClock() {
		setCurrentTime();
	}

	// 构造一个具体时间的时钟面板
	public StaticClock(int hour, int minute, int second) {
		this.hour = hour;
		this.minute = minute;
		this.second = second;
	}

	/** Return hour */
	public int getHour() {
		return hour;
	}

	/** Return minute */
	public int getMinute() {
		return minute;
	}

	public Dimension getPreferredSize() {
		return new Dimension(200, 200);
	}

	/** Return second */
	public int getSecond() {
		return second;
	}

	// 绘制时钟
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);

		// 初始化参数
		clockRadius = (int) (Math.min(getSize().width, getSize().height) * 0.9 * 0.5);
		xCenter = (getSize().width) / 2;
		yCenter = (getSize().height) / 2;

		// 绘制时钟外围
		g.setColor(Color.black);
		g.drawOval(xCenter - clockRadius, yCenter - clockRadius,
				2 * clockRadius, 2 * clockRadius);

		// 绘制秒针
		int sLength = (int) (clockRadius * 0.7);
		int xSecond = (int) (xCenter + sLength
				* Math.sin(second * (2 * Math.PI / 60)));
		int ySecond = (int) (yCenter - sLength
				* Math.cos(second * (2 * Math.PI / 60)));
		g.setColor(Color.red);
		g.drawLine(xCenter, yCenter, xSecond, ySecond);

		// 绘制分针
		int mLength = (int) (clockRadius * 0.6);
		int xMinute = (int) (xCenter + mLength
				* Math.sin(minute * (2 * Math.PI / 60)));
		int yMinute = (int) (yCenter - mLength
				* Math.cos(minute * (2 * Math.PI / 60)));
		g.setColor(Color.blue);
		g.drawLine(xCenter, yCenter, xMinute, yMinute);

		// 绘制时针
		int hLength = (int) (clockRadius * 0.5);
		int xHour = (int) (xCenter + hLength
				* Math.sin((hour + minute / 60.0) * (2 * Math.PI / 12)));
		int yHour = (int) (yCenter - hLength
				* Math.cos((hour + minute / 60.0) * (2 * Math.PI / 12)));
		g.setColor(Color.green);
		g.drawLine(xCenter, yCenter, xHour, yHour);

		// 绘制刻度
		for (int i = 0; i < 60; i++) {
			double percent;
			if (i % 5 == 0) {
				percent = 0.9;
			} else {
				percent = 0.95;
			}
			int xOuter = (int) (xCenter + clockRadius
					* Math.sin(i * (2 * Math.PI / 60)));
			int yOuter = (int) (yCenter - clockRadius
					* Math.cos(i * (2 * Math.PI / 60)));
			int xInner = (int) (xCenter + percent * clockRadius
					* Math.sin(i * (2 * Math.PI / 60)));
			int yInner = (int) (yCenter - percent * clockRadius
					* Math.cos(i * (2 * Math.PI / 60)));
			g.setColor(Color.black);
			g.drawLine(xOuter, yOuter, xInner, yInner);
		}

		// 绘制时间数字
		g.setColor(Color.black);
		for (int i = 0; i < 12; i++) {
			int x = (int) (xCenter + 0.8 * clockRadius
					* Math.sin(i * (2 * Math.PI / 12)));
			int y = (int) (yCenter - 0.8 * clockRadius
					* Math.cos(i * (2 * Math.PI / 12)));
			g.drawString("" + ((i == 0) ? 12 : i), x, y);
		}
	}

	// 设置当前时间
	public void setCurrentTime() {
		Calendar calendar = new GregorianCalendar();
		this.hour = calendar.get(Calendar.HOUR_OF_DAY);
		this.minute = calendar.get(Calendar.MINUTE);
		this.second = calendar.get(Calendar.SECOND);
	}

	/** Set a new hour */
	public void setHour(int hour) {
		this.hour = hour;
		repaint();
	}

	/** Set a new minute */
	public void setMinute(int minute) {
		this.minute = minute;
		repaint();
	}

	/** Set a new second */
	public void setSecond(int second) {
		this.second = second;
		repaint();
	}
}
