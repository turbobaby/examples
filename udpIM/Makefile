all: udpclient udpserv

udpclient: udpclient.c
	gcc $< -o $@

udpserv: udpserv.c
	gcc $< -o $@

clean:
	rm -f udpclient udpserv *.o *~ core
