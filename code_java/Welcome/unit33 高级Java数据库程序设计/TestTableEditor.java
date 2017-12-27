import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

@SuppressWarnings("serial")
public class TestTableEditor extends JApplet {
	private JComboBox<?> jcboDriver = new JComboBox<Object>(new String[] {
			"sun.jdbc.odbc.JdbcOdbcDriver", "com.mysql.jdbc.Driver",
			"oracle.jdbc.driver.OracleDriver" });
	private JComboBox<?> jcboURL = new JComboBox<Object>(new String[] {
			"jdbc:odbc:exampleMDBDataSource",
			"jdbc:mysql://localhost/javabook",
			"jdbc:oracle:thin:@liang.armstrong.edu:1521:orcl" });

	private JButton jbtConnect = new JButton("Connect to DB & Get Table");
	private JTextField jtfUserName = new JTextField();
	private JPasswordField jpfPassword = new JPasswordField();
	private JTextField jtfTableName = new JTextField();
	private TableEditor tableEditor1 = new TableEditor();
	private JLabel jlblStatus = new JLabel();

	/** Creates new form TestTableEditor */
	public TestTableEditor() {
		JPanel jPane1 = new JPanel(new GridLayout(5, 0));
		jPane1.add(jcboDriver);
		jPane1.add(jcboURL);
		jPane1.add(jtfUserName);
		jPane1.add(jpfPassword);
		jPane1.add(jtfTableName);

		JPanel jPanel2 = new JPanel(new GridLayout(5, 0));
		jPanel2.add(new JLabel("JDBC Driver"));
		jPanel2.add(new JLabel("Database URL"));
		jPanel2.add(new JLabel("Username"));
		jPanel2.add(new JLabel("Password"));
		jPanel2.add(new JLabel("Table Name"));

		JPanel jPane3 = new JPanel(new BorderLayout());
		jPane3.add(jbtConnect, BorderLayout.SOUTH);
		jPane3.add(jPanel2, BorderLayout.WEST);
		jPane3.add(jPane1, BorderLayout.CENTER);
		tableEditor1.setPreferredSize(new Dimension(400, 200));

		jcboURL.setEditable(true);
		jcboDriver.setEditable(true);

		add(new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, jPane3, tableEditor1),
				BorderLayout.CENTER);
		add(jlblStatus, BorderLayout.SOUTH);

		jbtConnect.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				try {
					Connection connection = getConnection();
					tableEditor1.setConnectionAndTable(connection, jtfTableName
							.getText().trim());
				} catch (Exception ex) {
					jlblStatus.setText(ex.toString());
				}
			}
		});
	}

	/** Connect to a database */
	private Connection getConnection() throws Exception {
		System.out.println((String) jcboDriver.getSelectedItem());
		Class.forName(((String) jcboDriver.getSelectedItem()).trim());
		System.out.println("Drive loaded");

		Connection connection = DriverManager.getConnection(((String) jcboURL
				.getSelectedItem()).trim(), jtfUserName.getText().trim(),
				new String(jpfPassword.getPassword()));
		jlblStatus.setText("Database connected");
		return connection;
	}

	/** Main method */
	public static void main(String[] args) {
		TestTableEditor applet = new TestTableEditor();
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(3);
		frame.setTitle("TestTableEditor");
		frame.add(applet, BorderLayout.CENTER);
		applet.init();
		applet.start();
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
	}
}
