


class B {
public void f() {
    System.out.println("This is the original method f in class B.");
}
}

// 定义类 A
class A {
    public static void main(String[] args) {
        // 创建类 B 的匿名内部类对象，覆盖方法 f
        B b = new B() {
            @Override
            public void f() {
                System.out.println("This is the overridden method f in the anonymous inner class.");
            }
        };

        // 调用覆盖后的方法 f
        b.f();
    }
}
