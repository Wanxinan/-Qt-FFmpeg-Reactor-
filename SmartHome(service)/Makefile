INC_DIR := include
SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS := $(patsubst $(SRC_DIR)/%.cc, %.o, $(SRCS))
LIBS := 

CXX := g++

CXXFLAGS := -w -g $(addprefix -I, $(INC_DIR)) $(LIBS) -lcrypt -lmysqlclient -lcurl -lavformat -lavcodec -lavutil -lswscale -lssl -lcrypto

EXE := ./server.exe

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
