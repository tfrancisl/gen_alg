TARGET = tic_tac_toe
OBJS = tic_tac_toe_main.o tic_tac_toe_game.o individual.o bitset_genetics.o
CFLAGS = -Wall -I. -g


$(TARGET): $(OBJS)
	g++  $(CFLAGS) $(OBJS) -o $(TARGET)

clean: 
	rm -rf $(OBJS) chromosome.* $(TARGET)