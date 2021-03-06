#ifndef ZENGUI_FONT_HPP
#define ZENGUI_FONT_HPP

namespace zn
{
namespace ui
{
	/*
		This is all what the GUI knows about fonts.
		how to draw them is up to you.
	*/
	struct Font
	{
		void * userdata;
		std::string ID;
		std::string src; // filename

		Font() : userdata(nullptr) {}
	};

} // namespace ui
} // namespace zn

#endif // ZENGUI_FONT_HPP

