#include "Scene.h"
#include "init.h"

HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

LPDIRECT3D8			pD3D = NULL;	// Used to create the D3DDevice
LPDIRECT3DDEVICE8	pd = NULL;		// Our rendering device

char	WindowName[]="011_environment_mapped_bump_mapping";
bool	keys[256];			// Array Used For The Keyboard Routine
bool	fullscreen=1;		// Fullscreen Flag
bool	active=1;			// Window Active Flag
int		error=0;			// if is 1 then that is error and program will by close
int		screen_x=800;		// horizontal screen size
int		screen_y=600;		// vertical screen size
int		screen_bit=16;		// cout of bits on 1 pixel
int	english=1;
Scene *scene=NULL;

D3DCAPS8 caps;				// the capabilities of the hardware
int software_vertexprocessing=0;
int MultiSample=0;
int synchronization=0;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// declaration WndProc
void KillDXWindow(void);								// declaration KillGLWindow
int Load_init_txt();

BOOL CreateDXWindow(char* title, int width, int height, int bits, bool fullscreen_)
{
	fullscreen=fullscreen_;

	// Create the Direct3D object
	pD3D = Direct3DCreate8( D3D_SDK_VERSION );
	if( pD3D == NULL )
	{
		english ? MessageBox( NULL, "Couldn't initialize Direct3D\nMake sure you have DirectX 8.1 or later installed.", "Error", MB_OK | MB_ICONERROR):
		          MessageBox( NULL, "Nemozem inicializovat DX8.1\nPravdepodobne DX8.1 nie je spravne nainstalovany","Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	WNDCLASSEX	wc;		// miesto pre strukturu pouzitu na registraciu triedy okna
	wc.cbSize			= sizeof(WNDCLASSEX); //velkost struktury WINDOWCLASSEX
	wc.style				= CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;/* | CS_OWNDC;  /*nastavenie stylu okna
				CS_HREDRAW - prekreslenie okna pri zmene horizontalne velkosti okna
				CS_VREDRAW - -----------""-------------- vertikalnej  ------""-----
				CS_OWNDC - vyhradi jedinecny kontext zariadenia pre kazde okno  */
	wc.lpfnWndProc		= (WNDPROC) WndProc; //smernika na funkciu na spracovanie sprav
	wc.cbClsExtra		= 0; //specifikuje pocet extra bytov na konci struktury triedy okien na ukladanie informacii
	wc.cbWndExtra		= 0; //-------""--------------------, ktore sa maju vyhradit po vyskyte okna
	wc.hInstance		= hInstance;		//handle (ukazovatel) na instalaciu (paremeter z WinMain)
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINLOGO)); //handle ikony  ktora sa ma pouzit pre danu truedu okien
	wc.hCursor			= LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));		// LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_MAIN)); //handle kurzora ktory ----------------""----------------
	wc.hbrBackground	= NULL;		//handle stetca pouziteho na vyfarbenie pozadia okna
	wc.lpszMenuName		= NULL;		//ukazovatel na sz(StringZero) v ktorom je nazov pre menu (!)triedy
	wc.lpszClassName	= "DX";	//ukaz na sz, ktory obsahuje nazov triedy. Tento nazov sa pouzie v parametry lpClassName vo funkcii CreateWindowEx
	wc.hIconSm			= LoadIcon(hInstance,MAKEINTRESOURCE(101)); //	NULL; //handle k ikone, ktora sa ma pouzit v titulkovom pruhu okien, vytvorenych touto triedou. Ak je NULL pouzije sa hIcon

	if(!RegisterClassEx(&wc))		//registracia novej tiredy okna, pouzivanych na vytavaranie okien
	{	
		KillDXWindow(); 
		english ? MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONERROR):
		          MessageBox(NULL,"Chyba pri registracii triedy okna.","ERROR",MB_OK|MB_ICONERROR);
		return 0;
	}
	
	DWORD		dwStyle;
	// Set the window's initial style
	dwStyle = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_VISIBLE;
	dwStyle = WS_OVERLAPPEDWINDOW;

	// Set the window's initial width
	RECT		WindowRect;					// miesto pre velkost okna
	SetRect( &WindowRect, 0, 0, width, height );
	AdjustWindowRect( &WindowRect, dwStyle, FALSE );

	// Create the render window
	hWnd = CreateWindow( "DX", title, dwStyle,
						   CW_USEDEFAULT, CW_USEDEFAULT,
						   WindowRect.right-WindowRect.left,
						   WindowRect.bottom-WindowRect.top,
						   NULL,
						   NULL,
						   hInstance, NULL);

	if (!hWnd)		//ak sa okno nepodarilo vytvorit koniec
	{	
		KillDXWindow();
		english ? MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION):
		          MessageBox(NULL,"Chyba pri tvorbe okna.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if( !fullscreen )
	{
		// Set windowed-mode style
		SetWindowLong( hWnd, GWL_STYLE, GetWindowLong( hWnd, GWL_STYLE ) );
	}
	else
	{
		// Set fullscreen-mode style
		SetWindowLong( hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
	}

	D3DPRESENT_PARAMETERS d3dpp;

	// Set up the presentation parameters
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferCount        = 1;
	*(int*)&d3dpp.MultiSampleType= MultiSample;					// D3DMULTISAMPLE_NONE
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;		// D3DSWAPEFFECT_FLIP
	d3dpp.hDeviceWindow          = hWnd;
	d3dpp.Windowed               = !fullscreen;
	d3dpp.EnableAutoDepthStencil = 1;					// 1 if use z-buffer
		
/*				total		z-bits		stencil-bits
D3DFMT_D32		32			32
D3DFMT_D15S1	16			15			1
D3DFMT_D24S8	32			24			8
D3DFMT_D16		16			16
D3DFMT_D24X8	32			24
D3DFMT_D24X4S4	32			24			4
*/
	if     (bits==16)d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// 16 bits
	else if(bits==24)d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// 16 bits
	else if(bits==32)d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;	// 24 bits
	else             d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;	// 24 bits
	
	if(fullscreen)
	{
		if(synchronization)
			d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		else
			d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	
/*					total	R	G	B	A
D3DFMT_R8G8B8		24		8	8	8
D3DFMT_A8R8G8B8		32		8	8	8	8	ARGB
D3DFMT_X8R8G8B8		32		8	8	8
D3DFMT_R5G6B5		16		5	6	5
D3DFMT_X1R5G5B5		16		5	5	5
D3DFMT_A1R5G5B5		16		5	5	5	1	ARGB
D3DFMT_A4R4G4B4		16		4	4	4	4	ARGB
D3DFMT_X4R4G4B4		16		4	4	4
D3DFMT_A2B10G10R10	32	   10  10  10	2
*/
	// Get the current desktop display mode
	D3DDISPLAYMODE d3ddm;
	if( FAILED( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{		KillDXWindow(); MessageBox(NULL,"Error in GetAdapterDisplayMode.","ERROR",MB_OK|MB_ICONEXCLAMATION);return FALSE;	}
	if( !fullscreen )
	{

		RECT		rcWindowClient;
		GetClientRect( hWnd, &rcWindowClient );
		d3dpp.BackBufferWidth  = rcWindowClient.right - rcWindowClient.left;
		d3dpp.BackBufferHeight = rcWindowClient.bottom - rcWindowClient.top;
		d3dpp.BackBufferFormat = d3ddm.Format;
	}
	else
	{
		d3dpp.BackBufferWidth  = width;
		d3dpp.BackBufferHeight = height;
		if(bits==16)d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		else if(bits==24)d3dpp.BackBufferFormat = D3DFMT_R8G8B8;
		else if(bits==32)d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		else d3dpp.BackBufferFormat = d3ddm.Format;
	}

    // Create the device
	if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL,
										hWnd,
									//	D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										D3DCREATE_HARDWARE_VERTEXPROCESSING,
										&d3dpp, &pd ) ) )
	{
		if( pd != NULL)pd->Release();
		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_HAL,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &pd ) ) )
		{
			KillDXWindow(); 
			english ? MessageBox(NULL,"Couldn't create Direct3D device interface.", "Error", MB_OK | MB_ICONERROR):
			          MessageBox(NULL,"Nemozem vytvorit rozhranie pre kreslenie.\r\nPravdepodobne zariadenie nepodporuje pozadovane rozlisenie a rezim.\r\nCreat IDirect3DDevice8 fail.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			
			return FALSE;
		}
		software_vertexprocessing = D3DUSAGE_SOFTWAREPROCESSING;
	}

	if( !fullscreen  )
	{
		SetWindowPos( hWnd, HWND_NOTOPMOST,
					  WindowRect.left+20, WindowRect.top+30,
					  ( WindowRect.right - WindowRect.left ),
					  ( WindowRect.bottom - WindowRect.top ),
					  SWP_SHOWWINDOW );
	}

	// Store device Caps
	pd->GetDeviceCaps( &caps );

	// Turn off culling, so we see the front and back of the triangle
	pd->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// Turn off D3D lighting, since we are providing our own vertex colors
	pd->SetRenderState( D3DRS_LIGHTING, FALSE );

	// Device state would normally be set here

	ShowWindow(hWnd,SW_SHOW);		//zvyditelnie okna, posiela spravu WM_SHOWWINDOW
	SetForegroundWindow(hWnd);		//nastavuje okno na to s ktorym pracuje uzivatel
	SetFocus(hWnd);					//nastavy vstup klavesnice na toto okno
	// inicializacia sceny
	scene = new Scene;				// vytvarame scenu
	if(scene==NULL || error)		// ak sa nevytvorila, alebo pri vytvarani vznikla chyba
	{
		if(scene!=NULL)delete scene;	// ak sa vytvorila, uvolnime
		KillDXWindow();					// vypneme OpenGL
		MessageBox(hWnd,english ? "Couldn't initialize scene.":"Chyba pri vytvarani sceny.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;						// koniec, aplikacia sa ukonci
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////
void KillDXWindow(void)					// rušíme okno
{	
//	if (fullscreen)ChangeDisplaySettings(NULL,0);	// zmena grafickeho rezimu na prednastavene rozlisenie

	if( pd != NULL)pd->Release();

	if( pD3D != NULL)pD3D->Release();

	if (hWnd && !DestroyWindow(hWnd))   // ukoncuje aplikaciu, posiela spravu WM_DESTROY
	{	
		MessageBox(NULL,english ? "Error: hWnd.":"Chyba: hWnd.","ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;
	}
	if (!UnregisterClass("DX",hInstance)) //removes a window class, freeing the memory required for the class.
	{	
		MessageBox(NULL,english ? "Couldn't unregister window class":"Nejde odregistrovat okno.","ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;
	}
}


#define WM_MOUSEWHEEL                   0x020A
/*********************************************************************************************/
/*Funkcia na spracovanie sprav. Spravy su posielane do funkcie pomocou DispatchMessage(&msg);*/

LRESULT CALLBACK WndProc(HWND	hWnd,UINT	uMsg,WPARAM	wParam,LPARAM	lParam)
{
	switch (uMsg)
	{	
		case WM_ACTIVATE:			//generovane ak je okno aktivizovane, alebo deaktivizovane
		{	
			if (!HIWORD(wParam))	//HIWORD(wParam) -priznak minimalizovaneho okna
			{active=TRUE;}			//okno nie je minimalizovane
			else active=FALSE;		//okno     je minimalizovane
			return 0;
		}
		case WM_SYSCOMMAND:{  switch (wParam){  case SC_SCREENSAVE: case SC_MONITORPOWER: return 0;}break;}
		case WM_CLOSE:{PostQuitMessage(0);return 0;}	// generuje WM_QUIT a wParam=0
		case WM_KEYDOWN:{keys[wParam] = 1;return 0;}
		case WM_KEYUP:	{keys[wParam] = 0;return 0;}
		case WM_LBUTTONDOWN:{keys[VK_LBUTTON] = TRUE;return 0;}
		case WM_LBUTTONUP:{keys[VK_LBUTTON] = FALSE;return 0;}
		case WM_RBUTTONDOWN:{keys[VK_RBUTTON] = TRUE;return 0;}
		case WM_RBUTTONUP:{keys[VK_RBUTTON] = FALSE;return 0;}
		case WM_SIZE:	{if(scene!=NULL)scene->ReSizeDXScene(LOWORD(lParam),HIWORD(lParam));return 0;}  // zmena velkosti
		case WM_MOUSEWHEEL:	
			{
				int wheel=((short) HIWORD(wParam))/120;
				if(wheel==1)keys[VK_F13]=1;
				else if(wheel==-1)keys[VK_F14]=1;
			} 
			break;	// wheel rotation
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

//////////////////////////////////////////////////////////////////////
/**********VSTUPNA FUNCKIA*****************************************************/
int WINAPI WinMain(HINSTANCE hInstance_,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{	
	MSG		msg;		//mesto pre strukturu MESSAGE - pre spravy
	BOOL	finish=0;
	hInstance=hInstance_;
	
	if( Load_init_txt())
	if( ( english ? 
		MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION) :
		MessageBox(NULL,"Cela obrazovka?", "Nastavenie obrazovky (pocas behu pomocou F4)",MB_YESNO|MB_ICONQUESTION)
			) ==IDNO )
	{	fullscreen=FALSE;}
	// vytvarame okno a incializujeme scenu
	if (!CreateDXWindow(WindowName,screen_x,screen_y,screen_bit, fullscreen))return 0;
	
	while(!finish)		//pokial nie je koniec
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	 /*ak nie je pristupna sprave vrati 0
		ak je prisupna sparava vrati nenulu a zapise ju do struktury msg,
		NULL - HWND spracuva spravy z aktualneho vlakna, PM_REMOVE - presuva spravu z radu sprav
		na rozdiel od GetMessage, PeekMessage necaka na pijatie dalsiej sprave pred vratenim (hodnoty)*/
		{				//sprava je pristupna - treba ju spracovat
			if (msg.message==WM_QUIT)finish=1;
			else
			{	
				TranslateMessage(&msg);	//preklada virtualne spravy (VK_*) na znakove spravy (WM_CHAR)
				DispatchMessage(&msg);	//posiela spravy aplikacie prislusnej funkcie (WndProc)na spracovanie
			}							//- "" -	  vracia to co vracia WndProc
		}
		else	//ziadna sprava - cinnost ktoru ma vykonavat mimo spracovavania sprav
		{
			if (active)scene->DrawDXScene();
			if (keys[VK_ESCAPE]) finish=1;		//ak je stlaceny Esc ukonc
			if (keys[VK_F4])			//cez F1 prepinanie z Fullscreen do okna
			{	
				keys[VK_F4]=FALSE;
				if(scene!=NULL)delete scene;
				scene = NULL;
				KillDXWindow();
				fullscreen=!fullscreen;
				if (!CreateDXWindow(WindowName,screen_x,screen_y,screen_bit,fullscreen))return 0;
			}
		}
	}

	if(scene!=NULL)delete scene;
	scene = NULL;
	KillDXWindow();
	return (msg.wParam);		//funkcia WinMain ma vratit hodnotu, ktora bola pouzita z funkciou PostQuitMessage
}

int GetVariable(char *temp, FILE *sub, char* varname, char* varvalue, int size_strings)
{
	if(fgets(temp, size_strings, sub) == NULL)
	{
		temp[0] = NULL;
		return 0;
	}
	varname[0]=NULL;
	varvalue[0]=NULL;
	char rovnasa=0;
	for(int i=0,j=0,k=0; temp[i]!=NULL&&temp[i]!='\n'&&temp[i]!='/'; i++)		// prejdeme vsetky znaky
	{
		if(temp[i]==' ' || temp[i]=='\t')continue;
		if(temp[i]=='='){ rovnasa=1;continue;}
		if(rovnasa)
		{
			varvalue[k]=temp[i]; k++;
		}
		else
		{
			varname[j]=temp[i]; j++;
		}
	}
	varname[j]=NULL;
	varvalue[k]=NULL;
	return 1;
}

int Load_init_txt()
{
	FILE* sub=NULL;
	char temp[100];
	char varname[100],varvalue[100];
	int ret=0;

	sub = fopen( "data/init.txt", "rb");
	if(sub == NULL)return 1;

	while(GetVariable( temp, sub, varname, varvalue, 100))
	{
		float fl=(float)atof(varvalue);
		int in=atoi(varvalue);
		strlwr(varname);				// Convert a string to lowercase
		if(!strcmp("width",varname))screen_x=in;
		else if(!strcmp("height",varname))screen_y=in;
		else if(!strcmp("colorbit",varname))screen_bit=in;
		else if(!strcmp("fullscreen",varname))
		{
			switch(varvalue[0])
			{
				case 'A':	case 'a':	case 'Y':	case 'y':	fullscreen = 1;	break;
				case 'N':	case 'n':	fullscreen = 0;	break;
				case 'M':	case 'm':	ret=1; break;
			}
		}
		else if(!strcmp("language",varname))
		{
			if(in==0)english=1;
			else if(in==1)english=0;
		}
		else if(!strcmp("synchronization",varname))
		{
			switch(varvalue[0])
			{
				case 'A':	case 'a':	case 'Y':	case 'y':	synchronization = 1;	break;
				case 'N':	case 'n':	synchronization = 0;	break;
			}
		}
		else if(!strcmp("multisample",varname))MultiSample = in;
	}
	fclose(sub);
	if(screen_x==0)screen_x=800;
	if(screen_y==0)screen_y=600;
	if(screen_bit==0)screen_bit=16;
	return ret;
}