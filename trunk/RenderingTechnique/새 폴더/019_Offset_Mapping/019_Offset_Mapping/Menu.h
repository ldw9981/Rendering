#ifndef _MENU_H
#define _MENU_H

#include "init.h"				// zakladne nastavenia a zakladne hlavickove subory
#include "Font_bitmap.h"		// trieda pre vypisovanie textu pomocou bitmapoveho fontu
#include <vector>
using namespace std;

#define MAX_TEXT_LENGHT (screen_x_font/11-14)

#define ITEM_TEXT		1
#define ITEM_KEY		2
#define ITEM_BOOL		3
#define ITEM_INT		4
#define ITEM_FLOAT		5
#define ITEM_KEY_CHANGE	6
#define ITEM_RANGE_INT	7

struct zMenu_item_int
{
	char name[MAX_TEXT_LENGHT+1];
	int value;			// int, float
};

struct zMenu_item
{
	char name[MAX_TEXT_LENGHT+1];
	int key;		// -1 nepridelena klavesa
	int type;		// bool, int, float, 
	void *p;		// pointer
	vector<zMenu_item_int> IntItems;		// if type == ITEM_INT
	int IntIndex;
	struct zMenu_item_float					// if type == ITEM_FLOAT
	{
		float min,max,aktual;			// value = min+aktual*(max-min); aktual <0,1>
	}float_item;
	struct zMenu_item_int_range				// if type == ITEM_INT_RANGE
	{
		int min,max,step,aktual;
	}int_range_item;

	int show_bit_mask;	// this item is show only if (show_number & *p_show_number) is true, if p_show_number != NULL
	int* p_show_number;

	int isShow()
	{
		if(!p_show_number)return 1;
		return (show_bit_mask&*p_show_number);
	}
};

class Menu  
{
public:
	int getItemIndex(int showedItemIndex);
	void SetShowCondition( int show_bit_mask_);
	void SetShowCondition( int show_bit_mask_, int* p_show_number_);
	int CreateVertexBufferForRectange(float left, float top, float right, float bottom, float alpha);
	void AddKeyChange( char* name, int key=-1, void* p=NULL);			// if push key *p set to 1
	void AddBool( char* name, int key=-1, void* p=NULL, int value=0);	// if push key *p = ! *p
	void AddText( char* text);											// only text
	void AddKey( char* name, int key=-1, void* p=NULL);					// *p have same value as keys[key]
	
	void AddInt( char* name, int key=-1, void* p=NULL);					// start new list
	void AddIntItem( char* name, void* p);								// add item to list, is this item is selected, *p of list is set to this *p
	void AddIntSetIndex( int index);									// set index which is show

	void AddFloat( char* name, int key, void* p, float min, float max, float aktual);		// aktual must be in [0,1]
	void AddFloatComp( char* name, int key, void* p, float min, float max, float value);	// value must be in [min,max]

	void AddRangeInt(char* name, int key, void* p, int min, int max, int step, int aktual);

	void ChangeSetting( float timeframe=0.02f);							// change set, id call if menu is'n draw
	void Render( float timeframe=0.02);									// draw menu and change setting
	Menu();
	int Init( Font_bitmap *font);
	virtual ~Menu();
	Font_bitmap	*f;
	vector<zMenu_item> items;
	int		akt_item;			// riadok
	POINT	last_mouse_pos;
	LPDIRECT3DVERTEXBUFFER8 pvb_rectange;	// Buffers to hold vertices

	int show_bit_mask;		// this is use for automatic set zMenu_item::show_bit_mask, when item is added
	int* p_show_number;		// this is use for automatic set zMenu_item::p_show_number, when item is added
};

#endif // _MENU_H
