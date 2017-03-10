#include "stdafx.h"
#include <stdio.h>
#include "hand.h"
#include "blackjack.h"

// class to keep track of dealer or player hand
int class_hand::increment_wins() {
	this->wins++;
	return wins;
}

int class_hand::get_wins() {
	return wins;
}

int class_hand::get_num_cards() {
	return num_cards;
}

int class_hand::increment_cards() {
	this->num_cards++;
	return num_cards;
}

void class_hand::reset_hand() {
	num_cards = 0;
	total = 0;
}

// add a card to end of array cards
void class_hand::add_card(struct_card card) {
	if (num_cards < MAX_HAND_CARDS) {
		cards[num_cards] = card;
	}
	num_cards++;
}

// get current card at index. do bounds checking
struct_card class_hand::get_card(int index) {
	if (index < 0) {
		index = 0;
	}
	if (index >= MAX_HAND_CARDS) {
		index = MAX_HAND_CARDS - 1;
	}
	return this->cards[index];
}

// get the total of the hand.
// if any aces, get the highest hand <= 21
int class_hand::get_total() {
	int total = 0;
	int num_aces = 0;
	for (int x = 0; x < num_cards; x++) {
		if (cards[x].num_values > 1) { // if ace
			num_aces++;
			total += cards[x].values[1];
		} else { // any other card
			total += cards[x].values[0];
		}
	}
	// if any aces, get the highest hand <= 21
	while (num_aces > 0 && total > BLACKJACK_TOTAL) {
		num_aces--;
		total -= ACE_DIFF_VALUE;
	}
	return total;
}

// get the total of the hand in string form
// if any aces, get the highest hand <= 21 and include the next lowest hand
char *class_hand::get_total_string(char *buf) {
	int total = 0;
	int num_aces = 0;
	for (int x = 0; x < num_cards; x++) {
		if (cards[x].num_values > 1) { // if ace
			num_aces++;
			total += cards[x].values[1]; 
		} else { // any other card
			total += cards[x].values[0];
		}
	}
	sprintf(buf, "%d", total);
	// if any aces, get the highest hand <= 21 and include the next lowest hand
	while (num_aces > 0) {
		num_aces--;
		sprintf(buf, "%d or %d", total - ACE_DIFF_VALUE, total);
		total -= ACE_DIFF_VALUE;
		if (total <= BLACKJACK_TOTAL) {
			break;
		}
	}
	return buf;
}
