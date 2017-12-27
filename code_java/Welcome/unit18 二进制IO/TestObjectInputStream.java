import java.io.*;

public class TestObjectInputStream {
	public static void main(String[] args) throws ClassNotFoundException,
			IOException {
		ObjectInputStream input = new ObjectInputStream(new FileInputStream(
				"Object.dat"));
		String name = input.readUTF();
		double score = input.readDouble();
		java.util.Date date = (java.util.Date) (input.readObject());
		System.out.println(name + " " + score + " " + date);
		input.close();
	}
}
