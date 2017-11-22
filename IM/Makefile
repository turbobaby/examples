all: imclient imserver

imclient: imclient.c
	gcc $< -o $@

imserver: imserver.c
	gcc $< -o $@

clean:
	rm -f imclient imserver *.o *~ core
