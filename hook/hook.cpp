#include "../includes.h"

std::unique_ptr<CHook>m_pHook = std::make_unique<CHook>();

using fSetCursorPos = BOOL(WINAPI*)(int, int);
fSetCursorPos pSetCursorPos = NULL;

using fPresent = HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT);
fPresent pPresent = NULL;

using fResizeBuffers = HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
fResizeBuffers pResizeBuffers = NULL;

IDXGISwapChain* swapchain = nullptr;
ID3D11Device* device = nullptr;
ID3D11DeviceContext* context = nullptr;
ID3D11RenderTargetView* render_view = nullptr;

static bool renderview_lost = true;

enum IDXGISwapChainvTable //for dx10 / dx11
{
	QUERY_INTERFACE,
	ADD_REF,
	RELEASE,
	SET_PRIVATE_DATA,
	SET_PRIVATE_DATA_INTERFACE,
	GET_PRIVATE_DATA,
	GET_PARENT,
	GET_DEVICE,
	PRESENT,
	GET_BUFFER,
	SET_FULLSCREEN_STATE,
	GET_FULLSCREEN_STATE,
	GET_DESC,
	RESIZE_BUFFERS,
	RESIZE_TARGET,
	GET_CONTAINING_OUTPUT,
	GET_FRAME_STATISTICS,
	GET_LAST_PRESENT_COUNT
};

DWORD splash_timer = 0;
bool splash = true;

void InitImGui()
{
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();

	auto& style = ImGui::GetStyle();

	style.FrameRounding = 3.f;
	style.ChildRounding = 3.f;
	style.ChildBorderSize = 1.f;
	style.ScrollbarSize = 0.6f;
	style.ScrollbarRounding = 3.f;
	style.GrabRounding = 3.f;
	style.WindowRounding = 0.f;

	style.Colors[ImGuiCol_FrameBg] = ImColor(200, 200, 200);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(220, 220, 220);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(230, 230, 230);
	style.Colors[ImGuiCol_Separator] = ImColor(180, 180, 180);
	style.Colors[ImGuiCol_CheckMark] = ImColor(255, 172, 19);
	style.Colors[ImGuiCol_SliderGrab] = ImColor(255, 172, 19);
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 172, 19);
	style.Colors[ImGuiCol_ScrollbarBg] = ImColor(120, 120, 120);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(255, 172, 19);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab);
	style.Colors[ImGuiCol_Header] = ImColor(160, 160, 160);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(200, 200, 200);
	style.Colors[ImGuiCol_Button] = ImColor(180, 180, 180);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(200, 200, 200);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(230, 230, 230);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.78f, 0.78f, 0.78f, 1.f);
	style.Colors[ImGuiCol_WindowBg] = ImColor(220, 220, 220, 0.7 * 255);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.72f, 0.72f, 0.72f, 0.70f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 0.87f);
	style.Colors[ImGuiCol_Text] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.72f, 0.72f, 0.72f, 0.76f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.61f, 0.61f, 0.61f, 0.79f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.71f, 0.71f, 0.71f, 0.80f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.77f, 0.77f, 0.77f, 0.84f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.73f, 0.73f, 0.73f, 0.82f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.58f, 0.58f, 0.58f, 0.84f);

	auto& io = ImGui::GetIO();
	io.IniFilename = NULL;
	m_pMenuFonts->LoadFonts();
	ImGui_ImplWin32_Init(globals::hGame);
	ImGui_ImplDX11_Init(device, context);
	ImGui_ImplDX11_CreateDeviceObjects();

	ImGuiWindowFlags flags_color_edit = ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoInputs;
	ImGui::SetColorEditOptions(flags_color_edit);

	splash_timer = GetTickCount();
}

std::string loading_shit(const char* str)
{
	struct member
	{
		DWORD timer;
		std::string points;
	};
	static std::map <const char*, member> timer_map;
	if (!timer_map[str].timer)
		timer_map[str].timer = GetTickCount();
	else
	{
		if (GetTickCount() - timer_map[str].timer > 1000)
		{
			timer_map[str].points += '.';
			timer_map[str].timer = NULL;
		}
		if (timer_map[str].points == "....")
			timer_map[str].points.clear();
	}
	return std::string(str) + timer_map[str].points;
}

void BeginScene()
{
	if (globals::unload_dll)
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	if (splash)
	{
		vars::bMenuOpen = true;

		ImGui::SetNextWindowPos(ImVec2(), ImGuiCond_::ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_::ImGuiCond_Always);
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.0f));
		ImGui::Begin("##splash", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings);
		ImGui::PushFont(m_pMenuFonts->font_Title);
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 0.7f));
		auto text_sz_title = ImGui::CalcTextSize("ALTERNATIVE");
		static float offset_y_title = 0.f;
		ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (text_sz_title.x / 2.f));
		ImGui::SetCursorPosY(((ImGui::GetIO().DisplaySize.y / 2.f) - (text_sz_title.y / 2.f) - 5.f) - offset_y_title);
		ImGui::Text("ALTERNATIVE");
		ImGui::PopFont();
		ImGui::PushFont(m_pMenuFonts->font_AfterTitle);
		auto user_hello = std::string(std::string("Hello, ") + vars::user::username).c_str();
		ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (ImGui::CalcTextSize(user_hello).x / 2.f));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 100.f);
		ImGui::Text(user_hello);
		ImGui::SetCursorPosY((ImGui::GetIO().DisplaySize.y / 2.f) + ((ImGui::GetIO().DisplaySize.y / 2.f) / 2.f) + 70.f);
		char* text = NULL;
		switch (m_pFeatures->m_InitializetionStatus)
		{
		case CFeatures::INITIALIZATION_STATUS::FIRERATE_SEARCH:
			text = "search weapon firerate instructions";
			ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (ImGui::CalcTextSize(text).x / 2.f));
			ImGui::Text("%s", loading_shit(text).c_str());
			break;
		case CFeatures::INITIALIZATION_STATUS::OVERHEATINGWEAPON_SEARCH:
			text = "search weapon overheating instructions";
			ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (ImGui::CalcTextSize(text).x / 2.f));
			ImGui::Text("%s", loading_shit(text).c_str());
			break;
		case CFeatures::INITIALIZATION_STATUS::DECREASBULLET_SEARCH:
			text = "search fire movement decrease bullet instructions";
			ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (ImGui::CalcTextSize(text).x / 2.f));
			ImGui::Text("%s", loading_shit(text).c_str());
			break;
		case CFeatures::INITIALIZATION_STATUS::RECOILPITCH_SEARCH:
			text = "search weapon recoil pitch instructions";
			ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (ImGui::CalcTextSize(text).x / 2.f));
			ImGui::Text("%s", loading_shit(text).c_str());
			break;
		case CFeatures::INITIALIZATION_STATUS::RECOILYAW_SEARCH:
			text = "search weapon recoil yaw instructions";
			ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (ImGui::CalcTextSize(text).x / 2.f));
			ImGui::Text("%s", loading_shit(text).c_str());
			break;
		case CFeatures::INITIALIZATION_STATUS::SPREAD_SEARCH:
			text = "search weapon spread instructions";
			ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (ImGui::CalcTextSize(text).x / 2.f));
			ImGui::Text("%s", loading_shit(text).c_str());
			break;
		default:
			text = "initializing";
			ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x / 2.f) - (ImGui::CalcTextSize(text).x / 2.f));
			ImGui::Text("%s", loading_shit(text).c_str());
			break;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor();
		ImGui::End();
		ImGui::PopStyleColor(2);

		if (m_pFeatures->m_InitializetionStatus == CFeatures::INITIALIZATION_STATUS::OK && GetTickCount() - splash_timer > 8000)
			splash = false;

		static auto once = []()
		{
			CreateThread(NULL, NULL, [](void* arg) -> DWORD { m_pFeatures->Initialize(); return 1337; }, NULL, NULL, NULL);
			return true;
		}();
	}
	else
	{
		if (vars::bMenuOpen)
		{
			ImGui::GetIO().MouseDrawCursor = true;
			m_pMenu->Run();
		}
		else
			ImGui::GetIO().MouseDrawCursor = false;

		m_pFeatures->ScreenInfo();

		m_pFeatures->Debug();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::Begin("##BackBuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings);
		ImGui::SetWindowPos(ImVec2(), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

		m_pFeatures->Run();

		ImGui::GetCurrentWindow()->DrawList->PushClipRectFullScreen();
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	ImGui::EndFrame();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

HRESULT __fastcall Present_Hooked(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags)
{
	if (renderview_lost)
	{
		if (SUCCEEDED(pChain->GetDevice(__uuidof(ID3D11Device), (void**)&device)))
		{
			device->GetImmediateContext(&context);

			ID3D11Texture2D* pBackBuffer;
			pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			device->CreateRenderTargetView(pBackBuffer, NULL, &render_view);
			pBackBuffer->Release();

#ifdef _USERDEBUG
			std::cout << __FUNCTION__ << " > renderview successfully received!" << std::endl;
#endif
			renderview_lost = false;
		}
	}

	static auto once = [pChain, SyncInterval, Flags]()
	{
		InitImGui();
#ifdef _USERDEBUG
		std::cout << __FUNCTION__ << " > first called!" << std::endl;
#endif
		return true;
	}();

	context->OMSetRenderTargets(1, &render_view, NULL);

	if (globals::screen_bypass_active == FALSE)
		BeginScene();

	return pPresent(pChain, SyncInterval, Flags);
}

HRESULT __fastcall ResizeBuffers_hooked(IDXGISwapChain* pChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT Flags)
{
#ifdef _USERDEBUG
	static auto once = []()
	{
		std::cout << __FUNCTION__ << " > first called!" << std::endl;
		return true;
	}();
#endif

	render_view->Release();
	render_view = nullptr;
	renderview_lost = true;

	ImGui_ImplDX11_CreateDeviceObjects();
	ImGui_ImplDX11_InvalidateDeviceObjects();

	return pResizeBuffers(pChain, BufferCount, Width, Height, NewFormat, Flags);
}

BOOL WINAPI SetCursorPosHooked(int x, int y)
{
#ifdef _USERDEBUG
	static auto once = []()
	{
		std::cout << __FUNCTION__ << " first called!" << std::endl;
		return true;
	}();
#endif

	if (vars::bMenuOpen)
		return TRUE;

	return pSetCursorPos(x, y);
}

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc_Hooked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _USERDEBUG
	static auto once = []()
	{
		std::cout << __FUNCTION__ << " first called!" << std::endl;
		return true;
	}();
#endif

	switch (uMsg)
	{
	case WM_KILLFOCUS:
		globals::killed_focus = true;
		break;
	case WM_SETFOCUS:
		globals::killed_focus = false;
		break;
	default: break;
	}

	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
	{
		vars::bMenuOpen = !vars::bMenuOpen;
		return FALSE;
	}

	if (vars::bMenuOpen && ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return TRUE;
	}

	return CallWindowProc(m_pHook->pWndProc, hwnd, uMsg, wParam, lParam);
}

std::string GetFileName()
{
	std::string path("screenshots\\");
	CreateDirectory(path.c_str(), NULL);

	time_t time_;
	time(&time_);

	return path + std::to_string(time_) + ".bmp";
}

void SaveBitBltResult(HDC hdcDst, HDC hdcSrc, int width, int height)
{
	HBITMAP hbitmapsrc = CreateCompatibleBitmap(hdcDst, width, height);
	HBITMAP hbitmapdst = (HBITMAP)SelectObject(hdcDst, hbitmapsrc);

	BITMAPINFOHEADER bi{ 0 };
	BYTE* bmp{ nullptr };
	DWORD dwBmpSize;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;
	bmp = new BYTE[dwBmpSize];

	BITMAPFILEHEADER bmfHeader{ 0 };
	bmfHeader.bfSize = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	bmfHeader.bfType = 0x4D42;

	GetDIBits(hdcSrc, hbitmapdst, 0, (UINT)height, bmp, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	auto path = GetFileName();

	HANDLE hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (void*)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (void*)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (void*)bmp, dwBmpSize, &dwBytesWritten, NULL);

	CloseHandle(hFile);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void CHook::SetupDX11Hook()
{
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC scd{};
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = globals::hGame;
	scd.SampleDesc.Count = 1;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = TRUE;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &feature_level, 1, D3D11_SDK_VERSION, &scd, &swapchain, &device, NULL, &context)))
	{
#ifdef _USERDEBUG
		std::cout << "failed to create device\n";
#endif
		return;
	}

	void** pVTableSwapChain = *reinterpret_cast<void***>(swapchain);

	this->pPresentAddress = reinterpret_cast<LPVOID>(pVTableSwapChain[IDXGISwapChainvTable::PRESENT]);
	this->pResizeBuffersAddress = reinterpret_cast<LPVOID>(pVTableSwapChain[IDXGISwapChainvTable::RESIZE_BUFFERS]);

	if (MH_CreateHook(this->pPresentAddress, &Present_Hooked, (LPVOID*)&pPresent) != MH_OK
		|| MH_EnableHook(this->pPresentAddress) != MH_OK)
	{ 
#ifdef _USERDEBUG
		std::cout << "failed create hook present\n";
#endif
		return; 
	}

	if (MH_CreateHook(pResizeBuffersAddress, &ResizeBuffers_hooked, (LPVOID*)&pResizeBuffers) != MH_OK
		|| MH_EnableHook(pResizeBuffersAddress) != MH_OK)
	{
#ifdef _USERDEBUG
		std::cout << "failed create hook resizebuffers\n";
#endif
		return;
	}
}

void CHook::SetupSetCursorPosHook()
{
	this->pSetCursorPosAddress = (LPVOID)SetCursorPos;

	if (MH_CreateHook(pSetCursorPosAddress, &SetCursorPosHooked, (PVOID*)&pSetCursorPos) != MH_OK)
		return;

	if (MH_EnableHook(pSetCursorPosAddress) != MH_OK)
		return;
}

void CHook::SetupWndProcHook()
{
	this->pWndProc = (WNDPROC)SetWindowLongPtr(globals::hGame, GWLP_WNDPROC, (LONG_PTR)WndProc_Hooked);
}
void(__fastcall* UndefinedFunction_1471ebf50_gate)(__int64 param_1, __int64 param_2);


void UndefinedFunction_1471ebf50(__int64 param_1, __int64 param_2)
{
	m_pFeatures->m_mtxSyncDataGrabber.lock();

	if (m_pFeatures->m_PlayersData.size() != 0)
	{
		bool founded = false;

		for (auto& data : m_pFeatures->m_PlayersData)
		{
			if (data.m_pPlayer == param_1)
			{
				founded = true;
			}
		}

		if (!founded)
		{
			VERY_GOOD_STRUCTURE_1337 playerData;
			playerData.m_pPlayer = param_1;
			m_pFeatures->m_PlayersData.push_back(playerData);
		}
	}
	else
	{
		VERY_GOOD_STRUCTURE_1337 playerData;
		playerData.m_pPlayer = param_1;
		m_pFeatures->m_PlayersData.push_back(playerData);
	}

	m_pFeatures->m_mtxSyncDataGrabber.unlock();

	UndefinedFunction_1471ebf50_gate(param_1, param_2);
}

void CHook::CreateAlternativeGetPlayerPosAddress()
{
	/*
		Address of signature = bf1.exe + 0x071EBF50
		"\x48\x89\x00\x48\x89\x00\x00\x55\x57\x41\x00\x48\x8D\x00\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x00\x00\x00\x00\x00\x0F\x29\x00\x00\x00\x00\x00\x48\x89", "xx?xx??xxx?xx?????xxx????xx?????xx?????xx"
		"48 89 ? 48 89 ? ? 55 57 41 ? 48 8D ? ? ? ? ? 48 81 EC ? ? ? ? 0F 29 ? ? ? ? ? 0F 29 ? ? ? ? ? 48 89"
	*/

	auto adddress = (void*)memory_utils::find_pattern(memory_utils::get_base(),
		"\x48\x89\x00\x48\x89\x00\x00\x55\x57\x41\x00\x48\x8D\x00\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x00\x00\x00\x00\x00\x0F\x29\x00\x00\x00\x00\x00\x48\x89",
		"xx?xx??xxx?xx?????xxx????xx?????xx?????xx"); //0x1471ebf50

	MH_CreateHook(adddress, UndefinedFunction_1471ebf50, (void**)&UndefinedFunction_1471ebf50_gate);
	MH_EnableHook(adddress);
}