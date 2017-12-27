import java.rmi.*;

public interface TicTacToeInterface extends Remote {
	public char connect(CallBack client) throws RemoteException;

	public void myMove(int row, int column, char token) throws RemoteException;
}
