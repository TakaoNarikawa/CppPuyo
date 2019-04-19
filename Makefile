LDLIBS := -lncurses
CC := g++

p_cui: p_cui.o
p_cui.o: p_system.hpp

clean:
	$(RM) *.o
	$(RM) *.out
	$(RM) puyo