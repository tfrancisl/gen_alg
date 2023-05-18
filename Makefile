OBJS = bitset_genetics.o
TARGETS = plants_eaters tic_tac_toe

all: plants_eaters $(OBJS)

plants_eaters: $(OBJS)
	cp $(OBJS) games/plants_eaters/
	cp chromosome.* games/plants_eaters/
	$(MAKE) -C games/plants_eaters

tic_tac_toe: $(OBJS)
	$(MAKE) -C games/tic_tac_toe

clean:
	rm -rf $(OBJS) $(TARGETS)
	$(MAKE) clean -C games/plants_eaters

