#pragma once

#include "resource.h"

const int BLACKJACK_TOTAL = 21;
const int ACE_DIFF_VALUE = 10;
const int DEALER_STAND = 17;
const int REDEAL_CARDS_LEFT = 12;

void show_cards(bool);
void deal_hand(); 
void reset_window();
LPWSTR convertUnicode(LPWSTR wstr, char *str);
void hit_button();
void stand_button();
void deal_button();
