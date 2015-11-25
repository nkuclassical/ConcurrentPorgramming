package part2;

import java.util.concurrent.BlockingQueue;

//Producer thread parses the XML and pushes pages onto a shared queue
public class Producer implements Runnable {

	private BlockingQueue<Page> queue;
	private String fileName;
	private int numPages;
	private int numThread;
	
	public Producer(int numPages, String fileName, BlockingQueue<Page> queue) {
		this.numPages=numPages;
		this.fileName = fileName;
		this.queue = queue;
		this.numThread=1;
	}
	
	public Producer(int numPages, String fileName, BlockingQueue<Page> queue, int numThread) {
		this.numPages = numPages;
		this.fileName = fileName;
		this.queue = queue;
		this.numThread=numThread;
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		Iterable<Page> allPages = new Pages(this.numPages, this.fileName);
		for (Page pg : allPages) {
			try {
				queue.put(pg);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		for(int i=0;i<this.numThread;i++){
			try {
				queue.put(new PoisonPill());
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}

