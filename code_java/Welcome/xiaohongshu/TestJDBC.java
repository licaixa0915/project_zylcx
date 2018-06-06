import java.sql.*;

public class TestJDBC {
	
	Connection conn;
    PreparedStatement ps;
    ResultSet rs;
    
    /* 连接数据库 */
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
    
    /* 查询数据库记录 */
    public void query() {
        // 1、执行静态SQL语句。通常通过Statement实例实现。   
        // 2、执行动态SQL语句。通常通过PreparedStatement实例实现。   
        // 3、执行数据库存储过程。通常通过CallableStatement实例实现。
        System.out.println("query");
        UserInfo u;

        String sql="select * from userInfo";
        try {
            conn = getConnection();//连接数据库
            ps = conn.prepareStatement(sql);// 创建Satement并设置参数
            rs = ps.executeQuery(sql);  // 执行SQL语句
            // 4.处理结果集
            while(rs.next()){
                u = new UserInfo();
                u.setUserId(rs.getInt("userId"));
                u.setUserName(rs.getString("userName"));
                u.setPassword(rs.getString("password"));
                u.setRemark(rs.getString("remark"));
                System.out.println("uesrId：" + u.getUserId() +", uesrName：" +u.getUserName()
                	+ ", password："+ u.getPassword() + ", remark：" + u.getRemark());
            }
        } catch (SQLException e) {
            //  Auto-generated catch block
            e.printStackTrace();
        }finally{
            // 释放资源
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
    
    /* 添加数据库纪录 */
    public int insert(UserInfo user){
        int row = 0;
        System.out.println("insert");

        String sql = "insert into userInfo(userId,userName,password,remark) values(?,?,?,?)";
        try {
            conn = getConnection();// 连接数据库
            ps = conn.prepareStatement(sql);// 创建Satement并设置参数
            // rs=ps.executeQuery();  // 执行SQL语句,仅仅用于查找語句
            // sql语句中写了几个字段，下面就必須要有几个字段
            ps.setInt(1, user.getUserId());
            ps.setString(2, user.getUserName());
            ps.setString(3, user.getPassword());
            ps.setString(4, user.getRemark());
            // 处理结果集
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

    /* 更新数据库纪录 */
    public int update(UserInfo user){
        int row = 0;
        System.out.println("update");

        String sql = "update userInfo set userName=?,password=?,remark=? where userId=?";
        try {
            conn = getConnection(); //连接数据库
            ps = conn.prepareStatement(sql); // 创建Satement并设置参数

            // sql语句中写了几个字段，下面就必須要有几个字段
            ps.setString(1, user.getUserName());
            ps.setString(2, user.getPassword());
            ps.setString(3, user.getRemark());
            ps.setInt(4, user.getUserId());
            // 处理结果集
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

    /* 删除数据库纪录 */
    public int delete(UserInfo user){
        int row = 0;
        System.out.println("delete");
        
        String sql="delete from userInfo where userId=?";

        try {
            conn = getConnection();//连接数据库
            ps = conn.prepareStatement(sql); // 创建Satement并设置参数

            ps.setInt(1, user.getUserId());
            // 处理结果集
            row = ps.executeUpdate();
            System.out.println(row);
        } catch (SQLException e) {
            //  Auto-generated catch block
            e.printStackTrace();
        }finally{
            //释放资源
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
            conn = getConnection();//连接数据库
            ps = conn.prepareStatement(sql); // 创建Satement并设置参数

            // 处理结果集
            row = ps.executeUpdate();
            System.out.println(row);
        } catch (SQLException e) {
            //  Auto-generated catch block
            e.printStackTrace();
        }finally{
            //释放资源
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
