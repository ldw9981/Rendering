#ifndef _CAMERA_H
#define _CAMERA_H

#include "init.h"
#include "Matrix.h"

#define ROTATE_WALK			1
#define ROTATE_WALK_2		2
#define ROTATE_FLY			3
#define ROTATE_ABOUT_POINT	256

#define MOUSE_SPEED 0.005f

class camera  
{
public:
	void Reset();
	void WalkRotate2(float x, float y, float z);
	void FlyRotate( float x, float y, float z);
	void SetTransform();
	void WalkRotate(float x, float y, float z);
	void MoveWorld(float fps, int mode=ROTATE_WALK);
//	void MoveToCentralPoint(){ glTranslatef(vcp.x, vcp.y, vcp.z); }
	void setKeys();
	vec vp;			// View position			- pozicia pohladu (oèí)
	vec vp_old;		// View position old		- pozicia pohladu (oèí) v predchadzajucom snimku, pouzitie pre detekciu kolizie
	vec vd;			// View direction vector	- smerovy vektor
	vec vu;			// View up direction		- vektor zmerujuci hore - normalovy vektor hornej plochy kamery
	vec vr;			// View right direction		- vektor smerujuci vpravo
	vec vcp;		// View central point
	float	vpd;		// View point delta

	// keys setting - nastavenie klaves
	struct k_keys_setting
	{
		unsigned char reset[3];		// reset position and rotation camera
		unsigned char vdadd[3];		// move in direction vd
		unsigned char vdsub[3];		// move in inverse vd
		unsigned char vuadd[3];		// move in direction vu
		unsigned char vusub[3];		// move in inverse direction vu
		unsigned char vradd[3];		// move in direction vr
		unsigned char vrsub[3];		// move in inverse directoin vr
		unsigned char vpdadd[3];	// delta++ for ROTATE_ABOUT_POINT
		unsigned char vpdsub[3];	// delta-- for ROTATE_ABOUT_POINT
		unsigned char vdrotadd[3];	// rotate about vd +
		unsigned char vdrotsub[3];	// rotate about vd -
		unsigned char vurotadd[3];	// rotate about vu +
		unsigned char vurotsub[3];	// rotate about vu -
		unsigned char vrrotadd[3];	// rotate about vr +
		unsigned char vrrotsub[3];	// rotate about vr -
		unsigned char speed[3];		// speed
	}k;
	// premenne zacinajuce k_ ovplyvnuju nastavenie struktury k, 
	// preto po zmene treba zavolat funkciu setKeys()
	struct set_camera_setting
	{
		unsigned char invertMouse;
		unsigned char changeYposition;		// meni y poziciu pri posuvani v smere vd
		unsigned char remap_vd_vpd;			// klavesi na vd budu menit vpd
		unsigned char k_remap_mouse_button_vd_vpd;	// tlacitka na mysi budu menit vpd
		unsigned char block_change_vcp;		// blokovat zmenu pozicie stredu pri rotacii okolo bodu
		unsigned char in_3ds_mode_y_move_remap_to_z_move;
	}set;
private:
	POINT last;							// last mouse position
	unsigned char pushButton;
	unsigned char useMouse_q3_style;	// automaticky reset na stred
	unsigned char useMouse_3ds_style;	// pouziva mys iba pri stlaceni tlacitok
public:
	void set_3ds_mouse_style();
	void set_q3_mouse_style();
	camera();
	virtual ~camera();
};

#endif // _CAMERA_H