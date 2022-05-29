#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <gui/view_dispatcher.h>
#include <stdlib.h>

#include "confirm_view.h"

// #include <elements/button_element.h>

typedef struct {
    Gui* gui;
    Confirm* confirmView;
    View* mainView;
    ViewDispatcher* view_dispatcher;
} ExitConfirm;

typedef enum {
    MainView,
    ConfirmView
} ViewID;


void enter_callback(void* ctx){
    FURI_LOG_I("ec", "enter_callback");
    ExitConfirm* inst = ctx;
    view_dispatcher_switch_to_view(inst->view_dispatcher, ConfirmView);
}

static uint32_t exit_cb(void* ctx){
    UNUSED(ctx);
    FURI_LOG_I("ec", "exit_cb");

    return VIEW_NONE;
}

void instant_exit(void* ctx){
    ExitConfirm* instance = ctx;
    view_dispatcher_switch_to_view(instance->view_dispatcher, VIEW_NONE);
}

static uint32_t prev_cb(void * ctx){
    FURI_LOG_I("ec", "prev_cb");
    UNUSED(ctx);
    return MainView;
}

void main_view_draw_cb(Canvas* canvas, void* ctx){
    canvas_draw_str(canvas, 25, 25, "mainview");
}

bool main_view_input_cb(InputEvent* event, void* ctx){
    ExitConfirm* instance = ctx;
    bool consumed = false;

    if(event->type==InputTypePress){
        switch(event->key){
            case InputKeyOk:
                enter_callback(instance);
                consumed = true;
            default:
                consumed = false;
        }
    }
    return consumed;
}






ExitConfirm* exit_confirm_alloc(){
    ExitConfirm* instance = malloc(sizeof(ExitConfirm));
    instance->gui = furi_record_open("gui");

    instance->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(instance->view_dispatcher);
    view_dispatcher_attach_to_gui(instance->view_dispatcher, instance->gui, ViewDispatcherTypeFullscreen);
    
    
    instance->confirmView = confirm_view_alloc();
    view_dispatcher_add_view(instance->view_dispatcher, ConfirmView, confirm_get_view((instance->confirmView)));
    view_set_previous_callback(confirm_get_view((instance->confirmView)), prev_cb);
    confirm_set_back_callback(instance->confirmView, instant_exit, instance);

    View* view = view_alloc();
    view_set_context(view, instance);
    view_set_draw_callback(view, main_view_draw_cb);
    view_set_input_callback(view, main_view_input_cb);
    view_set_previous_callback(view, exit_cb);
    instance->mainView = view;

    view_dispatcher_add_view(instance->view_dispatcher, MainView, instance->mainView);


    return instance;
}


void exit_confirm_free(ExitConfirm* instance){
    view_dispatcher_remove_view(instance->view_dispatcher, ConfirmView);
    confirm_view_free(instance->confirmView);

    view_dispatcher_remove_view(instance->view_dispatcher, MainView);
    view_free(instance->mainView);

    view_dispatcher_free(instance->view_dispatcher);

    furi_record_close("gui");

    free(instance);
}

int exit_confirm_run(ExitConfirm* instance){
    view_dispatcher_switch_to_view(instance->view_dispatcher, MainView);
    FURI_LOG_I("ec", "switched to conf");
    view_dispatcher_run(instance->view_dispatcher);
    FURI_LOG_I("ec", "ran viewdispatcher");
    return 0;
}

int32_t exit_confirm_app(void* p){
    UNUSED(p);
    ExitConfirm* exit_confirm = exit_confirm_alloc();
    FURI_LOG_I("ec", "allocated");
    exit_confirm_run(exit_confirm);
    FURI_LOG_I("ec", "ran");
    exit_confirm_free(exit_confirm);
    return 0;
}