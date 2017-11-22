import java.io.*;
public class FileDemo
{
	public static void main(String[] args)
	{
		File f=new File("demo.txt");
		if(f.exists())
			f.delete();
		else try
		{
			f.createNewFile();
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
		}

		// getName()方法，取得文件名
		System.out.println("文件名："+f.getName());
		// getPath()方法，取得文件路径
		System.out.println("文件路径："+f.getPath());
		// getAbsolutePath()方法，得到绝对路径名
		System.out.println("绝对路径："+f.getAbsolutePath());
		// getParent()方法，得到父文件夹名
		System.out.println("父文件夹名称："+f.getParent());
		// exists()，判断文件是否存在
		System.out.println(f.exists()?"文件存在":"文件不存在");
		// canWrite()，判断文件是否可写
		System.out.println(f.canWrite()?"文件可写":"文件不可写");
		// canRead()，判断文件是否可读
		System.out.println(f.canRead()?"文件可读":"文件不可读");
		/// isDirectory()，判断是否是目录
		System.out.println(f.isDirectory()?"是":"不是"+"目录");
		// isFile()，判断是否是文件
		System.out.println(f.isFile()?"是文件":"不是文件");
		// isAbsolute()，是否是绝对路径名称
		System.out.println(f.isAbsolute()?"是绝对路径":"不是绝对路径");
		// lastModified()，文件最后的修改时间
		System.out.println("文件最后修改时间："+f.lastModified());
		// length()，文件的长度
		System.out.println("文件大小："+f.length()+" Bytes");
	}
} 
