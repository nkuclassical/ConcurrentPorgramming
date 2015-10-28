all: trafficmgr
trafficmgr: trafficmgr.o monitor.o q.o
	$(CC)  -pthread -Wall -pedantic-errors trafficmgr.o monitor.o q.o -o trafficmgr
trafficmgr.o: trafficmgr.c
	$(CC) -pthread -c -Wall -pedantic-errors -o trafficmgr.o trafficmgr.c
monitor..o: monitor.c
	$(CC) -pthread -c -Wall -pedantic-errors -o monitor.o monitor.c
q.o: q.c
	$(CC) -pthread -c -Wall -pedantic-errors -o q.o q.c
clean:
	rm *o trafficmgr
