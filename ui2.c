#include <stdio.h>
#include <SDL2/SDL.h>
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/forge.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/patch/patch.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#include <SDL2/SDL_syswm.h>
#include <xcb/xcb.h>

#define SDLMETER_UI_URI "http://m4l3z.abod.co/plugin/sdlmeter#ui"
#define SDLMETER_URI "http://m4l3z.abod.co/plugin/sdlmeter"

// Drawing area size
#define DAWIDTH  (640)
#define DAHEIGHT (480)
const char* WINDOW_TITLE = "SDL Meter ";


typedef struct {

    LV2UI_Write_Function write;
    LV2UI_Controller controller;
    LV2UI_Widget* widget;
    SDL_Surface *sdlscreen;
    SDL_Surface *meter;
    SDL_Window* window;
    void* parentXwindow;
}sdlmeterUI;

static LV2UI_Handle instantiate(const LV2UI_Descriptor*  descriptor, const char*  plugin_uri,
        const char*  bundle_path, LV2UI_Write_Function      write_function, LV2UI_Controller    controller, LV2UI_Widget*             widget, const LV2_Feature* const* features)
{
            sdlmeterUI* ui = (sdlmeterUI*)malloc(sizeof(sdlmeterUI));
            ui->write = write_function;
            ui->controller = controller;
            ui->sdlscreen = NULL;
            ui->meter = NULL;
            ui->widget = widget;
            LV2UI_Resize *host_resize = NULL;
            ui->parentXwindow = 0;

            if (strcmp(plugin_uri, SDLMETER_URI) != 0) {
                        printf(stderr, "Error: this GUI does not support plugin with URI %s\n", plugin_uri);
                                return NULL;
            }


            for(int i =0; features[i]; i++)
            {
                if(!strcmp(features[i]->URI, LV2_UI__parent))
                    {
                        ui->parentXwindow = features[i]->data;
                        printf("Found parent window : %i", ui->parentXwindow);
                    }
                else if(!strcmp(features[i]->URI, LV2_UI__resize))
                    host_resize = features[i]->data;
            }
            if(!ui->parentXwindow)
            {
                free(ui);
                return NULL;
            }

            char winhack[100];
            sprintf(winhack, "SDL_WINDOWID=%ld", ui->parentXwindow);
            putenv(winhack);
            
            //ui->window = SDL_CreateWindowFrom(ui->parentXwindow);
            ui->window = SDL_CreateWindow("test", 0, 0, 640, 480, NULL);
            if(!ui->window) printf("\n unable to create sdl window with winhack from %ld", ui->parentXwindow);
            
            ui->sdlscreen=  SDL_CreateRGBSurface(0, 640, 480, 32,
                                                       0, 0, 0, 0);

            SDL_FillRect(ui->sdlscreen,  NULL, SDL_MapRGB(ui->sdlscreen->format, 255, 0, 0));
            SDL_BlitSurface(ui->sdlscreen, 0, SDL_GetWindowSurface(ui->window),0);
            SDL_UpdateWindowSurface(ui->window );
            
            *(uintptr_t *)widget = (uintptr_t)ui->widget;
            if(host_resize)
                host_resize->ui_resize(host_resize->handle, 640, 480);
            SDL_Quit();

            return (LV2UI_Handle) ui;

}

static void cleanup(LV2UI_Handle handle)
{
            
   sdlmeterUI* ui = (sdlmeterUI*)handle;
   SDL_Quit();
   free(ui);
}


static int ui_show(LV2UI_Handle handle)
{
    sdlmeterUI* ui = (sdlmeterUI*)handle;
   // SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,SDL_HWSURFACE | SDL_DOUBLEBUF );
    /*
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_WM_SetCaption( WINDOW_TITLE, 0 );
    SDL_FillRect(ui->screen,  NULL, SDL_MapRGB(ui->screen->format, 255, 0, 0));
    SDL_Flip(ui->screen); 
    SDL_UpdateRect(ui->screen ,0 ,0 ,0 ,0);
    */
            //SDL_Init(SDL_INIT_VIDEO);
            //SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,SDL_HWSURFACE | SDL_DOUBLEBUF );
            //SDL_Window* window =SDL_CreateWindowFrom((const void *) ui->widget);
            //ui->screen= SDL_GetWindowSurface( ui->window );
            //SDL_FillRect(ui->screen,  NULL, SDL_MapRGB(ui->screen->format, 255, 0, 0));
            //SDL_UpdateWindowSurface( ui->window );
            //ui->widget = SDL_GetWindowID(window);
           SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, "1");

            ui->window = SDL_CreateWindowFrom(ui->parentXwindow);
            if(!ui->window) {printf("\n couldnt create sdl window");}
            ui->sdlscreen = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
            SDL_BlitSurface(ui->sdlscreen, 0, SDL_GetWindowSurface(ui->window),0);
            if(!SDL_UpdateWindowSurface(ui->window))  {printf("\n couldnt update ui");}
    
    return 0;
}

static int ui_hide(LV2UI_Handle handle)
{
    sdlmeterUI* ui = (sdlmeterUI*)handle;
    
    
    SDL_Quit();
    return 0;
}
static void send_ui_state(LV2UI_Handle handle)
{
    sdlmeterUI* ui = (sdlmeterUI*)handle;
}

static void send_ui_disable(LV2UI_Handle handle)
{
    sdlmeterUI* ui = (sdlmeterUI*)handle;
}

static void send_ui_enable(LV2UI_Handle handle)
{
    sdlmeterUI* ui = (sdlmeterUI*)handle;
}
static void port_event(LV2UI_Handle handle, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void* buffer)
{
    sdlmeterUI* ui = (sdlmeterUI*)handle;

}

static const void*
extension_data(const char* uri)
{
    return NULL;
}


static const LV2UI_Descriptor descriptor = {
    SDLMETER_UI_URI,
    instantiate,
    cleanup,
    port_event,
    extension_data,
    NULL
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
