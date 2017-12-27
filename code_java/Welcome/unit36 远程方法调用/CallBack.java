import java.rmi.*;

public interface CallBack extends Remote {
	public void takeTurn(boolean turn) throws RemoteException;

	public void notify(String message) throws RemoteException;

	public void mark(int row, int column, char token) throws RemoteException;
}
