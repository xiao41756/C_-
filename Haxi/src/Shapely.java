 abstract class Shape {
    public abstract double calArea();
}
class Triangle extends Shape {
    private double base;
    private double height;
    public Triangle(double base, double height) {
        this.base = base;
        this.height = height;
    }//构建
    public double calArea() {
        return 0.5 * base * height;
    }//计算面积

}
 class Rectangle extends Shape {
    private double width;
    private double height;
    public Rectangle(double width, double height) {
        this.width = width;
        this.height = height;
    }//构建
    public double calArea() {
        return width * height;
    }//计算面积

}

 public class Shapely {
     public static void main(String[] args) {
         Shape triangle = new Triangle(5, 10);
         Shape rectangle = new Rectangle(4, 6);//创建三角形和矩形对象
         System.out.println("三角形的面积为：" + triangle.calArea());
         System.out.println("矩形的面积为：" + rectangle.calArea()); //计算并输出面积
         
     }
 }