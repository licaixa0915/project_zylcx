import java.awt.Color;
import java.awt.event.*;
import java.util.*;

public class CircleModel {
	private double radius = 20;
	private boolean filled;
	private Color color;
	private ArrayList<ActionListener> actionListenerList;

	public double getRadius() {
		return radius;
	}

	public void setRadius(double radius) {
		this.radius = radius;
		processEvent(new ActionEvent(this, ActionEvent.ACTION_PERFORMED,
				"radius"));
	}

	public boolean isFilled() {
		return filled;
	}

	public void setFilled(boolean filled) {
		this.filled = filled;
		processEvent(new ActionEvent(this, ActionEvent.ACTION_PERFORMED,
				"filled"));
	}

	public Color getColor() {
		return color;
	}

	public void setColor(Color color) {
		this.color = color;
		processEvent(new ActionEvent(this, ActionEvent.ACTION_PERFORMED,
				"color"));
	}

	public synchronized void addActionListener(ActionListener l) {
		if (actionListenerList == null)
			actionListenerList = new ArrayList<ActionListener>();
		actionListenerList.add(l);
	}

	public synchronized void removeActionListener(ActionListener l) {
		if (actionListenerList != null && actionListenerList.contains(l))
			actionListenerList.remove(l);
	}

	@SuppressWarnings("unchecked")
	private void processEvent(ActionEvent e) {
		ArrayList<ActionListener> list;
		synchronized (this) {
			if (actionListenerList == null)
				return;
			list = (ArrayList<ActionListener>) actionListenerList.clone();
		}

		for (int i = 0; i < list.size(); i++) {
			ActionListener listener = (ActionListener) list.get(i);
			listener.actionPerformed(e);
		}
	}
}
