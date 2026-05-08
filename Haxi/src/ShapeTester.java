import java.util.Scanner;

public class ShapeTester {
    public static void main(String[] args) {
        Scanner sc=new Scanner(System.in);
        Circle cricle=new Circle(1 );//创建圆对象
        System.out.println("请输入圆的半径：");
        double radius =sc.nextDouble();
        cricle.setRadius(radius);//设置半径
        System.out.println("圆的面积为："+cricle.getArea());
        System.out.println("圆的周长为："+cricle.getPerimeter());

    }
}
