# Net

A sample demo for network server.

main loop:
	
	one listener: accept and add client to multiplex  
	
	n processor:  do multiplex loop(epoll, select and kqueue) and process.  
	
	update(TODO): monitor library and config change and update system.

protocol:
  
	echo, ping, time, heart...
  
realtime update and config: TODO
  
database: TODO
