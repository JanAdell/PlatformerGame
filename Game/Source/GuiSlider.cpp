#include "GuiSlider.h"
#include "Window.h"
#include "Audio.h"

GuiSlider::GuiSlider(int id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->sliderBounds = this->bounds;
    this->text = text;
    this->minValue == bounds.x;
    this->maxValue == bounds.w + bounds.x;
    hoverFx = app->audio->LoadFx("Assets/audio/fx/hover.ogg");
    clickFx = app->audio->LoadFx("Assets/audio/fx/click.ogg");
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
    this->minValue = bounds.x - (this->value);
    this->maxValue = bounds.x + ((280 - this->value));
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

            // TODO.
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }
        }
        if (state==GuiControlState::PRESSED)
        {
            if ((app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)&&(mouseX>minValue) && (mouseX < maxValue))
            {
                bounds.x = mouseX - (bounds.w / 2) / app->win->GetScale();
                ChangeSliderValue();
                NotifyObserver();
                if (id == 2) app->audio->PlayFx(clickFx, 0);
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::Draw()
{
    SDL_Rect rect = { sliderBounds.x - 15, sliderBounds.y - 1, 300, 30 };
    
    app->render->DrawRectangle({ rect.x - app->render->camera.x, rect.y - app->render->camera.y ,rect.w,rect.h }, 255, 105, 180, 255);
      

    switch (state)
    {
    case 
        GuiControlState::DISABLED: app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 100, 100, 100, 255);
        break;
    case 
        GuiControlState::NORMAL: app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 140, 75, 185, 255);
        break;
    case 
        GuiControlState::FOCUSED: app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 200, 100, 220, 255);
        break;
    case 
        GuiControlState::PRESSED: app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 250, 75, 150, 255);
        break;
    case 
        GuiControlState::SELECTED: app->render->DrawRectangle({ bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, bounds.w, bounds.h }, 0, 255, 0, 255);
        break;
    default:
        break;
    }

    return false;
}

void GuiSlider::ChangeSliderValue()
{
    this->value = bounds.x - minValue;

    if (this->value > 280) 
        this->value = 280;

    else if (this->value <= 5) 
        this->value = 0;

   if (bounds.x >= 770) 
       bounds.x = 770;

   if (bounds.x <= 550) 
       bounds.x = 550;
}

int GuiSlider::ReturnValue() const
{
    return this->value;
}