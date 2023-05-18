OBJS = bitset_genetics.o
TARGETS = plants_eaters tic_tac_toe

all: plants_eaters tic_tac_toe $(OBJS)

plants_eaters: $(OBJS)
	cp $(OBJS) games/plants_eaters/
	cp chromosome.* games/plants_eaters/
	$(MAKE) -C games/plants_eaters

tic_tac_toe: $(OBJS)
	cp $(OBJS) games/tic_tac_toe/
	cp chromosome.* games/tic_tac_toe/
	$(MAKE) -C games/tic_tac_toe

clean:
	rm -rf $(OBJS) $(TARGETS)
	$(MAKE) clean -C games/plants_eaters
	$(MAKE) clean -C games/tic_tac_toe

