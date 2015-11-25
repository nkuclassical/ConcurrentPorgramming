package part2;

import java.util.HashMap;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;

//Consumer thread removes pages from the shared queue, divides each page into
//tokens,
public class Consumer implements Runnable {
	private BlockingQueue<Page> queue;
	private static HashMap<String, Integer> tokenFreq = null;
	private static ConcurrentHashMap<String, Integer> tokenFreqCon = null;

	public Consumer(BlockingQueue<Page> queue,
			HashMap<String, Integer> tokenFreq) {
		this.queue = queue;
		Consumer.tokenFreq = tokenFreq;
	}

	public Consumer(BlockingQueue<Page> queue,
			ConcurrentHashMap<String, Integer> tokenFreqCon) {

		this.queue = queue;
		Consumer.tokenFreqCon = tokenFreqCon;
	}

	private static void countToken(String tok) {
		Integer currentCount = tokenFreq.get(tok);
		if (currentCount == null)
			tokenFreq.put(tok, 1);
		else
			tokenFreq.put(tok, currentCount + 1);
	}

	private synchronized static void countTokenCon(String tok) {
		Integer currentCount = tokenFreqCon.get(tok);
		if (currentCount == null)
			tokenFreqCon.put(tok, 1);
		else
			tokenFreqCon.put(tok, currentCount + 1);
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		Page pg;
		try {
			pg = queue.take();
			while (!pg.isPoisonPill()) {
				Iterable<String> allTokens = new Words(pg.getText());
				for (String s : allTokens) {
					if (Consumer.tokenFreqCon == null)
						countToken(s);
					else
						countTokenCon(s);
				}
				pg = queue.take();
			}

		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
