// blackjack.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "blackjack.h"
#include "../../DeckLibrary/DeckLibrary/DeckLibrary.h"
#include "hand.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LOADSTRING 100

using namespace DeckLibrary;

char *suit_string[] = { "S", "H", "D", "C" };
char *rank_string[] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };

// Global Variables:
HINSTANCE hInst;                                // current instance

typedef void(* LPFNDLLSHUFFLE)();
typedef int(* LPFNDLLREMAININGCARDS)();
typedef struct_card(* LPFNDLLNDEXTCARD)();
typedef struct_card(* LPFNDLLCURRENTCARD)();

HINSTANCE hDLL;
LPFNDLLSHUFFLE lpfnDllShuffle;    // Function pointer  
LPFNDLLREMAININGCARDS lpfnDllRemainingCards;    // Function pointer  
LPFNDLLNDEXTCARD lpfnDllNextCard;    // Function pointer  
LPFNDLLCURRENTCARD lpfnDllCurrentCard;    // Function pointer  
DWORD dwParam1;
UINT  uParam2, uReturnVal;

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// window handles for text controls and buttong
HWND hwndHitButton;
HWND hwndStandButton;
HWND hwndDealButton;
HWND hwndCardsLeftEdit;
HWND hwndDealerEdit[MAX_HAND_CARDS];
HWND hwndDealerValue;
HWND hwndDealerWins;
HWND hwndPlayerEdit[MAX_HAND_CARDS];
HWND hwndPlayerValue;
HWND hwndPlayerWins;

class_hand dealer;  // create a hand object to hold dealer cards
class_hand player;  // create a hand object to hold player cards

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

	hDLL = LoadLibrary(L"../../DeckLibrary/Debug/DeckLibrary.dll");
	if (hDLL == NULL)
	{
		return -1;
	}

	if (hDLL != NULL)
	{
		lpfnDllShuffle = (LPFNDLLSHUFFLE)GetProcAddress(hDLL, "shuffle");
		if (!lpfnDllShuffle)
		{
			// handle the error  
			FreeLibrary(hDLL);
			return -1;
		}
	}

	if (hDLL != NULL)
	{
		lpfnDllRemainingCards = (LPFNDLLREMAININGCARDS)GetProcAddress(hDLL, "remaining_cards");
		if (!lpfnDllRemainingCards)
		{
			// handle the error  
			FreeLibrary(hDLL);
			return -1;
		}
	}

	if (hDLL != NULL)
	{
		lpfnDllNextCard = (LPFNDLLNDEXTCARD)GetProcAddress(hDLL, "next_card");
		if (!lpfnDllNextCard)
		{
			// handle the error  
			FreeLibrary(hDLL);
			return -1;
		}
	}

	if (hDLL != NULL)
	{
		lpfnDllCurrentCard = (LPFNDLLCURRENTCARD)GetProcAddress(hDLL, "current_card");
		if (!lpfnDllCurrentCard)
		{
			// handle the error  
			FreeLibrary(hDLL);
			return -1;
		}
	}

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

   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, 590, 425, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   hwndHitButton = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"Hit",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   50,         // x position 
	   270,         // y position 
	   80,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU)IDC_HIT_BUTTON,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);      // Pointer not needed.

   if (!hwndHitButton)
   {
	   return FALSE;
   }

   hwndStandButton = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"Stand",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   160,         // x position 
	   270,         // y position 
	   80,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU)IDC_STAND_BUTTON,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);      // Pointer not needed.

   if (!hwndStandButton)
   {
	   return FALSE;
   }

   hwndDealButton = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"Deal",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   270,         // x position 
	   270,         // y position 
	   80,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU)IDC_DEAL_BUTTON,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);      // Pointer not needed.

   if (!hwndStandButton)
   {
	   return FALSE;
   }

   HWND hwndExitButton = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"Exit",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   440,         // x position 
	   270,         // y position 
	   80,        // Button width
	   50,        // Button height
	   hWnd,     // Parent window
	   (HMENU)IDC_EXIT_BUTTON,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);      // Pointer not needed.

   if (!hwndExitButton)
   {
	   return FALSE;
   }
   
   hwndCardsLeftEdit = CreateWindow(
	   L"EDIT",  // Predefined class; Unicode assumed 
	   L"Cards left = 52",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   100,         // x position 
	   10,         // y position 
	   200,        // Button width
	   30,        // Button height
	   hWnd,     // Parent window
	   NULL,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);      // Pointer not needed.

   if (!hwndCardsLeftEdit)
   {
	   return FALSE;
   }

   for (int x = 0; x < MAX_HAND_CARDS; x++) {

	   hwndDealerEdit[x] = CreateWindow(
		   L"EDIT",  // Predefined class; Unicode assumed 
		   L"Ace Spade",      // Button text 
		   WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
		   50 + 60*x,         // x position 
		   50,         // y position 
		   50,        // Button width
		   40,        // Button height
		   hWnd,     // Parent window
		   NULL,       // No menu.
		   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		   NULL);      // Pointer not needed.

	   if (!hwndDealerEdit[x])
	   {
		   return FALSE;
	   }
   }

   hwndDealerValue = CreateWindow(
	   L"EDIT",  // Predefined class; Unicode assumed 
	   L"Dealer = 20",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
	   100,         // x position 
	   110,         // y position 
	   210,        // Button width
	   40,        // Button height
	   hWnd,     // Parent window
	   NULL,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);      // Pointer not needed.

   if (!hwndDealerValue)
   {
	   return FALSE;
   }

   hwndDealerWins = CreateWindow(
	   L"EDIT",  // Predefined class; Unicode assumed 
	   L"Wins = 0",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
	   340,         // x position 
	   110,         // y position 
	   100,        // Button width
	   40,        // Button height
	   hWnd,     // Parent window
	   NULL,       // No menu.
	   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
	   NULL);      // Pointer not needed.

   if (!hwndDealerWins)
   {
	   return FALSE;
   }

   for (int x = 0; x < MAX_HAND_CARDS; x++) {

	   hwndPlayerEdit[x] = CreateWindow(
		   L"EDIT",  // Predefined class; Unicode assumed 
		   L"Ace Spade",      // Button text 
		   WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
		   50 + 60 * x,         // x position 
		   150,         // y position 
		   50,        // Button width
		   40,        // Button height
		   hWnd,     // Parent window
		   NULL,       // No menu.
		   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		   NULL);      // Pointer not needed.

	   if (!hwndPlayerEdit[x])
	   {
		   return FALSE;
	   }

	   hwndPlayerValue = CreateWindow(
		   L"EDIT",  // Predefined class; Unicode assumed 
		   L"Player = 10",      // Button text 
		   WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
		   100,         // x position 
		   210,         // y position 
		   210,        // Button width
		   40,        // Button height
		   hWnd,     // Parent window
		   NULL,       // No menu.
		   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		   NULL);      // Pointer not needed.

	   if (!hwndPlayerValue)
	   {
		   return FALSE;
	   }

	   hwndPlayerWins = CreateWindow(
		   L"EDIT",  // Predefined class; Unicode assumed 
		   L"Wins = 0",      // Button text 
		   WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
		   340,         // x position 
		   210,         // y position 
		   100,        // Button width
		   40,        // Button height
		   hWnd,     // Parent window
		   NULL,       // No menu.
		   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		   NULL);      // Pointer not needed.

	   if (!hwndPlayerWins)
	   {
		   return FALSE;
	   }
   }

   deal_button();

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
			case IDC_EXIT_BUTTON:
				DestroyWindow(hWnd);
				break;
			case IDC_HIT_BUTTON:
				hit_button();
				break;
			case IDC_STAND_BUTTON:
				stand_button();
				break;
			case IDC_DEAL_BUTTON:
				deal_button();
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

// clear the card text boxes so can start a new hand
void reset_window() {
	for (int x = 0; x < MAX_HAND_CARDS; x++) {
		SetWindowText(hwndDealerEdit[x], NULL);
	}
	for (int x = 0; x < MAX_HAND_CARDS; x++) {
		SetWindowText(hwndPlayerEdit[x], NULL);
	}
}

// fill window text boxes with correct text from hands
// input: bool show_dealer - whether or not to show dealers first card and total
void show_cards(bool show_dealer) {
	char str[128];
	char buf[128];
	wchar_t wstr[256];

	// show remaining cards
	sprintf(buf, "Cards remaining = %d", lpfnDllRemainingCards());
	// show dealer cards
	SetWindowText(hwndCardsLeftEdit, convertUnicode(wstr, buf));
	for (int x = 0; x < dealer.get_num_cards(); x++) {  // loop through dealer cards
		if (!show_dealer && x == 0) {	// dont show one dealer card until player is finished
			//sprintf(buf, " - -");
			sprintf(buf, "-%s-%s", rank_string[dealer.get_card(x).rank], suit_string[dealer.get_card(x).suit]);
		} else {
			sprintf(buf, " %s %s", rank_string[dealer.get_card(x).rank], suit_string[dealer.get_card(x).suit]);
		}
		SetWindowText(hwndDealerEdit[x], convertUnicode(wstr, buf));
	}
	if (show_dealer) { // only show dealer total after player is finished
		//sprintf(buf, "Dealer Total = %d", dealer.get_total());
		sprintf(buf, "Dealer Total = %s,%d", dealer.get_total_string(str), dealer.get_total());
	} else {
		//sprintf(buf, "Dealer Total = ?");
		sprintf(buf, "Dealer ??????? %s,%d", dealer.get_total_string(str), dealer.get_total());
	}
	// if needed, concatonate bust, push, or wins to total
	if (dealer.get_total() > BLACKJACK_TOTAL) {
		strcat(buf, " BUST");
	} else if (show_dealer) {
		if (dealer.get_total() == player.get_total()) {
			strcat(buf, " PUSH");
		} else if (dealer.get_total() > player.get_total() && dealer.get_total() <= BLACKJACK_TOTAL) {
			strcat(buf, " WINS");
		}
	}
	SetWindowText(hwndDealerValue, convertUnicode(wstr, buf));
	// show dealer wins
	sprintf(buf, "Wins = %d", dealer.get_wins());
	SetWindowText(hwndDealerWins, convertUnicode(wstr, buf));
	
	// show player cards
	for (int x = 0; x < player.get_num_cards(); x++) { // loop through player cards
		sprintf(buf, " %s %s", rank_string[player.get_card(x).rank], suit_string[player.get_card(x).suit]);
		SetWindowText(hwndPlayerEdit[x], convertUnicode(wstr, buf));
	}
	// show player total
	if (show_dealer) {
		//sprintf(buf, "Player Total = %d", player.get_total());
		sprintf(buf, "Player Total = %s,%d", player.get_total_string(str), player.get_total());
	} else {
		//sprintf(buf, "Player Total = %s", player.get_total_string(str));
		sprintf(buf, "Player Total = %s,%d", player.get_total_string(str), player.get_total());
	}
	// if needed, concatonate bust, push, or wins to total
	if (player.get_total() > BLACKJACK_TOTAL) {
		strcat(buf, " BUST");
	} else if (show_dealer) {
		if (dealer.get_total() == player.get_total()) {
			strcat(buf, " PUSH");
		} else if (player.get_total() > dealer.get_total() && player.get_total() <= BLACKJACK_TOTAL) {
			strcat(buf, " WINS");
		}
	}
	SetWindowText(hwndPlayerValue, convertUnicode(wstr, buf));
	// show player wins
	sprintf(buf, "Wins = %d", player.get_wins());
	SetWindowText(hwndPlayerWins, convertUnicode(wstr, buf));
}

// convert char* to unicode string
// input: char * str
// input and return: LPWSTR wstr
LPWSTR convertUnicode(LPWSTR wstr, char *str) {
	mbstowcs(wstr, str, strlen(str) + 1);//Plus null
	return wstr;
}

// process hit button press
void hit_button() {
	// get one player card
	player.add_card(lpfnDllNextCard()); // call DLL for next card
	if (player.get_total() > BLACKJACK_TOTAL) {	// if bust, dealer wins
		// new button states
		EnableWindow(hwndHitButton, false);
		EnableWindow(hwndStandButton, false);
		EnableWindow(hwndDealButton, true);
		dealer.increment_wins();
		show_cards(true); // update window
	} else if (player.get_total() == BLACKJACK_TOTAL) { // if blackjack, automatically stand
		stand_button();
	} else {	// keep playing
		show_cards(false); // update window
	}
}

// process stand button press
void stand_button() {
	// now dealer hits
	while (dealer.get_total() < DEALER_STAND) {  // continue hitting until >= 17
		dealer.add_card(lpfnDllNextCard());	// call DLL for next card
	}
	if (player.get_total() > dealer.get_total() || dealer.get_total() > BLACKJACK_TOTAL) {  // if player wins
		player.increment_wins();
	} else if (player.get_total() < dealer.get_total()) { // if dealer wins
		dealer.increment_wins();
	}
	// set button states
	EnableWindow(hwndHitButton, false);
	EnableWindow(hwndStandButton, false);
	EnableWindow(hwndDealButton, true);
	show_cards(true);  // update window
}

// process deal button press
// also used when starting window
void deal_button() {
	reset_window();		// clear card values
	if (lpfnDllRemainingCards() < REDEAL_CARDS_LEFT) {	// reshuffle when cards get too low
		lpfnDllShuffle();
	}
	dealer.reset_hand();	// empty dealer hand
	player.reset_hand();	// empty player hand
	// deal dealer and player 2 cards each
	struct_card ace_card;
	ace_card.num_values = 2;
	ace_card.suit = spades;
	ace_card.rank = ace;
	ace_card.values[0] = 1;
	ace_card.values[1] = 11;
	dealer.add_card(ace_card);
	//dealer.add_card(lpfnDllNextCard());
	dealer.add_card(lpfnDllNextCard());
	player.add_card(ace_card);
	//player.add_card(lpfnDllNextCard());
	player.add_card(lpfnDllNextCard());
	// new button states
	EnableWindow(hwndHitButton, true);
	EnableWindow(hwndStandButton, true);
	EnableWindow(hwndDealButton, false);
	// if blackjack, automatically stand
	if (player.get_total() == BLACKJACK_TOTAL || dealer.get_total() == BLACKJACK_TOTAL) {	
		stand_button();
	} else {
		show_cards(false);		// otherwise, show card info in window
	}
}
