import java.io.* ;
public class Person implements Serializable
{
	private String name ;
	private int age ;

	public Person(String name,int age)
	{
		this.name = name ;
		this.age = age ;
	}

	public String toString()
	{
		return " 姓名："+this.name+"，年龄："+this.age ;
	}
}
