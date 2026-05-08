public class Persontext {
    public static void main(String[] args) {
        Person person = new Person("Alice", 30);
        System.out.println("Name: " + person.getName());
        System.out.println("Age: " + person.getAge());
        //测试setter和getter
        String st=person.toString();
        System.out.println(st);
        //测试toString
        Person otherPerson = new Person("Bob", 25);
        boolean t=person.equals(otherPerson);
        System.out.println(t);
        //测试equals
        int hashcode=person.hashCode();
        System.out.println(hashcode);
        //测试hashCode
    }
}
