#include <IS/Application/Application.h>
#include <IS/Physics/Core/PhysicObject.h>
#include <iostream>
#include <csignal>

void sig_hang(int s)
{
	std::cerr << "Impossible to find a plugin" << std::endl;
	(void)s;
	exit(0);
}

int	main()
{
	IS::Application	app;

	std::signal(SIGABRT, &sig_hang);
	try
	{
		app.loop();
	}
	catch (Ogre::Exception &e)
	{
		std::cerr << "And error has occured : " << e.getFullDescription().c_str() << std::endl;
	}
	catch (...)
	{
	  std::cout << "CEGUI error." << std::endl;
	}
	return 0;
}
