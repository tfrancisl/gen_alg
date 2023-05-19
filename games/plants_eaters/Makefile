TARGET = plants_eaters
OBJS = plants_eaters_main.o entity.o gen_alg_game.o bitset_genetics.o
CFLAGS = -Wall -I.


$(TARGET): $(OBJS)
	g++  $(CFLAGS) $(OBJS) -o $(TARGET)


clean: 
	rm -rf $(OBJS) chromosome.* $(TARGET)