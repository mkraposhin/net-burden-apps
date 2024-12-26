all : leading_peer subordinate_peer

clean:
	rm -rf *.o
	rm -rf leading_peer subordinate_peer

COMPILE_OPTS=-fPIC -O3
LINK_OPTS=-pthread

leading_peer : leading_peer.o udp_leading_peer.o udp_peer.o debug_print.o
	g++ $(LINK_OPTS) leading_peer.o udp_leading_peer.o udp_peer.o debug_print.o -o leading_peer

leading_peer.o : leading_peer.cc udp_leading_peer.h udp_peer.h debug_print.h
	g++ $(COMPILE_OPTS) leading_peer.cc -c

udp_leading_peer.o : udp_leading_peer.h debug_print.h udp_leading_peer.cc udp_peer.h udp_peer.cc
	g++ $(COMPILE_OPTS) udp_leading_peer.cc -c

subordinate_peer : subordinate_peer.o udp_subordinate_peer.o udp_peer.o debug_print.o
	g++ $(LINK_OPTS) subordinate_peer.o udp_subordinate_peer.o udp_peer.o debug_print.o -o subordinate_peer

subordinate_peer.o : subordinate_peer.cc udp_subordinate_peer.h udp_peer.h debug_print.h
	g++ $(COMPILE_OPTS) subordinate_peer.cc -c

udp_subordinate_peer.o : udp_subordinate_peer.h debug_print.h udp_subordinate_peer.cc udp_peer.h udp_peer.cc
	g++ $(COMPILE_OPTS) udp_subordinate_peer.cc -c

udp_peer.o : udp_peer.h debug_print.h udp_peer.cc
	g++ $(COMPILE_OPTS) udp_peer.cc -c -DBOOST_BIND_GLOBAL_PLACEHOLDERS

debug_print.o : debug_print.h debug_print.cc
	g++ $(COMPILE_OPTS) debug_print.cc -c -DBOOST_BIND_GLOBAL_PLACEHOLDERS

