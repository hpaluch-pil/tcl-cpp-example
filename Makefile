CXXFLAGS := -Wall -g
LDFLAGS  := -g

APP := tcl_ex

all : $(APP)

$(APP) : $(APP).o

$(APP).o : $(APP).cpp


.PHONY: clean
clean:
	rm -f $(APP) $(APP).o

