class CImGuiFonts
{
public:
	void LoadFonts();
	ImFont* font_Main;
	ImFont* font_Credits;
	ImFont* font_Title;
	ImFont* font_AfterTitle;
};
extern std::unique_ptr<CImGuiFonts>m_pMenuFonts;
