struct VERY_GOOD_STRUCTURE_1337
{
	std::uintptr_t m_pPlayer; //maybe ClientSoldierEntity?
};

struct players_data_s
{
	int m_Index;
	char* m_cszName;
	int m_iTeam;
	ClientVehicleEntity* m_ClientVehicleEntity;
	ClientSoldierEntity* m_ClientSoldierEntity;
};

enum BONE
{
	NONE,
	HEAD,
	NECK,
	SPINE
};

struct extra_players_data_s
{
	DWORD64 m_pEntity;
	int m_Index;
	char* m_cszName;
	int m_iTeam;
	float m_flPlayerHealth;
	float m_flMaxPlayerHealth;
	float m_flVehicleHealth;
	float m_flMaxVehicleHealth;
	bool m_bIsHorse;
	bool m_IsVisible;
	bool m_InVehicle;
	float m_flYaw;
	Vector m_vOrigin;
	Vector m_vBoundBoxMax;
	Vector m_vBoundBoxMin;
	Vector vBoneOrigin[4];
};

enum LINE_STATUS_BAR
{
	LEFT,
	RIGHT,
	UPPER,
	BOTTOM
};

class CFeatures
{
public:
	enum INITIALIZATION_STATUS {
		OK,
		NOT_INITIALIZED,
		FIRERATE_SEARCH,
		OVERHEATINGWEAPON_SEARCH,
		DECREASBULLET_SEARCH,
		RECOILPITCH_SEARCH,
		RECOILYAW_SEARCH,
		SPREAD_SEARCH,
		FIRERATE_NOT_FOUND,
		OVERHEATINGWEAPON_NOT_FOUND,
		DECREASBULLET_NOT_FOUND,
		RECOILPITCH_NOT_FOUND,
		RECOILYAW_NOT_FOUND,
		SPREAD_NOT_FOUND,
		INITIALIZATION_STATUS_SIZE
	};

	INITIALIZATION_STATUS m_InitializetionStatus = NOT_INITIALIZED;

	void Initialize();

	void Run();

	void ScreenInfo();

	void Debug();

	void InrecreaseFireRate(bool bIsEnable);

	void SetupFirerate(float flRate);

	void NoRecoil(bool bIsEnable);

	void NoSpread(bool bIsEnable);
private:
	bool UpdateLocalPlayer();

	void UpdatePlayers();

	void ClearPlayersData();

	void Aimbot();

	void PlayerESP();

	void DrawScreen(Vector origin, Vector myOrigin, float myYaw, ImColor col);

	ImColor PlayerColor(int iTeam, bool isVehicle, bool isHorse, const bool isVisible);

	void DrawScreenshotCounter();

	void DrawBox(float x, float y, float w, float h, const ImColor col);

	void DrawName(const char* pcszPlayerName, float x, float y, float w, ImColor col);

	void DrawStatusLine(float x, float y, float w, float h, float status_value, float max_of_status_value, ImColor col, LINE_STATUS_BAR status_side = LINE_STATUS_BAR::LEFT);

	void DrawDistance(float x, float y, float w, float h, float distance);

	void Draw3DCircle(Vector vOrigin, ImColor col);

	void DrawSkeleton();

	extra_players_data_s g_Local;
	std::deque<players_data_s>g_Players;
	std::deque<extra_players_data_s>g_ExtraPlayers;

	DWORD64 qwWeaponFireRateIntstructionAddress;
	DWORD64 qwWeaponFirerateSpeed;
	DWORD64 qwWeaponOverheatingInstruction;
	DWORD64 qwWeaponDecreaseBullet;
	DWORD64 qwWeaponRecoilPitchAxis;
	DWORD64 qwWeaponRecoilYawAxis;
	DWORD64 qwWeaponSpread;
public:
	std::deque<VERY_GOOD_STRUCTURE_1337> m_PlayersData;
	std::mutex m_mtxSyncDataGrabber;
};
extern std::unique_ptr<CFeatures>m_pFeatures;

