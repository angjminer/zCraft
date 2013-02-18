/*
PerspectiveMapViewer.cpp
Copyright (C) 2010-2012 Marc GILLERON
This file is part of the zCraft project.
*/

#include <sstream>
#include "engine/opengl/opengl.hpp"
#include "zcraft/games/PerspectiveMapViewer.hpp"
#include "zcraft/face.hpp"

using namespace zn;

namespace zcraft
{
	PerspectiveMapViewer::PerspectiveMapViewer(u32 width, u32 height)
	: 	ABasicGame(width, height, "zCraft :: 3D map viewer - indev")
	{}

	PerspectiveMapViewer::~PerspectiveMapViewer()
	{}

	bool PerspectiveMapViewer::init()
	{
		if(!gl::initExtensions(true, true))
			return false;

		//zcraft::init();

		// Load a font
		if(!m_font.loadFromFile("assets/fonts/arial32.fnt"))
			return false;

		// Init camera
		m_camera.setPosition(Vector3f(0, -5, 16));
		m_camera.updateViewport(Vector2f(
			m_window.getSize().x, m_window.getSize().y));

		// Init map stream
		m_map.addListener(&m_meshMap);
		m_mapStreamer = new MapStreamer(m_map, 8);
		m_mapStreamer->update(Vector3i(0, 0, 0), true); // first update
		m_mapStreamer->setSave(false); // Save turned off atm.

		return true;
	}

	void PerspectiveMapViewer::update(const zn::Time & delta)
	{
		// Update camera
		m_camera.update(delta, m_window);

		updatePointedVoxel();

		// Update map stream
		Vector3f camPos = m_camera.getPosition();
		Vector3i pos(camPos.x, camPos.y, camPos.z);
		Vector3i bpos(pos.x >> 4, pos.y >> 4, pos.z >> 4);
		m_mapStreamer->update(bpos);
	}

	void PerspectiveMapViewer::renderScene(const zn::Time & delta)
	{
		//glClearColor(0.1f, 0.5f, 0.9f, 1);
		glClearColor(0.5f, 0.75f, 1.0f, 1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_CULL_FACE);
		glHint(GL_LINE_SMOOTH, GL_FASTEST);

		/* Scene */

	#if defined ZN_OPENGL2

		glPushMatrix();

		m_camera.look();

		glDisable(GL_BLEND);

		// Draw the map
		m_meshMap.drawAll();

		// Pointed voxel
		if(m_raycast.collision)
		{
			glPushMatrix();
			glTranslatef(
				0.5f + m_raycast.hit.pos.x,
				0.5f + m_raycast.hit.pos.y,
				0.5f + m_raycast.hit.pos.z);
			glColor3ub(255,255,255);
			glLineWidth(2.f);
			gl::drawCubeLines(0.501f);
			glPopMatrix();
		}

		// Axes
		glDisable(GL_TEXTURE_2D);
		glLineWidth(4);
		gl::drawAxes(1);
		glLineWidth(1);

		// Transparent objects
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Grid
		glColor3f(0.f,0.f,0.f);
		glPushMatrix();
		glScalef(16, 16, 16);
		gl::drawGrid(0, 0, 16);
		glPopMatrix();

		glPopMatrix();

	#elif defined ZN_OPENGL3
		#warning "zcraft::PerspectiveMapViewer doesn't supports OpenGL3"
	#endif // defined

//		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}

	void PerspectiveMapViewer::renderGUI(const Time & delta)
	{
		/* HUD */

	#if defined ZN_OPENGL2

		glColor4ub(255,255,255,255);

		// Target
		glLineWidth(2);
		gl::drawCross(m_window.getSize().x / 2, m_window.getSize().y / 2, 16);

	#elif defined ZN_OPENGL2
		#warning "zcraft::PerspectiveMapViewer doesn't supports OpenGL3"
	#endif // defined

		std::stringstream ss;

		// Tips
		ss << "WASD/ZQSD to move, arrows to rotate, +/- to go up and down, "
			<< "ESC to disable camera\n";

		// FPS
		ss << "FPS=" << (int)delta.hz();

		// MapStreamThread
		MapStreamThread::RunningInfo threadInfo = m_mapStreamer->getThreadInfo();
		ss << "\nRem:" << threadInfo.remainingRequests
			<< ", L:" << threadInfo.loadedCount
			<< ", G:" << threadInfo.generatedCount
			<< ", S:" << threadInfo.savedCount
			<< ", D:" << threadInfo.droppedCount;

		// MeshMap
		ss << "\nBlocks: " << m_map.getBlockCount()
			<< " Meshs: " << m_meshMap.getCount();

		// Position
		Vector3f fpos = m_camera.getPosition();
		ss << "\nPos: " << Vector3i(floor(fpos.x), floor(fpos.y), floor(fpos.z));

		m_font.draw(ss.str(), 0, 0);
	}

	void PerspectiveMapViewer::dispose()
	{
		m_meshMap.clear();
	}

	void PerspectiveMapViewer::resized(const Vector2i & newSize)
	{
		m_camera.updateViewport(Vector2f(newSize.x, newSize.y));
	}

	void PerspectiveMapViewer::processEvent(const sf::Event & event)
	{
		if(event.type == sf::Event::MouseWheelMoved)
		{
			m_camera.mouseWheelMoved(event.mouseWheel.delta);
		}
		else if(event.type == sf::Event::MouseButtonPressed)
		{
			if(m_raycast.collision)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					m_map.setVoxel(m_raycast.hit.pos, Voxel(voxel::AIR));
				}
				else if(event.mouseButton.button == sf::Mouse::Right)
				{
					m_map.setVoxel(m_raycast.hitPrevious.pos, Voxel(voxel::STONE));
				}
			}
		}
		else if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Key::Escape)
				m_camera.setEnabled(!m_camera.isEnabled());
		}
	}

	void PerspectiveMapViewer::updatePointedVoxel()
	{
		m_raycast =
			m_map.raycastToSolidVoxel(
				m_camera.getPosition(),
				m_camera.getForward(), 16);
	}

} // namespace zcraft




