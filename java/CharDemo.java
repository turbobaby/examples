import java.io.*;
public class CharDemo
{
	public static void main(String args[])
	{
		File f = new File("chartemp.txt") ;
		Writer out = null ;
		try
		{
			out = new FileWriter(f) ;
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}

		// 声明一个 String 类型对象
		String str = "Hello World!!!" ;
		try
		{
			// 将 str 内容写入到文件之中
			out.write(str) ;
		}
		catch (IOException e1)
		{
			e1.printStackTrace();
		}

		try
		{
			out.close() ;
		}
		catch (IOException e2)
		{
			e2.printStackTrace();
		}

		// 以下为读文件操作
		Reader in = null ;
		try
		{
			in = new FileReader(f) ;
		}
		catch (FileNotFoundException e3)
		{
			e3.printStackTrace();
		}

		// 开辟一个空间用于接收文件读进来的数据
		char c1[] = new char[1024] ;
		int i = 0 ;
		try
		{
			// 将 c1 的引用传递到 read()方法之中，同时此方法返回读入数据的个数
			i = in.read(c1) ;
		}
		catch (IOException e4)
		{
			e4.printStackTrace();
		}

		try
		{
			in.close() ;
		}
		catch (IOException e5)
		{
			e5.printStackTrace();
		}

		//将字符数组转换为字符串输出
		System.out.println(new String(c1,0,i)) ;
	}
}
