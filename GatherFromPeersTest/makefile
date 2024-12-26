all : sending_peer gathering_peer

clean:
	rm -rf *.o
	rm -rf sending_peer gathering_peer

COMPILE_OPTS=-fPIC -O3
LINK_OPTS=-pthread

sending_peer : sending_peer.o udp_sending_peer.o udp_peer.o debug_print.o
	g++ $(LINK_OPTS) sending_peer.o udp_sending_peer.o udp_peer.o debug_print.o -o sending_peer

sending_peer.o : sending_peer.cc udp_sending_peer.h udp_peer.h debug_print.h
	g++ $(COMPILE_OPTS) sending_peer.cc -c

udp_sending_peer.o : udp_sending_peer.h debug_print.h udp_sending_peer.cc udp_peer.h
	g++ $(COMPILE_OPTS) udp_sending_peer.cc -c

gathering_peer : gathering_peer.o udp_gathering_peer.o udp_peer.o debug_print.o
	g++ $(LINK_OPTS) gathering_peer.o udp_gathering_peer.o udp_peer.o debug_print.o -o gathering_peer

gathering_peer.o : gathering_peer.cc udp_gathering_peer.h udp_peer.h debug_print.h
	g++ $(COMPILE_OPTS) gathering_peer.cc -c

udp_gathering_peer.o : udp_gathering_peer.h debug_print.h udp_gathering_peer.cc udp_peer.h
	g++ $(COMPILE_OPTS) udp_gathering_peer.cc -c

udp_peer.o : udp_peer.h debug_print.h udp_peer.cc
	g++ $(COMPILE_OPTS) udp_peer.cc -c -DBOOST_BIND_GLOBAL_PLACEHOLDERS

debug_print.o : debug_print.h debug_print.cc
	g++ $(COMPILE_OPTS) debug_print.cc -c

