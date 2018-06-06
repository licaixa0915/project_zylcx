import java.sql.*;

public class TestJDBC {
	
	Connection conn;
    PreparedStatement ps;
    ResultSet rs;
    
    /* �������ݿ� */
    public Connection getConnection() {
    	String url = "jdbc:mysql://localhost/javabook";
    	String userName = "root";
        String password = "Changeme_123";
        
        try {
            Class.forName("com.mysql.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            //  Auto-generated catch block
            System.out.println("can not found driver");
            e.printStackTrace();
        }
        try {
            conn = DriverManager.getConnection(url, userName, password);
            if(conn!=null){
                System.out.println("connection successful");
            }
        } catch (SQLException e) {
            //  Auto-generated catch block
            System.out.println("connection fail");
            e.printStackTrace();
        }
        
        return conn;
    }
    
    /* ��ѯ���ݿ��¼ */
    public void query() {
        // 1��ִ�о�̬SQL��䡣ͨ��ͨ��Statementʵ��ʵ�֡�   
        // 2��ִ�ж�̬SQL��䡣ͨ��ͨ��PreparedStatementʵ��ʵ�֡�   
        // 3��ִ�����ݿ�洢���̡�ͨ��ͨ��CallableStatementʵ��ʵ�֡�
        System.out.println("query");
        UserInfo u;

        String sql="select * from userInfo";
        try {
            conn = getConnection();//�������ݿ�
            ps = conn.prepareStatement(sql);// ����Satement�����ò���
            rs = ps.executeQuery(sql);  // ִ��SQL���
            // 4.��������
            while(rs.next()){
                u = new UserInfo();
                u.setUserId(rs.getInt("userId"));
                u.setUserName(rs.getString("userName"));
                u.setPassword(rs.getString("password"));
                u.setRemark(rs.getString("remark"));
                System.out.println("uesrId��" + u.getUserId() +", uesrName��" +u.getUserName()
                	+ ", password��"+ u.getPassword() + ", remark��" + u.getRemark());
            }
        } catch (SQLException e) {
            //  Auto-generated catch block
            e.printStackTrace();
        }finally{
            // �ͷ���Դ
            try {
                rs.close();
                ps.close();
                conn.close();
            } catch (SQLException e) {
                //  Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
    
    /* ������ݿ��¼ */
    public int insert(UserInfo user){
        int row = 0;
        System.out.println("insert");

        String sql = "insert into userInfo(userId,userName,password,remark) values(?,?,?,?)";
        try {
            conn = getConnection();// �������ݿ�
            ps = conn.prepareStatement(sql);// ����Satement�����ò���
            // rs=ps.executeQuery();  // ִ��SQL���,�������ڲ����Z��
            // sql�����д�˼����ֶΣ�����ͱ��Ҫ�м����ֶ�
            ps.setInt(1, user.getUserId());
            ps.setString(2, user.getUserName());
            ps.setString(3, user.getPassword());
            ps.setString(4, user.getRemark());
            // ��������
            row = ps.executeUpdate();
            System.out.println(row + " " + user.getUserId() + " " + user.getUserName()
                  + " " + user.getPassword() + " " + user.getRemark());
        } catch (SQLException e) {
            //  Auto-generated catch block
            e.printStackTrace();
        }finally{
            try {
                ps.close();
                conn.close();
            } catch (SQLException e) {
                //  Auto-generated catch block
                e.printStackTrace();
            }
            
        }
        return row;
    }

    /* �������ݿ��¼ */
    public int update(UserInfo user){
        int row = 0;
        System.out.println("update");

        String sql = "update userInfo set userName=?,password=?,remark=? where userId=?";
        try {
            conn = getConnection(); //�������ݿ�
            ps = conn.prepareStatement(sql); // ����Satement�����ò���

            // sql�����д�˼����ֶΣ�����ͱ��Ҫ�м����ֶ�
            ps.setString(1, user.getUserName());
            ps.setString(2, user.getPassword());
            ps.setString(3, user.getRemark());
            ps.setInt(4, user.getUserId());
            // ��������
            row = ps.executeUpdate();
            System.out.println(row + " " + user.getUserId() + " " + user.getUserName()
            	+ " " + user.getPassword() + " " + user.getRemark());
        } catch (SQLException e) {
            //  Auto-generated catch block
            e.printStackTrace();
        }finally{
            try {
                ps.close();
                conn.close();
            } catch (SQLException e) {
                //  Auto-generated catch block
                e.printStackTrace();
            }
        }
        return row;
    }

    /* ɾ�����ݿ��¼ */
    public int delete(UserInfo user){
        int row = 0;
        System.out.println("delete");
        
        String sql="delete from userInfo where userId=?";

        try {
            conn = getConnection();//�������ݿ�
            ps = conn.prepareStatement(sql); // ����Satement�����ò���

            ps.setInt(1, user.getUserId());
            // ��������
            row = ps.executeUpdate();
            System.out.println(row);
        } catch (SQLException e) {
            //  Auto-generated catch block
            e.printStackTrace();
        }finally{
            //�ͷ���Դ
            try {
                ps.close();
                conn.close();
            } catch (SQLException e) {
                //  Auto-generated catch block
                e.printStackTrace();
            }
        }
        return row;
    }
    
    public int deleteAll(){
        int row = 0;
        System.out.println("deleteAll");
        String sql = "delete from userInfo";

        try {
            conn = getConnection();//�������ݿ�
            ps = conn.prepareStatement(sql); // ����Satement�����ò���

            // ��������
            row = ps.executeUpdate();
            System.out.println(row);
        } catch (SQLException e) {
            //  Auto-generated catch block
            e.printStackTrace();
        }finally{
            //�ͷ���Դ
            try {
                ps.close();
                conn.close();
            } catch (SQLException e) {
                //  Auto-generated catch block
                e.printStackTrace();
            }
        }
        return row;
    }

	public static void main(String[] args) {
		TestJDBC j=new TestJDBC();
		j.deleteAll();
		j.query();
		
  		UserInfo u = new UserInfo();
  		u.setUserId(5);
  		u.setUserName("cool");
  		u.setPassword("123abc");
  		j.insert(u);
  		j.query();

  		u.setPassword("123abcefg");
  		j.update(u);
  		j.query();
  		
  		j.delete(u);
  		j.query();
	}
}
