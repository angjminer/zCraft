/*
GameContainer.hpp
Copyright (C) 2010-2012 Marc GILLERON
This file is part of the zCraft project.
*/

#ifndef ZN_AGAMECONTAINER_HPP
#define ZN_AGAMECONTAINER_HPP

#include <string>
#include <exception>
#include "engine/core/types.hpp"
#include "Game.hpp"

namespace zn
{
	struct VideoSettings
	{
		u32 width = 800;
		u32 height = 600;
		bool resizeable = true;
		u32 openGLMajorVersion = 2;
		u32 openGLMinorVersion = 0;
	};

	class AGameContainer
	{
	public:

		AGameContainer(IGame * game);
		virtual ~AGameContainer();

		virtual void setVideoMode(VideoSettings videoSettings, std::string title="") = 0;

		virtual void setTitle(const std::string & title) = 0;
		virtual const std::string & getTitle() = 0;

		virtual u32 getWidth() const = 0;
		virtual u32 getHeight() const = 0;

		virtual void setFullscreen(bool fullscreen) = 0;
		virtual bool isFullscreen() const = 0;

		virtual void start() = 0; // Throws an exception if a fatal error occur

	protected:

		VideoSettings m_videoSettings;
		IGame * m_game;

	};

} // namespace zn

#endif // ZN_GAMECONTAINER_HPP


