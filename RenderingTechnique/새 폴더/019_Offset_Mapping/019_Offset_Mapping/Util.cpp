// Util.cpp: implementation of the Util class.
//
//////////////////////////////////////////////////////////////////////

#include "Util.h"

//////////////////////////////////////////////////////////////////////
// uVertexShader Class
//////////////////////////////////////////////////////////////////////

int uVertexShader::LoadVS(char *filename, DWORD *pDeclaration, char *verprog)
{
	int er=0;
	// Create the vertex shader.
	LPD3DXBUFFER pCode;                             // assembled shader code
	LPD3DXBUFFER pError;
	if(hVertexShader!=NULL)pd->DeleteVertexShader( hVertexShader);
	if(filename==NULL)
		D3DXAssembleShader( verprog, strlen(verprog), 0, NULL, &pCode, &pError );  // assemble shader code
	else
	{
		if( D3DXAssembleShaderFromFile( filename, 0, NULL, &pCode, &pError) != D3D_OK)er=1;  // assemble shader code
	}

	if(pError!=NULL)
	{
		char text[150];
		char* error_text = new char[pError->GetBufferSize()+1];
		memcpy(error_text,pError->GetBufferPointer(),pError->GetBufferSize());
		error_text[pError->GetBufferSize()]=NULL;
		pError->Release();
		
		if(filename!=NULL)
			sprintf( text, "ERROR in vertex shader: %s", filename);
		else
			sprintf( text, "ERROR in vertex shader: %s");

		MessageBox(hWnd, error_text, text, MB_OK);
		delete [] error_text;
		hVertexShader = NULL;
		return 0;
	}
	else if(er==1)
	{
		MessageBox(hWnd, filename, english ? "Couldn't find file for vertex shader":"Nenajdeny subor pre vertex shader", MB_OK);
		return 0;
	}

	if(	pd->CreateVertexShader( pDeclaration, (DWORD*)pCode->GetBufferPointer(), &hVertexShader, software_vertexprocessing )
		== D3D_OK )
	{
		pCode->Release();
		return 1;
	}
	pCode->Release();
	MessageBox(hWnd, filename, english ? "ERROR in vertex shader":"Chyba pri nacitavani vertex shader", MB_OK);
	return 0;
}

//////////////////////////////////////////////////////////////////////
// uPixelShader Class
//////////////////////////////////////////////////////////////////////

int uPixelShader::LoadPS(char *filename, char *pixelprog)
{
	int er=0;
	// Create the pixel shader.
	LPD3DXBUFFER pCode;                             // assembled shader code
	LPD3DXBUFFER pError;
	if(hPixelShader!=NULL) pd->DeletePixelShader( hPixelShader);
	if(filename==NULL)
		D3DXAssembleShader( pixelprog, strlen(pixelprog), 0, NULL, &pCode, &pError );  // assemble shader code
	else
	{
		if( D3DXAssembleShaderFromFile( filename, 0, NULL, &pCode, &pError) != D3D_OK)er=1;  // assemble shader code
	}

	if(pError!=NULL)
	{
		char text[150];
		char* error_text = new char[pError->GetBufferSize()+1];
		memcpy(error_text,pError->GetBufferPointer(),pError->GetBufferSize());
		error_text[pError->GetBufferSize()]=NULL;
		pError->Release();

		if(filename!=NULL)
			sprintf( text, "ERROR in pixel shader: %s", filename);
		else
			sprintf( text, "ERROR in pixel shader: %s");

		MessageBox(hWnd, error_text, text, MB_OK);
		delete [] error_text;
		hPixelShader = NULL;
		return 0;
	}
	else if(er==1)
	{
		MessageBox(hWnd, filename, english ? "Couldn't find file for pixel shader":"Nenajdeny subor pre pixel shader", MB_OK);
		return 0;
	}

	if(	pd->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &hPixelShader )
		== D3D_OK )
	{
		pCode->Release();
		return 1;
	}
	pCode->Release();
	MessageBox(hWnd, filename, english ? "ERROR in pixel shader":"Chyba pri nacitavani pixel shader", MB_OK);
	return 0;
}


vec CheckCollision( vector<z_face> &collision, vec vp, vec vpold, float radius)
{
	//	if(vpold_vp.Length()==0)return vpold;
	vec vpold_vp = vp-vpold;		// smer pohybu
	vec vpold_vp_povodny = vpold_vp;// smer pohybu
	int iter=0;
	float radius2 = radius*radius;

	vec newmove = vpold_vp;
	vec newClosestPointOnSphere;
	vec newContactPointSphereToTriangle;

	do
	{
		float distanceCenterPointOnTriangle=1.e+15f;
		int smykanie=0;
		for( int i=0; i<collision.size(); i++)
		{
			vec normal = collision[i].normal;
			
			vec vp_move;
			vec center = vpold;
			float distance = PlaneDistance( normal, collision[i].a);		// D = - (Ax+By+Cz)
			// ---------------------------------------------------------
			// najdeme kolidujuci bod na guli
			vec ClosestPointOnSphere;		// najblizsi bod gule k rovine aktualneho trojuholnika
			// najdeme ho ako priesecnik priamky prechadzajucej stredom gule a smerovym vektorom = normalovemu vektoru roviny (trojuholnika)
			// vypocitame ho, ale jednodusie tak, ze pripocitame (opocitame) k stredu vektor normala*radius
			
			if( PlanePointDelta( normal, distance, center) > 0 )
			{
				// center je na strane normaly, blizsi bod je v opacnom smere ako smer normaly
				ClosestPointOnSphere = -radius*normal+center;
			}
			else
			{
				// center je na opacnej strane ako normala, blizsi bod je v smere normaly
				ClosestPointOnSphere = radius*normal+center;
			}

			// ---------------------------------------------------------
			// najdeme kolidujuci bod na trojuholniku
			// najprv najdeme kolidujuci bod vzhladom na rovinu v ktorej lezi trojuholnik
			vec contactPointSphereToPlane;			// kolidujuci bod na rovine trojuholnika s gulou
			float distanceCenterToPlane;			// vzdialenost stredu gule k rovine
			// zistime ci rovina pretina gulu
			if( SpherePlaneCollision( center, 0.9999f*radius, normal, distance, &distanceCenterToPlane)==1 )
			{
				// gula pretina rovinu
				// kolidujuci bod je bod na rovine najblizsi k stredu gule
				// je vzdialeny od roviny na distanceCenterToPlane, pretoze pocitame bod na rovine pouzijeme -
				contactPointSphereToPlane = center-distanceCenterToPlane*normal;
			}
			else
			{
				// nie sme v kolizii z gulov, ak sa pohybujeme v smere od roviny, nemoze nastat ziadna kolizia
				// ak sa pohybujeme v smere kolmom na normalovy vektor roviny, tak isto kolizia nehrozi
				// kvoli nepresnosti vypoctov umoznime pohyb aj ked velmi malou castou smeruje do roviny
			//	if( DOT3( vpold_vp, center-ClosestPointOnSphere) >= 0)
				if( DOT3( vpold_vp, center-ClosestPointOnSphere) > -0.000001f)
				{
					continue;
				}
				// gula nepretina rovinu
				// kolidujuci bod je priesecnik roviny a priamky vedenej z bodu ClosestPointOnSphere
				// v smere pohybu t.j. z vpold do vp

				float t = LinePlaneIntersectionDirParameter( ClosestPointOnSphere, vpold_vp, normal, distance);
				// t > 1.f, priesecnik z rovinou je dalej ako vpold_vp od bodu ClosestPointOnSphere
				if(t>1.f)
					continue;	// za cely krok vpold_vp sa s tymto trojuholnikom nestretneme
				else if( t<-radius/vpold_vp.Length())		// priesecnik je za gulou, v smere pohybu tuto rovinu nestretneme
					continue;
				else 
					contactPointSphereToPlane = ClosestPointOnSphere+t*vpold_vp;
			}
			// najdeme kolidujuci bod na trojuholniku
			vec contactPointSphereToTriangle;
			// ak sa bod contactPointSphereToPlane nenachadza v trojuholniku 
			// najdeme najblizsi bod trojuholnika k bodu contactPointSphereToTriangle
			if( !PointInsidePolygon( contactPointSphereToPlane, collision[i].a, collision[i].b, collision[i].c) )
			{
				// najdeme najblizsi bod k contactPointSphereToPlane na hranach trojuholnika
				// z tychto vyberieme najblizi k contactPointSphereToPlane
				vec closest_ab = ClosestPointOnLine( collision[i].a, collision[i].b, contactPointSphereToPlane);
				vec closest_bc = ClosestPointOnLine( collision[i].b, collision[i].c, contactPointSphereToPlane);
				vec closest_ca = ClosestPointOnLine( collision[i].c, collision[i].a, contactPointSphereToPlane);
				
				float dist_ab = Distance2( closest_ab, contactPointSphereToPlane);
				float dist_bc = Distance2( closest_bc, contactPointSphereToPlane);
				float dist_ca = Distance2( closest_ca, contactPointSphereToPlane);
				
				if( dist_ab<dist_bc)
				{
					if(dist_ab<dist_ca)
						contactPointSphereToTriangle = closest_ab;
					else
						contactPointSphereToTriangle = closest_ca;
				}
				else
				{
					if(dist_bc<dist_ca)
						contactPointSphereToTriangle = closest_bc;
					else
						contactPointSphereToTriangle = closest_ca;
				}

				// kedze kolidujuci bod na trojuholniku je iny ako kolidujuci bod na rovine
				// zmeni sa aj kolidujuci bod na guli - ClosestPointOnSphere
				// vypocitame ho ako priesecnik gule a priamky z bodu contactPointSphereToTriangle
				// v smere pohybu t.j. z vpold do vp
				double t1,t2;

				if( LineSphereIntersectionDir( contactPointSphereToTriangle, vpold_vp, center, radius, &t1, &t2) )
				{
					if( t1<=0 && t2<0)
					{
						// gula je pred trojuholnikom
						// berieme bod s t1, lebo ten je blizsie k stene (t1>t2)
						if( t1<-1.f)continue;		// tento trojuholnik nas nezaujima lebo nekoliduje po cely tento krok
						ClosestPointOnSphere = t1*vpold_vp+contactPointSphereToTriangle;

						// mozeme sa pohnut iba tolko pokial sa colidujuci bod na guli nedotkne 
						// kolidujuceho bodu na trojuholniku
						vp_move = contactPointSphereToTriangle - ClosestPointOnSphere;
					}
					else if( t1>0 && t2<0)
					{
						// gula je v stene, vratime ju von zo steny
						// berieme bod, ktory je blizsie k rovine
						vec t1point = t1*vpold_vp+contactPointSphereToTriangle;
						vec t2point = t2*vpold_vp+contactPointSphereToTriangle;

					/*	if(PlanePointDistance( normal, distance, t1point)<=PlanePointDistance( normal, distance, t2point) )
							ClosestPointOnSphere = t1point;
						else 
							ClosestPointOnSphere = t2point;
					*/
						if( ABS(t1) < ABS(t2) )
							ClosestPointOnSphere = t1point;
						else
							ClosestPointOnSphere = t2point;
					
						// mozeme sa pohnut iba tolko pokial sa colidujuci bod na guli nedotkne 
						// kolidujuceho bodu na trojuholniku
						vp_move = contactPointSphereToTriangle - ClosestPointOnSphere;
					}
					else // if( t1>0 && t2>0)
					{
						// gula je za trojuholnikom, gula nekoliduje s trojuholnikom v tomto smere pohybu
						continue;
					}
				}
				else
				{
					// nie je priesecnik, gula je mimo trojuholnika
					continue;
				}
			}
			else
			{
				// bod je vnutri trojuholnika
				contactPointSphereToTriangle = contactPointSphereToPlane;
				
				// mozeme sa pohnut iba tolko pokial sa colidujuci bod na guli nedotkne 
				// kolidujuceho bodu na trojuholniku
				vp_move = contactPointSphereToTriangle - ClosestPointOnSphere;
			}

			// zistime vzdialenost kontaktneho bodu na trojuholniku ku stredu gule
			float dist = Distance2(contactPointSphereToTriangle,center);
			if(dist<radius2)		// ak je mensi ako polomer, gula je v kolizii z polygonom
			{
				if(dist<distanceCenterPointOnTriangle)	// ak vzdialenost je mensia ako ineho bodu v kolizii, nahradime ho
				{
					distanceCenterPointOnTriangle=dist;
					newmove = vp_move;
					newClosestPointOnSphere = ClosestPointOnSphere;
					newContactPointSphereToTriangle = contactPointSphereToTriangle;
				}
			}
			else
			{
				if(distanceCenterPointOnTriangle>5.e+14f)	// nenasiel sa ziaden bod vnutri gule
				{
					if( vp_move.Length2() < newmove.Length2() )		// berieme kratsi
					{
						newmove = vp_move;
						newClosestPointOnSphere = ClosestPointOnSphere;
						newContactPointSphereToTriangle = contactPointSphereToTriangle;
					}
				}
			}
			smykanie=1;
		}

		if(smykanie)
		{
			vec normal=vpold-newClosestPointOnSphere;
			float distance = PlaneDistance( normal, newContactPointSphereToTriangle);
			vec delta = LinePlaneIntersectionDir( newClosestPointOnSphere+vpold_vp, normal, normal, distance)-newContactPointSphereToTriangle;
		//	vec	newvp = newClosestPointOnSphere+vpold_vp;
		//	float distancepoint = PlanePointDelta( normal, distance, newvp);
		//	vec intersec = -distancepoint*normal+newvp;
		//	vec delta = intersec-newContactPointSphereToTriangle;

			// taky klzavy pohyb, ktory ide proti povodnemu pohybu zamietneme
			if( DOT3(vpold_vp_povodny, delta) < 0)delta.clear();

			vpold += newmove;				// posunieme sa po najblizi kolidujuci bod
			vpold += 0.000001f*normal;
			vp = vpold + delta;				// cielovy bod posunieme o deltu klzanim
			vpold_vp = vp-vpold;			// novy vektor pohybu
			newmove = vpold_vp;
			iter++;
		}
		else
		{
			vpold += newmove;
			vpold_vp.clear();
			iter=1000;
		}
	}
	while( (vpold_vp.Length2()>1.e-8f)&&(iter<10) );
	return vpold;
}

int CheckCollisionGround( vector<z_face> &collision, vec center, float radius, float angle, float mindist)
{
	float ground_skew = (float)cos(angle*PI180)*radius;
	vec vpold_vp = mindist*vec(0,-1,0);

	for( int i=0; i<collision.size(); i++)
	{
		vec normal = collision[i].normal;
		float distance = PlaneDistance( normal, collision[i].a);		// D = - (Ax+By+Cz)
		// ---------------------------------------------------------
		// najdeme kolidujuci bod na guli
		vec ClosestPointOnSphere;		// najblizsi bod gule k rovine aktualneho trojuholnika
		// najdeme ho ako priesecnik priamky prechadzajucej stredom gule a smerovym vektorom = normalovemu vektoru roviny (trojuholnika)
		// vypocitame ho, ale jednodusie tak, ze pripocitame (opocitame) k stredu vektor normala*radius
		
		if( PlanePointDelta( normal, distance, center) > 0 )
		{
			// center je na strane normaly, blizsi bod je v opacnom smere ako smer normaly
			ClosestPointOnSphere = -radius*normal+center;
		}
		else
		{
			// center je na opacnej strane ako normala, blizsi bod je v smere normaly
			ClosestPointOnSphere = radius*normal+center;
		}

		// ---------------------------------------------------------
		// najdeme kolidujuci bod na trojuholniku
		// najprv najdeme kolidujuci bod vzhladom na rovinu v ktorej lezi trojuholnik
		vec contactPointSphereToPlane;			// kolidujuci bod na rovine trojuholnika s gulou
		float distanceCenterToPlane;			// vzdialenost stredu gule k rovine
		// zistime ci rovina pretina gulu
		if( SpherePlaneCollision( center, 0.9999f*radius, normal, distance, &distanceCenterToPlane)==1 )
		{
			// gula pretina rovinu
			// kolidujuci bod je bod na rovine najblizsi k stredu gule
			// je vzdialeny od roviny na distanceCenterToPlane, pretoze pocitame bod na rovine pouzijeme -
			contactPointSphereToPlane = center-distanceCenterToPlane*normal;
		}
		else
		{
			// nie sme v kolizii z gulov, ak sa pohybujeme v smere od roviny, nemoze nastat ziadna kolizia
			// ak sa pohybujeme v smere kolmom na normalovy vektor roviny, tak isto kolizia nehrozi
			// kvoli nepresnosti vypoctov umoznime pohyb aj ked velmi malou castou smeruje do roviny
		//	if( DOT3( vpold_vp, center-ClosestPointOnSphere) >= 0)
			if( DOT3( vpold_vp, center-ClosestPointOnSphere) > -0.000001f)
			{
				continue;
			}
			// gula nepretina rovinu
			// kolidujuci bod je priesecnik roviny a priamky vedenej z bodu ClosestPointOnSphere
			// v smere pohybu t.j. z vpold do vp

			float t = LinePlaneIntersectionDirParameter( ClosestPointOnSphere, vpold_vp, normal, distance);
			// t > 1.f, priesecnik z rovinou je dalej ako vpold_vp od bodu ClosestPointOnSphere
			if(t>1.f)
				continue;	// za cely krok vpold_vp sa s tymto trojuholnikom nestretneme
			else if( t<-radius/vpold_vp.Length())		// priesecnik je za gulou, v smere pohybu tuto rovinu nestretneme
				continue;
			else 
				contactPointSphereToPlane = ClosestPointOnSphere+t*vpold_vp;
		}
		// najdeme kolidujuci bod na trojuholniku
		vec contactPointSphereToTriangle;
		// ak sa bod contactPointSphereToPlane nenachadza v trojuholniku 
		// najdeme najblizsi bod trojuholnika k bodu contactPointSphereToTriangle
		if( !PointInsidePolygon( contactPointSphereToPlane, collision[i].a, collision[i].b, collision[i].c) )
		{
			// najdeme najblizsi bod k contactPointSphereToPlane na hranach trojuholnika
			// z tychto vyberieme najblizi k contactPointSphereToPlane
			vec closest_ab = ClosestPointOnLine( collision[i].a, collision[i].b, contactPointSphereToPlane);
			vec closest_bc = ClosestPointOnLine( collision[i].b, collision[i].c, contactPointSphereToPlane);
			vec closest_ca = ClosestPointOnLine( collision[i].c, collision[i].a, contactPointSphereToPlane);
			
			float dist_ab = Distance2( closest_ab, contactPointSphereToPlane);
			float dist_bc = Distance2( closest_bc, contactPointSphereToPlane);
			float dist_ca = Distance2( closest_ca, contactPointSphereToPlane);
			
			if( dist_ab<dist_bc)
			{
				if(dist_ab<dist_ca)
					contactPointSphereToTriangle = closest_ab;
				else
					contactPointSphereToTriangle = closest_ca;
			}
			else
			{
				if(dist_bc<dist_ca)
					contactPointSphereToTriangle = closest_bc;
				else
					contactPointSphereToTriangle = closest_ca;
			}

			// kedze kolidujuci bod na trojuholniku je iny ako kolidujuci bod na rovine
			// zmeni sa aj kolidujuci bod na guli - ClosestPointOnSphere
			// vypocitame ho ako priesecnik gule a priamky z bodu contactPointSphereToTriangle
			// v smere pohybu t.j. z vpold do vp
			double t1,t2;

			if( LineSphereIntersectionDir( contactPointSphereToTriangle, vpold_vp, center, radius, &t1, &t2) )
			{
				if( t1<=0 && t2<0)
				{
					// gula je pred trojuholnikom
					// berieme bod s t1, lebo ten je blizsie k stene (t1>t2)
					if( t1<-1.f)continue;		// tento trojuholnik nas nezaujima lebo nekoliduje po cely tento krok
					ClosestPointOnSphere = t1*vpold_vp+contactPointSphereToTriangle;
					if( (center.y-ClosestPointOnSphere.y)>ground_skew )return 1;
					else continue;
					// mozeme sa pohnut iba tolko pokial sa colidujuci bod na guli nedotkne 
					// kolidujuceho bodu na trojuholniku
				//	vp_move = contactPointSphereToTriangle - ClosestPointOnSphere;
				}
				else if( t1>0 && t2<0)
				{
					// gula je v stene, vratime ju von zo steny
					// berieme bod, ktory je blizsie k rovine
					vec t1point = t1*vpold_vp+contactPointSphereToTriangle;
					vec t2point = t2*vpold_vp+contactPointSphereToTriangle;

				/*	if(PlanePointDistance( normal, distance, t1point)<=PlanePointDistance( normal, distance, t2point) )
						ClosestPointOnSphere = t1point;
					else 
						ClosestPointOnSphere = t2point;
				*/
					if( ABS(t1) < ABS(t2) )
						ClosestPointOnSphere = t1point;
					else
						ClosestPointOnSphere = t2point;
				
					if( (center.y-ClosestPointOnSphere.y)>ground_skew )return 1;
					else continue;
					// mozeme sa pohnut iba tolko pokial sa colidujuci bod na guli nedotkne 
					// kolidujuceho bodu na trojuholniku
				//	vp_move = contactPointSphereToTriangle - ClosestPointOnSphere;
				}
				else // if( t1>0 && t2>0)
				{
					// gula je za trojuholnikom, gula nekoliduje s trojuholnikom v tomto smere pohybu
					continue;
				}
			}
			else
			{
				// nie je priesecnik, gula je mimo trojuholnika
				continue;
			}
		}
		else
		{
			if( (center.y-ClosestPointOnSphere.y)>ground_skew )return 1;
			else continue;

			// bod je vnutri trojuholnika
		//	contactPointSphereToTriangle = contactPointSphereToPlane;
			
			// mozeme sa pohnut iba tolko pokial sa colidujuci bod na guli nedotkne 
			// kolidujuceho bodu na trojuholniku
		//	vp_move = contactPointSphereToTriangle - ClosestPointOnSphere;
		}
/*			if( LineSphereIntersectionDir( contactPointSphereToTriangle, vpold_vp, center, radius, &t1, &t2) )
			{
				if( t1<=0 && t2<0)
				{
					// gula je pred trojuholnikom
					// berieme bod s t1, lebo ten je blizsie k stene (t1>t2)
					if( t1<-1.f)continue;		// tento trojuholnik nas nezaujima lebo nekoliduje po cely tento krok
					return 1;
				}
				else if( t1>0 && t2<0)
				{
					return 1;		// gula je v stene
				}
				else // if( t1>0 && t2>0)
				{
					// gula je za trojuholnikom, gula nekoliduje s trojuholnikom v tomto smere pohybu
					continue;
				}
			}
			else
			{
				// nie je priesecnik, gula je mimo trojuholnika
				continue;
			}
		}
		else
		{
			return 1;		// bod je vnutri trojuholnika
		}*/
	}
	return 0;
}

uStatistic stat;

HRESULT pd__DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	stat.DrawPrimitive_count++;
	stat.triangle_count += PrimitiveCount;
	switch(PrimitiveType)
	{
	case D3DPT_POINTLIST: stat.vertex_count += PrimitiveCount; break;
	case D3DPT_LINELIST: stat.vertex_count += 2*PrimitiveCount; break;
	case D3DPT_LINESTRIP: stat.vertex_count += PrimitiveCount+1; break;
	case D3DPT_TRIANGLELIST: stat.vertex_count += 3*PrimitiveCount; break;
	case D3DPT_TRIANGLESTRIP: stat.vertex_count += PrimitiveCount+2; break;
	case D3DPT_TRIANGLEFAN: stat.vertex_count += PrimitiveCount+2; break;
	}
	return pd->DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount);
//	return D3D_OK;
}
