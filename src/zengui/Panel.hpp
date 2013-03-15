/*
Panel.hpp
Copyright (C) 2010-2012 Marc GILLERON
This file is part of the zCraft project.
*/

#ifndef ZENGUI_PANEL_HPP_INCLUDED
#define ZENGUI_PANEL_HPP_INCLUDED

#include "Container.hpp"

namespace zn
{
namespace ui
{
	class Panel : public AContainer
	{
	public :

		Panel() : AContainer() {}
		virtual ~Panel() {}

		virtual render(IRenderer & r) override
		{
			if(r_theme != nullptr)
				r_theme->drawPanel(r, *this);
		}

	};

} // namespace ui
} // namespace zn

#endif // ZENGUI_PANEL_HPP_INCLUDED