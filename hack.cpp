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

#include "stdafx.h"

/*
	TRAINER
*/

trainer::trainer()
{
	m_keyTmr = clock();
}

trainer::~trainer(){}
void	trainer::checkKeys(){}

bool	trainer::checkKeyState(int key)
{
	if
	(
		clock() - m_keyTmr > 100	&&
		(GetAsyncKeyState(key) & 0x8001) == 0x8001
	)
	{
		m_keyTmr = clock();
		return true;
	}
	return false;
}


/*
	HACK
*/


hack::hack(){}

hack::~hack(){}

void hack::checkKeys()
{
	if(checkKeyState(g_pSettings->m_iKeys[keyExit]))
	{
		g_bKillHack = true;
		killProgram();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenu]))
	{
		g_pSettings->toggleMenu();
		return;
	}

	//hotkeys
	if(checkKeyState(g_pSettings->m_iKeys[keyHotTeleport]))
	{
		g_pHack->teleportWaypoint();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyHotWanted]))
	{
		g_pHack->notWanted();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyHotHealth]))
	{
		g_pHack->restoreHealth();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyHotAmmo]))
	{
		g_pHack->fillAmmo();
		return;
	}

	//if menu is not active, no need to check other keys
	if(!g_pSettings->isMenuActive())
		return;

	//menu navigation
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuDown]))
	{
		g_pSettings->menuDown();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuUp]))
	{
		g_pSettings->menuUp();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuRight]))
	{
		g_pSettings->menuRight();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuLeft]))
	{
		g_pSettings->menuLeft();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuTabNext]))
	{
		g_pSettings->menuTabRight();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuTabPrev]))
	{
		g_pSettings->menuTabLeft();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuSave]))
	{
		featTeleport* tp	= dynamic_cast<featTeleport*>(g_pSettings->getFeatureCur(g_pSettings->getActiveFeature()));
		if(tp == nullptr || tp->m_tpType != tp_saved)
			return;
		m_player.getPos();
		tp->m_v3Pos.x = m_player.m_v3Pos.x;
		tp->m_v3Pos.y = m_player.m_v3Pos.y;
		g_pSettings->m_iniParser.setValue<float>(tp->m_szIniKey + "_x", m_player.m_v3Pos.x, "Teleport");
		g_pSettings->m_iniParser.setValue<float>(tp->m_szIniKey + "_y", m_player.m_v3Pos.y, "Teleport");
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuSelect]))
	{
		g_pSettings->menuSelect();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuBack]))
	{
		g_pSettings->menuBack();
		return;
	}
}

BYTE	hack::initPointers()
{
	BYTE r	= 0;

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_hModule + ADDRESS_WORLD, &m_dwpWorldBase);
	if(m_dwpWorldBase == 0)
		return INITPTR_INVALID_WORLD;

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpWorldBase + OFFSET_PLAYER, &m_dwpPlayerBase);
	if(m_dwpPlayerBase == 0)
		return INITPTR_INVALID_PLAYER;
	m_player.m_dwpBase		= m_dwpPlayerBase;
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_ENTITY_POSBASE, &m_player.m_dwpPosBase);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_PLAYER_INFO, &m_player.m_dwpPlayerInfo);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_ENTITY_ATTACKER, &m_dwpAttackerBase);

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_PLAYER_VEHICLE, &m_dwpVehicleBase);
	if(m_dwpVehicleBase == 0)
		r |=	INITPTR_INVALID_VEHICLE;
	else
	{
		m_vehicle.m_dwpBase		= m_dwpVehicleBase;
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpVehicleBase + OFFSET_ENTITY_POSBASE, &m_vehicle.m_dwpPosBase);
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpVehicleBase + OFFSET_VEHICLE_HANDLING, &m_vehicle.m_handlingCur.m_dwpHandling);
	}

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_WEAPON_MANAGER, &m_dwpWeaponManager);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpWeaponManager + OFFSET_WEAPON_CURRENT, &m_dwpWeaponCur);
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0)
		r |=	INITPTR_INVALID_WEAPON;
	else
	{
		m_weapon.m_weapDataCur.m_dwpWeapon = m_dwpWeaponCur;
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpWeaponCur + OFFSET_WEAPON_AMMOINFO, &m_dwpAmmoInfo);
		m_weapon.m_dwpAmmoInfo			= m_dwpAmmoInfo;
	}

	return r;
}

void	hack::getWaypoint()
{
	g_pMemMan->readMem<v2>((DWORD_PTR) m_hModule + ADDRESS_WAYPOINT, &m_v2Waypoint);
	return;
}

void	hack::getObjective()
{
	g_pMemMan->readMem<v3>((DWORD_PTR) m_hModule + ADDRESS_OBJECTIVE, &m_v3Objective);
	return;
}

void	hack::teleport(v3 v)
{
	m_player.getInVehicle();
	if(m_player.m_bIsInVehicle)
	{
		m_vehicle.setPos(v);
		return;
	}
	m_player.setPos(v);
	return;
}

bool	hack::teleportWaypoint()
{
	getWaypoint();
	if(m_v2Waypoint.x == 64000.f && m_v2Waypoint.y == 64000.f)
		return false;

	v3 dest(m_v2Waypoint.x, m_v2Waypoint.y, -225.f);
	teleport(dest);
	return true;
}


bool	hack::teleportObjective()
{
	getObjective();
	if(m_v3Objective.x == 64000.f && m_v3Objective.y == 64000.f && m_v3Objective.z == 64000.f)
		return false;

	v3 dest(m_v3Objective.x, m_v3Objective.y, m_v3Objective.z + 1.f);
	teleport(dest);
	return true;
}

void	hack::restoreHealth()
{
	m_player.getHealth();
	if(m_player.m_flArmor < 50.f || m_player.m_cmHp.cur < m_player.m_cmHp.max)
		m_player.setHealth(m_player.m_cmHp.max, 50.f);
	return;
}

void	hack::restoreVehicleHealth()
{
	m_vehicle.getHealth();
	if((m_vehicle.m_cmHp.cur < m_vehicle.m_cmHp.max && m_vehicle.m_cmHp.cur > 0.f) ||
		(m_vehicle.m_cmHpVehicle.cur < m_vehicle.m_cmHpVehicle.max && m_vehicle.m_cmHpVehicle.cur > 0.f))
		m_vehicle.setHealth(m_vehicle.m_cmHp.max);
	return;
}

void	hack::restoreStamina()
{
	m_player.getStamina();
	if(m_player.m_cmStamina.cur < m_player.m_cmStamina.max)
		m_player.setStamina(m_player.m_cmStamina.max);
	return;
}

void	hack::notWanted()
{
	m_player.getWanted();
	if(m_player.m_dwWanted == 0)
		return;
	m_player.setWanted(0);
	return;
}

void	hack::killNpc()
{
	/*if(m_dwpAttackerBase == 0)
		return;
	for(int i = 0; i < 3; i++)
	{
		DWORD_PTR	npc;
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpAttackerBase + (i * OFFSET_ATTACKER_DISTANCE), &npc);
		if(npc == m_dwpPlayerBase || npc == 0)
			continue;
		float		health;
		g_pMemMan->readMem<float>((DWORD_PTR) npc + OFFSET_ENTITY_HEALTH, &health);
		float v	= 0.f;
		if(health > v)
			g_pMemMan->writeMem<float>((DWORD_PTR) npc + OFFSET_ENTITY_HEALTH, &v);
	}*/
	return;
}

void	hack::fillAmmo()
{
	if(!m_weapon.findAmmoBase())
		return;
	m_weapon.getCurAmmo();
	m_weapon.getMaxAmmo();
	if(m_weapon.m_dwCurAmmo != m_weapon.m_dwMaxAmmo)
		m_weapon.setCurAmmo(m_weapon.m_dwMaxAmmo);
	return;
}

void	hack::noSpread(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_fSpread != m_weapon.m_weapDataRestore.m_fSpread)
				m_weapon.setSpread(m_weapon.m_weapDataRestore.m_fSpread);
			feature->m_bRestored = true;
		}
		return;
	}
	if(m_weapon.m_weapDataCur.m_fSpread != 0)
		m_weapon.setSpread(0);
	return;
}

void	hack::noRecoil(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_fRecoil != m_weapon.m_weapDataRestore.m_fRecoil)
				m_weapon.setRecoil(m_weapon.m_weapDataRestore.m_fRecoil);
			feature->m_bRestored = true;
		}
		return;
	}
	if(m_weapon.m_weapDataCur.m_fRecoil != 0)
		m_weapon.setRecoil(0);
	return;
}

void	hack::quickReload(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_fReload != m_weapon.m_weapDataRestore.m_fReload || m_weapon.m_weapDataCur.m_fReloadVeh != m_weapon.m_weapDataRestore.m_fReloadVeh)
			{
				m_weapon.setReloadSpeed(m_weapon.m_weapDataRestore.m_fReload);
				m_weapon.setReloadVehicle(m_weapon.m_weapDataRestore.m_fReloadVeh);
			}
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_weapon.m_weapDataRestore.m_fReload * static_cast<featSlider*>(feature)->m_fValue;
	if(m_weapon.m_weapDataCur.m_fReload != fValue)
		m_weapon.setReloadSpeed(fValue);
	if(m_weapon.m_weapDataCur.m_fReloadVeh != 0)
		m_weapon.setReloadVehicle(0);
	return;
}

void	hack::bulletDamage(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_fDamage != m_weapon.m_weapDataRestore.m_fDamage)
				m_weapon.setBulletDamage(m_weapon.m_weapDataRestore.m_fDamage);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_weapon.m_weapDataRestore.m_fDamage * static_cast<featSlider*>(feature)->m_fValue;
	if(m_weapon.m_weapDataCur.m_fDamage != fValue)
		m_weapon.setBulletDamage(fValue);
	return;
}

void	hack::weaponRange(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_fRange != m_weapon.m_weapDataRestore.m_fRange)
				m_weapon.setRange(m_weapon.m_weapDataRestore.m_fRange);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_weapon.m_weapDataRestore.m_fRange * static_cast<featSlider*>(feature)->m_fValue;
	if(m_weapon.m_weapDataCur.m_fRange != fValue)
		m_weapon.setRange(fValue);
	return;
}

void	hack::weaponSpin(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_fSpinUp != m_weapon.m_weapDataRestore.m_fSpinUp || m_weapon.m_weapDataCur.m_fSpin != m_weapon.m_weapDataRestore.m_fSpin)
			{
				m_weapon.setSpinUp(m_weapon.m_weapDataRestore.m_fSpinUp);
				m_weapon.setSpin(m_weapon.m_weapDataRestore.m_fSpin);
			}
			feature->m_bRestored = true;
		}
		return;
	}
	if(m_weapon.m_weapDataCur.m_fSpinUp != 0 || m_weapon.m_weapDataCur.m_fSpin != 0)
	{
		m_weapon.setSpin(0);
		m_weapon.setSpinUp(0);
	}
	return;
}

void	hack::runSpeed(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_player.getRunSpeed();
			if(m_player.m_flRunSpd > 1.f)
				m_player.setRunSpeed(1.f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getRunSpeed();
	float fValue	= static_cast<featSlider*>(feature)->m_fValue;
	if(m_player.m_flRunSpd != fValue)
		m_player.setRunSpeed(fValue);
	return;
}

void	hack::swimSpeed(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_player.getSwimSpeed();
			if(m_player.m_flSwimSpd > 1.f)
				m_player.setSwimSpeed(1.f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getSwimSpeed();
	float fValue	= static_cast<featSlider*>(feature)->m_fValue;
	if(m_player.m_flSwimSpd != fValue)
		m_player.setSwimSpeed(fValue);
	return;
}

void	hack::godMode(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_player.getGod();
			if(m_player.m_btGod > 0)
				m_player.setGod(0);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getGod();
	if(m_player.m_btGod < 1)
		m_player.setGod(1);
	return;
}

void	hack::frameFlags(feat* featSuperJump, feat* featExplosiveMelee, feat* featFireAmmo, feat* featExplosiveAmmo)
{
	DWORD dwValue	= 0;
	m_player.getFrameFlags();
	if(featSuperJump->m_bOn)
		dwValue		|= 1 << 64;
	if(featExplosiveMelee->m_bOn)
		dwValue		|= 1 << 32;
	if(featFireAmmo->m_bOn)
		dwValue		|= 1 << 16;
	if(featExplosiveAmmo->m_bOn)
		dwValue		|= 1 << 8;
	if(m_player.m_dwFrameFlags != dwValue)
		m_player.setFrameFlags(dwValue);
	return;
}

void	hack::vehicleGod(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_vehicle.getGod();
			if(m_vehicle.m_btGod > 0)
				m_vehicle.setGod(0);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getGod();
	if(m_vehicle.m_btGod < 1)
		m_vehicle.setGod(1);
	return;
}

void	hack::infAmmo(feat* feature)
{
	BYTE	cur[3]		= {};
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			g_pMemMan->readMem<BYTE>((DWORD_PTR) m_hModule + ADDRESS_AMMO, cur, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
			BYTE	value[3]	= {0x41, 0x2B, 0xD1};
			if(cur[0] != value[0])
				g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_hModule + ADDRESS_AMMO, value, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
			feature->m_bRestored = true;
		}
		return;
	}
	g_pMemMan->readMem<BYTE>((DWORD_PTR) m_hModule + ADDRESS_AMMO, cur, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
	BYTE	value[3]	= {0x90, 0x90, 0x90};
	if(cur[0] != value[0])
		g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_hModule + ADDRESS_AMMO, value, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
	return;
}

void	hack::noReload(feat* feature)
{
	BYTE	cur[3]		= {};
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			g_pMemMan->readMem<BYTE>((DWORD_PTR) m_hModule + ADDRESS_MAGAZINE, cur, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
			BYTE	value[3]	= {0x41, 0x2B, 0xC9};
			if(cur[0] != value[0])
				g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_hModule + ADDRESS_MAGAZINE, value, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
			feature->m_bRestored = true;
		}
		return;
	}
	g_pMemMan->readMem<BYTE>((DWORD_PTR) m_hModule + ADDRESS_MAGAZINE, cur, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
	BYTE	value[3]	= {0x90, 0x90, 0x90};
	if(cur[0] != value[0])
		g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_hModule + ADDRESS_MAGAZINE, value, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
	return;
}

void	hack::seatbelt(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_player.getSeatbelt();
			if(m_player.m_btSeatbelt & 0x01)
				m_player.setSeatbelt(m_player.m_btSeatbelt ^ 0x01);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getSeatbelt();
	if(!(m_player.m_btSeatbelt & 0x01))
			m_player.setSeatbelt(m_player.m_btSeatbelt | 0x01);
	return;
}

void hack::noRagdoll(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_player.getRagdoll();
			if(!(m_player.m_btRagdoll & 0x20))
				m_player.setRagdoll(m_player.m_btRagdoll | 0x20);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getRagdoll();
	if(m_player.m_btRagdoll & 0x20)
		m_player.setRagdoll(m_player.m_btRagdoll ^ 0x20);
	return;
}

void hack::vehicleAccel(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_vehicle.m_handlingCur.m_fAcceleration != m_vehicle.m_handlingRestore.m_fAcceleration)
				m_vehicle.setAcceleration(m_vehicle.m_handlingRestore.m_fAcceleration);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_vehicle.m_handlingRestore.m_fAcceleration * static_cast<featSlider*>(feature)->m_fValue;
	if(m_vehicle.m_handlingCur.m_fAcceleration != fValue)
		m_vehicle.setAcceleration(fValue);
	return;
}

void hack::vehicleBrake(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_vehicle.m_handlingCur.m_fBrakeForce != m_vehicle.m_handlingRestore.m_fBrakeForce)
				m_vehicle.setBrakeForce(m_vehicle.m_handlingRestore.m_fBrakeForce);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_vehicle.m_handlingRestore.m_fBrakeForce * static_cast<featSlider*>(feature)->m_fValue;
	if(m_vehicle.m_handlingCur.m_fBrakeForce != fValue)
		m_vehicle.setBrakeForce(fValue);
	return;
}

void hack::neverWanted(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_player.getWantedCanChange();
			if(m_player.m_flWantedCanChange != 1.f)
				m_player.setWantedCanChange(1.f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getWantedCanChange();
	if(m_player.m_flWantedCanChange != 0.f)
		m_player.setWantedCanChange(0.f);
	return;
}

void hack::vehicleTraction(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_vehicle.m_handlingCur.m_fTractionCurveMin != m_vehicle.m_handlingRestore.m_fTractionCurveMin)
				m_vehicle.setTractionCurveMin(m_vehicle.m_handlingRestore.m_fTractionCurveMin);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_vehicle.m_handlingRestore.m_fTractionCurveMin * static_cast<featSlider*>(feature)->m_fValue;
	if(m_vehicle.m_handlingCur.m_fTractionCurveMin != fValue)
		m_vehicle.setTractionCurveMin(fValue);
	return;
}

void hack::vehicleGravity(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_vehicle.getGravity();
			if(m_vehicle.m_fGravity != 9.8f)
				m_vehicle.setGravity(9.8f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getGravity();
	float fValue	= static_cast<featSlider*>(feature)->m_fValue;
	if(m_vehicle.m_fGravity != fValue)
		m_vehicle.setGravity(fValue);
	return;
}

void hack::vehicleBulletproofTires(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_vehicle.getBulletproofTires();
			if(m_vehicle.m_btBulletproofTires & 0x20)
				m_vehicle.setBulletproofTires(m_vehicle.m_btBulletproofTires ^ 0x20);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getBulletproofTires();
	if(!(m_vehicle.m_btBulletproofTires & 0x20))
		m_vehicle.setBulletproofTires(m_vehicle.m_btBulletproofTires | 0x20);
	return;
}

void hack::wanted(feat* feature)
{
	if(!feature->m_bOn)
		return;
	m_player.getWanted();
	float fValue	= static_cast<featSlider*>(feature)->m_fValue;
	if(m_player.m_dwWanted != (DWORD) fValue)
		m_player.setWanted((DWORD) fValue);
	return;
}

void	hack::bulletBatch(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_dwBulletBatch != m_weapon.m_weapDataRestore.m_dwBulletBatch)
				m_weapon.setBulletBatch(m_weapon.m_weapDataRestore.m_dwBulletBatch);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_weapon.m_weapDataRestore.m_dwBulletBatch * static_cast<featSlider*>(feature)->m_fValue;
	fValue	= (fValue > 25.f) ? 25.f : fValue;
	if(m_weapon.m_weapDataCur.m_dwBulletBatch != fValue)
		m_weapon.setBulletBatch((DWORD) fValue);
	return;
}

void	hack::muzzleVelocity(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_fMuzzleVelocity != m_weapon.m_weapDataRestore.m_fMuzzleVelocity)
				m_weapon.setMuzzleVelocity(m_weapon.m_weapDataRestore.m_fMuzzleVelocity);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_weapon.m_weapDataRestore.m_fMuzzleVelocity * static_cast<featSlider*>(feature)->m_fValue;
	if(m_weapon.m_weapDataCur.m_fMuzzleVelocity != fValue)
		m_weapon.setMuzzleVelocity(fValue);
	return;
}

void	hack::vehicleDeformation(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_vehicle.m_handlingCur.m_fDeformationDamageMult != m_vehicle.m_handlingRestore.m_fDeformationDamageMult)
				m_vehicle.setDeformationDamageMult(m_vehicle.m_handlingRestore.m_fDeformationDamageMult);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= static_cast<featSlider*>(feature)->m_fValue;
	if(m_vehicle.m_handlingCur.m_fDeformationDamageMult != fValue)
		m_vehicle.setDeformationDamageMult(fValue);
	return;
}

void	hack::vehicleUpShift(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_vehicle.m_handlingCur.m_fUpShift != m_vehicle.m_handlingRestore.m_fUpShift)
				m_vehicle.setUpShift(m_vehicle.m_handlingRestore.m_fUpShift);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= static_cast<featSlider*>(feature)->m_fValue;
	if(m_vehicle.m_handlingCur.m_fUpShift != fValue)
		m_vehicle.setUpShift(fValue);
	return;
}

void	hack::batchSpread(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_weapon.m_weapDataCur.m_fBatchSpread != m_weapon.m_weapDataRestore.m_fBatchSpread)
				m_weapon.setBatchSpread(m_weapon.m_weapDataRestore.m_fBatchSpread);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= static_cast<featSlider*>(feature)->m_fValue;
	if(m_weapon.m_weapDataCur.m_fBatchSpread != fValue)
		m_weapon.setBatchSpread(fValue);
	return;
}

void	hack::vehicleSuspensionForce(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			if(m_vehicle.m_handlingCur.m_fSuspensionForce != m_vehicle.m_handlingRestore.m_fSuspensionForce)
				m_vehicle.setSuspensionForce(m_vehicle.m_handlingRestore.m_fSuspensionForce);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue	= m_vehicle.m_handlingRestore.m_fSuspensionForce * static_cast<featSlider*>(feature)->m_fValue;
	if(m_vehicle.m_handlingCur.m_fSuspensionForce != fValue)
		m_vehicle.setSuspensionForce(fValue);
	return;
}
/*
void hack::vehicleDisableDoors(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_vehicle.getOpenableDoors();
			if(m_vehicle.m_btOpenableDoors[0] != m_vehicle.m_btOpenableDoors[1])
				m_vehicle.setOpenableDoors(m_vehicle.m_btOpenableDoors[1]);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getOpenableDoors();
	if(m_vehicle.m_btOpenableDoors[0] != 0)
	{
		m_vehicle.m_btOpenableDoors[1]	= m_vehicle.m_btOpenableDoors[0];
		if(m_vehicle.m_btOpenableDoors[0] > 1)		//crash protection
			m_vehicle.setOpenableDoors(0);
	}
	return;
}*/

void hack::vehicleInfAlarm(feat* feature)
{
	if(!feature->m_bOn)
	{
		if(!feature->m_bRestored)
		{
			m_vehicle.getAlarmLength();
			if(m_vehicle.m_dwAlarmLength != 0)
				m_vehicle.setAlarmLength(0);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getAlarmLength();
	if(m_vehicle.m_dwAlarmLength != 0xFF)
		m_vehicle.setAlarmLength(0xFF);
	return;
}