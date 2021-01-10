#include "GuiCheckBox.h"
#include "Window.h"
#include "Audio.h"

GuiCheckBox::GuiCheckBox(int id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // mouse offset
        mouseX -= app->render->camera.x / app->win->GetScale();
        mouseY -= app->render->camera.y / app->win->GetScale();

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x - app->render->camera.x) && (mouseX < (bounds.x - app->render->camera.x + bounds.w)) &&
            (mouseY > bounds.y - app->render->camera.y) && (mouseY < (bounds.y - app->render->camera.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw()
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
    {
        if (checked) app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 100, 100, 100, 100);
        else app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 100, 100, 100, 100);
    } break;
    case GuiControlState::NORMAL: 
    {
        hover = true;
        click = true;
        if (checked) app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 105, 240, 240, 255);
        else app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 105, 240, 240, 255);
    } break;
    case GuiControlState::FOCUSED:
        app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 255, 230, 100, 255);
        click = true;
        if (hover == true)
        {
            app->audio->PlayFx(hoverFx);
            hover = false;
        }
        break;
    case GuiControlState::PRESSED: 
        app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 230, 105, 200, 255);
        hover = true;
        if (click == true)
        {
            app->audio->PlayFx(clickFx);
            click = false;
        }
        break;
    case GuiControlState::SELECTED: 
        app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 180, 205, 85, 255);
        hover = true;
        click = true;
        break;
    default:
        break;
    }

    return false;
}
