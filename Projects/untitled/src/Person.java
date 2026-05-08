public class Person {
    private String name;
    private int age;

    public Person(String name,int age)
    {
        this.name=name;
        this.age=age;
    }//构造函数

    //构造getter和setter
    public String getName()
    {
        return name;
    }

    public void setName(String name)
    {
        this.name=name;
    }

    public int getAge()
    {
        return age;
    }

    public void setAge(int age)
    {
        this.age=age;
    }

    //重写toString
    @Override
    public String toString(){
        return "Person{"+"name='"+name+'\''+",age="+age+'}';
    }
    //重写equals
    @Override
    public boolean equals(Object o)
    {
        if(this==o) return true;
        if(o==null||getClass()!=o.getClass()) return false;
        Person person=(Person) o;
        return age==person.age && name.equals(person.name);
    }
    //重写hashCode
    @Override
    public int hashCode()
    {
        int result = name.hashCode();
        result=31*result+age;
        return  result;
    }
}
