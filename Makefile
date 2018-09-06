##
##
##
EXES = client server workder publisher subscriber rep_server req_client
RM :=rm -f 

CXXFLAGS = -g -Wall -D_GLIBCXX_USE_CXX11_ABI=0 -rdynamic -ldl
CPPFLAGS = -I./include
LIBS = -L./lib -lpthread -lzmq -lrt

all: client server workder publisher subscriber rep_server req_client

show:
	@echo "EXES=$(EXES)"
	@echo "SRC=$(SRC)"
	@echo "OBJ=$(OBJ)"


########################################
client: src/ROUTER_DEALER/client.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

server: src/ROUTER_DEALER/server.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)


workder: src/ROUTER_DEALER/workder.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

#######################################

publisher: src/PUB_SUB/publisher.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

subscriber: src/PUB_SUB/subscriber.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

#######################################

rep_server: src/REQ_REP/rep_server.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

req_client: src/REQ_REP/req_client.o
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

#######################################

clean:
	$(RM) $(EXES) src/ROUTER_DEALER/*.o src/PUB_SUB/*.o src/REQ_REP/*.o

