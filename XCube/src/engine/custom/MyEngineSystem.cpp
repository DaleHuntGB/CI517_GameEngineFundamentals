#include "MyEngineSystem.h"
#include "../EventEngine.h"
#include "../AudioEngine.h"

// DH: Update GUI -> Loop through all elements in the GUIElement Array and update each one.
void GUIController::GUIUpdate(Point2 MousePointerPos, std::shared_ptr<EventEngine> eventSystem, std::shared_ptr<AudioEngine> sfx)
{
	for (GUIElement* UIElement : GUIElements)
	{
		UIElement->eUpdate(MousePointerPos, eventSystem, sfx);
	}
}

// DH: DrawGUI -> Loop through all elements in the GUIElement Array and draw each one.
void GUIController::DrawGUI(std::shared_ptr<GraphicsEngine> gfx)
{
	for (GUIElement* UIElement : GUIElements)
	{
		UIElement->eDraw(gfx);
	}
}

void GUIElement::eUpdate(Point2 MousePointerPos, std::shared_ptr<EventEngine> eventSystem, std::shared_ptr<AudioEngine> sfx)
{
	
}

// DH: Update drawn element.
void GUIElement::eDraw(std::shared_ptr<GraphicsEngine> gfx)
{
	
}

Button::Button(int buttonX, int buttonY, int buttonWidth, int buttonHeight, bool buttonCenterV, bool buttonCenterH, SDL_Color buttonColor, SDL_Color buttonBorderColor, SDL_Color buttonHoverColor, const std::string buttonText, SDL_Color buttonTextColor, int buttonTextFontSize)
{
	ePosX = buttonX;
	ePosY = buttonY;
	eWidth = buttonWidth;
	eHeight = buttonHeight;
	eCenterV = buttonCenterV;
	eCenterH= buttonCenterH;
	eText = buttonText;
	eTextFontSize = buttonTextFontSize;
	eColor = buttonColor;
	eBorderColor = buttonBorderColor;
	eHoverColor = buttonHoverColor;
	eTextColor = buttonTextColor;

	if (eCenterH == true)
	{
		ePosX = DEFAULT_WINDOW_WIDTH / 2 - (eWidth / 2);
	}

	if (eCenterV == true)
	{
		ePosY = DEFAULT_WINDOW_HEIGHT / 2 - (eHeight / 2);
	}

	ButtonRect = new SDL_Rect { ePosX, ePosY, eWidth, eHeight };
}


void Button::SetElementFunction(FunctionType function)
{
	eFunction = function;
}


void Button::eUpdate(Point2 MousePointerPos, std::shared_ptr<EventEngine> eventSystem, std::shared_ptr<AudioEngine> sfx)
{
	SDL_Rect MousePointerPosRect = { MousePointerPos.x, MousePointerPos.y, 1, 1 };
	if (SDL_HasIntersection(ButtonRect, &MousePointerPosRect))
	{
		if (eventSystem->isPressed(BTN_LEFT))
		{
			RunElementFunction();
			PlayButtonSound(sfx);
		}
		isMouseOver = true;
	}
	else
	{
		isMouseOver = false;
	}
}

void Button::eDraw(std::shared_ptr<GraphicsEngine> gfx)
{
	if (isMouseOver == true)
	{
		gfx->setDrawColor(eHoverColor);
	}
	else
	{
		gfx->setDrawColor(eColor);
	}
	gfx->fillRect(ButtonRect);
	gfx->setDrawColor(eBorderColor);
	gfx->drawRect(ButtonRect);
	gfx->setDrawColor(eTextColor);
	TTF_Font* GUIFont = ResourceManager::loadFont("res/fonts/DHFont.ttf", eTextFontSize);
	gfx->UseGUIFont(GUIFont);
	const char* UITextChar = eText.c_str();
	int UITextCharW, UITextCharH;
	TTF_SizeUTF8(gfx->GetFont(), UITextChar, &UITextCharW, &UITextCharH);
	gfx->DrawGUIText(eText, ((ePosX + ButtonRect->w / 2) - (UITextCharW / 2)), ((ePosY + ButtonRect->h / 2) - (UITextCharH / 2)), eTextFontSize);
	TTF_CloseFont(GUIFont);
}

