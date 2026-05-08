public class Circle {
    private  double radius;//半径
    public Circle(double radius) {
        this.radius = radius;
    }//构造方法
    public void setRadius(double newRadius) {
       this.radius=newRadius;
    }//设置半径
    public double getArea() {
        return radius*radius*Math.PI;
    }//获取面积
    public double getPerimeter() {
        return 2*radius*Math.PI;
    }//获取周长

}

