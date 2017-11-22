 export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH
 javac NativeDemo.java
 gcc  -I/usr/lib/jvm/java-1.7.0-openjdk/include -I/usr/lib/jvm/java-1.7.0-openjdk/include/linux/ -o NativeDemo.so -shared -Wl,-soname,NativeDemo.so NativeDemo.c  -lc -fPIC
 file NativeDemo.so
 mv NativeDemo.so libNativeDemo.so
 java -cp . NativeDemo
