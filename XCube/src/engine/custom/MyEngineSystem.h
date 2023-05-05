#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include "../EngineCommon.h"
#include "../EventEngine.h"
#include "../GraphicsEngine.h"
#include "../ResourceManager.h"
#include <vector>
#include <functional>

#include <vector>
class MyEngineSystem {
	friend class XCube2Engine;
public:
private:
	int ePosX, ePosY, eWidth, eHeight;
};

class GUIElement
{
public:
	int ePosX, ePosY, eWidth, eHeight, eTextFontSize;
	bool eCenterV, eCenterH, isMouseOver = false;
	SDL_Color eColor, eBorderColor, eTextColor, eHoverColor;
	std::string eText;

	Mix_Music* ButtonClickSFX = ResourceManager::loadMP3("C:\\University Work\\Game Engine Fundamentals\\XCube\\build\\res\\DH_Audio\\ButtonClick.mp3");
	Mix_Music* EasterEggSFX = ResourceManager::loadMP3("C:\\University Work\\Game Engine Fundamentals\\XCube\\build\\res\\DH_Audio\\EasterEggSound.mp3");

	virtual void eUpdate(Point2 MousePointerPos, std::shared_ptr<EventEngine> eventSystem, std::shared_ptr<AudioEngine> sfx);
	virtual void eDraw(std::shared_ptr<GraphicsEngine> gfx);
	
	using FunctionType = std::function<void()>;
	FunctionType eFunction;
	GUIElement() : eFunction([]() {}) {};

	virtual void SetElementFunction(FunctionType function)
	{
		eFunction = function;
	}

	void RunElementFunction()
	{
		if (eFunction)
		{
			eFunction();
		}
	}

	void PlayButtonSound(std::shared_ptr<AudioEngine> sfx)
	{
		sfx->playMP3(ButtonClickSFX, 1);
	}

	void SetEasterEggSound()
	{
		ButtonClickSFX = EasterEggSFX;
	}
};

class Button : public GUIElement
{
public:
	Button(int buttonX, int buttonY, int buttonWidth, int buttonHeight, bool buttonCenterV, bool buttonCenterH, SDL_Color buttonColor, SDL_Color buttonBorderColor, SDL_Color buttonHoverColor, const std::string buttonText, SDL_Color buttonTextColor, int buttonTextFontSize);
	SDL_Rect* ButtonRect;

	void eUpdate(Point2 MousePointerPos, std::shared_ptr<EventEngine> eventSystem, std::shared_ptr<AudioEngine> sfx);
	void eDraw(std::shared_ptr<GraphicsEngine> gfx);
	void SetElementFunction(FunctionType function);

private:

};

class GUIController
{
public:
	std::vector<GUIElement*> GUIElements;
	void GUIUpdate(Point2 MousePointerPos, std::shared_ptr<EventEngine> eventSystem, std::shared_ptr<AudioEngine> sfx);
	void DrawGUI(std::shared_ptr<GraphicsEngine> gfx);
};

#endif