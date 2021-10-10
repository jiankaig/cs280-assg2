CXX       := g++
CXX_FLAGS := -Werror -Wall -Wextra -Wconversion -std=c++14 -pedantic -Wno-deprecated 

BIN     := bin
SRC     := src
INCLUDE := include
LIB     := lib
LIBRARIES   := 
EXECUTABLE  := vpl_execution
TEST	:= test

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "🚀 Executing..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	@echo "🚧 Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "🧹 Clearing..."
	-rm $(BIN)/*

out:
	./$(BIN)/$(EXECUTABLE) 1 > $(BIN)/myout1.txt
	./$(BIN)/$(EXECUTABLE) 2 > $(BIN)/myout2.txt
	./$(BIN)/$(EXECUTABLE) 3 > $(BIN)/myout3.txt
	./$(BIN)/$(EXECUTABLE) 4 > $(BIN)/myout4.txt
	./$(BIN)/$(EXECUTABLE) 5 > $(BIN)/myout5.txt
	cat $(BIN)/myout1.txt $(BIN)/myout2.txt $(BIN)/myout3.txt $(BIN)/myout4.txt $(BIN)/myout5.txt > $(TEST)/myout.txt
	diff $(TEST)/myout.txt $(TEST)/output-sample-LP64.txt > $(TEST)/diff.txt

diff:
	@echo "testing with sample output"
	diff $(TEST)/myout.txt $(TEST)/output-sample-LP64.txt