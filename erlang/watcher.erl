-module(watcher).
-export([watcher_start/1]).

create_sensor_list(SensorNum,List)->
	if SensorNum==0 ->
		List;
	true ->
		{Pid,_}=spawn_monitor(sensor,sensor_thread,[self()]),
		create_sensor_list(SensorNum-1,[Pid|List])
	end.

find_index(SensorId,SensorList,Index)->
    if SensorList==[]->
        -1;
    true->
        [Head | Tail] = SensorList,
        if Head == SensorId ->
            Index;
        true ->
            find_index(SensorId, Tail, Index + 1)
        end
    end. 

watcher_thread(SensorList) ->    
    receive
        {SensorId,Measurement} ->
        	if Measurement == 11 ->
        		io:fwrite("Sensor ~p in has crashed! Measurement is~p; replacing...~n", [SensorId, Measurement]),
            	{Pid, _ } = spawn_monitor(sensor, sensor_thread, [self()]),
                Sensorindex=find_index(SensorId,SensorList,0),

                {Head,[_|Tail]}=lists:split(Sensorindex,SensorList),
                NewList=lists:append([Head, [Pid | Tail]]),
                watcher_thread(NewList);
            true->
            	io:fwrite("received message '~p' from Sensor ~p~n", [Measurement, SensorId]),
                watcher_thread(SensorList)
        	end
    end.
    
watcher_start(SensorNum) ->
	SensorList = create_sensor_list(SensorNum, []),
	watcher_thread(SensorList).