#include "../includes.h"
#include "../imgui/imgui_fonts.h"

std::unique_ptr<CImGuiFonts>m_pMenuFonts = std::make_unique<CImGuiFonts>();

void CImGuiFonts::LoadFonts()
{
	auto& io = ImGui::GetIO();
	io.Fonts->AddFontFromMemoryTTF(Stolzl_Light, IM_ARRAYSIZE(Stolzl_Light), 15.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	this->font_Main = io.Fonts->AddFontFromMemoryTTF(Montserrat_Light, IM_ARRAYSIZE(Montserrat_Light), 21.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	this->font_Title = io.Fonts->AddFontFromMemoryTTF(Montserrat_Light, IM_ARRAYSIZE(Montserrat_Light), 120.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	this->font_AfterTitle = io.Fonts->AddFontFromMemoryTTF(Stolzl_Light, IM_ARRAYSIZE(Stolzl_Light), 55.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
}
