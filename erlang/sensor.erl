-module(sensor).
-export([sensor_thread/1]).

sensor_thread(WatchId) ->
	random:seed(now()),
	Measurement = random:uniform(11),
	Sleep_time = random:uniform(100),
	timer:sleep(Sleep_time),

	if Measurement == 11 ->
		WatchId ! {self(),Measurement},
		exit("anomalous_reading");
	true ->
		WatchId ! {self(),Measurement},
		sensor_thread(WatchId)
	end.