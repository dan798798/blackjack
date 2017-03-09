// blackjack.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "blackjack.h"

void test_deck();

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BLACKJACK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BLACKJACK));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLACKJACK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BLACKJACK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	test_deck();

   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

enum enum_suit { spades, hearts, diamonds, clubs };
char *suit_string[] = { "Spades", "Hearts", "Diamonds", "Clubs" };
enum enum_rank { n2,n3,n4,n5,n6,n7,n8,n9,n10,jack,queen,king,ace };
char *rank_string[] = { "2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace" };

const int MAX_VALUES = 2; // for ace with 2 values

class class_card { // public card with getters for external use
	enum_suit suit;
	enum_rank rank;
	int num_values;
	int values[MAX_VALUES];
public:
	class_card(enum_suit, enum_rank, int, int, int);
	// getters for current card
	enum_suit get_suit();
	enum_rank get_rank();
	int get_num_values();
	int *get_values();
};

// constructor for public card
class_card::class_card(enum_suit suit, enum_rank rank, int num_values, int value0, int value1) {
	this->suit = suit;
	this->rank = rank;
	this->num_values = num_values;
	this->values[0] = value0;
	this->values[1] = value1;
}
	
// public getters for card
enum_suit class_card::get_suit() {
	return this->suit;
}

enum_rank class_card::get_rank() {
	return this->rank;
}

int class_card::get_num_values() {
	return this->num_values;
}

int *class_card::get_values() {
	return this->values;
}

struct struct_card { // private card for internal deck use
	enum_suit suit;
	enum_rank rank;
	int num_values;
	int values[MAX_VALUES];
};

const int MAX_CARDS = 52;

class class_deck {
	const int NUM_SUITS = 4;
	const int NUM_CARDS_PER_SUIT = MAX_CARDS / NUM_SUITS;
	const int INITIAL_CARD_VALUE = 2;
	const int NUM_NUMERIC_CARDS = 9;
	const int ACE_VALUE0 = 1;
	const int ACE_VALUE1 = 11;
	const int FACE_VALUE = 10;

	struct_card cards[MAX_CARDS];
	int index = 0;  // current card of deck

public:
	class_deck();
	void print_deck();  // print deck to console for debugging

	// commands for deck
	void shuffle();
	int remaining_cards();
	class_card next_card();
	class_card current_card();

};

// constructor to initialize deck of card with correct values and shuffle
class_deck::class_deck() {
	srand((unsigned int)time(NULL));  // randomize deck to current time
	int x = 0;
	for (int y = 0; y < NUM_SUITS; y++) { // loop for each suit
		for (int z = 0; z < NUM_CARDS_PER_SUIT; z++) { // loop for every card in suit
			cards[x].suit = (enum_suit)y;
			cards[x].rank = (enum_rank)z;
			if (z < NUM_NUMERIC_CARDS) {			// numeric cards
				cards[x].values[0] = z + INITIAL_CARD_VALUE;
				cards[x].num_values = 1;
			} else if (z + 1 == NUM_CARDS_PER_SUIT) { // ace
				cards[x].values[0] = ACE_VALUE0;
				cards[x].values[1] = ACE_VALUE1;
				cards[x].num_values = 2;
			} else {								// face cards
				cards[x].values[0] = FACE_VALUE;
				cards[x].num_values = 1;
			}
			x++;
		}
	}
	shuffle();
};

int class_deck::remaining_cards() { // 0 when cant get any new cards
	return MAX_CARDS - index - 1;
}

class_card class_deck::next_card() {
	index++; // increment current card of deck, and validate value
	return current_card();
}

class_card class_deck::current_card() {
	if (index >= MAX_CARDS) {
		index = MAX_CARDS - 1;
	}
	if (index < 0) {
		index = 0;
	}
	class_card card(cards[index].suit, cards[index].rank, cards[index].num_values, cards[index].values[0], cards[index].values[1]);
	return card;
}

// randomly draw a new card for every card in deck
void class_deck::shuffle() {
	for (int x = 0; x < MAX_CARDS; x++) { // loop for all cards
		int rand_index = rand() % MAX_CARDS;
		struct_card temp_card = cards[x];	// swap current card with random card
		cards[x] = cards[rand_index];
		cards[rand_index] = temp_card;
	}
	index = 0;
}

// output to visual studio debug window
void console(char *str) {
	strcat(str, "\n");
	wchar_t wstr[512];
	mbstowcs(wstr, str, strlen(str) + 1);//Plus null
	LPWSTR ptr = wstr;
	OutputDebugString(wstr);
}

// print deck to console
void class_deck::print_deck() {
	char str[256] = "";
	for (int x = 0; x < MAX_CARDS; x++) {
		sprintf(str, "%d: value = %d ", x, cards[x].values[0]);
		strcat(str, suit_string[cards[x].suit]);
		strcat(str, " ");
		strcat(str, rank_string[cards[x].rank]);
		console(str);
	}
	sprintf(str, "index = %d ", index);
	console(str);
}

// simple test of deck with shuffle to console
void test_deck() {
	class_deck deck;
	deck.print_deck();
	deck.next_card();
	deck.print_deck();
	for (int x = 1; x < MAX_CARDS+1; x++) {
		deck.next_card();
	}
	deck.print_deck();
}









