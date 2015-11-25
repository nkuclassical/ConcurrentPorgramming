package part2;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class TokenCount3 {
	private static final ConcurrentHashMap<String, Integer> tokenFreqCon = new ConcurrentHashMap<String, Integer>();
	private static BlockingQueue<Page> queue = new ArrayBlockingQueue<Page>(100);

	public static void main(String[] args) throws Exception {
		if (args.length != 2) {
			System.out
					.println("usage: java TokenCount number-of-pages XML-file");
			System.exit(0);
		}
		Integer numPages = Integer.parseInt(args[0]);
		Integer numProcessors = Runtime.getRuntime().availableProcessors();
		String fileName = args[1];

		System.out.println(Runtime.getRuntime().availableProcessors()
				+ " available processors");

		ExecutorService pool = Executors.newCachedThreadPool();
		Producer producer = new Producer(numPages, fileName, queue,
				numProcessors - 1);
		Thread producer_t = new Thread(producer);
		for (int i = 0; i < numProcessors - 1; i++) {
			pool.submit(new Consumer(queue, tokenFreqCon));
		}

		final long before = System.nanoTime();
		producer_t.start();
		pool.shutdown();
		producer_t.join();
		pool.awaitTermination(Long.MAX_VALUE, TimeUnit.SECONDS);

		final long after = System.nanoTime();
		/* ... end timed code */

		System.out.println("Time to process " + numPages + " pages = "
				+ (after - before) / 1000000 + " milliseconds");
		Set<Entry<String, Integer>> entries = tokenFreqCon.entrySet();
		ArrayList<Entry<String, Integer>> list = new ArrayList<Entry<String, Integer>>(
				entries);
		Collections.sort(list, new Comparator<Map.Entry<String, Integer>>() {
			public int compare(Map.Entry<String, Integer> obj1,
					Map.Entry<String, Integer> obj2) {
				return (obj2.getValue()).compareTo(obj1.getValue());
			}
		});
		for (int i = 0; i < 30; i++)
			System.out.println(list.get(i).getKey() + " appears "
					+ list.get(i).getValue() + " times");
	}

}
