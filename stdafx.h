/*
	Copyright 2016-2017 sub1to

	This file is part of subVersion GTA:O SC External Hack.

    subVersion GTA:O SC External Hack is free software: you can redistribute
	it and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    subVersion GTA:O SC External Hack is distributed in the hope that it
	will be useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
	the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with subVersion GTA:O SC External Hack.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef STDAFX_H
#define STDAFX_H

#include <windows.h>
#include <windowsx.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <psapi.h>
#include <vector>
#include <regex>

#include <Dwmapi.h>
#pragma comment(lib,"Dwmapi.lib")

#include "d3d9/d3d9.h"
#include "d3d9/d3dx9.h"
#pragma comment(lib,"d3d9/d3dx9.lib")
#pragma comment(lib,"d3d9/d3d9.lib")

#include "resource.h"

#include "vector.h"
#include "memManager.h"
#include "D3D9Render.h"
#include "settings.h"
#include "entity.h"
#include "hack.h"


//Addresses from GTAV.exe module
#define ADDRESS_WORLD					0x24e6d90		//42
#define ADDRESS_WAYPOINT				0x1f6ef80
#define ADDRESS_OBJECTIVE				0x1f6ef80
#define ADDRESS_AMMO					0x102F8E9		//Ammo dec code; 41 2B D1 E8; 90 90 90 E8
#define ADDRESS_MAGAZINE				0x102F8A4		//Magazine dec code; 41 2B C9 3B C8 0F; 90 90 90 3B C8 0F
/*
#define ADDRESS_WORLD 0x24E9E50
#define ADDRESS_OBJECTIVE  0x1F73420
#define ADDRESS_WAYPOINT 0x1F73420
#define ADDRESS_AMMO  0x1030A3D
#define ADDRESS_MAGAZINE  0x1030A0D*/

#define OFFSET_PLAYER								        0x08			//playerbase
#define OFFSET_PLAYER_PED							        0x8				//playerbase
#define OFFSET_NET_PLAYER								0x1C8

#define OFFSET_ENTITY_POSBASE								0x30			//base of a structure that contains entity coords
#define	OFFSET_ENTITY_POSBASE_COS							0x20
#define	OFFSET_ENTITY_POSBASE_SIN							0x30
#define OFFSET_ENTITY_POSBASE_POS							0x50			//vector3
#define OFFSET_ENTITY_POS								0x90			//vector3
#define OFFSET_ENTITY_HEALTH								0x0280			//entity health (except for vehicles); float cur, float max		OLD 0x280
#define OFFSET_ENTITY_HEALTH_MAX							0x2A0			//they moved this away from curHealth in 1.36 :(
#define OFFSET_ENTITY_ATTACKER								0x2A8			//base to a list of the last 3 entities that attacked the current entity
#define OFFSET_ENTITY_GOD								0x189			//godmode; on = 1, off = 0; byte


#define OFFSET_PLAYER_ARMOR								0x14E0 			//armour OLD 0x14B0
#define OFFSET_PLAYER_INFO								0x10C8			//playerInfo struct												//#define OFFSET_PLAYER_INFO								0xCD0			// OLD 0x10B8			//playerInfo struct
#define OFFSET_PED_MODEL_INFO								0x0020			// Ped model info
#define OFFSET_PLAYER_ROCKSTAR_ID							0x0070			//	ROCKSTAR ID PLAYER
#define OFFSET_PLAYER_INFO_WANTED_CAN_CHANGE				                0x71C			//fWantedCanChange
#define OFFSET_PLAYER_INFO_WANTED							0x868			//wanted level; DWORD											//#define OFFSET_PLAYER_INFO_WANTED							0x798			//wanted level; DWORD
#define	OFFSET_PLAYER_INFO_RUN_SPD							0xCD0			//run speed; def 1; float		OLD 0xE8 and 0x14C 
#define OFFSET_PLAYER_INFO_SWIM_SPD							0x150			//swim speed; def 1; float		OLD 0x148
#define OFFSET_PLAYER_INFO_FRAMEFLAGS						        0x1F9			//frame flags; DWORD			OLD 0x1F8						//#define OFFSET_PLAYER_INFO_FRAMEFLAGS						0x190			//frame flags; DWORD
#define OFFSET_PLAYER_INFO_STAMINA							0x0CD4			//fStamina, fStaminaMax			OLD 0xC00
#define OFFSET_PLAYER_MAX_STAMINA_REGEN						        0x0CD8			//	MAX STAMINA
#define OFFSET_PLAYER_VEHICLE								0xD30			//ptr to last used vehicle
#define OFFSET_PLAYER_NAME								0x84			//	OLD 0x7C
#define OFFSET_PLAYER_RAGDOLL								0x10B8			//byte; CPed.noRagdoll: 0x20 = off; 0x00/0x01 = on				//#define OFFSET_PLAYER_RAGDOLL								0x10A8			//byte; CPed.noRagdoll: 0x20 = off; 0x00/0x01 = on
#define OFFSET_PLAYER_SEATBELT								0xC00			//byte; CPed.seatBelt: 0xC8 = off; 0xC9 = on					//#define OFFSET_PLAYER_SEATBELT							0x13EC			//byte; CPed.seatBelt: 0xC8 = off; 0xC9 = on
#define OFFSET_PLAYER_INVEHICLE								0x1477																			//#define OFFSET_PLAYER_INVEHICLE								0x146B
#define OFFSET_PLAYER_WANTED								0x868			//	OLD 0x0848
#define OFFSET_PLAYER_STEALTH_WALK							0x016C			//	STEALTH WALK SPEED		OLD 0x168
#define OFFSET_NET_PLAYER_INFO								0xA8
#define OFFSET_PLAYER_INFO_NAME								0x84
#define OFFSET_PLAYER_INFO_NPC_IGNORE						        0x850			//npc ignore; DWORD; everyone = 0x450000;
#define OFFSET_PLAYER_WATER_PROOF							0x188			//water proof; DWORD; +0x1000000 = on
#define OFFSET_PLAYER_VEHICLE_DAMAGE_MP						        0xCFC			//super punck/kick;float;

#define OFFSET_VEHICLE_HEALTH								0x908			//vehicle health; 0.f-1000.f									//#define OFFSET_VEHICLE_HEALTH								0x84C			
#define	OFFSET_VEHICLE_HEALTH2								0x844			//vehicle health2; 0.f-1000.f
#define OFFSET_VEHICLE_HANDLING								0x938																			//#define OFFSET_VEHICLE_HANDLING								0x878
#define OFFSET_VEHICLE_HANDLING_MASS						        0xC				//fMass
#define OFFSET_VEHICLE_HANDLING_BUOYANCY					        0x40			//fBuoyancy

#define OFFSET_VEHICLE_GRAVITY								0xC5C			//fGravity														//#define OFFSET_VEHICLE_GRAVITY								0xB7C			
#define OFFSET_VEHICLE_BULLETPROOF_TIRES					        0x943			//btBulletproofTires; (btBulletproofTires & 0x20) ? true : false//#define OFFSET_VEHICLE_BULLETPROOF_TIRES					0x883			//btBulletproofTires;  (btBulletproofTires & 0x20) ? true : false
#define OFFSET_VEHICLE_HANDLING_SUSPENSION_HEIGH			                0xD0			//fSuspensionHeight
#define OFFSET_VEHICLE_HANDLING_COLISION_DAMAGE_MP			                0xF0			//fColisionDamageMult
#define OFFSET_VEHICLE_HANDLING_WEAPON_DAMAGE_MP			                0xF4			//fWeaponDamageMult
#define OFFSET_VEHICLE_HANDLING_DOWNSHIFT					        0x5C
#define OFFSET_VEHICLE_HANDLING_HANDBRAKEFORCE				                0x7C			//fHandbrakeForce
#define OFFSET_VEHICLE_HANDLING_ENGINE_DAMAGE_MP			                0xFC			//fEngineDamageMult
#define OFFSET_VEHICLE_HANDLING_ACCELERATION				                0x4C
#define OFFSET_VEHICLE_HANDLING_BRAKEFORCE					        0x6C
#define OFFSET_VEHICLE_HANDLING_TRACTION_CURVE_MIN			                0x90			//fTractionCurveMin
#define OFFSET_VEHICLE_HANDLING_DEFORM_MULTIPLIER			                0xF8			//fDeformationDamageMult
#define OFFSET_VEHICLE_HANDLING_UPSHIFT						        0x58
#define OFFSET_VEHICLE_HANDLING_SUSPENSION_FORCE			                0xBC			//fSuspensionForce 
#define OFFSET_VEHICLE_BOOST								0x320			//fBoost
#define OFFSET_VEHICLE_RECHARGE_SPEED						        0x324			//fRocketRechargeSpeed
#define OFFSET_VEHICLE_MISSLES								0x1280			//btVehicleMissles
#define OFFSET_VEHICLE_BOMBS								0x1294			//btAircraftBombs
#define OFFSET_VEHICLE_COUNTERMEASURES						        0x1298			//btAircraftCountermeasures
#define OFFSET_VEHICLE_MK2_MISSLES							0x1284			//btOppressorMK2Misseles
#define OFFSET_VEHICLE_TAMPA_MISSLES						        0x127C			//btTampaMissles
#define	OFFSET_VEHICLE_CUSTOM								0x48
#define	OFFSET_VEHICLE_CUSTOM_EMS							0x3D6			//btEngineManagementSystem; 0x3 = max
#define	OFFSET_VEHICLE_CUSTOM_BRAKES						        0x3D7			//btBrakes; 0x6 = max
#define	OFFSET_VEHICLE_CUSTOM_TRANSMISSION					        0x3D8			//btTransmission; 0x8 = max
#define	OFFSET_VEHICLE_CUSTOM_SUSPENSION					        0x3DA			//btSuspension; 0x1B = max
#define	OFFSET_VEHICLE_CUSTOM_ARMOR							0x3DB			//btArmor; 0x1B = max
#define	OFFSET_VEHICLE_CUSTOM_TURBO_TUNING					        0x3DD			//btTurboTuning; 0x1 = on
#define	OFFSET_VEHICLE_CUSTOM_NEON_LIGHT_R					        0x3A2			//btNeonLightRed
#define	OFFSET_VEHICLE_CUSTOM_NEON_LIGHT_G					        0x3A1			//btNeonLightGreen
#define	OFFSET_VEHICLE_CUSTOM_NEON_LIGHT_B					        0x3A0			//btNeonLightBlue
#define	OFFSET_VEHICLE_CUSTOM_NEON_LIGHTS_L					        0x402			//btNeonLightLeft; 0x0 = off; 0x1 = on
#define	OFFSET_VEHICLE_CUSTOM_NEON_LIGHTS_R					        0x403			//btNeonLightRight; 0x0 = off; 0x1 = on
#define	OFFSET_VEHICLE_CUSTOM_NEON_LIGHTS_F					        0x404			//btNeonLightFront; 0x0 = off; 0x1 = on
#define	OFFSET_VEHICLE_CUSTOM_NEON_LIGHTS_B					        0x405			//btNeonLightBack; 0x0 = off; 0x1 = on
#define	OFFSET_VEHICLE_CUSTOM_TYRE_SMOKE					        0x3DF			//btTyreSmoke; 0x1 = on
#define	OFFSET_VEHICLE_CUSTOM_TYRE_SMOKE_R					        0x3FC			//btTyreSmokeRed
#define	OFFSET_VEHICLE_CUSTOM_TYRE_SMOKE_G					        0x3FD			//btTyreSmokeGreen
#define	OFFSET_VEHICLE_CUSTOM_TYRE_SMOKE_B					        0x3FE			//btTyreSmokeBlue
#define	OFFSET_VEHICLE_CUSTOM_LIMO_WINDOWS					        0x3FF			//btLimoWindows; 0x1 = on


//feature indexing
#define FEATURE_P_GOD				0x00
#define FEATURE_P_NEVERWANTED		0x01
#define FEATURE_P_ANTINPC			0x02
#define FEATURE_V_GOD				0x03
#define FEATURE_W_SPREAD			0x04
#define FEATURE_W_RECOIL			0x05
#define FEATURE_W_RELOAD			0x06
#define FEATURE_W_DAMAGE			0x07
#define FEATURE_W_AMMO				0x08
#define FEATURE_W_RANGE				0x09
#define FEATURE_W_SPINUP			0x0A
#define FEATURE_P_RUNSPD			0x0B
#define FEATURE_P_SWIMSPD			0x0C
#define FEATURE_P_TRUEGOD			0x0D
#define FEATURE_P_SUPERJUMP			0x0E
#define FEATURE_P_EXPLOSIVEMELEE	0x0F
#define FEATURE_W_EXPLOSIVEAMMO		0x10
#define FEATURE_W_FIREAMMO			0x11
#define FEATURE_V_TRUEGOD			0x12
#define FEATURE_P_NORAGDOLL			0x13
#define FEATURE_V_SEATBELT			0x14
#define FEATURE_W_NORELOAD			0x15
#define FEATURE_V_ACCELERATION		0x16
#define FEATURE_V_BRAKEFORCE		0x17
#define FEATURE_V_TRACTION			0x18
#define FEATURE_V_GRAVITY			0x19
#define FEATURE_V_BULLETPROOFTIRES	0x1A
#define FEATURE_P_WANTED			0x1B
#define FEATURE_W_BULLET_BATCH		0x1C
#define FEATURE_W_MUZZLE_VELOCITY	0x1D
#define FEATURE_V_DEFORMATION		0x1E
#define FEATURE_V_UPSHIFT			0x1F
#define FEATURE_W_BATCH_SPREAD		0x20
#define FEATURE_V_SUSPENSION_FORCE	0x21
#define FEATURE_P_STAMINA			0x22
//#define FEATURE_V_DISABLE_DOORS		0x23
#define FEATURE_V_INF_CAR_ALARM		0x24

#define OFFSET_WEAPON_MANAGER								0x10D8			//from playerbase												//#define OFFSET_WEAPON_MANAGER								0x10C8			
#define OFFSET_WEAPON_CURRENT								0x20			//from weapon manager
#define OFFSET_WEAPON_AMMOINFO								0x60			//from weaponbase												//#define OFFSET_WEAPON_AMMOINFO								0x48			//from weaponbase
#define OFFSET_WEAPON_AMMOINFO_MAX							0x28			//ammoinfo
#define OFFSET_WEAPON_AMMOINFO_CUR_1						        0x08			//ptr lvl 1, ptr 1
#define OFFSET_WEAPON_AMMOINFO_CUR_2						        0x00			//ptr tr lvl 2, ptr 1
#define OFFSET_WEAPON_AMMOINFO_CURAMMO						        0x18			//offset to cur ammo
#define OFFSET_WEAPON_AMMOINFO_TYPE							0x0C			//offset to projectile type?
#define OFFSET_WEAPON_SPREAD								0x7C			//float set to 0												//#define OFFSET_WEAPON_SPREAD								0x5C			
#define OFFSET_WEAPON_BULLET_DMG							0xB0			//float times 10 (so when 0, it will stay 0)					//#define OFFSET_WEAPON_BULLET_DMG							0x98			
#define OFFSET_WEAPON_RELOAD_MULTIPLIER						        0x134			//float times 10												//#define OFFSET_WEAPON_RELOAD_MULTIPLIER						0x114			
#define OFFSET_WEAPON_RECOIL								0x2F4			//float set to 0												//#define OFFSET_WEAPON_RECOIL								0x2A4			
#define OFFSET_WEAPON_MODEL_HASH							0x14
#define OFFSET_WEAPON_NAME_HASH								0x10
#define OFFSET_WEAPON_RELOAD_VEHICLE						        0x130																			//#define OFFSET_WEAPON_RELOAD_VEHICLE						0x110
#define OFFSET_WEAPON_RANGE								0x28C																			//#define OFFSET_WEAPON_RANGE									0x25C
#define OFFSET_WEAPON_SPINUP								0x144																			//#define OFFSET_WEAPON_SPINUP								0x124
#define OFFSET_WEAPON_SPIN								0x148																			//#define OFFSET_WEAPON_SPIN									0x128
#define OFFSET_WEAPON_BULLET_BATCH							0x124			//dwBulletInBatch												//#define OFFSET_WEAPON_BULLET_BATCH							0x100			
#define OFFSET_WEAPON_MUZZLE_VELOCITY						        0x11C			//fMuzzleVelocity												//#define OFFSET_WEAPON_MUZZLE_VELOCITY						0xFC			
#define OFFSET_WEAPON_IMPACT_TYPE							0x20			//dwImpactType; 1: Fists,3; Bullets,5: Explosives
#define OFFSET_WEAPON_IMPACT_EXPLOSION						        0x24			//dwImpactExplosion
#define OFFSET_WEAPON_PENETRATION							0x110			//fPenetration
#define OFFSET_WEAPON_FORCE_ON_PED							0xDC			//fForceOnPed
#define OFFSET_WEAPON_FORCE_ON_VEHICLE						        0xE0			//fForceOnVehicle(Bullet Mass)
#define OFFSET_WEAPON_FORCE_ON_HELI							0xE4			//fForceOnHeli
#define OFFSET_WEAPON_BATCH_SPREAD							0x104			//fBatchSpread


void	killProgram	();
DWORD	strToVk(std::string str);

extern HWND		g_hWnd;
extern int		g_iFeature[MAX_MENU_FEATURES];

extern bool		g_bKillSwitch;
extern bool		g_bKillRender;
extern bool		g_bKillAttach;
extern bool		g_bKillHack;

#endif
