LDLIBS := -lncurses
CC := g++

p_system: p_system.o
p_system.o: p_system.hpp p_cui.hpp fielddata.hpp

clean:
	$(RM) *.o
	$(RM) *.out
	$(RM) puyo