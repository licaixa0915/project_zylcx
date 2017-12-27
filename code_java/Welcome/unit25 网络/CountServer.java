import java.io.*;
import java.net.*;

public class CountServer {
	public static void main(String[] args) {
		new CountServer();
	}
	private int count;

	private RandomAccessFile raf;

	@SuppressWarnings("resource")
	public CountServer() {
		try {
			ServerSocket serverSocket = new ServerSocket(8000);
			System.out.println("Server started ");
			raf = new RandomAccessFile("count.dat", "rw");
			if (raf.length() == 0)
				count = 0;
			else
				count = raf.readInt();
			while (true) {
				Socket socket = serverSocket.accept();
				DataOutputStream outputToClient = new DataOutputStream(socket
						.getOutputStream());
				count++;
				outputToClient.writeInt(count);
				raf.seek(0);
				raf.writeInt(count);
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}
}
