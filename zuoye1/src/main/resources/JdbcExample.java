
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

    public class JdbcExample {
        public static void main(String[] args) {
            // MySQL 数据库连接 URL
            String url = "jdbc:mysql://localhost:3306/testdb";  // 修改数据库的 URL、用户名和密码
            String username = "root";  // 数据库用户名
            String password = "password";  // 数据库密码

            // 创建连接对象
            Connection connection = null;

            try {
                // 加载 MySQL 驱动
                Class.forName("com.mysql.cj.jdbc.Driver");

                // 建立数据库连接
                connection = DriverManager.getConnection(url, username, password);

                // 创建 Statement 对象
                Statement statement = connection.createStatement();

                // 执行查询
                String query = "SELECT * FROM users";  // 假设有一个 users 表
                ResultSet resultSet = statement.executeQuery(query);

                // 输出查询结果
                while (resultSet.next()) {
                    int id = resultSet.getInt("id");
                    String name = resultSet.getString("name");
                    System.out.println("ID: " + id + ", Name: " + name);
                }

            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                // 关闭连接
                try {
                    if (connection != null) {
                        connection.close();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
