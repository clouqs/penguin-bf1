#include "../includes.h"

std::unique_ptr<CFeatures>m_pFeatures = std::make_unique<CFeatures>();

void CFeatures::Initialize()
{
	//hide shoots 
	//ptr: F3 0F 11 81 D4 01 00 00
	//bytes: bf1.exe+1AF0CAB - F3 0F11 81 D4010000  

	/*
		Address of signature = bf1.exe + 0x01AF008B
		"\xF3\x0F\x00\x00\x00\x00\x00\x00\x89\x87\x00\x00\x00\x00\xE9", "xx??????xx????x"
		"F3 0F ? ? ? ? ? ? 89 87 ? ? ? ? E9"
	*/

	this->m_InitializetionStatus = INITIALIZATION_STATUS::FIRERATE_SEARCH;

	if (this->qwWeaponFireRateIntstructionAddress = memory_utils::find_pattern(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\x00\x00\x00\x89\x87\x00\x00\x00\x00\xE9", "xx??????xx????x"))
	{
		this->qwWeaponFirerateSpeed = this->qwWeaponFireRateIntstructionAddress + 0x6;
	}

	if (!this->qwWeaponFireRateIntstructionAddress)
		this->m_InitializetionStatus = INITIALIZATION_STATUS::FIRERATE_NOT_FOUND;

	/*
		Address of signature = bf1.exe + 0x08BEB79F
		"\xF3\x0F\x00\x00\x00\x00\x00\x00\x72\x00\x80\xB9\xA4\x01\x00\x00", "xx??????x?xxxxxx"
		"F3 0F ? ? ? ? ? ? 72 ? 80 B9 A4 01 00 00"
	*/

	this->m_InitializetionStatus = INITIALIZATION_STATUS::OVERHEATINGWEAPON_SEARCH;

	this->qwWeaponOverheatingInstruction = memory_utils::find_pattern(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\x00\x00\x00\x72\x00\x80\xB9\xA4\x01\x00\x00", "xx??????x?xxxxxx");

	if (!this->qwWeaponOverheatingInstruction)
		this->m_InitializetionStatus = INITIALIZATION_STATUS::OVERHEATINGWEAPON_NOT_FOUND;

	/*
		Address of signature = bf1.exe + 0x08BE0F83
		"\x89\x93\x00\x00\x00\x00\x85\xD2\x75", "xx????xxx"
		"89 93 ? ? ? ? 85 D2 75"
	*/

	this->m_InitializetionStatus = INITIALIZATION_STATUS::DECREASBULLET_SEARCH;

	this->qwWeaponDecreaseBullet = memory_utils::find_pattern(memory_utils::get_base(), "\x89\x93\x00\x00\x00\x00\x85\xD2\x75", "xx????xxx");

	if (!this->qwWeaponDecreaseBullet)
		this->m_InitializetionStatus = INITIALIZATION_STATUS::DECREASBULLET_NOT_FOUND;

	/*
		Address of signature = bf1.exe + 0x0724ACA6
		"\xF3\x44\x00\x00\x00\x00\x76\x00\xF3\x44", "xx????x?xx"
		"F3 44 ? ? ? ? 76 ? F3 44"
	*/

	this->m_InitializetionStatus = INITIALIZATION_STATUS::RECOILPITCH_SEARCH;

	this->qwWeaponRecoilPitchAxis = memory_utils::find_pattern(memory_utils::get_base(), "\xF3\x44\x00\x00\x00\x00\x76\x00\xF3\x44", "xx????x?xx");

	if (!this->qwWeaponRecoilPitchAxis)
		this->m_InitializetionStatus = INITIALIZATION_STATUS::RECOILPITCH_NOT_FOUND;

	/*
		Address of signature = bf1.exe + 0x0729FDB2
		"\xF3\x0F\x00\x00\x00\xE8\x00\x00\x00\x00\xF3\x0F\x00\x00\x00\x00\x00\x00\x44\x0F", "xx???x????xx??????xx"
		"F3 0F ? ? ? E8 ? ? ? ? F3 0F ? ? ? ? ? ? 44 0F"
	*/

	this->m_InitializetionStatus = INITIALIZATION_STATUS::RECOILYAW_SEARCH;

	this->qwWeaponRecoilYawAxis = memory_utils::find_pattern(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xE8\x00\x00\x00\x00\xF3\x0F\x00\x00\x00\x00\x00\x00\x44\x0F", "xx???x????xx??????xx");

	if (!this->qwWeaponRecoilYawAxis)
		this->m_InitializetionStatus = INITIALIZATION_STATUS::RECOILYAW_NOT_FOUND;

	/*
		Address of signature = bf1.exe + 0x0729F9BA
		"\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x0F\x2F\x00\x00\x00\x00\x00\x76", "xx???xx???xx???xx??xx?????x"
		"F3 0F ? ? ? F3 0F ? ? ? F3 0F ? ? ? F3 0F ? ? 0F 2F ? ? ? ? ? 76"
	*/

	this->m_InitializetionStatus = INITIALIZATION_STATUS::SPREAD_SEARCH;

	this->qwWeaponSpread = memory_utils::find_pattern(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x0F\x2F\x00\x00\x00\x00\x00\x76", "xx???xx???xx???xx??xx?????x");

	if (!this->qwWeaponSpread)
		this->m_InitializetionStatus = INITIALIZATION_STATUS::SPREAD_NOT_FOUND;

	/*static auto qwAssaultGameClassRecoilInsructionAddress = memory_utils::find_pattern(memory_utils::get_base(), "\xF3\x0F\x58\x56\x6C\xF3", "xxxxxx");

	static auto once = []()
	{
		std::cout << __FUNCTION__ << ": 0x" << std::hex << qwAssaultGameClassRecoilInsructionAddress << std::endl;
		return true;
	}();

	if (bIsEnable)
	{
		memory_utils::patch_instruction(qwAssaultGameClassRecoilInsructionAddress, "\x90\x90\x90\x90\x90", 5);
	}
	else
	{
		memory_utils::patch_instruction(qwAssaultGameClassRecoilInsructionAddress, "\xF3\x0F\x58\x56\x6C", 5);
	}*/

#ifdef _USERDEBUG
		std::cout << __FUNCTION__ << ": 0x" << std::hex << this->qwWeaponFireRateIntstructionAddress << std::endl;
		std::cout << __FUNCTION__ << ": 0x" << std::hex << this->qwWeaponFirerateSpeed << std::endl;
		std::cout << __FUNCTION__ << ": 0x" << std::hex << this->qwWeaponOverheatingInstruction << std::endl;
		std::cout << __FUNCTION__ << ": 0x" << std::hex << this->qwWeaponDecreaseBullet << std::endl;
		std::cout << __FUNCTION__ << ": 0x" << std::hex << this->qwWeaponRecoilPitchAxis << std::endl;
		std::cout << __FUNCTION__ << ": 0x" << std::hex << this->qwWeaponRecoilYawAxis << std::endl;
		std::cout << __FUNCTION__ << ": 0x" << std::hex << this->qwWeaponSpread << std::endl;
#endif

	this->m_InitializetionStatus = INITIALIZATION_STATUS::OK;
}

void CFeatures::Run()
{
	if (vars::global::enable == false)
		return;

	this->m_mtxSyncDataGrabber.lock();

	for (auto it = this->m_PlayersData.begin(); it < this->m_PlayersData.end(); it++) //collect bad ptrs
	{
		auto& data = *it;

		if (!memory_utils::is_valid_ptr((void*)data.m_pPlayer))
			it = m_pFeatures->m_PlayersData.erase(it);
	}

	this->m_mtxSyncDataGrabber.unlock();

	if (this->UpdateLocalPlayer() == false)
	{
		if (vars::visuals::deployment_ESP)
		{
			for (auto& data : this->m_PlayersData)
			{
				auto vec = *(Vector*)((std::uint64_t)data.m_pPlayer + 0x7e0);

				Vector vBot = vec;
				Vector vTop = vec + Vector(0.f, 1.7f, 0.f);

				//FilledAddCircle3D(vBot, vars::visuals::dead_circle_radius, ImColor(1.f, 0.f, 0.f, 1.f));

				m_pDrawing->AddCircle3D(vBot, 10.f, vars::visuals::dead_circle_radius, ImColor(1.f, 0.f, 1.f));
			}
		}
		return;
	}

	this->UpdatePlayers();

	this->Aimbot();

	this->PlayerESP();

	this->DrawScreenshotCounter();

	this->ClearPlayersData();
}

bool CFeatures::UpdateLocalPlayer()
{
	auto* cClientPlayer = GetLocalPlayer();

	if (!IsValidPtr(cClientPlayer))
		return false;

	auto* cClientSoldier = cClientPlayer->clientSoldierEntity;

	if (!IsValidPtr(cClientSoldier))
		return false;

	this->g_Local.m_cszName = cClientPlayer->name;
	this->g_Local.m_IsVisible = !(bool)cClientSoldier->occluded;
	this->g_Local.m_iTeam = cClientPlayer->teamId;
	this->g_Local.m_flYaw = cClientSoldier->authorativeYaw;
	this->g_Local.m_InVehicle = cClientPlayer->InVehicle();
	this->g_Local.m_vOrigin = cClientSoldier->location;
	
	return true;
}

void CFeatures::UpdatePlayers()
{
	for (int i = 0; i <= 64; i++)
	{
		auto* cPlayer = GetPlayerById(i);

		if (!IsValidPtr(cPlayer))
			continue;

		players_data_s player;
		player.m_Index = i;
		player.m_cszName = cPlayer->name;
		player.m_iTeam = cPlayer->teamId;
		player.m_ClientSoldierEntity = cPlayer->clientSoldierEntity;
		player.m_ClientVehicleEntity = cPlayer->clientVehicleEntity;
		this->g_Players.push_back(player);
	}

	for (auto p : this->g_Players)
	{
		auto* cPlayerSoldier = p.m_ClientSoldierEntity;

		if (!IsValidPtr(cPlayerSoldier))
			continue;

		if (cPlayerSoldier->IsDead())
			continue;

		if (cPlayerSoldier->IsValid() == false)
			continue;

		/*if (GetAsyncKeyState('V'))
		{
			if (p.m_iTeam != g_Local.m_iTeam)
			{
				if (cPlayerSoldier->occluded == 0)
					*(BYTE*)(cPlayerSoldier + 0x324) = 0xF1;
				else
					*(BYTE*)(cPlayerSoldier + 0x324) = 0xF0;
				*(DWORD*)(cPlayerSoldier + 0x1A) = 0xA1;
			}
		}*/

		extra_players_data_s player_soldier;
		player_soldier.m_pEntity = (DWORD64)cPlayerSoldier;
		player_soldier.m_Index = p.m_Index;
		player_soldier.m_cszName = p.m_cszName;
		player_soldier.m_iTeam = p.m_iTeam;
		player_soldier.m_IsVisible = !cPlayerSoldier->occluded;
		player_soldier.m_flPlayerHealth = cPlayerSoldier->healthcomponent->m_Health;
		player_soldier.m_flMaxPlayerHealth = cPlayerSoldier->healthcomponent->m_MaxHealth;
		player_soldier.m_InVehicle = false;
		player_soldier.m_vOrigin = cPlayerSoldier->location;
		player_soldier.m_vBoundBoxMax = cPlayerSoldier->GetAABB().max;
		player_soldier.m_vBoundBoxMin = cPlayerSoldier->GetAABB().min;
		Vector vCurrentBone;
		cPlayerSoldier->GetBonePos(UpdatePoseResultData::BONES::BONE_HEAD, vCurrentBone);
		player_soldier.vBoneOrigin[BONE::HEAD] = vCurrentBone;
		cPlayerSoldier->GetBonePos(UpdatePoseResultData::BONES::BONE_NECK, vCurrentBone);
		player_soldier.vBoneOrigin[BONE::NECK] = vCurrentBone;
		cPlayerSoldier->GetBonePos(UpdatePoseResultData::BONES::BONE_SPINE, vCurrentBone);
		player_soldier.vBoneOrigin[BONE::SPINE] = vCurrentBone;
		this->g_ExtraPlayers.push_back(player_soldier);
	}

	for (auto p : this->g_Players)
	{
		auto* cVehicleSoldier = p.m_ClientVehicleEntity;

		if (!IsValidPtr(cVehicleSoldier))
			continue;

		auto* cEntityData = cVehicleSoldier->GetEntityData();

		if (!IsValidPtr(cEntityData))
			continue;

		if (cVehicleSoldier->IsDead())
			continue;

		extra_players_data_s player_on_vehicle;
		player_on_vehicle.m_Index = p.m_Index;
		player_on_vehicle.m_cszName = p.m_cszName;
		player_on_vehicle.m_iTeam = p.m_iTeam;
		player_on_vehicle.m_IsVisible = true;
		player_on_vehicle.m_flPlayerHealth = cVehicleSoldier->GetHealthComponent()->m_Health;
		player_on_vehicle.m_flMaxPlayerHealth = cVehicleSoldier->GetHealthComponent()->m_MaxHealth;
		player_on_vehicle.m_flVehicleHealth = cVehicleSoldier->GetHealthComponent()->m_VehicleHealth;
		player_on_vehicle.m_flMaxVehicleHealth = cEntityData->m_FrontMaxHealth;
		player_on_vehicle.m_InVehicle = true;
		player_on_vehicle.m_bIsHorse = cVehicleSoldier->IsHorse();
		player_on_vehicle.m_vOrigin = cVehicleSoldier->GetVehiclePosition();
		player_on_vehicle.m_vBoundBoxMin = Vector();
		player_on_vehicle.m_vBoundBoxMax = Vector(0.f, 1.9f, 0.f);
		this->g_ExtraPlayers.push_back(player_on_vehicle);
	}
}

void CFeatures::ClearPlayersData()
{
	this->g_Players.clear();
	this->g_ExtraPlayers.clear();
}

void CFeatures::Debug()
{
	if (!vars::extra::debug)
		return;

	ImGui::Begin("Debug", nullptr);
	ImGui::Text("Recoil: %.f", get_recoil());
	ImGui::End();
}

void CFeatures::Aimbot()
{
	if (globals::killed_focus)
		return;

	if (vars::aimbot::enable == false)
		return;

	if (vars::bMenuOpen)
		return;

	if (vars::aimbot::disable_on_vehicle && this->g_Local.m_InVehicle) 
		return;	

	float flFov = vars::aimbot::fov * 10.f; //75

	float flAimSpeed = vars::aimbot::smooth + 0.2f;

	int iBone = 0;

	switch (vars::aimbot::bone)
	{
	case 0:
		iBone = BONE::HEAD;
		break;
	case 1:
		iBone = BONE::NECK;
		break;
	case 2:
		iBone = BONE::SPINE;
	default:
		break;
	}

	bool isPressedMLeftButton = GetAsyncKeyState(VK_LBUTTON);
	bool isPressedMRightButton = GetAsyncKeyState(VK_RBUTTON);

	bool event = vars::aimbot::aiming_on_rmb ?
		isPressedMLeftButton == false && isPressedMRightButton == false
		: isPressedMLeftButton == false;

	if (event)
		return;

	int iTarget = INT_MAX;

	float flMaxClosetsToScreenCenterX = flFov;
	float flMaxClosetsToScreenCenterY = flFov;

	float flScreenCenter[2] = {
			ImGui::GetIO().DisplaySize.x / 2.f,
			ImGui::GetIO().DisplaySize.y / 2.f
	};

	for (int i = 0; i < this->g_ExtraPlayers.size(); i++)
	{
		if (vars::aimbot::only_enemy && this->g_ExtraPlayers[i].m_iTeam == this->g_Local.m_iTeam)
			continue;

		if (this->g_ExtraPlayers[i].m_InVehicle == false && this->g_ExtraPlayers[i].m_IsVisible == false)
			continue;

		if (vars::aimbot::aim_from_vehicles_only_horse && this->g_ExtraPlayers[i].m_InVehicle && this->g_ExtraPlayers[i].m_bIsHorse == false)
			continue;

		Vector vTarget;

		if (this->g_ExtraPlayers[i].m_InVehicle)
		{
			vTarget = this->g_ExtraPlayers[i].m_vOrigin + this->g_ExtraPlayers[i].m_vBoundBoxMax;
			if (vTarget.Distance(this->g_Local.m_vOrigin) > 100.f)
				continue;
		}
		else
		{
			vTarget = this->g_ExtraPlayers[i].vBoneOrigin[iBone];
		}

		if (vTarget.IsZero())
			continue;
		
		float flScreenTarget[2];
		if (WorldToScreen(vTarget, flScreenTarget))
		{
			float flDifferenceFromScreenCenter[2] = {
				abs(flScreenCenter[0] - flScreenTarget[0]),
				abs(flScreenCenter[1] - flScreenTarget[1])
			};

			if (flDifferenceFromScreenCenter[0] <= flFov && flDifferenceFromScreenCenter[1] <= flFov
				&& flDifferenceFromScreenCenter[0] < flMaxClosetsToScreenCenterX
				&& flDifferenceFromScreenCenter[1] < flMaxClosetsToScreenCenterY)
			{
				flMaxClosetsToScreenCenterX = flDifferenceFromScreenCenter[0];
				flMaxClosetsToScreenCenterY = flDifferenceFromScreenCenter[1];
				iTarget = i;
			}
		}
	}

	if (iTarget == INT_MAX)
		return;

	//flVisCheckDelay[iTarget] = 1.f;

	//m_pDrawing->AddText(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f + 100.f, ImColor(1.f, 1.f, 1.f), vars::font::size, FL_CENTER_X, "index: %d, name: %s", iTarget, this->g_ExtraPlayers[iTarget].m_cszName);

	if (iBone == BONE::NONE)
		return;

	Vector vTarget;

	if (this->g_ExtraPlayers[iTarget].m_InVehicle)
		vTarget = this->g_ExtraPlayers[iTarget].m_vOrigin + this->g_ExtraPlayers[iTarget].m_vBoundBoxMax;
	else
		vTarget = this->g_ExtraPlayers[iTarget].vBoneOrigin[iBone];

	float flScreenTarget[2];

	if (WorldToScreen(vTarget, flScreenTarget))
	{
		float flTarget[2] = { 0.f, 0.f };

		for (int i = 0; i < 2; i++)
		{
			if (flScreenTarget[i] != 0.f)
			{
				flTarget[i] = ((flScreenTarget[i] - flScreenCenter[i]) / flAimSpeed);
			}

			flTarget[i] /= 10.f;

			if (abs(flTarget[i]) < 1.f)
			{
				if (flTarget[i] > 0.f)
					flTarget[i] = 1.f;
				if (flTarget[i] < 0.f)
					flTarget[i] = -1.f;
			}
		}

		float flRecoil = 0.f;

		if (vars::aimbot::enable_recoil_compesation)
		{
			flRecoil = vTarget.Distance(this->g_Local.m_vOrigin) > 70.f
				? 0.f : vars::aimbot::smooth > 0.f ? get_recoil() * vars::aimbot::recoil_compesation / ((ImClamp(flAimSpeed, 0.2f, 0.5f) + 1.f) * 2.f)
				: get_recoil() * vars::aimbot::recoil_compesation;
		}
		
		POINT point;
		GetCursorPos(&point);
		point.x += flTarget[0];
		point.y += flTarget[1] + flRecoil;
		SendMessage(globals::hGame, WM_MOUSEMOVE, 0, MAKELPARAM(point.x, point.y));	
	}
}

void CFeatures::PlayerESP()
{
	if (vars::visuals::player_type == 2)
		return;

	for (auto p : this->g_ExtraPlayers)
	{
		if (vars::visuals::player_type == 0 && p.m_iTeam == this->g_Local.m_iTeam)
			continue;

		auto col_box = PlayerColor(p.m_iTeam, p.m_InVehicle, p.m_bIsHorse, p.m_IsVisible);
			
		auto vTop = p.m_vOrigin + p.m_vBoundBoxMax;
		auto vBot = p.m_vOrigin + p.m_vBoundBoxMin;
	
		this->Draw3DCircle(vBot, col_box);

		float flTop[2], flBot[2];
		if (WorldToScreen(vTop, flTop) && WorldToScreen(vBot, flBot))
		{
			float h = flBot[1] - flTop[1];
			float w = h / 2;
			float x = flBot[0] - w / 2;
			float y = flTop[1];

			this->DrawBox(x, y, w, h, col_box);

			auto spotting_component = memory_utils::read<DWORD_PTR>({ p.m_pEntity, 0x768 });
			char buf[64]{ 0 };
			sprintf(buf, "%I64X", spotting_component);
			//printf("%s\n", buf);

			this->DrawName(p.m_cszName, x, y, w, col_box);

			if (vars::visuals::health)
			{
				if (p.m_InVehicle)
					this->DrawStatusLine(x, y, w, h, p.m_flVehicleHealth, p.m_flMaxVehicleHealth, ImColor(0.f, 0.f, 1.f), LINE_STATUS_BAR::RIGHT);

				this->DrawStatusLine(x, y, w, h, p.m_flPlayerHealth, p.m_flMaxPlayerHealth, ImColor(0.f, 1.f, 0.f), LINE_STATUS_BAR::LEFT);
			}

			this->DrawDistance(x, y, w, h, p.m_vOrigin.Distance(this->g_Local.m_vOrigin));
		}
		else
		{
			this->DrawScreen(p.m_vOrigin, this->g_Local.m_vOrigin, this->g_Local.m_flYaw, col_box);
		}
	}
}

void CFeatures::DrawBox(float x, float y, float w, float h, const ImColor col)
{
	m_pDrawing->DrawEspBox(vars::visuals::box_type, x, y, w, h, col.Value.x, col.Value.y, col.Value.z, col.Value.w);
}

void CFeatures::DrawName(const char *pcszPlayerName, float x, float y, float w, ImColor col)
{
	if (vars::visuals::name == false)
		return;

	if (pcszPlayerName == NULL)
		return;

	ImFont* Font = ImGui::GetIO().Fonts->Fonts[0];
	ImVec2 text_size = Font->CalcTextSizeA(vars::font::size ? vars::font::size : Font->FontSize, FLT_MAX, 0, "");

	m_pDrawing->AddText(x + w / 2.f, y - text_size.y - 2.f, ImColor(1.f, 1.f, 1.f, col.Value.w), vars::font::size, FL_CENTER_X, u8"%s", pcszPlayerName);
}

void CFeatures::DrawStatusLine(float x, float y, float w, float h, float status_value, float max_of_status_value, ImColor col, LINE_STATUS_BAR status_side)
{
	if (status_value <= 0.f)
		return;

	status_value = ImClamp(status_value, 0.f, max_of_status_value);

	const auto size_h = h / max_of_status_value * status_value;
	const auto size_w = w / max_of_status_value * status_value;

	const auto thickness = 2.f;

	switch (status_side)
	{
	case LINE_STATUS_BAR::LEFT:
		m_pDrawing->DrawFillArea(x - thickness - 1.9f, y + h, thickness, -size_h, ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w));

		if (vars::visuals::box_type == 2 || vars::visuals::box_type == 4 || vars::visuals::box_type == 6)
			m_pDrawing->DrawBox(x - thickness - 2.9f, y - 1.f, thickness + 2.f, h + 2.f, ImColor(0.f, 0.f, 0.f, col.Value.w));

		break;
	case LINE_STATUS_BAR::RIGHT:
		m_pDrawing->DrawFillArea(x + w - thickness + (2.9f * 2.f), y + h, thickness, -size_h, ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w));

		if (vars::visuals::box_type == 2 || vars::visuals::box_type == 4 || vars::visuals::box_type == 6)
			m_pDrawing->DrawBox(x + w - thickness + (2.9f * 2.f) - 0.9f, y - 1.f, thickness + 2.f, h + 2.f, ImColor(0.f, 0.f, 0.f, col.Value.w));

		break;
	case LINE_STATUS_BAR::UPPER:
		m_pDrawing->DrawFillArea(x, y - thickness - 1.9f, size_w + 1.9f, thickness, ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w));

		if (vars::visuals::box_type == 2 || vars::visuals::box_type == 4 || vars::visuals::box_type == 6)
			m_pDrawing->DrawBox(x - 1.9f, y - thickness - 2.9f, w + 3.9f, thickness + 2.f, ImColor(0.f, 0.f, 0.f, col.Value.w));

		break;
	case LINE_STATUS_BAR::BOTTOM:
		m_pDrawing->DrawFillArea(x, y + h + thickness + 0.9f, size_w + 1.9f, thickness, ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w));

		if (vars::visuals::box_type == 2 || vars::visuals::box_type == 4 || vars::visuals::box_type == 6)
			m_pDrawing->DrawBox(x - 1.9f, y + h + thickness, w + 3.9f, thickness + 2.f, ImColor(0.f, 0.f, 0.f, col.Value.w));

		break;
	default: break;
	}
}

void CFeatures::DrawDistance(float x, float y, float w, float h, float distance)
{
	if (vars::visuals::distance == false)
		return;

	m_pDrawing->AddText(x + w / 2.f, y + h + 4.9f, ImColor(1.f, 1.f, 1.f), vars::font::size, FL_CENTER_X, "%.0f", distance);
}

void CFeatures::Draw3DCircle(Vector vOrigin, ImColor col)
{
	if (vars::visuals::radar_3d == false)
		return;

	m_pDrawing->AddCircle3D(vOrigin, 18.f, 0.8f, col);
}

void CFeatures::DrawSkeleton()
{
	//
}

void CFeatures::ScreenInfo()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.1f);
	ImGui::Begin("##info", nullptr, 
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 229.f, 8.f));
	ImGui::SetWindowSize(ImVec2(219.f, 10.f));
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%H:%M:%S", timeinfo);
	ImGui::Text("penguin | ");
	ImGui::SameLine();
	ImGui::Text("%.f fps | ", ImGui::GetIO().Framerate);
	float calc_x = ImGui::CalcTextSize(buffer).x;
	ImGui::SameLine(ImGui::GetWindowWidth() - calc_x - 9.f);
	ImGui::Text(buffer);
	ImGui::End();
	ImGui::PopStyleVar();
}

auto deg_to_rad = [](float x)
{
	return ((float)(x) * (float)(IM_PI / 180.f));
};

auto rad_to_deg = [](float x)
{
	return (float)(x) * (float)(180.f / IM_PI);
};

void VectorAnglesRadar(Vector& forward, Vector& angles)
{
	if (forward.x == 0.f && forward.y == 0.f)
	{
		angles.x = forward.z > 0.f ? -90.f : 90.f;
		angles.y = 0.f;
	}
	else
	{
		angles.x = rad_to_deg(atan2(-forward.z, forward.Length2D()));
		angles.y = rad_to_deg(atan2(forward.y, forward.x));
	}
	angles.z = 0.f;
}

void RotateTriangle(std::array<Vector, 3>& points, float rotation)
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points)
	{
		point = point - points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = deg_to_rad(rotation);
		const auto c = cosf(theta);
		const auto s = sinf(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point = point + points_center;
	}
}

Vector RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom)
{
	float zs = LocalPlayerPos.z - EntityPos.z;
	float xs = LocalPlayerPos.x - EntityPos.x;

	double Yaw = -(double)angle;

	float single = xs * (float)cos(Yaw) - zs * (float)sin(Yaw);
	float ypisilum1 = xs * (float)sin(Yaw) + zs * (float)cos(Yaw);

	single *= zoom;
	ypisilum1 *= zoom;

	single = single * 2.f;
	single = single + (float)(posX + sizeX / 2);

	ypisilum1 = ypisilum1 * 2.f;
	ypisilum1 = ypisilum1 + (float)(posY + sizeY / 2);

	if (single < (float)posX)
		single = (float)posX;

	if (ypisilum1 < (float)posY)
		ypisilum1 = (float)posY;

	if (single > (float)(posX + sizeX - 3))
		single = (float)(posX + sizeX - 3);

	if (ypisilum1 > (float)(posY + sizeY - 3))
		ypisilum1 = (float)(posY + sizeY - 3);


	return Vector(single, ypisilum1, 0);
}

float Interp(float s1, float s2, float s3, float f1, float f3)
{
	if (s2 == s1) return f1;
	if (s2 == s3) return f3;
	if (s3 == s1) return f1;

	return f1 + ((s2 - s1) / (s3 - s1)) * (f3 - f1);
}

void CFeatures::DrawScreen(Vector origin, Vector myOrigin, float myYaw, ImColor col)
{
	if (vars::visuals::offscreen == false)
		return;

	if (origin.Distance(myOrigin) >= vars::visuals::max_distance_offscreen)
		return;

	float alpha = Interp(0.0f, 
		(vars::visuals::max_distance_offscreen) - origin.Distance(myOrigin),
		10.f, 0.0f, vars::visuals::max_distance_offscreen);

	col.Value.w = alpha / 255.f;

	int radar_range = 45;

	int x, y;

	ImVec2 vSize = ImGui::GetWindowSize();
	ImVec2 vPos = ImGui::GetWindowPos();

	Vector EntityPos = RotatePoint(origin, this->g_Local.m_vOrigin, vPos.x, vPos.y, vSize.x, vSize.y, this->g_Local.m_flYaw, 1.f);

	x = EntityPos.x;
	y = EntityPos.y;

	auto angle = Vector();
	VectorAnglesRadar(Vector((float)(ImGui::GetIO().DisplaySize.x / 2) - x, (float)(ImGui::GetIO().DisplaySize.y / 2) - y, 0.f), angle);

	const auto angle_yaw_rad = deg_to_rad(angle.y + 180.f);
	const auto new_point_x = (ImGui::GetIO().DisplaySize.x / 2) + (radar_range) / 2 * 8 * cosf(angle_yaw_rad);
	const auto new_point_y = (ImGui::GetIO().DisplaySize.y / 2) + (radar_range) / 2 * 8 * sinf(angle_yaw_rad);

	std::array<Vector, 3> points
	{
		Vector(new_point_x - ((90) / 4 + 3.5f) / 2, new_point_y - ((radar_range) / 4 + 3.5f) / 2, 0.f),
		Vector(new_point_x + ((90) / 4 + 3.5f) / 4, new_point_y, 0.f),
		Vector(new_point_x - ((90) / 4 + 3.5f) / 2, new_point_y + ((radar_range) / 4 + 3.5f) / 2, 0.f)
	};

	RotateTriangle(points, angle.y + 180.f);

	m_pDrawing->AddTriangleFilled(ImVec2(points.at(0).x, points.at(0).y), ImVec2(points.at(1).x, points.at(1).y), ImVec2(points.at(2).x, points.at(2).y), col);
}

ImColor CFeatures::PlayerColor(int iTeam, bool isVehicle, bool isHorse, const bool isVisible)
{
	if (iTeam == this->g_Local.m_iTeam)
	{
		if (isVehicle == false)
		{
			if (isVisible)
				return ImColor(vars::visuals::col_teammate_vis[0], vars::visuals::col_teammate_vis[1], vars::visuals::col_teammate_vis[2], 1.f);
			else
				return ImColor(vars::visuals::col_teammate_invis[0], vars::visuals::col_teammate_invis[1], vars::visuals::col_teammate_invis[2], 1.f);
		}
		else
		{
			if (isHorse)
				return ImColor(vars::visuals::col_teammate_horse[0], vars::visuals::col_teammate_horse[1], vars::visuals::col_teammate_horse[2], 1.f);
			else
				return ImColor(vars::visuals::col_teammate_vehicle[0], vars::visuals::col_teammate_vehicle[1], vars::visuals::col_teammate_vehicle[2], 1.f);
		}
	}
	else if (iTeam != this->g_Local.m_iTeam)
	{
		if (isVehicle == false)
		{
			if (isVisible)
				return ImColor(vars::visuals::col_enemy_vis[0], vars::visuals::col_enemy_vis[1], vars::visuals::col_enemy_vis[2], 1.f);
			else
				return ImColor(vars::visuals::col_enemy_invis[0], vars::visuals::col_enemy_invis[1], vars::visuals::col_enemy_invis[2], 1.f);
		}
		else
		{
			if (isHorse)
				return ImColor(vars::visuals::col_enemy_horse[0], vars::visuals::col_enemy_horse[1], vars::visuals::col_enemy_horse[2], 1.f);
			else
				return ImColor(vars::visuals::col_enemy_vehicle[0], vars::visuals::col_enemy_vehicle[1], vars::visuals::col_enemy_vehicle[2], 1.f);
		}
	}
	else
		return ImColor(1.f, 1.f, 1.f);
}

void CFeatures::DrawScreenshotCounter()
{
	static int save_screenshot_count = globals::screenshot_counter;

	static float color_fade = 1.f;

	static auto col = ImColor(1.f, 1.f, 1.f);

	if (save_screenshot_count != globals::screenshot_counter)
	{
		color_fade = 0.f;
		save_screenshot_count = globals::screenshot_counter;
	}

	if (color_fade <= 1.f)
	{
		color_fade += 0.007f;
		col.Value.y = color_fade;
		col.Value.z = color_fade;
	}

	m_pDrawing->AddText(10, 10, col, 20, FL_NONE, "Screenshot: %d", globals::screenshot_counter);
	//m_pDrawing->AddText(10, 30, ImColor(0.5f, 1.f, 0.5f), 20, FL_NONE, "Taked fake screenshot: %d", globals::fake_screenshot_counter);
}

void CFeatures::InrecreaseFireRate(bool bIsEnable)
{
	if (bIsEnable)
	{
		//memory_utils::patch_instruction(this->qwWeaponFireRateIntstructionAddress, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
		memory_utils::patch_instruction(this->qwWeaponFireRateIntstructionAddress, "\xC7\x87\xD4\x01\x00\x00\x00\x00\x00\x00\x90\x90\x90\x90", 14);
		SetupFirerate(vars::aimbot::rate_on_firerate);
		memory_utils::patch_instruction(this->qwWeaponOverheatingInstruction, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
		memory_utils::patch_instruction(this->qwWeaponDecreaseBullet, "\x90\x90\x90\x90\x90\x90", 6);
	}
	else
	{
		//memory_utils::patch_instruction(this->qwWeaponFireRateIntstructionAddress, "\xF3\x0F\x11\x87\xD4\x01\x00\x00", 8);
		memory_utils::patch_instruction(this->qwWeaponFireRateIntstructionAddress, "\xF3\x0F\x11\x87\xD4\x01\x00\x00\x89\x87\x60\x01\x00\x00", 14);
		memory_utils::patch_instruction(this->qwWeaponOverheatingInstruction, "\xF3\x0F\x11\x81\x10\x01\x00\x00", 8);
		memory_utils::patch_instruction(this->qwWeaponDecreaseBullet, "\x89\x93\x00\x01\x00\x00", 6);
	}
}

void CFeatures::SetupFirerate(float flRate)
{
	if (vars::aimbot::increase_fire_rate && flRate > 0.f && this->qwWeaponFirerateSpeed)
		*(float*)this->qwWeaponFirerateSpeed = flRate;
}

void CFeatures::NoRecoil(bool bIsEnable)
{
	if (bIsEnable)
	{
		memory_utils::patch_instruction(this->qwWeaponRecoilPitchAxis, "\xC7\x47\x6C\x00\x00\x00\x00\x90", 8);
		memory_utils::patch_instruction(this->qwWeaponRecoilYawAxis, "\x90\x90\x90\x90\x90", 5);
	}
	else
	{
		memory_utils::patch_instruction(this->qwWeaponRecoilPitchAxis, "\xF3\x44\x0F\x11\x77\x6C\x76\x0D", 8);
		memory_utils::patch_instruction(this->qwWeaponRecoilYawAxis, "\xF3\x0F\x11\x46\x68", 5);
	}
}

void CFeatures::NoSpread(bool bIsEnable)
{
	if (bIsEnable)
		memory_utils::patch_instruction(this->qwWeaponSpread, "\xC7\x47\x40\x00\x00\x00\x00\x90\x90\x90", 10);
	else
		memory_utils::patch_instruction(this->qwWeaponSpread, "\xF3\x0F\x11\x47\x40\xF3\x0F\x11\x57\x44", 10);
}


