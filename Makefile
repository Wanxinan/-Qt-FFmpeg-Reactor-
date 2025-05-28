INC_DIR:= ./ ./include
SRC_DIR:= ./src
SRCS:= $(wildcard $(SRC_DIR)/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -llog4cpp -lcrypt
CXXFLAGS:= -g 
EXE:= SMHS.exe

$(EXE):$(OBJS)
	g++ -o $@ $^  $(LIBS) $(addprefix -I, $(INC_DIR)) $(CXXFLAGS)

%.o:%.cc
	g++ -o $@ -c $^ $(LIBS) $(addprefix -I, $(INC_DIR)) $(CXXFLAGS) 

clean:
	rm $(EXE) $(OBJS)

echo:
	@echo $(SRCS)
	@echo $(OBJS)
