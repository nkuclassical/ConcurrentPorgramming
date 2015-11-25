package part2;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.ConcurrentHashMap;

public class ConsumerCall implements
		Callable<ConcurrentHashMap<String, Integer>> {
	private BlockingQueue<Page> queue;

	private static ConcurrentHashMap<String, Integer> tokenFreqCon = null;

	public ConsumerCall(BlockingQueue<Page> queue) {
		this.queue = queue;
		ConsumerCall.tokenFreqCon = new ConcurrentHashMap<String, Integer>();
	}

	private static void countTokenCon(String tok) {
		Integer currentCount = tokenFreqCon.get(tok);
		if (currentCount == null)
			tokenFreqCon.put(tok, 1);
		else
			tokenFreqCon.put(tok, currentCount + 1);
	}

	@Override
	public ConcurrentHashMap<String, Integer> call() throws Exception {
		// TODO Auto-generated method stub
		Page pg = queue.take();
		while (!pg.isPoisonPill()) {
			Iterable<String> allTokens = new Words(pg.getText());
			for (String s : allTokens) {
				countTokenCon(s);
			}
			pg = queue.take();
		}
		return tokenFreqCon;

	}

}
