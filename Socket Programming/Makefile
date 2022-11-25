seq:
	gcc server2a.c -o server2a
	./server2a
	gcc clienta.c -o clienta
	./clienta

conc_f:
	gcc server2b.c -o server2b
	./server2b
	gcc clientb.c -o clientb
	./clientb

conc_t:
	gcc server2c.c -o server2c
	./server2c
	gcc clientb.c -o clientb
	./clientb

sel:
	gcc server_select.c -o server_select
	./server_select
	gcc clientb.c -o clientb
	./clientb

pol:
	gcc server_poll.c -o server_poll
	./server_poll
	gcc clientb.c -o clientb
	./clientb

epol:
	gcc server_epoll.c -o server_epoll
	./server_epoll
	gcc clientb.c -o clientb
	./clientb



	