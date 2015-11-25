-module(main).
-export([start/0]).

start()->
    {ok,[SensorNum]}=io:fread("How many sensor?","~d"),
    create_watcher(SensorNum).

create_watcher(SensorNum)->
    if SensorNum == 0->
        0;
    true -> 
        if SensorNum >= 10 ->
            spawn(watcher, watcher_start, [10]),
            create_watcher(SensorNum - 10);
        true ->
             spawn(watcher, watcher_start,[SensorNum]),
            create_watcher(0)
        end
    end.
