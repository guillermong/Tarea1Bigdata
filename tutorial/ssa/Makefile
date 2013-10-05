CPP=g++


#CPPFLAGS=-g3 -Wall -DVERBOSE
#CPPFLAGS=-O9 -Wall -DNDEBUG 
CPPFLAGS=-O9 -Wall -DVERBOSE

INCLUDES=-I../../includes/ -I.
LIB=../../lib/libcds.a
OBJECTS=ssa.o 
BINS=build_index dump_bwt countssa locatessa genpalabras

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

all: clean stats $(OBJECTS) $(BINS)

stats:
	@echo
	@echo " COMPILING SSA"
	@echo " ###################"
	@echo "  * Compiler flags: $(CPPFLAGS)"
	@echo "  * Include dirs: $(INCLUDES)"
	@echo
	@echo

clean:
	@echo " [CLN] Removing object files"
	@rm -f $(OBJECTS) $(BINS)

genpalabras: 
	@echo " [BLD] Building genpalabras"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o genpalabras genpalabras.cpp $(OBJECTS) $(LIB)

countssa: 
	@echo " [BLD] Building countssa"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o countssa countssa.cpp $(OBJECTS) $(LIB)

locatessa: 
	@echo " [BLD] Building locatessa"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o locatessa locatessa.cpp $(OBJECTS) $(LIB)

dump_bwt: 
	@echo " [BLD] Building dump_bwt"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o dump_bwt dump_bwt.cpp $(OBJECTS) $(LIB)

build_index: 
	@echo " [BLD] Building build_index"
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -o build_index build_index.cpp $(OBJECTS) $(LIB)

