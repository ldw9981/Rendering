// camera.cpp: implementation of the camera class.
//
//////////////////////////////////////////////////////////////////////

#include "camera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

camera::camera()
{
	useMouse_q3_style = 0;
	useMouse_3ds_style = 0;
	set.invertMouse = 0;
	set.changeYposition = 1;
	set.remap_vd_vpd = 0;
	set.k_remap_mouse_button_vd_vpd = 1;
//	set.k_remap_mouse_button_vd_vpd = 0;
	set.block_change_vcp = 0;
	set.in_3ds_mode_y_move_remap_to_z_move = 0;
	pushButton = 0;
	keys[VK_LBUTTON]=0;
	keys[VK_RBUTTON]=0;
	
	set_q3_mouse_style();
//	set_3ds_mouse_style();
	setKeys();
	Reset();
}

camera::~camera()
{
	if(useMouse_q3_style)
	{
		SetCursorPos(last.x, last.y);
		ShowCursor(1);
	}
}

void camera::Reset()
{
	if(useMouse_q3_style)SetCursorPos(screen_x/2,screen_y/2);
	vp.set(0,0.0f,10.f);

	vd.set(0,0,-1);	
	vr.set(1,0,0); 
	vu.set(0,1,0);

	vcp.set(0,0,0);
	vpd = 3;
	vp_old = vp;
}

// na zaklade vstupu z klavesnice a mysi podla rychlosti fps a modu rotacie vypocita novu polohu sveta
// mod ROTATE_WALK, ROTATE_WALK_2, ROTATE_FLY,  a bitovo | ROTATE_ABOUT_POINT pre rotaciu okolo bodu, vcp vo vzdialenosti vpd
void camera::MoveWorld(float fps, int mode)
{
	POINT MousePoint;
	vp_old = vp;

	float rot_x=0,rot_y=0,rot_z=0,step;	// rotate
	float trn_x=0,trn_y=0,trn_z=0;		// translate x, y, z

	if(keys[k.speed[0]]||keys[k.speed[1]]||keys[k.speed[2]]) step=1.0f/fps;else step=5.0f/fps;
	
	if(keys[k.reset[0]]||keys[k.reset[1]]||keys[k.reset[2]])	// reset camera position and rotation
		Reset();

	if(keys[k.vdadd[0]]||keys[k.vdadd[1]]||keys[k.vdadd[2]])	// move in direction vd
		trn_z += step;
	if(keys[k.vdsub[0]]||keys[k.vdsub[1]]||keys[k.vdsub[2]])	// move in inverse vd
		trn_z -= step;
	if(keys[k.vuadd[0]]||keys[k.vuadd[1]]||keys[k.vuadd[2]])	// move in direction vu
		trn_y += step;
	if(keys[k.vusub[0]]||keys[k.vusub[1]]||keys[k.vusub[2]])	// move in inverse direction vu
		trn_y -= step;
	if(keys[k.vradd[0]]||keys[k.vradd[1]]||keys[k.vradd[2]])	// move in direction vr
		trn_x += step;
	if(keys[k.vrsub[0]]||keys[k.vrsub[1]]||keys[k.vrsub[2]])	// move in inverse directoin vr
		trn_x -= step;
	if(mode&ROTATE_ABOUT_POINT)
	{
		if(keys[k.vpdadd[0]]||keys[k.vpdadd[1]]||keys[k.vpdadd[2]])	// delta++
			vpd+= step;
		if(keys[k.vpdsub[0]]||keys[k.vpdsub[1]]||keys[k.vpdsub[2]])	// delta--
			vpd-= step;
		if(set.remap_vd_vpd)
		{
			vpd-=trn_z;
			trn_z=0;
		}
		if(vpd<0)vpd=0;
	}

	if(keys[k.vdrotadd[0]]||keys[k.vdrotadd[1]]||keys[k.vdrotadd[2]])	// rotate about vd +
		rot_z += 0.3f * step;
	if(keys[k.vdrotsub[0]]||keys[k.vdrotsub[1]]||keys[k.vdrotsub[2]])	// rotate about vd -
		rot_z -= 0.3f * step;
	if(keys[k.vurotadd[0]]||keys[k.vurotadd[1]]||keys[k.vurotadd[2]])	// rotate about vu +
		rot_y += 0.3f * step;
	if(keys[k.vurotsub[0]]||keys[k.vurotsub[1]]||keys[k.vurotsub[2]])	// rotate about vu -
		rot_y -= 0.3f * step;
	if(keys[k.vrrotadd[0]]||keys[k.vrrotadd[1]]||keys[k.vrrotadd[2]])	// rotate about vr +
		rot_x += 0.3f * step;
	if(keys[k.vrrotsub[0]]||keys[k.vrrotsub[1]]||keys[k.vrrotsub[2]])	// rotate about vr -
		rot_x -= 0.3f * step;
	
	if(useMouse_q3_style)
	{
		GetCursorPos(&MousePoint);
		rot_y -= (MousePoint.x - screen_x/2)*(float)MOUSE_SPEED;
		if(set.invertMouse)rot_x += (MousePoint.y - screen_y/2)*(float)MOUSE_SPEED;
		else rot_x -= (MousePoint.y - screen_y/2)*(float)MOUSE_SPEED;
		SetCursorPos(screen_x/2,screen_y/2);	// set to center
	}

	if(useMouse_3ds_style)
	{
		if(pushButton)
		{
			GetCursorPos(&MousePoint);

			float mdx = ((float)MousePoint.x - (float)last.x)*MOUSE_SPEED;
			float mdy = ((float)MousePoint.y - (float)last.y)*MOUSE_SPEED;

			if(keys[VK_LBUTTON] && keys[VK_RBUTTON])
			{
				if(set.in_3ds_mode_y_move_remap_to_z_move)
					trn_y -= 10*mdy;
				else
					trn_z -= 10*mdy;
				rot_z += mdx;
			//	vpd += mdy;
			}
			else if(keys[VK_LBUTTON])
			{
				rot_y -= mdx;
				rot_x -= mdy;
			}
			else if(keys[VK_RBUTTON])
			{
				trn_x += 10*mdx;
				if(set.in_3ds_mode_y_move_remap_to_z_move)
					trn_z -= 10*mdy;
				else
					trn_y -= 10*mdy;
			}
			else
			{
				pushButton=0;
				ShowCursor(1);
			}
			SetCursorPos( last.x, last.y);

		}
		else
		{
			if(keys[VK_LBUTTON] || keys[VK_RBUTTON])
			{
				pushButton=1;
				GetCursorPos(&last);
				ShowCursor(0);
			}
		}
		if(keys[VK_F13]){	vpd -= 1;	keys[VK_F13]=0;	}
		if(keys[VK_F14]){	vpd += 1;	keys[VK_F14]=0;	}
		if(vpd<0)vpd=0;
	}
	
	if(mode&ROTATE_ABOUT_POINT)			// ked rotujeme okolo bodu vcp
	{
		if(!set.block_change_vcp)vcp += trn_x*vr + trn_y*vu + trn_z*vd;	// posunutie bodu vcp
		if(!set.changeYposition) vcp.y -= trn_z*vd.y;
	}
	else
	{
		vp += trn_x*vr + trn_y*vu + trn_z*vd;		// translate - posunutie pozicie
		if(!set.changeYposition) vp.y -= trn_z*vd.y;
	}

	switch(mode&255)
	{
	case ROTATE_WALK:	WalkRotate( rot_x, rot_y, rot_z); break;
	case ROTATE_WALK_2:	WalkRotate2( rot_x, rot_y, rot_z); break;
	case ROTATE_FLY:	FlyRotate( rot_x, rot_y, rot_z); break;
	default:			WalkRotate( rot_x, rot_y, rot_z);
	}

	if(mode&ROTATE_ABOUT_POINT) vp = vcp - vd*vpd;		// ked rotujeme okolo bodu vcp
	SetTransform();
}

// WalkRotate - rotacia, ktora sa vyuziva pri chodeni (walk) v bludisku
// pri tejto rotacii sa rotuje vzdy okolu osi y t.j. v pripade ze sa pozerame prave v smere
// osi y (hore, alebo dole) tak sa scena otaca okolo vlastnej osi (neposuva sa vpravo alebo vlavo)
void camera::WalkRotate(float x, float y, float z)
{
	// x > 0 kamera sa toci hore
	// x < 0				dole
	// y > 0 kamera sa toci doprava
	// y < 0				dolava

	// rotacia okolo osi y
	vd.RotY( y/PI180);
	vr.RotY( y/PI180);
	vu.RotY( y/PI180);
	
	// rotacia okolo osi x, t.j. okolo vektora vr

	// vd sa nesmie dostat za 0,1,0 alebo, 0,-1,0
	vec vd_(vd);
	
	// rotacia vektora vd okolo vektora vr o uhol x
	vd_.ArbitraryRotate( x/PI180, vr);
	// ak je vd pred 0,1,0 a vd_ je za je zle, treba otocit tak aby vd = 0,1,0
	// ak je vd pred a vd_ za tak maju pri rovnakych suradniciach rozne znamienka
	// ak je vd a vd_ na tej istej strane maju rovnake znamienka

	if(vd_.x*vd.x<0 || vd_.z*vd.z<0)	// jeden je na jednej strane druhy je uz za
	{
		float angle=1;
		if(vd_.y<0)angle=-1;			// ak pozerame dole tak budeme rotovat dole
		if(vd_.y>0)vd_.set(0,1,0);		// ak vektor smeruje hore nastavyme ho na 0, 1,0
		else vd_.set(0,-1,0);			// ak vektor smeruje dole nastavyme ho na 0,-1,0
		angle *= (float)acos( DOT3(vd_,vd));	// uhol medzi starym vd a novym vd_
		vu.ArbitraryRotate( angle/PI180, vr);	// rotujeme vu
		vd = vd_;						// aktualny vd je vd_
	}
	else								// vd a vd_ su na tej istej strane
	{
		// ak vd.y==1 a x>0, smer rotacie je taky, ze ak by sme rotovali presli by sme na duhu stranu
		// ak vd.y==-1 a x<0,smer rotacie je taky, ze ak by sme rotovali presli by sme na duhu stranu
		// tieto pripady osetrime tak, ze vd a vr nebudeme vobec rotavat, v ostatnych pripadoch rotovat budeme
		if( !(vd.y==1 && x>0) && !(vd.y==-1 && x<0) )
		{
			vd.ArbitraryRotate( x/PI180, vr);
			vu.ArbitraryRotate( x/PI180, vr);
		}
	}
	vd.Normalize();
	vr.Normalize();
	vu.Normalize();
}

// WalkRotate2 - rotacia, ktora sa vyuziva pri chodeni (walk) v bludisku
// pri tejto rotacii sa rotuje vzdy okolu osi y t.j. v pripade ze sa pozerame prave v smere
// osi y (hore, alebo dole) tak sa scena otaca okolo vlastnej osi (neposuva sa vpravo alebo vlavo)
// narozdiel od WalkRotate tato name doraz na osi y, -y
void camera::WalkRotate2(float x, float y, float z)
{
	// rotacia okolo osi y
	vd.RotY( y/PI180);
	vr.RotY( y/PI180);
	vu.RotY( y/PI180);
	
	vd.ArbitraryRotate( x/PI180, vr);
	vu.ArbitraryRotate( x/PI180, vr);
	
	vd.Normalize();
	vr.Normalize();
	vu.Normalize();
}

void camera::SetTransform()
{
	mat4 m;
//	m.MakeMatrix( vr, vu, -vd);		// right-handed coordinate systems
	m.MakeMatrix( vr, vu, vd);		// left-handed coordinate systems
	m.Translate( -vp);
	pd->SetTransform( D3DTS_WORLD, (struct _D3DMATRIX *)(void*)&m );
//	m.glSetMatrix();
}

// FlyRotate - rotacia, ktora umoznuje volne rotacie okolo vsetkych osi
// aktualna rotacia sa vztahuje vzdy aktualny stav, vsetky osi sa mozu lubovolne rotovat
// rotocia okolo x, rotuje okolo vr, okolo y rotuje okolo y a rot. z okolo vd
void camera::FlyRotate(float x, float y, float z)
{


// x > 0 kamera sa toci hore
// x < 0				dole
// y > 0 kamera sa toci doprava
// y < 0				dolava
// z > 0 sa suradnicova sustava otoci v     smere hodinovych ruciciek
// z < 0                              proti smeru
	vd.ArbitraryRotate( y/PI180, vu);
	vr.ArbitraryRotate( y/PI180, vu);

	vr.Normalize();
	vd.ArbitraryRotate( x/PI180, vr);
	vu.ArbitraryRotate( x/PI180, vr);
	
	vd.Normalize();
	vr.ArbitraryRotate( z/PI180, vd);
	vu.ArbitraryRotate( z/PI180, vd);
	
	vr.Normalize();
	vu.Normalize();
}


void camera::setKeys()
{
	for(int i=0; i<3; i++)
	{
		k.reset[i]=255; k.speed[i]=255;
		k.vdadd[i]=255; k.vdsub[i]=255; k.vdrotadd[i]=255; k.vdrotsub[i]=255;
		k.vuadd[i]=255; k.vusub[i]=255; k.vurotadd[i]=255; k.vurotsub[i]=255;
		k.vradd[i]=255; k.vrsub[i]=255; k.vrrotadd[i]=255; k.vrrotsub[i]=255;
		k.vpdadd[i]=255; k.vpdsub[i]=255;
	}
	// reset
	k.reset[0] = 'R';
	k.speed[0] = VK_SHIFT;
	// move in direction vd
	k.vdadd[0] = VK_UP;			// arrow up
	if(useMouse_q3_style)k.vdadd[1] = VK_LBUTTON;		// left button on mouse
	k.vdadd[2] = 'W';
	// move in inverse vd
	k.vdsub[0] = VK_DOWN;			// arrow down
	if(useMouse_q3_style)k.vdsub[1] = VK_RBUTTON;
	k.vdsub[2] = 'S';
	// move in direction vu
	k.vuadd[0] = VK_HOME;
	// move in inverse direction vu
	k.vusub[0] = VK_END;
	// move in direction vr
	k.vradd[0] = VK_INSERT;
	k.vradd[1] = 'D';
	// move in inverse directoin vr
	k.vrsub[0] = VK_DELETE;
	k.vrsub[1] = 'A';
	// rotate about vd +
	k.vdrotadd[0] = 'E';
	// rotate about vd -
	k.vdrotsub[0] = 'Q';
	// rotate about vu +
	k.vurotadd[0] = VK_LEFT;		// arrow left
	// rotate about vu -
	k.vurotsub[0] = VK_RIGHT;
	// rotate about vr +
	k.vrrotadd[0] = VK_PRIOR;		// Page Down
	// rotate about vr -
	k.vrrotsub[0] = VK_NEXT;		// Page Up
	// delta ++ for ROTATE_ABOUT_POINT
	k.vpdadd[0] = VK_ADD;
	// delta -- for ROTATE_ABOUT_POINT
	k.vpdsub[0] = VK_SUBTRACT;

	if(useMouse_q3_style)
	{
		if(set.k_remap_mouse_button_vd_vpd)
		{
			k.vdadd[1] = 255; k.vdsub[1] = 255;
			k.vpdadd[1] = VK_RBUTTON;	k.vpdsub[1] = VK_LBUTTON;
		}
		else
		{
			k.vpdadd[1] = 255; k.vpdsub[1] = 255;
			k.vdadd[1] = VK_LBUTTON;	k.vdsub[1] = VK_RBUTTON;
		}
	}
}

void camera::set_q3_mouse_style()
{
	if(useMouse_3ds_style || (!useMouse_3ds_style&&!useMouse_q3_style))
	{
		GetCursorPos(&last);
		if(!pushButton)ShowCursor(0);
	}
	SetCursorPos(screen_x/2,screen_y/2);	// set to center
	if(set.k_remap_mouse_button_vd_vpd)
	{
		k.vdadd[1] = 255; k.vdsub[1] = 255;
		k.vpdadd[1] = VK_RBUTTON;	k.vpdsub[1] = VK_LBUTTON;
	}
	else
	{
		k.vpdadd[1] = 255; k.vpdsub[1] = 255;
		k.vdadd[1] = VK_LBUTTON;	k.vdsub[1] = VK_RBUTTON;
	}
	keys[VK_LBUTTON]=0;
	keys[VK_RBUTTON]=0;

	useMouse_q3_style = 1;
	useMouse_3ds_style = 0;
}

void camera::set_3ds_mouse_style()
{

	if(useMouse_q3_style)
	{
		SetCursorPos(last.x, last.y);
		ShowCursor(1);
	}

	if(set.k_remap_mouse_button_vd_vpd)
	{
		k.vpdadd[1] = 255;	k.vpdsub[1] = 255;
	}
	else
	{
		k.vdadd[1] = 255;	k.vdsub[1] = 255;
	}

	pushButton = 0;
	keys[VK_LBUTTON]=0;
	keys[VK_RBUTTON]=0;

	useMouse_q3_style = 0;
	useMouse_3ds_style = 1;
}


