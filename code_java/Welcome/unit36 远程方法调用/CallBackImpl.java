import java.rmi.*;
import java.rmi.server.*;

@SuppressWarnings("serial")
public class CallBackImpl extends UnicastRemoteObject implements CallBack {
	private TicTacToeClientRMI thisClient;

	public CallBackImpl(Object client) throws RemoteException {
		thisClient = (TicTacToeClientRMI) client;
	}

	public void takeTurn(boolean turn) throws RemoteException {
		thisClient.setMyTurn(turn);
	}

	public void notify(String message) throws RemoteException {
		thisClient.setMessage(message);
	}

	public void mark(int row, int column, char token) throws RemoteException {
		thisClient.mark(row, column, token);
	}
}
