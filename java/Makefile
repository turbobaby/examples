sources  = CharDemo.java  FileDemo.java  Hello.java  Person.java  RandomFileDemo.java  SerializableDemo.java  StreamDemo.java
classes  = CharDemo.class  FileDemo.class Hello.class  Person.class  RandomFileDemo.class  SerializableDemo.class  StreamDemo.class

%.class: %.java
	javac -cp . $<

all: $(sources) $(classes)

clean:
	rm -f $(classes)
