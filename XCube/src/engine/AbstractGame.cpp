#include "AbstractGame.h"
#include "../engine/EventEngine.h"

AbstractGame::AbstractGame() : running(true), paused(false), gameTime(0.0) {
	std::shared_ptr<XCube2Engine> engine = XCube2Engine::getInstance();

	GUIElement* ResumeButton = new Button(1, DEFAULT_WINDOW_HEIGHT * 0.3, 256, 64, false, true, SDL_COLOR_DARKGREY, SDL_COLOR_SLATEBLUE, SDL_COLOR_LIGHTGREY, "Resume Game", SDL_COLOR_WHITE, 12);
	ResumeButton->SetElementFunction([this]() {publicResume(); });

	GUIElement* ExitButton = new Button(1, DEFAULT_WINDOW_HEIGHT * 0.6, 256, 64, true, true, SDL_COLOR_DARKGREY, SDL_COLOR_SLATEBLUE, SDL_COLOR_LIGHTGREY, "Exit Game", SDL_COLOR_SLATEBLUE, 14);
	ExitButton->SetElementFunction([this]() {running = false; });

	GUIElement* EasterEggButton = new Button(1, DEFAULT_WINDOW_HEIGHT * 0.6, 256, 64, false, true, SDL_COLOR_DARKGREY, SDL_COLOR_SLATEBLUE, SDL_COLOR_LIGHTGREY, "CI517: Dale Hunt - 2023", SDL_COLOR_GREEN, 16);
	EasterEggButton->SetElementFunction([this]() { printf("\nDEBUG: Template Button - Add Function.") ;});
	EasterEggButton->SetEasterEggSound();

	GUIController.GUIElements.push_back(ResumeButton);
	GUIController.GUIElements.push_back(ExitButton);
	GUIController.GUIElements.push_back(EasterEggButton);

	// engine ready, get subsystems
	gfx = engine->getGraphicsEngine();
	sfx = engine->getAudioEngine();
	eventSystem = engine->getEventEngine();
	physics = engine->getPhysicsEngine();
    mySystem = engine->getMyEngineSystem();
}

AbstractGame::~AbstractGame() {
#ifdef __DEBUG
	debug("AbstractGame::~AbstractGame() started");
#endif

	// kill Game class' instance pointers
	// so that engine is isolated from the outside world
	// before shutting down
	gfx.reset();
	eventSystem.reset();

	// kill engine
	XCube2Engine::quit();

#ifdef __DEBUG
	debug("AbstractGame::~AbstractGame() finished");
	debug("The game finished and cleaned up successfully. Press Enter to exit");
	getchar();
#endif
}

int AbstractGame::runMainLoop() {
#ifdef __DEBUG
	debug("Entered Main Loop");
#endif
	while (running) {
		gfx->setFrameStart();
		eventSystem->pollEvents();

		if (eventSystem->isPressed(Key::ESC) || eventSystem->isPressed(Key::QUIT))
			running = false;

		handleKeyEvents();
		handleMouseEvents();

		if (!paused) {
			gfx->clearScreen();
			update();
			updatePhysics();
			gameTime += 0.016;	// 60 times a sec
			render();
			renderUI();
			gfx->showScreen();
		}
		else
		{
			gfx->clearScreen();
			GUIController.GUIUpdate(eventSystem->getMousePos(), eventSystem, sfx);
			GUIController.DrawGUI(gfx);
			gfx->showScreen();
		}

		gfx->adjustFPSDelay(16);	// atm hardcoded to ~60 FPS
	}

#ifdef __DEBUG
	debug("Exited Main Loop");
#endif

	return 0;
}

void AbstractGame::handleMouseEvents() {
	if (eventSystem->isPressed(Mouse::BTN_LEFT)) onLeftMouseButton();
	if (eventSystem->isPressed(Mouse::BTN_RIGHT)) onRightMouseButton();
}

void AbstractGame::updatePhysics() {
	physics->update();
}

void AbstractGame::onLeftMouseButton() {}
void AbstractGame::onRightMouseButton() {}
void AbstractGame::renderUI() {}

// DH: Register My RenderGUI Function
//void AbstractGame::RenderGUI() 
//{
//	// DH: Setting Variables To Have Default Window Height * Width
//	int GameWindowWidth = DEFAULT_WINDOW_WIDTH;
//	int GameWindowHeight = DEFAULT_WINDOW_HEIGHT;
//	// DH: Set Button Height & Width To Be 25% of the Window Size.
//	int ButtonWidth = 375;
//	int ButtonHeight = 64;
//
//	std::string ResumeButtonText = "Resume";
//	int ResumeButtonX = GameWindowWidth / 2 - (ButtonWidth / 2);
//	int ResumeButtonY = GameWindowHeight / 2 - (ButtonHeight * 2);
//	// DH: Assign All UI Buttons
//	Button ResumeButton(ResumeButtonX, ResumeButtonY, ButtonWidth, ButtonHeight);
//	Button RestartButton(GameWindowWidth / 2 - (ButtonWidth / 2), GameWindowHeight / 2 - (ButtonHeight / 2), ButtonWidth, ButtonHeight);
//	Button ExitButton(GameWindowWidth / 2 - (ButtonWidth / 2), GameWindowHeight / 2 + (ButtonHeight), ButtonWidth, ButtonHeight);
//	// DH: Only Check Mouse Position & Button Update on Button Click
//	if (eventSystem->isPressed(BTN_LEFT))
//	{
//		Point2 MousePointerPosition = eventSystem->getMousePos();
//
//		ResumeButton.UpdateResumeButton(MousePointerPosition);
//		RestartButton.UpdateRestartButton(MousePointerPosition);
//		ExitButton.UpdateExitButton(MousePointerPosition);
//	}
//	gfx->setDrawColor(SDL_COLOR_BLUE);
//	gfx->fillRect(&ResumeButton.buttonRect);
//	/*gfx->drawTexture(ResumeButton.buttonTexture, &ResumeButton.buttonDRect);*/
//	gfx->setDrawColor(SDL_COLOR_AQUA);
//	gfx->fillRect(&RestartButton.buttonRect);
//	gfx->setDrawColor(SDL_COLOR_YELLOW);
//	gfx->fillRect(&ExitButton.buttonRect);
//	// DH: Draw Borders Over Each Button
//	gfx->setDrawColor(SDL_COLOR_WHITE);
//	gfx->drawRect(&ResumeButton.buttonRect);
//	gfx->drawRect(&RestartButton.buttonRect);
//	gfx->drawRect(&ExitButton.buttonRect);
//	// DH: Draw Text On Each Button
//	gfx->setDrawColor(SDL_COLOR_BLACK);
//	gfx->drawText(ResumeButtonText, ResumeButtonX, ResumeButtonY);
//}