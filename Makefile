TCL_INC_DIR := /usr/include/tcl
TCL_LIB_VER := 8.6
VALGRIND_FLAGS := --leak-check=full

CPPFLAGS := -I$(TCL_INC_DIR)
CXXFLAGS := -Wall -g
LDFLAGS  := -g
LDLIBS   := -ltcl$(TCL_LIB_VER)

APP := tcl_ex

all : $(APP)

$(APP) : $(APP).o

$(APP).o : $(APP).cpp

.PHONY: run
run: $(APP)
	./$(APP)

.PHONY: valgrind
valgrind: $(APP)
	valgrind $(VALGRIND_FLAGS) ./$(APP)


.PHONY: clean
clean:
	rm -f $(APP) $(APP).o

