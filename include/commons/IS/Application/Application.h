#pragma once

#include <OGRE/Ogre.h>
#include <IS/Utils/Time/Time.h>
#include <IS/Scene/Scene.h>
#include <IS/Physics/Core/Physics.h>
#include <CEGUI/Logger.h>

class CeguiNonLogger : public CEGUI::Logger{
   void logEvent (const CEGUI::String&, CEGUI::LoggingLevel){}
   void setLogFilename(const CEGUI::String&, bool){}
};

namespace IS
{
	class Application : public Ogre::FrameListener
	{
	public:
		Application();
		virtual ~Application();

		void	loop();
		virtual bool	frameStarted(const Ogre::FrameEvent &evt);
		virtual bool	frameRenderingQueued(const Ogre::FrameEvent &evt);

		static void	quit();
		static void replay();

	private:
		void	setupResources();

	private:
		static Ogre::Root			*_root;
		static Ogre::RenderWindow	*_window;
		static bool			_loop;
		IS::Utils::Time		*_time;
		static IS::Physics *_physicEngine;
		static IS::Scene::Scene	*_currentScene;
		static bool			_isReset;
		CeguiNonLogger 		_ceguiNonLogger;
	};
}