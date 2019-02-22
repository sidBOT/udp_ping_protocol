#	udp_ping_protocol

##	The ping program uses the ICMP protocol to determine whether a specified endpoint is alive. When a host or router receives a ping request, IP software will return a reply known as an echo. The arrival of a reply lets the client know that the remote machine is alive.

#	Usage
	./pingserver <port> <awake_time> <sleep_time> 
	./pingclient <port> <ser1> <ser2> ..

#	Expected Output
	Reply from server 11.11.11.11, latency : 10000usec