#include "Game.h"
#include "SceneObject.h"
#include <string.h>


///@brieflistens for frame events. used to update Game object
class MyFrameListener : public Ogre::FrameListener
{
private:
	Game* gameObject;

public:
	///pass in the game class object so it can be referenced on frame events
	MyFrameListener(Game* game)
	{
		gameObject = game;
	}

	///updates the game at the start of every frame
	bool frameStarted(const Ogre::FrameEvent& evt)
	{
		gameObject->Update(evt.timeSinceLastFrame);

		return true;
	}
};


///constructor
Game::Game()
	: ApplicationContext("Pong - Andrew Boulanger")
{
}


///handles setup for ogre, input listening. creates lights, camera and other game objects
void Game::setup()
{
	//general app setup
	////////////////////////////////////////////////////////////////////////////////////////////////////
	ApplicationContext::setup();
	addInputListener(this);

	root = getRoot();
	scnMgr = root->createSceneManager();

	// register our scene with the RTSS
	RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);


	//lights  
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	scnMgr->setAmbientLight(ColourValue(1.0, 1.0, 1.0));

	Light* light = scnMgr->createLight("MainLight");
	light->setDiffuseColour(1.0, 0.5, 0.5);
	SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);

	lightNode->setPosition(20, 80, 50);


	//UI Elements
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	OgreBites::TrayManager* mTrayMgr = new OgreBites::TrayManager("Hud", getRenderWindow());
	mGameOverTray = new OgreBites::TrayManager("GameOverPanel", getRenderWindow());
	
	scnMgr->addRenderQueueListener(mOverlaySystem);

	mInfoLabel = mTrayMgr->createLabel(TL_TOP, "TInfo", "Pong by Andrew Boulanger", 350);

	mFpsLabel = mTrayMgr->createLabel(TL_BOTTOMLEFT, "FPS", "FPS: " + std::to_string(fps), 150);
	mScoreLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Score", "Score: " + std::to_string(score), 150);
	mTpuLabel = mTrayMgr->createLabel(TL_BOTTOMLEFT, "Time/Update", "Time/Update: " + std::to_string(tpu) , 170);
	mLivesLabel = mTrayMgr->createLabel(TL_TOPLEFT, "Lives", "Lives: " + std::to_string(lives), 150);

	addInputListener(mGameOverTray);

	mGameOverLabel = mGameOverTray->createLabel(TL_CENTER, "GameOver", "Game Over", 220);
	mFinalScoreLabel = mGameOverTray->createLabel(TL_CENTER, "FinalScore", "Your Score: " + std::to_string(score), 200);
	mRestartButton = mGameOverTray->createButton(TL_CENTER, "restartButton", "Replay", 200);

	mGameOverTray->hideAll();


	//background
	////////////////////////////////////////////////////////////////////////////////
	Entity* background = scnMgr->createEntity(SceneManager::PrefabType::PT_PLANE);
	background->setMaterial(Ogre::MaterialManager::getSingleton().getByName("AndrewsMaterial2"));
	SceneNode* backgroundNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	
	backgroundNode->setScale(0.0828f, 0.08f, 1.0f);
	backgroundNode->attachObject(background);



	//final touches
	//////////////////////////////////////////////////////////////////
	polyMode = Ogre::PolygonMode::PM_SOLID;
	
	createCamera();

	ball = new Ball(6, 11, scnMgr);
	paddle = new Paddle(scnMgr);

	Ogre::FrameListener* FrameListener = new MyFrameListener(this);
	mRoot->addFrameListener(FrameListener);
	UI_timer.reset();
	isGameRunning = true;
}


///basic camera Setup
void Game::createCamera()
{

	//! [camera]
	SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

	// create the camera
	cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(5); // specific to this sample
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);
	camNode->setPosition(0, 0, 15);
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Node::TS_WORLD);

	// and tell it to render into the main window
	getRenderWindow()->addViewport(cam);

	//! [camera]
}

///receives key pressed inputs. escape = quit app; w,s,up and down for paddle inputs; plus r for wire frame modes 
bool Game::keyPressed(const KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}

	if(evt.keysym.sym == SDLK_DOWN || evt.keysym.sym == 's')
	{
		verticalInput = -1;
	}
	else if(evt.keysym.sym == SDLK_UP || evt.keysym.sym == 'w')
	{
		verticalInput = 1;
	}


	if (evt.keysym.sym == 'r')
	{
		if (polyMode == PM_SOLID)
		{
			polyMode = Ogre::PolygonMode::PM_WIREFRAME;
		}
		else if (polyMode == PM_WIREFRAME)
		{
			polyMode = Ogre::PolygonMode::PM_POINTS;
		}
		else if (polyMode == PM_POINTS)
		{
			polyMode = Ogre::PolygonMode::PM_SOLID;
		}
		cam->setPolygonMode(polyMode);
	}



	return true;
}

///used to reset paddle inputs after releasing a key. Allows for continuous movement until a key is released
bool Game::keyReleased(const KeyboardEvent& evt)
{

	if (evt.keysym.sym == 's' || evt.keysym.sym == 'w' || evt.keysym.sym == SDLK_UP || evt.keysym.sym == SDLK_DOWN)
	{
		verticalInput = 0;
	}
	return true;
}

///calls update UI, ball and paddle, responds to losing lives and scoring point conditions. only runs while isGameRunning == true
void Game::Update(float deltaTime)
{
	if(isGameRunning)
	{
		bool wasBallMissed = ball->Update(deltaTime);

		if (verticalInput != 0)
		{
			paddle->GetInput(verticalInput);
			paddle->Update(deltaTime);
		}

		if (UI_timer.getMilliseconds() > 500)
		{
			UpdateFrameStats((int)(1 / deltaTime), (int)(deltaTime * 1000));
			UI_timer.reset();
		}

		if (wasBallMissed)
		{
			lives--;
			mLivesLabel->setCaption("Lives: " + std::to_string(lives));
			if(lives <= 0)
				StopRunning();
		}

		Vector3 normal;
		if (CheckPaddleCollision(normal) && ball->OnPaddleCollision(normal))
		{
			score++;
			mScoreLabel->setCaption("Score: " + std::to_string(score));
		}
	}
	else if (mRestartButton->getState() == BS_DOWN)
	{
		Reset();
	}
}




///updates fps and time per update, (use a timer to avoid calling every frame. they update too quickly to read otherwise)
void Game::UpdateFrameStats(int fps, int tpu)
{
	this->fps = fps ;
	mFpsLabel->setCaption("FPS: " + std::to_string(fps));
	this->tpu = tpu;
	mTpuLabel->setCaption( "Time/Update: " + std::to_string(tpu) + "ms");
}



///checks for ball and paddle for aabb - circle collision. returns true on intersection, saves the normal as an output parameter
bool Game::CheckPaddleCollision(Vector3& normal)
{
	Ogre::Vector2 distance;
	distance.x = abs(ball->GetPosition().x - paddle->GetPosition().x);
	distance.y = abs(ball->GetPosition().y - paddle->GetPosition().y);

	Ogre::Vector3 paddleSize = paddle->GetHalfSize();
	Ogre::Vector3 ballSize = ball->GetHalfSize();


	if(distance.x > paddleSize.x + ballSize.x) {return false;}
	if(distance.y > paddleSize.y + ballSize.y) {return false;}

	if(distance.y <= paddleSize.y ) //hit the side
	{	
		normal = Vector3(ball->GetPosition().x - paddle->GetPosition().x, 0, 0);
		return true;
	}
	if (distance.x <= paddleSize.x) //hit the top
	{
		normal = Vector3(0, ball->GetPosition().y - paddle->GetPosition().y, 0);
		return true;
	}

	float cornerDistance = (distance.x - paddleSize.x) * (distance.x - paddleSize.x) +
		(distance.y - paddleSize.y) * (distance.y - paddleSize.y);

	if (cornerDistance <= (ballSize.x * ballSize.x));
	{
		normal = Vector3(ball->GetPosition().x - paddle->GetPosition().x, ball->GetPosition().y - paddle->GetPosition().y, 0);
		return true;
	}
	
	return false;
}



//pauses updates, displays game over widget
void Game::StopRunning()
{
	isGameRunning = false;
	mGameOverTray->showAll();
	mFinalScoreLabel->setCaption("Your Score: " + std::to_string(score));
}


///restarts game, called when the replay button is pressed
void Game::Reset()
{
	mGameOverTray->hideAll();
	score = 0;
	lives = 5;
	isGameRunning = true;
	mLivesLabel->setCaption("Lives: " + std::to_string(lives));
	mScoreLabel->setCaption("Score: " + std::to_string(score));
}

///@brief initializes and runs the game class application
int main(int argc, char** argv)
{
	try
	{
		Game app;
		app.initApp();
		app.getRoot()->startRendering();
		app.closeApp();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error occurred during execution: " << e.what() << '\n';
		return 1;
	}

	return 0;
}