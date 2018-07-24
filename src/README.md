# Net

A sample demo for server and client.

								      event
			file_event				time_event				system_event
		listener  receiver			   TODO				reload   restart   

		
							  protocol
		recv -> data -> analyse -> operator -> package -> send/end



				mainloop(epoll or kqueue for each worker)

		worker1				   worker2     ....    workerN	(Round-Robin)
	  priority <= 1000        priority > 1000     priority > 1000
		listener     		   receiver   		    receiver       
		receiver
		 timer
        


