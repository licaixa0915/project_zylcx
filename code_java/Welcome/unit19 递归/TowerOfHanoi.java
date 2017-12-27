import javax.swing.JOptionPane;

public class TowerOfHanoi {
	public static void main(String[] args) {
		String intString = JOptionPane
				.showInputDialog("Enter number of disks: ");
		int n = Integer.parseInt(intString);
		System.out.println("The moves are:");
		moveDisks(n, 'A', 'B', 'C');
	}

	public static void moveDisks(int n, char fromTower, char toTower,
			char auxTower) {
		if (n == 1)
			System.out.println("Move disk " + n + " from " + fromTower + " to "
					+ toTower);
		else {
			moveDisks(n - 1, fromTower, auxTower, toTower);
			System.out.println("Move disk " + n + " from " + fromTower + " to "
					+ toTower);
			moveDisks(n - 1, auxTower, toTower, fromTower);
		}
	}
}
