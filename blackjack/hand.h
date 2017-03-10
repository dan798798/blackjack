#pragma once

#pragma once

#include "../../DeckLibrary/DeckLibrary/DeckLibrary.h"

const int MAX_HAND_CARDS = 8;

class class_hand {
	int wins = 0;
	int num_cards;
	int total;
	struct_card cards[MAX_HAND_CARDS];

public:
	int increment_wins();
	int get_wins();
	int get_num_cards();
	int increment_cards();
	void reset_hand();
	void add_card(struct_card);
	struct_card get_card(int index);
	int get_total();
	char *get_total_string(char *);
};
