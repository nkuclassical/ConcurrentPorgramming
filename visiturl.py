import urllib2
import threading

url="https://www.google.com"
class myThread(threading.Thread):
    def __init__(self, threadID, name):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
    def run(self):
        print "Starting " + self.name
        visiting(self.name)
        print "Exiting " + self.name


def visiting(threadName):
    localcount=0
    while localcount < 150:
        try:
            strhtml=urllib2.urlopen(url)
        except urllib2.HTTPError,e:
            print e.code
        except urllib2.URLError,e:
            print str(e)
        localcount=localcount+1
        print threadName+"   "+str(localcount)

try:
    thread1 = myThread(1, "Thread-1")
    thread2 = myThread(2, "Thread-2")
    thread3 = myThread(3, "Thread-3")
    thread4 = myThread(4, "Thread-4")

    # Start new Threads
    thread1.start()
    thread2.start()
    thread3.start()
    thread4.start()
except:
    print "Error: unable to open thread"
