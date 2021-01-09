#include "GuiButton.h"
#include "Window.h"
#include "Audio.h"

GuiButton::GuiButton(int id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
    hoverFx = app->audio->LoadFx("Assets/audio/fx/hover.ogg");
    clickFx = app->audio->LoadFx("Assets/audio/fx/click.ogg");
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // mouse offset
        mouseX -= app->render->camera.x / app->win->GetScale();
        mouseY -= app->render->camera.y / app->win->GetScale();

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiButton::Draw()
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: 
        app->render->DrawRectangle(bounds, 100, 100, 100, 255);
        break;
    case GuiControlState::NORMAL: 
        app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        hover = true;
        click = true;
        break;
    case GuiControlState::FOCUSED:
        app->render->DrawRectangle(bounds, 255, 255, 0, 255);
        click = true;
        if (hover == true)
        {
            app->audio->PlayFx(hoverFx);
            hover = false;
        }
        break;
    case GuiControlState::PRESSED: 
        app->render->DrawRectangle(bounds, 0, 255, 255, 255);
        hover = true;
        if (click == true) 
        { 
            app->audio->PlayFx(clickFx);
            click = false;
        }
        break;
    case GuiControlState::SELECTED: 
        app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        hover = true;
        click = true;
        break;
    default:
        break;
    }

    return false;
}
