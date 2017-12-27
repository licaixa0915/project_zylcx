import java.util.concurrent.*;

// 线程冲突,无法达到效果
public class AccountWithoutSync {
	private static class Account {
		private int balance = 0;

		public void deposit(int amount) {
			int newBalance = balance + amount;
			try {
				Thread.sleep(5);
			} catch (InterruptedException ex) {
			}
			balance = newBalance;
		}

		public int getBalance() {
			return balance;
		}
	}

	private static class AddAPennyTask implements Runnable {
		public void run() {
			account.deposit(1);
		}
	}

	private static Account account = new Account();

	public static void main(String[] args) {
		ExecutorService exectuor = Executors.newCachedThreadPool();
		for (int i = 0; i < 100; i++) {
			exectuor.execute(new AddAPennyTask());
		}
		exectuor.shutdown();
		while (!exectuor.isTerminated()) {
		}
		System.out.println("What is balance ? " + account.getBalance());
	}
}
