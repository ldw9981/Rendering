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
	struct zMenu_item_float
	{
		float min,max,aktual;			// value = min+aktual*(max-min); aktual <0,1>
	}float_item;
};

class Menu  
{
public:
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
};

#endif // _MENU_H
