#pragma once

#include <IS/Scene/Scene.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/CEGUI.h>
#include <IS/Map/Map.hpp>
#include <IS/GameObject/Components/Rule/Rule.h>

namespace IS
{
	namespace Scenes
	{
		class Menu : public IS::Scene::Scene
		{
		public:
			Menu(Ogre::Root *root, Ogre::RenderWindow *renderWindow, bool isSplash);
			virtual ~Menu();

			virtual void update();
			void init();
		protected :
			CEGUI::OgreRenderer	*_renderCegui;

		private:
			float timer;
			bool							good;
			Ogre::Root						*_root;
			Ogre::RenderWindow				*_renderWindow;
			std::shared_ptr<IS::Map>		 _map;
			IS::GameObject::Component::Rule *_rule;
			Ogre::ParticleSystem* particleSystem;
		};
	}
}