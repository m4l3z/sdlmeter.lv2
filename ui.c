#include <stdio.h>
#include <SDL.h>

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/forge.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/patch/patch.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#define SDLMETER_UI_URI "http://m4l3z.abod.co/plugin/sdlmeter#ui"
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "SDL Meter ";


typedef enum {
    AMP_INPUT  = 1,
    AMP_OUTPUT = 2
} PortIndex;

typedef struct {
    // Port buffers
    const float* gain;
    const float* input;
    float*       output;
} Meter;

typedef struct {
    LV2_Atom_Forge forge;


    LV2UI_Write_Function write;
    LV2UI_Controller controller;

    SDL_Surface *screen;
    SDL_Surface *meter;
   
}sdlmeterUI;

static LV2UI_Handle instantiate(const LV2UI_Descriptor*  descriptor, const char*  plugin_uri,
        const char*  bundle_path, LV2UI_Write_Function      write_function, LV2UI_Controller    controller, LV2UI_Widget*             widget, const LV2_Feature* const* features)
{
            sdlmeterUI* ui = (sdlmeterUI*)malloc(sizeof(sdlmeterUI));
            ui->write = write_function;
            ui->controller = controller;
            ui->screen = NULL;
            ui->meter = NULL;

            return ui;

}

static void cleanup(LV2UI_Handle handle)
{
            
   sdlmeterUI* ui = (sdlmeterUI*)handle;
   free(ui);
}


static int ui_show(LV2UI_Handle handle)
{
    sdlmeterUI* ui = (sdlmeterUI*)handle;
    SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,SDL_HWSURFACE | SDL_DOUBLEBUF );
    SDL_WM_SetCaption( WINDOW_TITLE, 0 );
    
    return 0;
}

int main(int argc, char **argv)
{
       SDL_Init( SDL_INIT_VIDEO );

       SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,SDL_HWSURFACE | SDL_DOUBLEBUF );
       SDL_WM_SetCaption( WINDOW_TITLE, 0 );
       
       SDL_Event event;
       int gameRunning = 1;

       while (gameRunning)
       {
           if (SDL_PollEvent(&event))
           {
               if (event.type == SDL_QUIT)
               {
                   gameRunning = 0;
               }
           }
       }
       SDL_Quit();

       return 0;
}

static const LV2UI_Descriptor descriptor = {
    SDLMETER_UI_URI,
    instantiate,
    cleanup
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
    switch (index) {
        case 0:
            return &descriptor;
        default:
            return NULL;
    }
}
