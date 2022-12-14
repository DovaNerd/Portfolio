#include "SceningTest.h"
#include "Cars.h"
#include "Obstacles.h"


SceningTest::SceningTest(GLFWwindow* inWind)
{
	window = inWind;
}

void SceningTest::Start()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	rampTex = Texture2D::LoadFromFile("images/tinyRamp.png");

	sceneBuff = m_Registry.create();
	cocoBuff = m_Registry.create();
	bloomBuff = m_Registry.create();
	blurBuff = m_Registry.create();
	gBuff = m_Registry.create();
	illumBuff = m_Registry.create();
	shadowBuff = m_Registry.create();
	pixelBuff = m_Registry.create();
	nightVisBuff = m_Registry.create();
	grainBuff = m_Registry.create();

	m_Registry.emplace<PostEffect>(sceneBuff);
	m_Registry.emplace<CubeCoCoEffect>(cocoBuff);
	m_Registry.emplace<CombinedBloom>(bloomBuff);
	m_Registry.emplace<Blur>(blurBuff);
	m_Registry.emplace<GBuffer>(gBuff);
	m_Registry.emplace<IlluminationBuffer>(illumBuff);
	m_Registry.emplace<Framebuffer>(shadowBuff);
	m_Registry.emplace<Pixelate>(pixelBuff);
	m_Registry.emplace<NightVision>(nightVisBuff);
	m_Registry.emplace<FilmGrain>(grainBuff);

	m_Registry.get<PostEffect>(sceneBuff).Init(width, height);
	m_Registry.get<CubeCoCoEffect>(cocoBuff).Init(width, height);
	m_Registry.get<CombinedBloom>(bloomBuff).Init(width, height);
	m_Registry.get<Blur>(blurBuff).Init(width, height);
	m_Registry.get<GBuffer>(gBuff).Init(width, height);
	m_Registry.get<IlluminationBuffer>(illumBuff).Init(width, height);
	m_Registry.get<Framebuffer>(shadowBuff).AddDepthTarget();
	m_Registry.get<Framebuffer>(shadowBuff).Init(shadowWidth, shadowHeight);
	m_Registry.get<Pixelate>(pixelBuff).Init(width, height);
	m_Registry.get<NightVision>(nightVisBuff).Init(width, height);
	m_Registry.get<FilmGrain>(grainBuff).Init(width, height);

	Sun._ambientPow = 0.2;
	Sun._ambientCol = glm::vec4(1.0, 1.0, 1.0,1.0);
	Sun._lightAmbientPow = 0.2;
	Sun._lightCol=glm::vec4(1.0, 1.0, 1.0,1.0);
	Sun._lightDirection = glm::vec4(1.370,2.760,-3.090,0.0);
	Sun._lightSpecularPow = 0.7;
	Sun._shadowBias = 0.05;
	
	m_Registry.get<IlluminationBuffer>(illumBuff).SetSun(Sun);

	m_Registry.get<CombinedBloom>(bloomBuff).SetThreshold(0.7);
	m_Registry.get<CombinedBloom>(bloomBuff).SetPasses(10);


	cubes.push_back(LUT3D("cubes/Neutral-512.cube"));
	cubes.push_back(LUT3D("cubes/Cool.cube"));
	cubes.push_back(LUT3D("cubes/Warm.cube"));
	cubes.push_back(LUT3D("cubes/Darken-512.cube"));

	AudioEngine& engine = AudioEngine::Instance();
	

	//play event
	AudioEvent& oldMusic = engine.GetEvent("Menu Music");
	oldMusic.Stop();

	AudioEvent& newMusic = engine.GetEvent("Ambient");
	newMusic.Play();

	AudioEvent& slipstream = engine.GetEvent("Slipstream");
	AudioEvent& drift = engine.GetEvent("Drift");
	AudioEvent& multiNitro = engine.GetEvent("MultiNitro");

	slipstream.StopImmediately();
	drift.StopImmediately();
	multiNitro.StopImmediately();






	camera = Camera::Create();
	std::string fileName = "monkey.obj";
	entt::entity testModel = m_Registry.create();
	entt::entity shader = m_Registry.create();
	m_PCar = m_Registry.create();
	m_enemy = m_Registry.create();
	m_Obstacle = m_Registry.create();
	entt::entity Track = m_Registry.create();
	m_Hazard = m_Registry.create();
	m_Gearbox = m_Registry.create();
	m_Gearbox2 = m_Registry.create();
	m_PGears = m_Registry.create();
	m_EGears = m_Registry.create();
	m_HBox = m_Registry.create();
	m_Htex = m_Registry.create();
	m_Hnumber = m_Registry.create();
	m_Pscore = m_Registry.create();
	m_Escore = m_Registry.create();
	m_Endbutton = m_Registry.create();
	m_A1 = m_Registry.create();
	m_A2 = m_Registry.create();
	//cards
	m_Card = m_Registry.create();
	m_TransparentBlack = m_Registry.create();
	m_PauseMenu = m_Registry.create();

#pragma region Track Objs creation

	//track and scenery
	m_Registry.emplace<syre::Mesh>(Track, "objects/Track1New.obj");
	m_Registry.emplace<syre::Transform>(Track, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(Track, "images/PossibleRoad.png");

	m_Registry.emplace<syre::Mesh>(m_Hazard, "objects/RoadHazard.obj");
	m_Registry.emplace<syre::Transform>(m_Hazard, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Hazard, "images/Apex.png");

	m_Registry.emplace<syre::Mesh>(m_Gearbox, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_Gearbox, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Gearbox, "images/GearBoxNeutral.png");

	m_Registry.emplace<syre::Mesh>(m_Gearbox2, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_Gearbox2, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Gearbox2, "images/New_Gearbox.png");

	m_Registry.emplace<syre::Mesh>(m_TransparentBlack, "objects/RoadHazard.obj");
	m_Registry.emplace<syre::Transform>(m_TransparentBlack, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_TransparentBlack, "images/TransparentBlack.png");

	m_Registry.emplace<syre::Mesh>(m_PauseMenu, "objects/RoadHazard.obj");
	m_Registry.emplace<syre::Transform>(m_PauseMenu, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_PauseMenu, "images/PauseMenu.png");

	m_Registry.emplace<syre::Mesh>(m_PGears, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_PGears, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_PGears, "images/PauseMenu.png");

	m_Registry.emplace<syre::Mesh>(m_EGears, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_EGears, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_EGears, "images/PauseMenu.png");
	
	m_Registry.emplace<syre::Mesh>(m_HBox, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_HBox, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_HBox, "images/HUD_Info_box.png");

	m_Registry.emplace<syre::Mesh>(m_Htex, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_Htex, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Htex, "images/Hairpin_HUD.png");

	m_Registry.emplace<syre::Mesh>(m_Hnumber, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_Hnumber, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Hnumber, "images/O1.png");

	m_Registry.emplace<syre::Mesh>(m_Pscore, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_Pscore, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Pscore, "images/Progress Bar.png");

	m_Registry.emplace<syre::Mesh>(m_Escore, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_Escore, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Escore, "images/Progress Bar.png");

	m_Registry.emplace<syre::Mesh>(m_Endbutton, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_Endbutton, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Endbutton, "images/EndTurn.png");

	m_Registry.emplace<syre::Mesh>(m_A1, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_A1, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_A1, "images/EndTurn.png");

	m_Registry.emplace<syre::Mesh>(m_A2, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_A2, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_A2, "images/EndTurn.png");

	entt::entity start = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(start);
	m_Registry.emplace<syre::Transform>(start, glm::vec3(15.0f, -50.0f, 0.0f), glm::vec3(90.0f, 0.0f, 190.0f), glm::vec3(1.0f));
	m_Registry.emplace<syre::Texture>(start, "images/START.png");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START1.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START1.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START2.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START3.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START4.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START5.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START6.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START7.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START8.obj");
	

	entt::entity finish = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(finish);
	m_Registry.emplace<syre::Transform>(finish, glm::vec3(-270.0f, -135.0f, 0.0f), glm::vec3(90.0f, 0.0f, 150.0f), glm::vec3(1.0f));
	m_Registry.emplace<syre::Texture>(finish, "images/Finish.png");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH1.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH1.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH2.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH3.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH4.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH5.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH6.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH7.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH8.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH9.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH10.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH11.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH12.obj");



	
	//trees
	entt::entity trackTrees = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackTrees, "objects/TreesMap.obj");
	m_Registry.emplace<syre::Transform>(trackTrees, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackTrees, "images/Tree.png");

	//terrain
	entt::entity trackTerrain = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackTerrain, "objects/TerrainMap.obj");
	m_Registry.emplace<syre::Transform>(trackTerrain, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackTerrain, "images/Terrain.png");

	//signs
	entt::entity trackSigns = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackSigns, "objects/SignsMap.obj");
	m_Registry.emplace<syre::Transform>(trackSigns, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackSigns, "images/Signs.png");

	//sharp rocks
	entt::entity trackSharpRocks = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackSharpRocks, "objects/SharpRockMap.obj");
	m_Registry.emplace<syre::Transform>(trackSharpRocks, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackSharpRocks, "images/SharpRock.png");

	//round rocks
	entt::entity trackRoundRocks = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackRoundRocks, "objects/RoundRockMap.obj");
	m_Registry.emplace<syre::Transform>(trackRoundRocks, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackRoundRocks, "images/RoundRock.png");

	//flowers
	entt::entity trackFlowers = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackFlowers, "objects/FlowersMap.obj");
	m_Registry.emplace<syre::Transform>(trackFlowers, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackFlowers, "images/Flower.png");

	//bush
	entt::entity trackBush = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackBush, "objects/BushMap.obj");
	m_Registry.emplace<syre::Transform>(trackBush, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackBush, "images/Bush.png");
	
	entt::entity butterflies = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(butterflies);
	m_Registry.get<syre::MorphRenderer>(butterflies).AddFrame("objects/ButterflyNeutral.obj");
	m_Registry.get<syre::MorphRenderer>(butterflies).AddFrame("objects/ButterflyDown.obj");
	m_Registry.get<syre::MorphRenderer>(butterflies).AddFrame("objects/ButterflyNeutral.obj");
	m_Registry.get<syre::MorphRenderer>(butterflies).AddFrame("objects/ButterflyUp.obj");

	m_Registry.emplace<syre::Texture>(butterflies, "images/butterfly.png");
	m_Registry.emplace<syre::TransformList>(butterflies);
	m_Registry.get<syre::TransformList>(butterflies).SetDefaultRot(glm::vec3(90.0f, -70.0f, 180.0f));
	m_Registry.get<syre::TransformList>(butterflies).SetDefaultSca(glm::vec3(0.2f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(0.0f, 0.0f, 0.0f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(3.0f, -70.0f, 1.0f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(35.0f, -97.f, 0.0f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(110.0f, -104.f, 0.0f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(153.0f, -205.f, 0.0f));

	entt::entity swayingTree = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(swayingTree);
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying1.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying2.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying3.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying4.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying5.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying6.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying7.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying8.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("objects/TreeSwaying9.obj");

	m_Registry.emplace<syre::Texture>(swayingTree, "images/Tree.png");
	m_Registry.emplace<syre::TransformList>(swayingTree);
	m_Registry.get<syre::TransformList>(swayingTree).SetDefaultRot(glm::vec3(90.0f, 0.0f, 0.0f));
	m_Registry.get<syre::TransformList>(swayingTree).SetDefaultSca(glm::vec3(1.0f));
	m_Registry.get<syre::TransformList>(swayingTree).AddPos(glm::vec3(-1.0f, -80.0f, 0.0f));

#pragma endregion





	m_Registry.emplace<Obstacles>(m_Obstacle);

	
	m_Registry.emplace<Cars>(m_PCar);
	m_Registry.emplace<syre::Mesh>(m_PCar, "objects/Car2.obj");
	m_Registry.emplace<syre::Transform>(m_PCar, glm::vec3(6.0f, 0.0f, 0.0f),glm::vec3(90.f,0.0f,0.0f),glm::vec3(1.0f));
	m_Registry.emplace<syre::Texture>(m_PCar, "images/Car2.png");
	m_Registry.emplace<syre::PathAnimator>(m_PCar,syre::PathType::BEZIER);
	auto& carPath = m_Registry.get<syre::PathAnimator>(m_PCar);

#pragma region Player routing

	carPath.AddPoint(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //start point
	carPath.AddPoint(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(6.0f, -70.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of first straight
	carPath.AddPoint(glm::vec3(6.0f, -70.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(10.0f, -100.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(35.0f, -100.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //end of first turn
	carPath.AddPoint(glm::vec3(35.0f, -100.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(35.0f, -100.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(110.0f, -101.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //end of 2nd straight 
	carPath.AddPoint(glm::vec3(110.0f, -101.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(155.0f, -85.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(155.0f, -140.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of 2nd turn 
	carPath.AddPoint(glm::vec3(155.0f, -140.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(155.0f, -140.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(155.0f, -205.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of 3rd str 
	carPath.AddPoint(glm::vec3(155.0f, -205.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(145.0f, -247.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(205.0f, -247.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //mid s  
	carPath.AddPoint(glm::vec3(205.0f, -247.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(235.0f, -230.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(236.f, -286.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of S 
	carPath.AddPoint(glm::vec3(236.f, -286.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(236.f, -286.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(236.f, -420.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of str
	carPath.AddPoint(glm::vec3(236.f, -420.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(240.f, -458.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(198.f, -458.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //mid u turn
	carPath.AddPoint(glm::vec3(198.f, -458.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(151.f, -466.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(161.f, -416.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //end u turn
	carPath.AddPoint(glm::vec3(161.f, -416.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(164.f, -381.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(134.f, -381.f, 0.0f), glm::vec3(90.0f, 0.0f, -100.0f)); //end turn 
	carPath.AddPoint(glm::vec3(134.f, -381.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(134.f, -381.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(63.f, -373.f, 0.0f), glm::vec3(90.0f, 0.0f, -100.0f)); //str
	carPath.AddPoint(glm::vec3(63.f, -373.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(24.f, -370.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(24.f, -400.f, 0.0f), glm::vec3(90.0f, 0.0f, -10.0f)); //turn
	carPath.AddPoint(glm::vec3(24.f, -400.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(24.f, -400.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(16.f, -470.f, 0.0f), glm::vec3(90.0f, 0.0f, -10.0f)); //str
	carPath.AddPoint(glm::vec3(16.f, -470.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(20.f, -502.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-26.f, -502.f, 0.0f), glm::vec3(90.0f, 0.0f, -100.0f)); //mid s 
	carPath.AddPoint(glm::vec3(-26.f, -502.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-73.f, -490.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-73.f, -537.f, 0.0f), glm::vec3(90.0f, 0.0f, -10.0f)); //end s 
	carPath.AddPoint(glm::vec3(-73.f, -537.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-64.f, -570.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-114.f, -570.f, 0.0f), glm::vec3(90.0f, 0.0f, -100.0f)); //mid u 
	carPath.AddPoint(glm::vec3(-114.f, -570.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-148.f, -580.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-148.f, -529.f, 0.0f), glm::vec3(90.0f, 0.0f, -190.0f)); //end u turn 
	carPath.AddPoint(glm::vec3(-148.f, -529.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-148.f, -529.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-130.f, -393.f, 0.0f), glm::vec3(90.0f, 0.0f, -190.0f)); //str
	carPath.AddPoint(glm::vec3(-130.f, -393.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-138.f, -350.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-88.f, -365.f, 0.0f), glm::vec3(90.0f, 0.0f, -280.0f)); //mid u 
	carPath.AddPoint(glm::vec3(-88.f, -365.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-70.f, -360.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-74.f, -405.f, 0.0f), glm::vec3(90.0f, 0.0f, -10.0f)); //end u 
	carPath.AddPoint(glm::vec3(-74.f, -405.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-76.f, -448.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-56.f, -448.f, 0.0f), glm::vec3(90.0f, 0.0f, 80.0f)); //mid u 
	carPath.AddPoint(glm::vec3(-56.f, -448.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-9.f, -460.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-9.f, -415.f, 0.0f), glm::vec3(90.0f, 0.0f, 170.0f)); //end u 
	carPath.AddPoint(glm::vec3(-9.f, -415.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-9.f, -415.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(1.5f, -346.f, 0.0f), glm::vec3(90.0f, 0.0f, 170.0f)); //str
	carPath.AddPoint(glm::vec3(1.5f, -346.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(30.f, -310.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-64.f, -289.f, 0.0f), glm::vec3(90.0f, 0.0f, 260.0f)); //turn 
	carPath.AddPoint(glm::vec3(-64.f, -289.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-64.f, -289.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-117.f, -286.f, 0.0f), glm::vec3(90.0f, 0.0f, 265.0f)); //str
	carPath.AddPoint(glm::vec3(-117.f, -286.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-133.f, -285.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-133.f, -252.f, 0.0f), glm::vec3(90.0f, 0.0f, 180.0f)); // mid u 
	carPath.AddPoint(glm::vec3(-133.f, -252.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-147.f, -219.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-107.f, -214.f, 0.0f), glm::vec3(90.0f, 0.0f, 100.0f)); //end u 
	carPath.AddPoint(glm::vec3(-107.f, -214.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-107.f, -214.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-34.f, -210.f, 0.0f), glm::vec3(90.0f, 0.0f, 100.0f)); //str
	carPath.AddPoint(glm::vec3(-34.f, -210.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(15.f, -220.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(8.f, -160.f, 0.0f), glm::vec3(90.0f, 0.0f, 190.0f)); //mid u
	carPath.AddPoint(glm::vec3(8.f, -160.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(16.f, -120.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-36.f, -125.f, 0.0f), glm::vec3(90.0f, 0.0f, 270.0f)); //end u
	carPath.AddPoint(glm::vec3(-36.f, -125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-36.f, -125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-108.f, -129.f, 0.0f), glm::vec3(90.0f, 0.0f, 270.0f)); //str
	carPath.AddPoint(glm::vec3(-108.f, -129.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-150.f, -133.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-145.f, -93.f, 0.0f), glm::vec3(90.0f, 0.0f, 180.0f)); //turn ///////
	carPath.AddPoint(glm::vec3(-145.f, -93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-145.f, -93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-145.f, -25.f, 0.0f), glm::vec3(90.0f, 0.0f, 175.0f)); //str
	carPath.AddPoint(glm::vec3(-145.f, -25.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-125.f, 14.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-195.f, 14.f, 0.0f), glm::vec3(90.0f, 0.0f, 270.0f)); //mid u////////////
	carPath.AddPoint(glm::vec3(-195.f, 14.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-220.f, 13.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-226.f, -3.f, 0.0f), glm::vec3(90.0f, 0.0f, 340.0f)); //end u ///////
	carPath.AddPoint(glm::vec3(-226.f, -3.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-226.f, -3.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-276.f, -119.f, 0.0f), glm::vec3(90.0f, 0.0f, 337.0f)); //str
	carPath.AddPoint(glm::vec3(-276.f, -119.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-276.f, -119.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.SpeedControl();
#pragma endregion

	m_Registry.emplace<Cars>(m_enemy);
	m_Registry.emplace<syre::Mesh>(m_enemy, "objects/Car2.obj");
	m_Registry.emplace<syre::Transform>(m_enemy, glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(90.f, 0.0f, 0.0f), glm::vec3(1.0f));
	m_Registry.emplace<syre::Texture>(m_enemy, "images/Car2-Blue.png");
	m_Registry.emplace<syre::PathAnimator>(m_enemy, syre::PathType::BEZIER);
	auto& enemyCarPath = m_Registry.get<syre::PathAnimator>(m_enemy);

#pragma region EnemyCar routing

	enemyCarPath.AddPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //start point
	enemyCarPath.AddPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(10.0f, -70.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of first straight
	enemyCarPath.AddPoint(glm::vec3(10.0f, -70.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(14.0f, -100.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(35.0f, -96.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //end of first turn
	enemyCarPath.AddPoint(glm::vec3(35.0f, -96.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(35.0f, -96.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(110.0f, -97.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //end of 2nd straight 
	enemyCarPath.AddPoint(glm::vec3(110.0f, -97.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(159.0f, -81.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(159.0f, -140.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of 2nd turn 
	enemyCarPath.AddPoint(glm::vec3(159.0f, -140.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(159.0f, -140.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(159.0f, -205.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of 3rd str 
	enemyCarPath.AddPoint(glm::vec3(159.0f, -205.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(149.0f, -243.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(205.0f, -243.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //mid s  
	enemyCarPath.AddPoint(glm::vec3(205.0f, -243.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(235.0f, -230.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(240.f, -286.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of S 
	enemyCarPath.AddPoint(glm::vec3(240.f, -286.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(240.f, -286.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(240.f, -420.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of str
	enemyCarPath.AddPoint(glm::vec3(240.f, -420.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(244.f, -458.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));            ///////

	enemyCarPath.AddPoint(glm::vec3(198.f, -462.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //mid u turn
	enemyCarPath.AddPoint(glm::vec3(198.f, -462.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(151.f, -470.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(157.f, -416.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //end u turn
	enemyCarPath.AddPoint(glm::vec3(157.f, -416.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(160.f, -381.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(134.f, -385.f, 0.0f), glm::vec3(90.0f, 0.0f, -100.0f)); //end turn 
	enemyCarPath.AddPoint(glm::vec3(134.f, -385.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(134.f, -385.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(63.f, -377.f, 0.0f), glm::vec3(90.0f, 0.0f, -100.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(63.f, -377.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(28.f, -374.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));      ////////

	enemyCarPath.AddPoint(glm::vec3(28.f, -400.f, 0.0f), glm::vec3(90.0f, 0.0f, -10.0f)); //turn
	enemyCarPath.AddPoint(glm::vec3(28.f, -400.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(28.f, -400.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(20.f, -470.f, 0.0f), glm::vec3(90.0f, 0.0f, -10.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(20.f, -470.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(24.f, -503.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-26.f, -506.f, 0.0f), glm::vec3(90.0f, 0.0f, -100.0f)); //mid s
	enemyCarPath.AddPoint(glm::vec3(-26.f, -506.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-77.f, -495.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-68.f, -537.f, 0.0f), glm::vec3(90.0f, 0.0f, -10.0f)); //end s
	enemyCarPath.AddPoint(glm::vec3(-68.f, -537.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-60.f, -570.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));   ///

	enemyCarPath.AddPoint(glm::vec3(-114.f, -574.f, 0.0f), glm::vec3(90.0f, 0.0f, -100.0f)); //mid u 
	enemyCarPath.AddPoint(glm::vec3(-114.f, -574.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-152.f, -582.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-152.f, -529.f, 0.0f), glm::vec3(90.0f, 0.0f, -190.0f)); //end u turn 
	enemyCarPath.AddPoint(glm::vec3(-152.f, -529.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-152.f, -529.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));    /////

	enemyCarPath.AddPoint(glm::vec3(-134.f, -393.f, 0.0f), glm::vec3(90.0f, 0.0f, -190.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-134.f, -393.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-142.f, -350.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-88.f, -361.f, 0.0f), glm::vec3(90.0f, 0.0f, -280.0f)); //mid u 
	enemyCarPath.AddPoint(glm::vec3(-88.f, -361.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-64.f, -356.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-70.f, -405.f, 0.0f), glm::vec3(90.0f, 0.0f, -10.0f)); //end u 
	enemyCarPath.AddPoint(glm::vec3(-70.f, -405.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-72.f, -450.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-56.f, -444.f, 0.0f), glm::vec3(90.0f, 0.0f, 80.0f)); //mid u 
	enemyCarPath.AddPoint(glm::vec3(-56.f, -444.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-11.f, -456.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-13.f, -415.f, 0.0f), glm::vec3(90.0f, 0.0f, 170.0f)); //end u 
	enemyCarPath.AddPoint(glm::vec3(-13.f, -415.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-13.f, -415.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-3.5f, -346.f, 0.0f), glm::vec3(90.0f, 0.0f, 170.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-3.5f, -346.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(26.f, -310.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));   /////////

	enemyCarPath.AddPoint(glm::vec3(-64.f, -293.f, 0.0f), glm::vec3(90.0f, 0.0f, 260.0f)); //turn 
	enemyCarPath.AddPoint(glm::vec3(-64.f, -293.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-64.f, -293.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));  /////

	enemyCarPath.AddPoint(glm::vec3(-117.f, -290.f, 0.0f), glm::vec3(90.0f, 0.0f, 265.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-117.f, -290.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-131.f, -287.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));  ///

	enemyCarPath.AddPoint(glm::vec3(-137.f, -252.f, 0.0f), glm::vec3(90.0f, 0.0f, 180.0f)); // mid u 
	enemyCarPath.AddPoint(glm::vec3(-137.f, -252.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-149.f, -219.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-107.f, -210.f, 0.0f), glm::vec3(90.0f, 0.0f, 100.0f)); //end u 
	enemyCarPath.AddPoint(glm::vec3(-107.f, -210.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-107.f, -210.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-34.f, -206.f, 0.0f), glm::vec3(90.0f, 0.0f, 100.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-34.f, -206.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(15.f, -216.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(4.f, -160.f, 0.0f), glm::vec3(90.0f, 0.0f, 190.0f)); //mid u
	enemyCarPath.AddPoint(glm::vec3(4.f, -160.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(12.f, -122.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-36.f, -129.f, 0.0f), glm::vec3(90.0f, 0.0f, 270.0f)); //end u
	enemyCarPath.AddPoint(glm::vec3(-36.f, -129.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-36.f, -129.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-108.f, -133.f, 0.0f), glm::vec3(90.0f, 0.0f, 270.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-108.f, -133.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-150.f, -137.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));  //////////

	enemyCarPath.AddPoint(glm::vec3(-149.f, -93.f, 0.0f), glm::vec3(90.0f, 0.0f, 180.0f)); //turn 
	enemyCarPath.AddPoint(glm::vec3(-149.f, -93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-149.f, -93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-149.f, -25.f, 0.0f), glm::vec3(90.0f, 0.0f, 175.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-149.f, -25.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-129.f, 14.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-195.f, 10.f, 0.0f), glm::vec3(90.0f, 0.0f, 270.0f)); //mid u
	enemyCarPath.AddPoint(glm::vec3(-195.f, 10.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-216.f, 9.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-222.f, -3.f, 0.0f), glm::vec3(90.0f, 0.0f, 340.0f)); //end u
	enemyCarPath.AddPoint(glm::vec3(-222.f, -3.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-222.f, -3.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-272.f, -121.f, 0.0f), glm::vec3(90.0f, 0.0f, 337.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-272.f, -121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-272.f, -121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.SpeedControl();

#pragma endregion


#pragma region Card/UI textures

	m_Registry.emplace<syre::Mesh>(m_Card, "objects/Card.obj");
	m_Registry.emplace<syre::Transform>(m_Card, glm::vec3(-3.0f, 0.0f, 2.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2f));
	m_Registry.emplace<syre::Texture>(m_Card, "images/NO2.png");



	cardTextures.push_back(syre::Texture("images/NO2.png"));
	cardTextures.push_back(syre::Texture("images/Drift.png"));
	cardTextures.push_back(syre::Texture("images/Slipstream.png"));
	cardTextures.push_back(syre::Texture("images/EBrake.png"));
	cardTextures.push_back(syre::Texture("images/QuickShiftN.png"));
	cardTextures.push_back(syre::Texture("images/Sabotage.png"));

	hazardTextures.push_back(syre::Texture("images/Apex.png"));
	hazardTextures.push_back(syre::Texture("images/Hairpin.png"));
	hazardTextures.push_back(syre::Texture("images/Chicane.png"));
	hazardTextures.push_back(syre::Texture("images/Rocks.png"));

	gearboxTextures.push_back(syre::Texture("images/Pedals_Neutral.png"));
	gearboxTextures.push_back(syre::Texture("images/Pedals_Gas.png"));
	gearboxTextures.push_back(syre::Texture("images/Pedals_Brake.png"));

	pGearTextures.push_back(syre::Texture("images/P1.png"));
	pGearTextures.push_back(syre::Texture("images/P1.png"));
	pGearTextures.push_back(syre::Texture("images/P2.png"));
	pGearTextures.push_back(syre::Texture("images/P3.png"));
	pGearTextures.push_back(syre::Texture("images/P4.png"));
	pGearTextures.push_back(syre::Texture("images/P5.png"));
	pGearTextures.push_back(syre::Texture("images/P6.png"));

	eGearTextures.push_back(syre::Texture("images/E1.png"));
	eGearTextures.push_back(syre::Texture("images/E1.png"));
	eGearTextures.push_back(syre::Texture("images/E2.png"));
	eGearTextures.push_back(syre::Texture("images/E3.png"));
	eGearTextures.push_back(syre::Texture("images/E4.png"));
	eGearTextures.push_back(syre::Texture("images/E5.png"));
	eGearTextures.push_back(syre::Texture("images/E6.png"));

	eneedleTextures.push_back(syre::Texture("images/Eneedle1.png"));
	eneedleTextures.push_back(syre::Texture("images/Eneedle1.png"));
	eneedleTextures.push_back(syre::Texture("images/Eneedle2.png"));
	eneedleTextures.push_back(syre::Texture("images/Eneedle3.png"));
	eneedleTextures.push_back(syre::Texture("images/Eneedle4.png"));
	eneedleTextures.push_back(syre::Texture("images/Eneedle5.png"));
	eneedleTextures.push_back(syre::Texture("images/Eneedle6.png"));

	pneedleTextures.push_back(syre::Texture("images/Pneedle1.png"));
	pneedleTextures.push_back(syre::Texture("images/Pneedle1.png"));
	pneedleTextures.push_back(syre::Texture("images/Pneedle2.png"));
	pneedleTextures.push_back(syre::Texture("images/Pneedle3.png"));
	pneedleTextures.push_back(syre::Texture("images/Pneedle4.png"));
	pneedleTextures.push_back(syre::Texture("images/Pneedle5.png"));
	pneedleTextures.push_back(syre::Texture("images/Pneedle6.png"));

	hnumberTextures.push_back(syre::Texture("images/O1.png"));
	hnumberTextures.push_back(syre::Texture("images/O1.png"));
	hnumberTextures.push_back(syre::Texture("images/O2.png"));
	hnumberTextures.push_back(syre::Texture("images/O3.png"));
	hnumberTextures.push_back(syre::Texture("images/O4.png"));
	hnumberTextures.push_back(syre::Texture("images/O5.png"));
	hnumberTextures.push_back(syre::Texture("images/O6.png"));

	htexTextures.push_back(syre::Texture("images/Apex_HUD.png"));
	htexTextures.push_back(syre::Texture("images/Hairpin_HUD.png"));
	htexTextures.push_back(syre::Texture("images/Chicane_HUD.png"));
	htexTextures.push_back(syre::Texture("images/Rocks_HUD.png"));

	progressBar1.push_back(syre::Texture("images/Progress Bar Top.png"));
	progressBar1.push_back(syre::Texture("images/Red1.png"));
	progressBar1.push_back(syre::Texture("images/Red2.png"));
	progressBar1.push_back(syre::Texture("images/Red3.png"));
	progressBar1.push_back(syre::Texture("images/Red4.png"));
	progressBar1.push_back(syre::Texture("images/Red5.png"));
	progressBar1.push_back(syre::Texture("images/Red6.png"));
	progressBar1.push_back(syre::Texture("images/Red7.png"));
	progressBar1.push_back(syre::Texture("images/Red8.png"));

	progressBar2.push_back(syre::Texture("images/Progress Bar Bottom.png"));
	progressBar2.push_back(syre::Texture("images/Blue1.png"));
	progressBar2.push_back(syre::Texture("images/Blue1.png"));
	progressBar2.push_back(syre::Texture("images/Blue1.png"));
	progressBar2.push_back(syre::Texture("images/Blue1.png"));
	progressBar2.push_back(syre::Texture("images/Blue1.png"));
	progressBar2.push_back(syre::Texture("images/Blue1.png"));
	progressBar2.push_back(syre::Texture("images/Blue1.png"));
	progressBar2.push_back(syre::Texture("images/Blue1.png"));

	ActionTextures.push_back(syre::Texture("images/A1.png"));
	ActionTextures.push_back(syre::Texture("images/A2.png"));
	ActionTextures.push_back(syre::Texture("images/A3.png"));
	ActionTextures.push_back(syre::Texture("images/A4.png"));

#pragma endregion
	flatShader = Shader::Create();
	flatShader->LoadShaderPartFromFile("flatVert.glsl", GL_VERTEX_SHADER);
	flatShader->LoadShaderPartFromFile("flatFrag.glsl", GL_FRAGMENT_SHADER);
	flatShader->Link();




	
	
	basicShader = Shader::Create();
	basicShader->LoadShaderPartFromFile("vertex_shader.glsl", GL_VERTEX_SHADER);
	basicShader->LoadShaderPartFromFile("shaders/gBuffer_pass_frag.glsl", GL_FRAGMENT_SHADER);
	basicShader->Link();

	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	float     lightAmbientPow = 0.1f;
	float     lightSpecularPow = 1.0f;
	glm::vec3 ambientCol = glm::vec3(1.0f);
	float     ambientPow = 0.5f;
	float     shininess = 4.0f;
	// These are our application / scene level uniforms that don't necessarily update
	// every frame
	basicShader->SetUniform("u_LightPos", lightPos);
	basicShader->SetUniform("u_LightCol", lightCol);
	basicShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	basicShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	basicShader->SetUniform("u_AmbientCol", ambientCol);
	basicShader->SetUniform("u_AmbientStrength", ambientPow);
	basicShader->SetUniform("u_Shininess", shininess);


	morphShader = Shader::Create();
	morphShader->LoadShaderPartFromFile("morph_vertex_shader.glsl", GL_VERTEX_SHADER);
	morphShader->LoadShaderPartFromFile("gfrag_shader.glsl", GL_FRAGMENT_SHADER);
	morphShader->Link();

	lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
	lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	lightAmbientPow = 0.1f;
	lightSpecularPow = 1.0f;
	ambientCol = glm::vec3(1.0f);
	ambientPow = 0.5f;
	shininess = 4.0f;
	// These are our application / scene level uniforms that don't necessarily update
	// every frame
	morphShader->SetUniform("u_LightPos", lightPos);
	morphShader->SetUniform("u_LightCol", lightCol);
	morphShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	morphShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	morphShader->SetUniform("u_AmbientCol", ambientCol);
	morphShader->SetUniform("u_AmbientStrength", ambientPow);
	morphShader->SetUniform("u_Shininess", shininess);


	simpleDepthShader = Shader::Create();
	simpleDepthShader->LoadShaderPartFromFile("shaders/simple_depth_vert.glsl", GL_VERTEX_SHADER);
	simpleDepthShader->LoadShaderPartFromFile("shaders/simple_depth_frag.glsl", GL_FRAGMENT_SHADER);
	simpleDepthShader->Link();


	auto& camComponent = camera;
	camComponent->SetPosition(glm::vec3(0, 3, 3)); // Set initial position
	camComponent->SetUp(glm::vec3(0, 0, 1)); // Use a z-up coordinate system
	camComponent->LookAt(glm::vec3(0.0f)); // Look at center of the screen
	camComponent->SetFovDegrees(100.0f); // Set an initial FOV

	// Creating the obstacle deck for this stage
	auto& obstacleComponent = m_Registry.get<Obstacles>(m_Obstacle);
	obstacleComponent.Adddeck(1);
	obstacleComponent.Adddeck(0);
	obstacleComponent.Adddeck(1);
	obstacleComponent.Adddeck(3);
	obstacleComponent.Adddeck(0);
	obstacleComponent.Adddeck(0);
	obstacleComponent.Adddeck(0);
	obstacleComponent.Adddeck(1);
	obstacleComponent.Adddeck(1);
	obstacleComponent.Adddeck(3);
	obstacleComponent.Adddeck(1);
	obstacleComponent.Adddeck(2);
	obstacleComponent.Adddeck(2);
	obstacleComponent.Adddeck(0);
	obstacleComponent.Adddeck(0);
	obstacleComponent.Adddeck(1);
	obstacleComponent.Adddeck(3);
	obstacleComponent.Adddeck(2);
	obstacleComponent.Adddeck(2);
	obstacleComponent.Adddeck(0);
	obstacleComponent.Adddeck(0);

	auto listView = m_Registry.view<syre::TransformList>();
	for (auto entity : listView)
	{
		m_Registry.get<syre::TransformList>(entity).SetCamera(camera);
	}
	auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
	PlayerComponent.Shuffle();

	lastFrame = glfwGetTime();
}


int SceningTest::Update()
{
	PostEffect* framebuffer = &m_Registry.get<PostEffect>(sceneBuff);
	CubeCoCoEffect* colorCorrect = &m_Registry.get<CubeCoCoEffect>(cocoBuff);
	CombinedBloom* bloom = &m_Registry.get<CombinedBloom>(bloomBuff);
	Blur* blur = &m_Registry.get<Blur>(blurBuff);
	GBuffer* g = &m_Registry.get<GBuffer>(gBuff);
	IlluminationBuffer* illum = &m_Registry.get<IlluminationBuffer>(illumBuff);
	Framebuffer* shadow = &m_Registry.get<Framebuffer>(shadowBuff);
	Pixelate* pixel = &m_Registry.get<Pixelate>(pixelBuff);
	NightVision* vision = &m_Registry.get<NightVision>(nightVisBuff);
	FilmGrain* grain = &m_Registry.get<FilmGrain>(grainBuff);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	framebuffer->Clear();
	colorCorrect->Clear();
	bloom->Clear();
	blur->Clear();
	g->Clear();
	shadow->Clear();
	illum->Clear();
	pixel->Clear();
	vision->Clear();
	grain->Clear();

	AudioEngine& engine = AudioEngine::Instance();

	AudioEvent& ambient = engine.GetEvent("Ambient");
	AudioBus& musicBus = engine.GetBus("Music");


	//get ref listener
	AudioListener& listener = engine.GetListener();
	listener.SetPosition(glm::vec3(5, 0, 0));

	engine.Update();

	if (isPaused)
	{
		return PausedUpdate();
	}
	thisFrame = glfwGetTime();
	float deltaTime = thisFrame - lastFrame;
	auto& camComponent = camera;
	//Getting relevant Variables for references in the update loop
	auto& obstacleComponent = m_Registry.get<Obstacles>(m_Obstacle);
	auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
	auto& EnemyComponent = m_Registry.get<Cars>(m_enemy);
	int temp = 0;
	bool done = false;
	bool Pemp = false;
	bool Eemp = false;
	if (start == 0)
	{
		start += 1;
		obstacleComponent.Draw();
	}
	glm::vec3 camX = glm::cross(camComponent->GetForward(), camComponent->GetUp());
	KeyEvents(deltaTime);
	flatShader->Bind();
	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.115f)));
	flatShader->SetUniform("aspect", camera->GetAspect());
#pragma region UI rendering

	// Here we render the cards sprites in the players hand based off of what their hand looks like from the Cars class
	for (int i = 0; i <= 4; i++)
	{
		//First we get the cards value from the curent slot in the players hand
		int cardVal = PlayerComponent.GetCard(i, true);
		//Then if its not empty we render the respective cards png into that slot
		if (cardVal != -1 && PlayerComponent.GetPosition1() == i || PlayerComponent.GetPosition2() == i && cardVal != -1)
		{
			flatShader->SetUniform("offset", glm::vec2(-0.1f + i / 4.2f, -.42f));

			cardTextures[cardVal].Bind();
			m_Registry.get<syre::Mesh>(m_Card).Render();
		}
		//If its supposed to be emtpy then we simply do not render a sprite with a texture
		else if (cardVal != -1)
		{
			flatShader->SetUniform("offset", glm::vec2(-0.1f + i / 4.2f, -.66f));

			cardTextures[cardVal].Bind();
			m_Registry.get<syre::Mesh>(m_Card).Render();
		}
	}

#pragma region Hud elements

	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.12f, 0.06f, 0.004f)));
	flatShader->SetUniform("offset", glm::vec2(-0.5, -0.8f));
	m_Registry.get<syre::Texture>(m_Endbutton).Bind();
	m_Registry.get<syre::Mesh>(m_Endbutton).Render();

	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.09f)));
	flatShader->SetUniform("offset", glm::vec2(0.90, 0.8f));
	int ObsVal = obstacleComponent.GetObs();
	hazardTextures[ObsVal].Bind();
	m_Registry.get<syre::Mesh>(m_Hazard).Render();

	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
	flatShader->SetUniform("offset", glm::vec2(-0.87, -0.62f));
	int GerValP = PlayerComponent.GetGear();
	pGearTextures[GerValP].Bind();
	m_Registry.get<syre::Mesh>(m_PGears).Render();

	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
	flatShader->SetUniform("offset", glm::vec2(-0.87, -0.62f));
	int GerValE = EnemyComponent.GetGear();
	eGearTextures[GerValE].Bind();
	m_Registry.get<syre::Mesh>(m_EGears).Render();

	int pos1 = PlayerComponent.GetPosition1();
	int pos2 = PlayerComponent.GetPosition2();

	if (pos1 != -1)
	{
		//flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.55, -0.60));
		ActionTextures[1].Bind();
		m_Registry.get<syre::Mesh>(m_A1).Render();
	}
	else if (pos1 == -1)
	{
		//flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.55, -0.60));
		ActionTextures[0].Bind();
		m_Registry.get<syre::Mesh>(m_A1).Render();
	}

	if (pos2 != -1)
	{
		//flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.45, -0.60));
		ActionTextures[3].Bind();
		m_Registry.get<syre::Mesh>(m_A2).Render();
	}
	else if (pos2 == -1)
	{
		//flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.45, -0.60));
		ActionTextures[2].Bind();
		m_Registry.get<syre::Mesh>(m_A2).Render();
	}

	if (helptog == true)
	{
		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.28f, 0.1f, 1.0f)));
		flatShader->SetUniform("offset", glm::vec2(0.57, 0.7f));
		int obs = obstacleComponent.GetObs();
		htexTextures[obs].Bind();
		m_Registry.get<syre::Mesh>(m_Htex).Render();

		if (obstacleComponent.GetObs() != 3)
		{
			flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.28f, 0.1f, 1.0f)));
			flatShader->SetUniform("offset", glm::vec2(0.54, 0.7f));
			int value = obstacleComponent.GetValue();
			hnumberTextures[value].Bind();
			m_Registry.get<syre::Mesh>(m_Hnumber).Render();
		}

		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.1f, 1.0f)));
		flatShader->SetUniform("offset", glm::vec2(0.55, 0.7f));
		m_Registry.get<syre::Texture>(m_HBox).Bind();
		m_Registry.get<syre::Mesh>(m_HBox).Render();
	}
	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
	flatShader->SetUniform("offset", glm::vec2(-0.87, -0.62f));
	m_Registry.get<syre::Texture>(m_Gearbox2).Bind();
	m_Registry.get<syre::Mesh>(m_Gearbox2).Render();
	

	if (PlayerComponent.GetBrk() == false && PlayerComponent.GetAcc() == false)
	{
		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.87, -0.75f));
		gearboxTextures[0].Bind();
		m_Registry.get<syre::Mesh>(m_Gearbox).Render();
	}
	else if (PlayerComponent.GetBrk() == true)
	{
		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.87, -0.75f));
		gearboxTextures[2].Bind();
		m_Registry.get<syre::Mesh>(m_Gearbox).Render();
	}
	else if (PlayerComponent.GetAcc() == true)
	{
		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.87, -0.75f));
		gearboxTextures[1].Bind();
		m_Registry.get<syre::Mesh>(m_Gearbox).Render();
	}

	int val1 = (round((float)PlayerComponent.GetScore() / (float)obstacleComponent.GetSize() * 8) - 1);
	if (val1 < 0)
	{
		val1 = 0;
	}
	//("%f, %f",Pl)

	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.1f, 0.004f)));
	flatShader->SetUniform("offset", glm::vec2(-0.6, 0.85f));
	progressBar1[val1].Bind();
	m_Registry.get<syre::Mesh>(m_Pscore).Render();
#pragma endregion

	int val2 = (round((float)EnemyComponent.GetScore() / (float)obstacleComponent.GetSize() * 8) - 1);
	if (val2 < 0)
	{
		val2 = 0;
	}

	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.1f, 0.004f)));
	flatShader->SetUniform("offset", glm::vec2(-0.6, 0.85f));
	progressBar2[val2].Bind();
	m_Registry.get<syre::Mesh>(m_Escore).Render();
#pragma endregion
	if (m_Registry.get<syre::PathAnimator>(m_PCar).HitMax() || m_Registry.get<syre::PathAnimator>(m_enemy).HitMax())
	{
		m_Registry.get<syre::PathAnimator>(m_PCar).Stop();
		m_Registry.get<syre::PathAnimator>(m_enemy).Stop();
		deltaTime = deltaTime / 8.0f;
	}
	else if (!(m_Registry.get<syre::PathAnimator>(m_PCar).HitMax() && m_Registry.get<syre::PathAnimator>(m_enemy).HitMax()))
	{
		m_Registry.get<syre::PathAnimator>(m_PCar).Resume();
		m_Registry.get<syre::PathAnimator>(m_enemy).Resume();
	}
#pragma region Enemy_AI

	//Contained in here is the logic for how the AI should behave based off of what the current obstacle is 
	
	if (obstacleComponent.GetEnd() != true)
	{
		// First we check to see if the AI is sabotaged and if we should just skip their turn and if its the start of a new turn
		if (EnemyComponent.GetSabo() == false && EnemyComponent.GetEnded() == false)
		{
			// Next we check to see what the current behavior of the AI is to decide how he should be playing
			if (speedDemon == true)
			{
				// If this behavior is active the AI will attempt to go as fast as possible at all time so we check to see if they are holding a card they will always play
				for (int i = 0; i <= 5; i++)
				{
					if (EnemyComponent.GetCard(i, true) == 0)
					{
						EnemyComponent.PlayCard(i, 0);
						break;
					}
				}
				// Then here we have some default actions the AI will perform should they not have the card they are supposed to play
				if (EnemyComponent.GetPosition1() == -1 || EnemyComponent.GetPosition2() == -1)
				{
					EnemyComponent.SetAcc();
				}
				if (EnemyComponent.GetGear() == 6)
				{
					speedDemon = false;
				}
			}
			// This is the alternate behavior for this AI where they will slow down until they hit gear 1 at which point they will swap to their alternate behavior
			else if (speedDemon == false)
			{
				// This is the actions the AI will take on their turn for the specific behvior
				if (EnemyComponent.GetGear() - 1 <= 1)
				{
					speedDemon = true;
				}
				EnemyComponent.SetBrk();

			}
			// We then resolve the actions the AI is going to take in his component
			EnemyComponent.ResolveCards();
			PlayerComponent.SetOppGear(EnemyComponent.GetGear());
		}

#pragma endregion
#pragma region Endturn Logic

		// Here we handle the logic for when the player has completed their turn and what happens based off who one on the current obstacle

		if (PlayerComponent.GetEnded() == true)
		{
			for (int i = 0; i <= 5; i++)
			{
				if (PlayerComponent.GetCard(i, true) == -1)
				{
					temp += 1;
				}
				if (temp == 6)
				{
					Pemp = true;
					temp = 0;
				}
			}
			for (int i = 0; i <= 5; i++)
			{
				if (EnemyComponent.GetCard(i, true) == -1)
				{
					temp += 1;
				}
				if (temp == 6)
				{
					Eemp = true;
					temp = 0;
				}
			}
			// In here is the logic for the figuring out who wins at the current obstacle
			if (obstacleComponent.GetObs() != 2)
			{
				// First We pass in the gear levels for both the enemy and the player
				if (obstacleComponent.Resolve(PlayerComponent.GetGear(), EnemyComponent.GetGear()) == 1)
				{
					// If the player wins we increase his score and adjust his speed in the animation so that he travels faster on the track
					PlayerComponent.IncreaseScore();
					m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(0.25, true);
					m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(0.25, false);
					// This is the logic for our rocks obstacle where the loser discards a random card from their hand
					if (obstacleComponent.GetObs() == 3)
					{
						if (Eemp != true)
						{
							while (done != true)
							{
								// This is us determining if there is a crad to discarrd from the enemy's deck and the selecting one at random and removing it
								srand((unsigned)time(0));
								temp = (rand() % 5);
								if (EnemyComponent.GetCard(temp, true) != -1)
								{
									EnemyComponent.RemoveCard(temp, true);
									break;
								}
							}
						}
					}
				}
				// Contained in here is the logic for if the enemy won the current obstacle
				else
				{
					// Again we increase the enemy's score and adjust their speed
					EnemyComponent.IncreaseScore();
					m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(0.25, false);
					m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(0.25, true);
					//Then we do additional effects if the current obstacle was a rocks
					if (obstacleComponent.GetObs() == 3)
					{
						if (Pemp != true)
						{
							while (done != true)
							{
								srand((unsigned)time(0));
								temp = (rand() % 5);

								if (PlayerComponent.GetCard(temp, true) != -1)
								{
									PlayerComponent.RemoveCard(temp, true);
									break;
								}
							}
						}
					}
				}
			}

			// Here we handle some additional card effects this is the logic for our card that swaps gears with their opponent at the start of a new obstacle
			if (EnemyComponent.GetCard(EnemyComponent.GetPosition1(), true) == 1 || EnemyComponent.GetCard(EnemyComponent.GetPosition2(),true) == 1)
			{
				// Here we store who ever is getting swapped with in a tem variable and handle the transfer of gear variables between the cars
				temp = EnemyComponent.GetGear();
				EnemyComponent.ChangeGears(PlayerComponent.GetGear());
				PlayerComponent.ChangeGears(temp);
			}
			// We repeat the same if the player played the card
			if (PlayerComponent.GetCard(PlayerComponent.GetPosition1(), true) == 1 || PlayerComponent.GetCard(PlayerComponent.GetPosition1(), true) == 1)
			{
				temp = PlayerComponent.GetGear();
				PlayerComponent.ChangeGears(EnemyComponent.GetGear());
				EnemyComponent.ChangeGears(temp);
			}

			// Here we handle the logic for our chicane obstacle which functions as an obstacle twice where the player and their opponent must compete to see who can get more points out of two mini obstacles
			if (obstacleComponent.GetObs() == 2)
			{
				// We check to see if the player and AI have completed to turns on the chicane
				if (obstacleComponent.GetP1wins() + obstacleComponent.GetP2wins() > 0)
				{
					// Then we resolve their effects like before
					if (obstacleComponent.Resolve(PlayerComponent.GetGear(), EnemyComponent.GetGear()) == true)
					{
						PlayerComponent.IncreaseScore();
						m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(0.25, true);
						m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(0.25, false);
					}
					else
					{
						EnemyComponent.IncreaseScore();
						m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(0.25, false);
						m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(0.25, true);
					}
				}
				// Then if they havn't completed two turns in the chicane we just resolve it normally
				else
				{
					obstacleComponent.Resolve(PlayerComponent.GetGear(), EnemyComponent.GetGear());
				}
			}

#pragma endregion
#pragma region Animator functionality

			//This is the animator functionality for moving the cars to the next obstacle marker on the SLERP path
			if (obstacleComponent.GetObs() != 2 && obstacleComponent.GetObs() != 3)
			{
				m_Registry.get<syre::PathAnimator>(m_PCar).IncrementSegment(2);
				m_Registry.get<syre::PathAnimator>(m_enemy).IncrementSegment(2);
			}
			else
			{
				m_Registry.get<syre::PathAnimator>(m_PCar).IncrementSegment(1);
				m_Registry.get<syre::PathAnimator>(m_enemy).IncrementSegment(1);
			}
#pragma endregion
			newvol += 0.3;
			engine.SetGlobalParameter("MusicVolume", newvol);
			// here we reset the overall actions both the player and enemy have taken
			PlayerComponent.ResetTurn();
			EnemyComponent.ResetTurn();
			obstacleComponent.Draw();
			showGear = false;
			if (obstacleComponent.GetObs() == 2)
			{
				PlayerComponent.SetPosition(-6);
				EnemyComponent.SetPosition(-6);
			}
			if (EnemyComponent.GetSabo() == true)
			{
				EnemyComponent.ChangeGears(1);
			}
		}
	}
#pragma region Endgame logic

	else
	{
		m_Registry.get<syre::PathAnimator>(m_PCar).IncrementSegment(2);
		m_Registry.get<syre::PathAnimator>(m_enemy).IncrementSegment(2);
		helptog = false;
		if (PlayerComponent.GetScore() >= EnemyComponent.GetScore())
		{
			flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.4f)));
			flatShader->SetUniform("offset", glm::vec2(0.0f, 0.4f));
			flatShader->SetUniform("aspect", 0.4f);
			Winner.Bind();
			m_Registry.get<syre::Mesh>(m_Hazard).Render();

			m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(1.0, true);
			m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(1.0, false);
			if (m_Registry.get<syre::PathAnimator>(m_PCar).GetHardStop())
			{
				//game is finished finished
				m_Registry.get<syre::PathAnimator>(m_enemy).Stop();
				//this is where we could go to next level
				won = true;
				bootToMenu += deltaTime;
				if (bootToMenu > 7.0f)
				{
					m_Registry.clear();

					return -1;
				}
			}
		}
		// This handles the graphical interface for who won or lost the race
		else if (PlayerComponent.GetScore() < EnemyComponent.GetScore())
		{
			flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.4f)));
			flatShader->SetUniform("offset", glm::vec2(0.0f, 0.4f));
			flatShader->SetUniform("aspect", 0.4f);
			Loser.Bind();
			m_Registry.get<syre::Mesh>(m_Hazard).Render();
			m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(1.0, false);
			m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(1.0, true);
			bootToMenu += deltaTime;
			if (bootToMenu > 7.0f)
			{
				m_Registry.clear();

				return -1;
			}
		}
	}
#pragma endregion
	auto pathView = m_Registry.view<syre::PathAnimator, syre::Transform>();
	for (auto entity : pathView)
	{
		auto& transform = m_Registry.get<syre::Transform>(entity);
		m_Registry.get<syre::PathAnimator>(entity).Update(transform, deltaTime);
	}
	flatShader->UnBind();
	auto renderView = m_Registry.view<syre::Mesh, syre::Transform, syre::Texture>();

	glm::mat4 lightProjectionMatrix = glm::ortho(-lr, lr, -ud, ud, -unear, ufar);
	glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(-illum->GetSunRef()._lightDirection), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 lightSpaceViewProj = lightProjectionMatrix * lightViewMatrix;
	illum->SetLightSpaceViewProj(lightSpaceViewProj);
	illum->SetCamPos(camera->GetPosition());

	glViewport(0, 0, shadowWidth, shadowHeight);
	simpleDepthShader->Bind();
	shadow->Bind();
	for (auto entity : renderView)
	{
		glm::mat4 transform = renderView.get<syre::Transform>(entity).GetModelMat();
		simpleDepthShader->SetUniformMatrix("u_LightSpaceMatrix", lightSpaceViewProj);
		simpleDepthShader->SetUniformMatrix("u_Model", transform);
		renderView.get<syre::Texture>(entity).Bind();
		renderView.get<syre::Mesh>(entity).Render();
	}
	simpleDepthShader->UnBind();
	shadow->Unbind();

	//framebuffer bound
	//framebuffer->BindBuffer(0);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glDisable(GL_BLEND);
	
	g->Bind();
	rampTex->Bind(20);


	basicShader->Bind();
	basicShader->SetUniform("u_CamPos", camComponent->GetPosition());
	basicShader->SetUniform("playerPos", m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	basicShader->SetUniform("enemyPos", m_Registry.get<syre::Transform>(m_enemy).GetPosition());
	basicShader->SetUniform("u_SpecularStrength", specularOn ? 0.7f : 0.0f);
	basicShader->SetUniform("u_AmbientStrength", ambientOn ? 0.3f : 0.0f);
	basicShader->SetUniform("u_DiffuseStrength", diffuseOn ? 1.0f : 0.0f);
	basicShader->SetUniform("u_CarEmissive", carLighting ? 1 : 0);
	basicShader->SetUniform("u_RampingSpec", rampOnSpec ? 1 : 0);
	basicShader->SetUniform("u_RampingDiff", rampOnDiff ? 1 : 0);



	for (auto entity : renderView)
	{
		glm::mat4 transform = renderView.get<syre::Transform>(entity).GetModelMat();
		basicShader->SetUniformMatrix("u_ModelViewProjection", camComponent->GetViewProjection() * transform);
		basicShader->SetUniformMatrix("u_Model", transform);
		basicShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		renderView.get<syre::Texture>(entity).Bind();
		renderView.get<syre::Mesh>(entity).Render();
	}

	auto listRenderView = m_Registry.view<syre::Mesh, syre::TransformList, syre::Texture>();
	for (auto entity : listRenderView)
	{
		listRenderView.get<syre::Texture>(entity).Bind();
		listRenderView.get<syre::TransformList>(entity).ListRender(basicShader, listRenderView.get<syre::Mesh>(entity), deltaTime);
	}

	auto morphRenderView = m_Registry.view<syre::MorphRenderer, syre::Transform, syre::Texture>();
	morphShader->Bind();

	morphShader->SetUniform("u_CamPos", camComponent->GetPosition());
	morphShader->SetUniform("playerPos", m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	morphShader->SetUniform("enemyPos", m_Registry.get<syre::Transform>(m_enemy).GetPosition());
	morphShader->SetUniform("u_SpecularStrength", specularOn ? 0.7f : 0.0f);
	morphShader->SetUniform("u_AmbientStrength", ambientOn ? 0.3f : 0.0f);
	morphShader->SetUniform("u_DiffuseStrength", diffuseOn ? 1.0f : 0.0f);
	morphShader->SetUniform("u_CarEmissive", carLighting ? 1 : 0);
	morphShader->SetUniform("u_RampingSpec", rampOnSpec ? 1 : 0);
	morphShader->SetUniform("u_RampingDiff", rampOnDiff ? 1 : 0);



	for (auto entity : morphRenderView)
	{
		float t = morphRenderView.get<syre::MorphRenderer>(entity).Update(deltaTime);
		glm::mat4 transform = morphRenderView.get<syre::Transform>(entity).GetModelMat();
		morphShader->SetUniformMatrix("u_ModelViewProjection", camComponent->GetViewProjection() * transform);
		morphShader->SetUniformMatrix("u_Model", transform);
		morphShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		morphShader->SetUniform("t", t);
		morphRenderView.get<syre::Texture>(entity).Bind();
		morphRenderView.get<syre::MorphRenderer>(entity).Render();
	}
	auto morphListRenderView = m_Registry.view<syre::MorphRenderer, syre::TransformList, syre::Texture>();
	for (auto entity : morphListRenderView)
	{
		float t = morphListRenderView.get<syre::MorphRenderer>(entity).Update(deltaTime);
		morphShader->SetUniform("t", t);
		morphListRenderView.get<syre::Texture>(entity).Bind();
		morphListRenderView.get<syre::TransformList>(entity).ListRender(morphShader, morphListRenderView.get<syre::MorphRenderer>(entity));
	}

	g->Unbind();

	shadow->BindDepthAsTexture(30);

	DirectionalLight& tempSun = illum->GetSunRef();
	tempSun._ambientPow = ambientOn ? 0.2f : 0.0f;
	tempSun._lightAmbientPow = ambientOn ? 0.2f : 0.0f;
	tempSun._lightSpecularPow = specularOn ? 0.7f : 0.0f;
	tempSun._lightCol = diffuseOn ? glm::vec4(1.0f, 1.0f, 1.0f,1.0f) : glm::vec4(0.0f, 0.0f, 0.0f,1.0f);

	illum->SetPlayerPos(m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	illum->SetEnemyPos(m_Registry.get<syre::Transform>(m_enemy).GetPosition());
	illum->SetEmissive(carLighting);
	illum->ApplyEffect(g);

	shadow->UnbindTexture(30);

	if (dispG)
	{
		if (indivgBuff)
			g->DrawBuffersToScreen(colTarg);
		else
			g->DrawBuffersToScreen();
	}
	else if (dispIllum)
	{
		illum->DrawIllumBuffer();
	}
	else
	{
		PostEffect* lastBuffer = illum;
		if (nightVising)
		{
			vision->ApplyEffect(lastBuffer);

			lastBuffer = vision;
		}
		if (blooming)
		{
			bloom->ApplyEffect(lastBuffer);

			lastBuffer = bloom;
		}
		if (blurring)
		{
			blur->ApplyEffect(lastBuffer);

			lastBuffer = blur;
		}
		if (correcting)
		{
			cubes[activeCube].bind(30);

			colorCorrect->ApplyEffect(lastBuffer);

			lastBuffer = colorCorrect;
		}
		if (pixelling)
		{
			pixel->ApplyEffect(lastBuffer);

			lastBuffer = pixel;
		}
		if (graining)
		{
			grain->ApplyEffect(lastBuffer);

			lastBuffer = grain;
		}
		

		lastBuffer->DrawToScreen();
	}
	//PostEffect* lastBuffer = framebuffer;
	//framebuffer->UnBindBuffer();

	


	if (!manualCamera)
	{
		camComponent->SetPosition(m_Registry.get<syre::Transform>(m_PCar).GetPosition() + glm::vec3(1.0f, 5.0f, 5.0f));
	}
	camComponent->SetForward(glm::normalize(m_Registry.get<syre::Transform>(m_PCar).GetPosition() - camComponent->GetPosition()));
	/*m_Registry.get<syre::TransformList>(m_Particles1).UpdateCurPos(m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	m_Registry.get<syre::TransformList>(m_Particles2).UpdateCurPos(m_Registry.get<syre::Transform>(m_enemy).GetPosition());*/

	lastFrame = thisFrame;
	return 0;
}

int SceningTest::PausedUpdate()
{
	PostEffect* framebuffer = &m_Registry.get<PostEffect>(sceneBuff);
	CubeCoCoEffect* colorCorrect = &m_Registry.get<CubeCoCoEffect>(cocoBuff);
	CombinedBloom* bloom = &m_Registry.get<CombinedBloom>(bloomBuff);
	Blur* blur = &m_Registry.get<Blur>(blurBuff);
	GBuffer* g = &m_Registry.get<GBuffer>(gBuff);
	IlluminationBuffer* illum = &m_Registry.get<IlluminationBuffer>(illumBuff);
	Framebuffer* shadow = &m_Registry.get<Framebuffer>(shadowBuff);
	Pixelate* pixel = &m_Registry.get<Pixelate>(pixelBuff);
	NightVision* vision = &m_Registry.get<NightVision>(nightVisBuff);
	FilmGrain* grain = &m_Registry.get<FilmGrain>(grainBuff);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	framebuffer->Clear();
	colorCorrect->Clear();
	bloom->Clear();
	blur->Clear();
	g->Clear();
	shadow->Clear();
	illum->Clear();
	pixel->Clear();
	vision->Clear();
	grain->Clear();

	thisFrame = glfwGetTime();
	float deltaTime = thisFrame - lastFrame;
	auto& camComponent = camera;
	auto& obstacleComponent = m_Registry.get<Obstacles>(m_Obstacle);
	auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
	auto& EnemyComponent = m_Registry.get<Cars>(m_enemy);
	int returning = KeyEvents(deltaTime);
	if (returning == -1)
	{
		return -1;
	}
	
	flatShader->Bind();
	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));
	flatShader->SetUniform("offset", glm::vec2(0.0, 0.0f));
	flatShader->SetUniform("aspect", camera->GetAspect());
	m_Registry.get<syre::Texture>(m_PauseMenu).Bind();
	m_Registry.get<syre::Mesh>(m_PauseMenu).Render();


	
	auto renderView = m_Registry.view<syre::Mesh, syre::Transform, syre::Texture>();

	glm::mat4 lightProjectionMatrix = glm::ortho(-lr, lr, -ud, ud, -unear, ufar);
	glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(-illum->GetSunRef()._lightDirection), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 lightSpaceViewProj = lightProjectionMatrix * lightViewMatrix;
	illum->SetLightSpaceViewProj(lightSpaceViewProj);
	illum->SetCamPos(camera->GetPosition());

	glViewport(0, 0, shadowWidth, shadowHeight);
	simpleDepthShader->Bind();
	shadow->Bind();
	for (auto entity : renderView)
	{
		glm::mat4 transform = renderView.get<syre::Transform>(entity).GetModelMat();
		simpleDepthShader->SetUniformMatrix("u_LightSpaceMatrix", lightSpaceViewProj);
		simpleDepthShader->SetUniformMatrix("u_Model", transform);
		renderView.get<syre::Texture>(entity).Bind();
		renderView.get<syre::Mesh>(entity).Render();
	}
	simpleDepthShader->UnBind();
	shadow->Unbind();

	//framebuffer bound
	//framebuffer->BindBuffer(0);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glDisable(GL_BLEND);

	g->Bind();
	rampTex->Bind(20);


	basicShader->Bind();
	basicShader->SetUniform("u_CamPos", camComponent->GetPosition());
	basicShader->SetUniform("playerPos", m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	basicShader->SetUniform("enemyPos", m_Registry.get<syre::Transform>(m_enemy).GetPosition());
	basicShader->SetUniform("u_SpecularStrength", specularOn ? 0.7f : 0.0f);
	basicShader->SetUniform("u_AmbientStrength", ambientOn ? 0.3f : 0.0f);
	basicShader->SetUniform("u_DiffuseStrength", diffuseOn ? 1.0f : 0.0f);
	basicShader->SetUniform("u_CarEmissive", carLighting ? 1 : 0);
	basicShader->SetUniform("u_RampingSpec", rampOnSpec ? 1 : 0);
	basicShader->SetUniform("u_RampingDiff", rampOnDiff ? 1 : 0);



	for (auto entity : renderView)
	{
		glm::mat4 transform = renderView.get<syre::Transform>(entity).GetModelMat();
		basicShader->SetUniformMatrix("u_ModelViewProjection", camComponent->GetViewProjection() * transform);
		basicShader->SetUniformMatrix("u_Model", transform);
		basicShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		renderView.get<syre::Texture>(entity).Bind();
		renderView.get<syre::Mesh>(entity).Render();
	}

	auto listRenderView = m_Registry.view<syre::Mesh, syre::TransformList, syre::Texture>();
	for (auto entity : listRenderView)
	{
		listRenderView.get<syre::Texture>(entity).Bind();
		listRenderView.get<syre::TransformList>(entity).ListRender(basicShader, listRenderView.get<syre::Mesh>(entity), deltaTime);
	}

	auto morphRenderView = m_Registry.view<syre::MorphRenderer, syre::Transform, syre::Texture>();
	morphShader->Bind();

	morphShader->SetUniform("u_CamPos", camComponent->GetPosition());
	morphShader->SetUniform("playerPos", m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	morphShader->SetUniform("enemyPos", m_Registry.get<syre::Transform>(m_enemy).GetPosition());
	morphShader->SetUniform("u_SpecularStrength", specularOn ? 0.7f : 0.0f);
	morphShader->SetUniform("u_AmbientStrength", ambientOn ? 0.3f : 0.0f);
	morphShader->SetUniform("u_DiffuseStrength", diffuseOn ? 1.0f : 0.0f);
	morphShader->SetUniform("u_CarEmissive", carLighting ? 1 : 0);
	morphShader->SetUniform("u_RampingSpec", rampOnSpec ? 1 : 0);
	morphShader->SetUniform("u_RampingDiff", rampOnDiff ? 1 : 0);



	for (auto entity : morphRenderView)
	{
		float t = morphRenderView.get<syre::MorphRenderer>(entity).GetT();
		glm::mat4 transform = morphRenderView.get<syre::Transform>(entity).GetModelMat();
		morphShader->SetUniformMatrix("u_ModelViewProjection", camComponent->GetViewProjection() * transform);
		morphShader->SetUniformMatrix("u_Model", transform);
		morphShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		morphShader->SetUniform("t", t);
		morphRenderView.get<syre::Texture>(entity).Bind();
		morphRenderView.get<syre::MorphRenderer>(entity).Render();
	}
	auto morphListRenderView = m_Registry.view<syre::MorphRenderer, syre::TransformList, syre::Texture>();
	for (auto entity : morphListRenderView)
	{
		float t = morphListRenderView.get<syre::MorphRenderer>(entity).GetT();
		morphShader->SetUniform("t", t);
		morphListRenderView.get<syre::Texture>(entity).Bind();
		morphListRenderView.get<syre::TransformList>(entity).ListRender(morphShader, morphListRenderView.get<syre::MorphRenderer>(entity));
	}

	g->Unbind();

	shadow->BindDepthAsTexture(30);

	illum->SetPlayerPos(m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	illum->SetEnemyPos(m_Registry.get<syre::Transform>(m_enemy).GetPosition());
	illum->ApplyEffect(g);

	shadow->UnbindTexture(30);

	if (dispG)
	{
		if (indivgBuff)
			g->DrawBuffersToScreen(colTarg);
		else
			g->DrawBuffersToScreen();
	}
	else if (dispIllum)
	{
		illum->DrawIllumBuffer();
	}
	else
	{
		PostEffect* lastBuffer = illum;
		if (nightVising)
		{
			vision->ApplyEffect(lastBuffer);

			lastBuffer = vision;
		}
		if (blooming)
		{
			bloom->ApplyEffect(lastBuffer);

			lastBuffer = bloom;
		}
		if (blurring)
		{
			blur->ApplyEffect(lastBuffer);

			lastBuffer = blur;
		}
		if (correcting)
		{
			cubes[activeCube].bind(30);

			colorCorrect->ApplyEffect(lastBuffer);

			lastBuffer = colorCorrect;
		}
		if (pixelling)
		{
			pixel->ApplyEffect(lastBuffer);

			lastBuffer = pixel;
		}
		if (graining)
		{
			grain->ApplyEffect(lastBuffer);

			lastBuffer = grain;
		}


		lastBuffer->DrawToScreen();
	}
	//PostEffect* lastBuffer = framebuffer;
	//framebuffer->UnBindBuffer();




	if (!manualCamera)
	{
		camComponent->SetPosition(m_Registry.get<syre::Transform>(m_PCar).GetPosition() + glm::vec3(1.0f, 5.0f, 5.0f));
	}
	camComponent->SetForward(glm::normalize(m_Registry.get<syre::Transform>(m_PCar).GetPosition() - camComponent->GetPosition()));
	/*m_Registry.get<syre::TransformList>(m_Particles1).UpdateCurPos(m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	m_Registry.get<syre::TransformList>(m_Particles2).UpdateCurPos(m_Registry.get<syre::Transform>(m_enemy).GetPosition());*/

	


	lastFrame = thisFrame;
	return returning;
}

void SceningTest::ImGUIUpdate()
{
	//auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
		// Implementation new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		CombinedBloom* bloom = &m_Registry.get<CombinedBloom>(bloomBuff);
		Blur* blur = &m_Registry.get<Blur>(blurBuff);
		IlluminationBuffer* illum = &m_Registry.get<IlluminationBuffer>(illumBuff);
		Pixelate* pixel = &m_Registry.get<Pixelate>(pixelBuff);
		int blurPasses = blur->GetPasses();
		float bloomThreshold = bloom->GetThreshold();
		int bloomPasses = bloom->GetPasses();
		int bloomBlurType = bloom->GetBlurType();
		int pixellationFactor = pixel->GetFactor();

		// ImGui context new frame
		ImGui::NewFrame();
		AudioEngine& audio = AudioEngine::Instance();
		int pauseStatus = audio.GetGlobalParameterValue("IsPaused");
		float sfxVol = audio.GetGlobalParameterValue("SFXVolume");
		float musVol = audio.GetGlobalParameterValue("MusicVolume");

		if (ImGui::Begin("Debug")) {
			// Render our GUI stuff
			if (ImGui::CollapsingHeader("Deferred"))
			{
				ImGui::DragFloat3("Light Direction/Position", glm::value_ptr(illum->GetSunRef()._lightDirection), 0.01f, -10.0f, 10.0f);
				ImGui::Checkbox("Display GBuffer", &dispG);
				if (dispG)
				{
					ImGui::Checkbox("Display Targets Individually", &indivgBuff);
					if (indivgBuff)
					{
						ImGui::SliderInt("Color Target", &colTarg, 0, 3);
					}
				}
				ImGui::Checkbox("Display Illumination Accumulation", &dispIllum);
				ImGui::SliderFloat("Left and Right ortho", &lr, 0.0f, 2000.0f);
				ImGui::SliderFloat("Up and Down ortho", &ud, 0.0f, 2000.0f);
				ImGui::SliderFloat("Near ortho", &unear, 0.0f, 2000.0f);
				ImGui::SliderFloat("Far ortho", &ufar, 0.0f, 2000.0f);

			}
			if (ImGui::CollapsingHeader("Post Processing"))
			{
				ImGui::Checkbox("Night Vision", &nightVising);
				ImGui::Checkbox("Bloom", &blooming);
				if (blooming)
				{
					ImGui::SliderFloat("Bloom Threshold", &bloomThreshold, 0, 1);
					ImGui::SliderInt("Bloom blur Passes", &bloomPasses, 0, 20);
					ImGui::SliderInt("Blur Type", &bloomBlurType, 0, 2);
					bloom->SetPasses(bloomPasses);
					bloom->SetThreshold(bloomThreshold);
					bloom->SetBlurType(bloomBlurType);
				}
				ImGui::Checkbox("Blur", &blurring);
				if (blurring)
				{
					ImGui::SliderInt("Blur blur Passes", &blurPasses, 0, 20);
					blur->SetPasses(blurPasses);
				}
				ImGui::Checkbox("Color Correction", &correcting);
				if (correcting)
				{
					ImGui::SliderInt("Active CoCo Effect", &activeCube, 0, cubes.size() - 1);
					ImGui::Text("0 is Neutral, 1 is Cool, 2 is Warm, 3 is Custom");
				}
				ImGui::Checkbox("Pixelation", &pixelling);
				if (pixelling)
				{
					ImGui::SliderInt("Pixellation Amount", &pixellationFactor, 2, 32);
					pixel->SetFactor(pixellationFactor);
				}
				//ImGui::Checkbox("Film Grain", &graining);
			}
			if (ImGui::CollapsingHeader("Lighting"))
			{
				ImGui::Checkbox("Ambient Lighting", &ambientOn);
				ImGui::Checkbox("Diffuse Lighting", &diffuseOn);
				ImGui::Checkbox("Specular Lighting", &specularOn);
				ImGui::Checkbox("Emissive Car Lighting", &carLighting);
				//ImGui::Checkbox("Specular Ramping", &rampOnSpec);
				//ImGui::Checkbox("Diffuse Ramping", &rampOnDiff);
			}
			if (ImGui::CollapsingHeader("FMOD"))
			{
				ImGui::SliderFloat("Effects Volume", &sfxVol, 0.f, 1.f);
				ImGui::SliderFloat("Music Volume", &musVol, 0.f, 1.f);
			}

			glm::vec3 camPos = camera->GetPosition();
			if (ImGui::Button(manualCamera?"Auto Camera": "Manual Camera"))
			{
				manualCamera = !manualCamera;
			}
			if (manualCamera)
			{
				ImGui::SliderFloat3("Camera Position", &camPos.x,-200.f, 200.f);
				camera->SetPosition(camPos);

			}
			if (ImGui::Button("1"))
			{
				dispG = false;
				dispIllum = false;
			}
			if (ImGui::Button("2"))
			{
				dispG = true;
				indivgBuff = true;
				colTarg = 3;
			}
			if (ImGui::Button("3"))
			{
				dispG = true;
				indivgBuff = true;
				colTarg = 1;
			}
			if (ImGui::Button("4"))
			{
				dispG = true;
				indivgBuff = true;
				colTarg = 0;
			}
			if (ImGui::Button("5"))
			{
				dispG = false;
				dispIllum = true;
			}
		}
		ImGui::End();
		audio.SetGlobalParameter("IsPaused",pauseStatus);
		audio.SetGlobalParameter("SFXVolume", sfxVol);
		audio.SetGlobalParameter("MusicVolume", musVol);


		// Make sure ImGui knows how big our window is
		ImGuiIO& io = ImGui::GetIO();
		int width{ 0 }, height{ 0 };
		glfwGetWindowSize(window, &width, &height);
		io.DisplaySize = ImVec2((float)width, (float)height);

		// Render all of our ImGui elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			// Update the windows that ImGui is using
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			// Restore our gl context
			glfwMakeContextCurrent(window);
		}
	
}

int SceningTest::GetID()
{
	return 1;
}


Camera::sptr& SceningTest::GetCam()
{
	// TODO: insert return statement here
	return camera;
}

int SceningTest::KeyEvents(float delta)
{
	AudioEngine& engine = AudioEngine::Instance();
	if (isPaused)
	{	
		double* x = new double;
		double* y = new double;

		glfwGetCursorPos(window, x, y);
		//printf("Mouse at X %f Y %f\n", *x, *y);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
		{
			escRelease = true;
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS&&escRelease)
		{
			isPaused = false;
			escRelease = false;

		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			if (552.0f < *x && *x < 729.0f)
			{
				if (323.0f < *y && *y < 367.0f)
				{
					isPaused = false;
				}
				else if (374.0f < *y && *y < 417.0f)
				{
					m_Registry.clear();
					return -1;
				}
				else if (425.0f < *y && *y < 474.0f)
				{
					return -2;
				}
			}
		}

		return 0;
	}
	else
	{
		Elapsedtime += delta;
		auto& camComponent = camera;
		auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
		auto& EnemyComponent = m_Registry.get<Cars>(m_enemy);
		int temp;
		glm::vec3 curCamPos = camComponent->GetPosition();
		glm::vec3 curCamFor = camComponent->GetForward();

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			curCamPos.x += 10.f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			curCamPos.x -= 10.f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			curCamPos.y -= 10.f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			curCamPos.y += 10.f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			curCamFor.x += 0.1f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			curCamFor.x -= 0.1f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			curCamFor.y -= 0.1f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			curCamFor.y += 0.1f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			PlayerComponent.Draw();
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
		{
			escRelease = true;
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS&&escRelease)
		{
			isPaused = true;
			escRelease = false;

		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && Elapsedtime >= 0.5)
		{
			double* x = new double;
			double* y = new double;

			glfwGetCursorPos(window, x, y);
			if (*x >= 70 && *x <= 95 && *y <= 615 && *y >= 597)
			{
				lbutton_down = true;
			}
			/*
			printf("Mouse at X ");
			printf("%f", *x);
			printf("\n");
			printf("Mouse at Y ");
			printf("%f", *y);
			*/
			if (m_Registry.get<syre::PathAnimator>(m_PCar).HitMax() || m_Registry.get<syre::PathAnimator>(m_enemy).HitMax())
			{
				for (float i = 0; i <= 5; i++)
				{
					if ((i * 165) + 478 <= *x && (i + 1) * 165 + 478 >= *x && *y >= 457 && *y <= 706 && PlayerComponent.GetCard(i, true) != -1)
					{
							PlayerComponent.PlayCard(i, 0);
						Elapsedtime = 0;
					}
				}
				if (*x >= 34 && *x <= 72 && *y <= 699 && *y >= 674)
				{
					PlayerComponent.SetBrk();
					Elapsedtime = 0;
				}
				else if (*x >= 98 && *x <= 129 && *y <= 709 && *y >= 666)
				{
					PlayerComponent.SetAcc();
					Elapsedtime = 0;
				}
			}
			if (*x >= 1157 && *x <= 1278 && *y <= 132 && *y >= 10)
			{
				if (helptog == true)
				{
					helptog = false;
					Elapsedtime = 0;
				}
				else if (helptog == false)
				{
					helptog = true;
					Elapsedtime = 0;
				}
			}

			if (*x >= 242 && *x <= 397 && *y <= 687 && *y >= 611)
			{
				if (m_Registry.get<syre::PathAnimator>(m_PCar).HitMax() || m_Registry.get<syre::PathAnimator>(m_enemy).HitMax())
				{
					if (PlayerComponent.GetPosition1() != -1 && PlayerComponent.GetPosition1() != -2 && PlayerComponent.GetPosition1() != -3)
					{
						if (PlayerComponent.GetCard(PlayerComponent.GetPosition1(), true) == 2)
						{
							EnemyComponent.ChangeGears(PlayerComponent.GetGear());
						}
						else if (PlayerComponent.GetCard(PlayerComponent.GetPosition1(), true) == 5)
						{
							EnemyComponent.SetSabo();
						}
					}

					if (PlayerComponent.GetPosition2() != -1 && PlayerComponent.GetPosition2() != -2 && PlayerComponent.GetPosition2() != -3)
					{
						if (PlayerComponent.GetCard(PlayerComponent.GetPosition2(), true) == 2)
						{
							EnemyComponent.ChangeGears(PlayerComponent.GetGear());
						}
						else if (PlayerComponent.GetCard(PlayerComponent.GetPosition2(), true) == 5)
						{
							EnemyComponent.SetSabo();
						}
					}
					PlayerComponent.ResolveCards();
				}
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			lbutton_down = false;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			m_Registry.get<syre::PathAnimator>(m_PCar).Reset();
		}
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			if (m_Registry.get<syre::PathAnimator>(m_PCar).HitMax() || m_Registry.get<syre::PathAnimator>(m_enemy).HitMax())
			{
				if (PlayerComponent.GetPosition1() != -1 && PlayerComponent.GetPosition1() != -2 && PlayerComponent.GetPosition1() != -3)
				{
					if (PlayerComponent.GetCard(PlayerComponent.GetPosition1(), true) == 2)
					{
						EnemyComponent.ChangeGears(PlayerComponent.GetGear());
					}
					else if (PlayerComponent.GetCard(PlayerComponent.GetPosition1(), true) == 5)
					{
						EnemyComponent.SetSabo();
					}
				}

				if (PlayerComponent.GetPosition2() != -1 && PlayerComponent.GetPosition2() != -2 && PlayerComponent.GetPosition2() != -3)
				{
					if (PlayerComponent.GetCard(PlayerComponent.GetPosition2(), true) == 2)
					{
						EnemyComponent.ChangeGears(PlayerComponent.GetGear());
					}
					else if (PlayerComponent.GetCard(PlayerComponent.GetPosition2(), true) == 5)
					{
						EnemyComponent.SetSabo();
					}
				}
				PlayerComponent.ResolveCards();
			}
		}
		camComponent->SetPosition(curCamPos);
		return 0;
	}
}

