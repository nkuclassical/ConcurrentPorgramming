package part2;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;



public class TokenCount1 {
	private static final HashMap<String, Integer> tokenFreq = new HashMap<String, Integer>();
	private static BlockingQueue<Page> queue = new ArrayBlockingQueue<Page>(100);

	public static void main(String[] args) throws Exception {
		if (args.length != 2) {
			System.out
					.println("usage: java TokenCount number-of-pages XML-file");
			System.exit(0);
		}
		Integer numPages = Integer.parseInt(args[0]);
		String fileName = args[1];

		System.out.println(Runtime.getRuntime().availableProcessors()
				+ " available processors");

		Producer producer = new Producer(numPages, fileName, queue);
		Consumer consumer = new Consumer(queue, tokenFreq);

		Thread producer_t = new Thread(producer);
		Thread consumer_t = new Thread(consumer);
		

		final long before = System.nanoTime();

		producer_t.start();
		consumer_t.start();

		producer_t.join();
		consumer_t.join();

		final long after = System.nanoTime();
		/* ... end timed code */

		System.out.println("Time to process " + numPages + " pages = "
				+ (after - before) / 1000000 + " milliseconds");
		Set<Entry<String, Integer>> entries = tokenFreq.entrySet();
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
