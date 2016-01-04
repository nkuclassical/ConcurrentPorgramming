byte launchPad;
byte intersection;
proctype monitor(byte dir){
	

	atomic{
		assert(launchPad==0);
		launchPad=dir;
	
	}


	
	atomic{
		assert(intersection==0);
		intersection=dir;
		launchPad=0;
	}
	
	atomic{
		intersection = 0;
	}
}





init {
    launchPad = 0;
	intersection =0;
    atomic {
	    run monitor(1);
		run monitor(2);
        run monitor(3);
		run monitor(4)	
    }
    (_nr_pr==1) ->printf("end of simulation\n");
}
