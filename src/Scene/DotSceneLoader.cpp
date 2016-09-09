#include <IS/Scene/DotSceneLoader.h>
#include <OGRE/Ogre.h>

void IS::Scene::DotSceneLoader::parseDotScene(const Ogre::String &SceneName, const Ogre::String &groupName, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode, const Ogre::String &sPrependNode)
{
	// set up shared object values
	m_sGroupName = groupName;
	mSceneMgr = yourSceneMgr;
	m_sPrependNode = sPrependNode;
	staticObjects.clear();
	dynamicObjects.clear();

	TiXmlDocument   *XMLDoc = 0;
	TiXmlElement   *XMLRoot;
	
	try
	{
		// Strip the path
		Ogre::String basename, path;
		Ogre::StringUtil::splitFilename(SceneName, basename, path);

		Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().
			openResource(basename, groupName);

		//DataStreamPtr pStream = ResourceGroupManager::getSingleton().
		//    openResource( SceneName, groupName );

		Ogre::String data = pStream->getAsString();
		// Open the .scene File
		XMLDoc = new TiXmlDocument();
		XMLDoc->Parse(data.c_str());
		pStream->close();
		pStream.setNull();

		if (XMLDoc->Error())
		{
			//We'll just log, and continue on gracefully
			Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] The TiXmlDocument reported an error");
			delete XMLDoc;
			return;
		}
	}
	catch (...)
	{
		//We'll just log, and continue on gracefully
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error creating TiXmlDocument");
		delete XMLDoc;
		return;
	}

	// Validate the File
	XMLRoot = XMLDoc->RootElement();
	if (Ogre::String(XMLRoot->Value()) != "scene") {
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error: Invalid .scene File. Missing <scene>");
		delete XMLDoc;
		return;
	}

	// figure out where to attach any nodes we create
	mAttachNode = pAttachNode;
	if (!mAttachNode)
		mAttachNode = mSceneMgr->getRootSceneNode();

	// Process the scene
	processScene(XMLRoot);

	// Close the XML File
	delete XMLDoc;
}

void IS::Scene::DotSceneLoader::processScene(TiXmlElement *XMLRoot)
{
	// Process the scene parameters
	Ogre::String version = getAttrib(XMLRoot, "formatVersion", "unknown");

	Ogre::String message = "[DotSceneLoader] Parsing dotScene file with version " + version;
	if (XMLRoot->Attribute("ID"))
		message += ", id " + Ogre::String(XMLRoot->Attribute("ID"));
	if (XMLRoot->Attribute("sceneManager"))
		message += ", scene manager " + Ogre::String(XMLRoot->Attribute("sceneManager"));
	if (XMLRoot->Attribute("minOgreVersion"))
		message += ", min. Ogre version " + Ogre::String(XMLRoot->Attribute("minOgreVersion"));
	if (XMLRoot->Attribute("author"))
		message += ", author " + Ogre::String(XMLRoot->Attribute("author"));

	Ogre::LogManager::getSingleton().logMessage(message);

	TiXmlElement *pElement;

	// Process nodes (?)
	pElement = XMLRoot->FirstChildElement("nodes");
	if (pElement)
		processNodes(pElement);

	// Process externals (?)
	pElement = XMLRoot->FirstChildElement("externals");
	if (pElement)
		processExternals(pElement);

	// Process environment (?)
	pElement = XMLRoot->FirstChildElement("environment");
	if (pElement)
		processEnvironment(pElement);

	// Process terrain (?)
	pElement = XMLRoot->FirstChildElement("terrain");
	if (pElement)
		processTerrain(pElement);

	// Process userDataReference (?)
	pElement = XMLRoot->FirstChildElement("userDataReference");
	if (pElement)
		processUserDataReference(pElement);

	// Process octree (?)
	pElement = XMLRoot->FirstChildElement("octree");
	if (pElement)
		processOctree(pElement);

	// Process light (?)
	pElement = XMLRoot->FirstChildElement("light");
	if (pElement)
		processLight(pElement);

	// Process camera (?)
	pElement = XMLRoot->FirstChildElement("camera");
	if (pElement)
		processCamera(pElement);
}

void IS::Scene::DotSceneLoader::processNodes(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process node (*)
	pElement = XMLNode->FirstChildElement("node");
	while (pElement)
	{
		processNode(pElement);
		pElement = pElement->NextSiblingElement("node");
	}

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if (pElement)
	{
		mAttachNode->setPosition(parseVector3(pElement));
		mAttachNode->setInitialState();
	}

	// Process rotation (?)
	pElement = XMLNode->FirstChildElement("rotation");
	if (pElement)
	{
		mAttachNode->setOrientation(parseQuaternion(pElement));
		mAttachNode->setInitialState();
	}

	// Process scale (?)
	pElement = XMLNode->FirstChildElement("scale");
	if (pElement)
	{
		mAttachNode->setScale(parseVector3(pElement));
		mAttachNode->setInitialState();
	}
}

void IS::Scene::DotSceneLoader::processExternals(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void IS::Scene::DotSceneLoader::processEnvironment(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process fog (?)
	pElement = XMLNode->FirstChildElement("fog");
	if (pElement)
		processFog(pElement);

	// Process skyBox (?)
	pElement = XMLNode->FirstChildElement("skyBox");
	if (pElement)
		processSkyBox(pElement);

	// Process skyDome (?)
	pElement = XMLNode->FirstChildElement("skyDome");
	if (pElement)
		processSkyDome(pElement);

	// Process skyPlane (?)
	pElement = XMLNode->FirstChildElement("skyPlane");
	if (pElement)
		processSkyPlane(pElement);

	// Process clipping (?)
	pElement = XMLNode->FirstChildElement("clipping");
	if (pElement)
		processClipping(pElement);

	// Process colourAmbient (?)
	pElement = XMLNode->FirstChildElement("colourAmbient");
	if (pElement)
		mSceneMgr->setAmbientLight(parseColour(pElement));

	// Process colourBackground (?)
	//! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
	pElement = XMLNode->FirstChildElement("colourBackground");
	if (pElement)
		;//mSceneMgr->set(parseColour(pElement));

		 // Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if (pElement)
		processUserDataReference(pElement);
}

void IS::Scene::DotSceneLoader::processTerrain(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void IS::Scene::DotSceneLoader::processUserDataReference(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	//! @todo Implement this
}

void IS::Scene::DotSceneLoader::processOctree(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void IS::Scene::DotSceneLoader::processLight(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");

	// Create the light
	Ogre::Light *pLight = mSceneMgr->createLight(name);
	if (pParent)
		pParent->attachObject(pLight);

	Ogre::String sValue = getAttrib(XMLNode, "type");
	if (sValue == "point")
		pLight->setType(Ogre::Light::LT_POINT);
	else if (sValue == "directional")
		pLight->setType(Ogre::Light::LT_DIRECTIONAL);
	else if (sValue == "spot")
		pLight->setType(Ogre::Light::LT_SPOTLIGHT);
	else if (sValue == "radPoint")
		pLight->setType(Ogre::Light::LT_POINT);

	pLight->setVisible(getAttribBool(XMLNode, "visible", true));
	pLight->setCastShadows(getAttribBool(XMLNode, "castShadows", true));

	TiXmlElement *pElement;

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if (pElement)
		pLight->setPosition(parseVector3(pElement));

	// Process normal (?)
	pElement = XMLNode->FirstChildElement("normal");
	if (pElement)
		pLight->setDirection(parseVector3(pElement));

	// Process colourDiffuse (?)
	pElement = XMLNode->FirstChildElement("colourDiffuse");
	if (pElement)
		pLight->setDiffuseColour(parseColour(pElement));

	// Process colourSpecular (?)
	pElement = XMLNode->FirstChildElement("colourSpecular");
	if (pElement)
		pLight->setSpecularColour(parseColour(pElement));

	// Process lightRange (?)
	pElement = XMLNode->FirstChildElement("lightRange");
	if (pElement)
		processLightRange(pElement, pLight);

	// Process lightAttenuation (?)
	pElement = XMLNode->FirstChildElement("lightAttenuation");
	if (pElement)
		processLightAttenuation(pElement, pLight);

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if (pElement)
		;//processUserDataReference(pElement, pLight);
}

void IS::Scene::DotSceneLoader::processCamera(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::Real fov = getAttribReal(XMLNode, "fov", 45);
	Ogre::Real aspectRatio = getAttribReal(XMLNode, "aspectRatio", 1.3333);
	Ogre::String projectionType = getAttrib(XMLNode, "projectionType", "perspective");

	// Create the camera
	Ogre::Camera *pCamera = mSceneMgr->createCamera(name);
	if (pParent)
		pParent->attachObject(pCamera);

	// Set the field-of-view
	//! @todo Is this always in degrees?
	pCamera->setFOVy(Ogre::Degree(fov));

	// Set the aspect ratio
	pCamera->setAspectRatio(aspectRatio);

	// Set the projection type
	if (projectionType == "perspective")
		pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
	else if (projectionType == "orthographic")
		pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

	TiXmlElement *pElement;

	// Process clipping (?)
	pElement = XMLNode->FirstChildElement("clipping");
	if (pElement)
	{
		Ogre::Real nearDist = getAttribReal(pElement, "near");
		pCamera->setNearClipDistance(nearDist);

		Ogre::Real farDist = getAttribReal(pElement, "far");
		pCamera->setFarClipDistance(farDist);
	}

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if (pElement)
		pCamera->setPosition(parseVector3(pElement));

	// Process rotation (?)
	pElement = XMLNode->FirstChildElement("rotation");
	if (pElement)
		pCamera->setOrientation(parseQuaternion(pElement));

	// Process normal (?)
	pElement = XMLNode->FirstChildElement("normal");
	if (pElement)
		;//!< @todo What to do with this element?

		 // Process lookTarget (?)
	pElement = XMLNode->FirstChildElement("lookTarget");
	if (pElement)
		;//!< @todo Implement the camera look target

		 // Process trackTarget (?)
	pElement = XMLNode->FirstChildElement("trackTarget");
	if (pElement)
		;//!< @todo Implement the camera track target

		 // Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if (pElement)
		;//!< @todo Implement the camera user data reference
}

void IS::Scene::DotSceneLoader::processNode(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Construct the node's name
	Ogre::String name = m_sPrependNode + getAttrib(XMLNode, "name");

	// Create the scene node
	Ogre::SceneNode *pNode;
	if (name.empty())
	{
		// Let Ogre choose the name
		if (pParent)
			pNode = pParent->createChildSceneNode();
		else
			pNode = mAttachNode->createChildSceneNode();
	}
	else
	{
		// Provide the name
		if (pParent)
			pNode = pParent->createChildSceneNode(name);
		else
			pNode = mAttachNode->createChildSceneNode(name);
	}

	// Process other attributes
	Ogre::String id = getAttrib(XMLNode, "id");
	bool isTarget = getAttribBool(XMLNode, "isTarget");

	TiXmlElement *pElement;

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if (pElement)
	{
		pNode->setPosition(parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process rotation (?)
	pElement = XMLNode->FirstChildElement("rotation");
	if (pElement)
	{
		pNode->setOrientation(parseQuaternion(pElement));
		pNode->setInitialState();
	}

	// Process scale (?)
	pElement = XMLNode->FirstChildElement("scale");
	if (pElement)
	{
		pNode->setScale(parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process lookTarget (?)
	pElement = XMLNode->FirstChildElement("lookTarget");
	if (pElement)
		processLookTarget(pElement, pNode);

	// Process trackTarget (?)
	pElement = XMLNode->FirstChildElement("trackTarget");
	if (pElement)
		processTrackTarget(pElement, pNode);

	// Process node (*)
	pElement = XMLNode->FirstChildElement("node");
	while (pElement)
	{
		processNode(pElement, pNode);
		pElement = pElement->NextSiblingElement("node");
	}

	// Process entity (*)
	pElement = XMLNode->FirstChildElement("entity");
	while (pElement)
	{
		processEntity(pElement, pNode);
		pElement = pElement->NextSiblingElement("entity");
	}

	// Process light (*)
	pElement = XMLNode->FirstChildElement("light");
	while (pElement)
	{
		processLight(pElement, pNode);
		pElement = pElement->NextSiblingElement("light");
	}

	// Process camera (*)
	pElement = XMLNode->FirstChildElement("camera");
	while (pElement)
	{
		processCamera(pElement, pNode);
		pElement = pElement->NextSiblingElement("camera");
	}

	// Process particleSystem (*)
	pElement = XMLNode->FirstChildElement("particleSystem");
	while (pElement)
	{
		processParticleSystem(pElement, pNode);
		pElement = pElement->NextSiblingElement("particleSystem");
	}

	// Process billboardSet (*)
	pElement = XMLNode->FirstChildElement("billboardSet");
	while (pElement)
	{
		processBillboardSet(pElement, pNode);
		pElement = pElement->NextSiblingElement("billboardSet");
	}

	// Process plane (*)
	pElement = XMLNode->FirstChildElement("plane");
	while (pElement)
	{
		processPlane(pElement, pNode);
		pElement = pElement->NextSiblingElement("plane");
	}

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if (pElement)
		processUserDataReference(pElement, pNode);
}

void IS::Scene::DotSceneLoader::processLookTarget(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	//! @todo Is this correct? Cause I don't have a clue actually

	// Process attributes
	Ogre::String nodeName = getAttrib(XMLNode, "nodeName");

	Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT;
	Ogre::String sValue = getAttrib(XMLNode, "relativeTo");
	if (sValue == "local")
		relativeTo = Ogre::Node::TS_LOCAL;
	else if (sValue == "parent")
		relativeTo = Ogre::Node::TS_PARENT;
	else if (sValue == "world")
		relativeTo = Ogre::Node::TS_WORLD;

	TiXmlElement *pElement;

	// Process position (?)
	Ogre::Vector3 position;
	pElement = XMLNode->FirstChildElement("position");
	if (pElement)
		position = parseVector3(pElement);

	// Process localDirection (?)
	Ogre::Vector3 localDirection = Ogre::Vector3::NEGATIVE_UNIT_Z;
	pElement = XMLNode->FirstChildElement("localDirection");
	if (pElement)
		localDirection = parseVector3(pElement);

	// Setup the look target
	try
	{
		if (!nodeName.empty())
		{
			Ogre::SceneNode *pLookNode = mSceneMgr->getSceneNode(nodeName);
			position = pLookNode->_getDerivedPosition();
		}

		pParent->lookAt(position, relativeTo, localDirection);
	}
	catch (Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error processing a look target!");
	}
}

void IS::Scene::DotSceneLoader::processTrackTarget(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String nodeName = getAttrib(XMLNode, "nodeName");

	TiXmlElement *pElement;

	// Process localDirection (?)
	Ogre::Vector3 localDirection = Ogre::Vector3::NEGATIVE_UNIT_Z;
	pElement = XMLNode->FirstChildElement("localDirection");
	if (pElement)
		localDirection = parseVector3(pElement);

	// Process offset (?)
	Ogre::Vector3 offset = Ogre::Vector3::ZERO;
	pElement = XMLNode->FirstChildElement("offset");
	if (pElement)
		offset = parseVector3(pElement);

	// Setup the track target
	try
	{
		Ogre::SceneNode *pTrackNode = mSceneMgr->getSceneNode(nodeName);
		pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
	}
	catch (Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error processing a track target!");
	}
}

void IS::Scene::DotSceneLoader::processEntity(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::String meshFile = getAttrib(XMLNode, "meshFile");
	Ogre::String materialFile = getAttrib(XMLNode, "materialFile");
	bool isStatic = getAttribBool(XMLNode, "static", false);;
	bool castShadows = getAttribBool(XMLNode, "castShadows", true);

	// TEMP: Maintain a list of static and dynamic objects
	if (isStatic)
		staticObjects.push_back(name);
	else
		dynamicObjects.push_back(name);

	TiXmlElement *pElement;

	// Process vertexBuffer (?)
	pElement = XMLNode->FirstChildElement("vertexBuffer");
	if (pElement)
		;//processVertexBuffer(pElement);

		 // Process indexBuffer (?)
	pElement = XMLNode->FirstChildElement("indexBuffer");
	if (pElement)
		;//processIndexBuffer(pElement);

		 // Create the entity
	Ogre::Entity *pEntity = 0;
	try
	{
		Ogre::MeshManager::getSingleton().load(meshFile, m_sGroupName);
		pEntity = mSceneMgr->createEntity(name, meshFile);
		pEntity->setCastShadows(castShadows);
		pParent->attachObject(pEntity);
		_entities.push_back(pEntity);

		if (!materialFile.empty())
			pEntity->setMaterialName(materialFile);
	}
	catch (Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error loading an entity!");
	}

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if (pElement)
		processUserDataReference(pElement, pEntity);


}

void IS::Scene::DotSceneLoader::processParticleSystem(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::String file = getAttrib(XMLNode, "file");

	// Create the particle system
	try
	{
		Ogre::ParticleSystem *pParticles = mSceneMgr->createParticleSystem(name, file);
		pParent->attachObject(pParticles);
	}
	catch (Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error creating a particle system!");
	}
}

void IS::Scene::DotSceneLoader::processBillboardSet(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	//! @todo Implement this
}

void IS::Scene::DotSceneLoader::processPlane(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	//! @todo Implement this
}

void IS::Scene::DotSceneLoader::processFog(TiXmlElement *XMLNode)
{
	// Process attributes
	Ogre::Real expDensity = getAttribReal(XMLNode, "expDensity", 0.001);
	Ogre::Real linearStart = getAttribReal(XMLNode, "linearStart", 0.0);
	Ogre::Real linearEnd = getAttribReal(XMLNode, "linearEnd", 1.0);

	Ogre::FogMode mode = Ogre::FOG_NONE;
	Ogre::String sMode = getAttrib(XMLNode, "mode");
	if (sMode == "none")
		mode = Ogre::FOG_NONE;
	else if (sMode == "exp")
		mode = Ogre::FOG_EXP;
	else if (sMode == "exp2")
		mode = Ogre::FOG_EXP2;
	else if (sMode == "linear")
		mode = Ogre::FOG_LINEAR;

	TiXmlElement *pElement;

	// Process colourDiffuse (?)
	Ogre::ColourValue colourDiffuse = Ogre::ColourValue::White;
	pElement = XMLNode->FirstChildElement("colourDiffuse");
	if (pElement)
		colourDiffuse = parseColour(pElement);

	// Setup the fog
	mSceneMgr->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
}

void IS::Scene::DotSceneLoader::processSkyBox(TiXmlElement *XMLNode)
{
	// Process attributes
	Ogre::String material = getAttrib(XMLNode, "material");
	Ogre::Real distance = getAttribReal(XMLNode, "distance", 5000);
	bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	TiXmlElement *pElement;

	// Process rotation (?)
	Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
	pElement = XMLNode->FirstChildElement("rotation");
	if (pElement)
		rotation = parseQuaternion(pElement);

	// Setup the sky box
	mSceneMgr->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
}

void IS::Scene::DotSceneLoader::processSkyDome(TiXmlElement *XMLNode)
{
	// Process attributes
	Ogre::String material = XMLNode->Attribute("material");
	Ogre::Real curvature = getAttribReal(XMLNode, "curvature", 10);
	Ogre::Real tiling = getAttribReal(XMLNode, "tiling", 8);
	Ogre::Real distance = getAttribReal(XMLNode, "distance", 4000);
	bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	TiXmlElement *pElement;

	// Process rotation (?)
	Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
	pElement = XMLNode->FirstChildElement("rotation");
	if (pElement)
		rotation = parseQuaternion(pElement);

	// Setup the sky dome
	mSceneMgr->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation, 16, 16, -1, m_sGroupName);
}

void IS::Scene::DotSceneLoader::processSkyPlane(TiXmlElement *XMLNode)
{
	// Process attributes
	Ogre::String material = getAttrib(XMLNode, "material");
	Ogre::Real planeX = getAttribReal(XMLNode, "planeX", 0);
	Ogre::Real planeY = getAttribReal(XMLNode, "planeY", -1);
	Ogre::Real planeZ = getAttribReal(XMLNode, "planeX", 0);
	Ogre::Real planeD = getAttribReal(XMLNode, "planeD", 5000);
	Ogre::Real scale = getAttribReal(XMLNode, "scale", 1000);
	Ogre::Real bow = getAttribReal(XMLNode, "bow", 0);
	Ogre::Real tiling = getAttribReal(XMLNode, "tiling", 10);
	bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	// Setup the sky plane
	Ogre::Plane plane;
	plane.normal = Ogre::Vector3(planeX, planeY, planeZ);
	plane.d = planeD;
	mSceneMgr->setSkyPlane(true, plane, material, scale, tiling, drawFirst, bow, 1, 1, m_sGroupName);
}

void IS::Scene::DotSceneLoader::processClipping(TiXmlElement *XMLNode)
{
	//! @todo Implement this

	// Process attributes
	Ogre::Real fNear = getAttribReal(XMLNode, "near", 0);
	Ogre::Real fFar = getAttribReal(XMLNode, "far", 1);
}

void IS::Scene::DotSceneLoader::processLightRange(TiXmlElement *XMLNode, Ogre::Light *pLight)
{
	// Process attributes
	Ogre::Real inner = getAttribReal(XMLNode, "inner");
	Ogre::Real outer = getAttribReal(XMLNode, "outer");
	Ogre::Real falloff = getAttribReal(XMLNode, "falloff", 1.0);

	// Setup the light range
	pLight->setSpotlightRange(Ogre::Angle(inner), Ogre::Angle(outer), falloff);
}

void IS::Scene::DotSceneLoader::processLightAttenuation(TiXmlElement *XMLNode, Ogre::Light *pLight)
{
	// Process attributes
	Ogre::Real range = getAttribReal(XMLNode, "range");
	Ogre::Real constant = getAttribReal(XMLNode, "constant");
	Ogre::Real linear = getAttribReal(XMLNode, "linear");
	Ogre::Real quadratic = getAttribReal(XMLNode, "quadratic");

	// Setup the Ogre::Light attenuation
	pLight->setAttenuation(range, constant, linear, quadratic);
}

Ogre::String IS::Scene::DotSceneLoader::getAttrib(TiXmlElement *XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue)
{
	if (XMLNode->Attribute(attrib.c_str()))
		return XMLNode->Attribute(attrib.c_str());
	else
		return defaultValue;
}

Ogre::Real IS::Scene::DotSceneLoader::getAttribReal(TiXmlElement *XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue)
{
	if (XMLNode->Attribute(attrib.c_str()))
		return Ogre::StringConverter::parseReal(XMLNode->Attribute(attrib.c_str()));
	else
		return defaultValue;
}

bool IS::Scene::DotSceneLoader::getAttribBool(TiXmlElement *XMLNode, const Ogre::String &attrib, bool defaultValue)
{
	if (!XMLNode->Attribute(attrib.c_str()))
		return defaultValue;

	if (Ogre::String(XMLNode->Attribute(attrib.c_str())) == "true")
		return true;

	return false;
}

Ogre::Vector3 IS::Scene::DotSceneLoader::parseVector3(TiXmlElement *XMLNode)
{
	return Ogre::Vector3(
		Ogre::StringConverter::parseReal(XMLNode->Attribute("x")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("y")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("z"))
		);
}

Ogre::Quaternion IS::Scene::DotSceneLoader::parseQuaternion(TiXmlElement *XMLNode)
{
	//! @todo Fix this crap!

	Ogre::Quaternion orientation;

	if (XMLNode->Attribute("qx"))
	{
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->Attribute("qx"));
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->Attribute("qy"));
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->Attribute("qz"));
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->Attribute("qw"));
	}
	else if (XMLNode->Attribute("axisX"))
	{
		Ogre::Vector3 axis;
		axis.x = Ogre::StringConverter::parseReal(XMLNode->Attribute("axisX"));
		axis.y = Ogre::StringConverter::parseReal(XMLNode->Attribute("axisY"));
		axis.z = Ogre::StringConverter::parseReal(XMLNode->Attribute("axisZ"));
		Ogre::Real angle = Ogre::StringConverter::parseReal(XMLNode->Attribute("angle"));;
		orientation.FromAngleAxis(Ogre::Angle(angle), axis);
	}
	else if (XMLNode->Attribute("angleX"))
	{
		Ogre::Vector3 axis;
		axis.x = Ogre::StringConverter::parseReal(XMLNode->Attribute("angleX"));
		axis.y = Ogre::StringConverter::parseReal(XMLNode->Attribute("angleY"));
		axis.z = Ogre::StringConverter::parseReal(XMLNode->Attribute("angleZ"));
		//orientation.FromAxes(&axis);
		//orientation.F
	}

	return orientation;
}

Ogre::ColourValue IS::Scene::DotSceneLoader::parseColour(TiXmlElement *XMLNode)
{
	return Ogre::ColourValue(
		Ogre::StringConverter::parseReal(XMLNode->Attribute("r")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("g")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("b")),
		XMLNode->Attribute("a") != NULL ? Ogre::StringConverter::parseReal(XMLNode->Attribute("a")) : 1
		);
}

Ogre::String IS::Scene::DotSceneLoader::getProperty(const Ogre::String &ndNm, const Ogre::String &prop)
{
	for (unsigned int i = 0; i < nodeProperties.size(); i++)
	{
		if (nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop)
		{
			return nodeProperties[i].valueName;
		}
	}

	return "";
}

void IS::Scene::DotSceneLoader::processUserDataReference(TiXmlElement *XMLNode, Ogre::Entity *pEntity)
{
	Ogre::String str = XMLNode->Attribute("id");
	pEntity->getUserObjectBindings().setUserAny(Ogre::Any(str));
}