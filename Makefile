OBJS = bitset_genetics.o chromosome.o
TARGETS = plants_eaters tic_tac_toe

all: plants_eaters $(OBJS)

plants_eaters: $(OBJS)
	$(MAKE) -C games/plants_eaters

tic_tac_toe: $(OBJS)
	$(MAKE) -C games/tic_tac_toe

clean:
	rm -rf $(OBJS) $(TARGETS)

