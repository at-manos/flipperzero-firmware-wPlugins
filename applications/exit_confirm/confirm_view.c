#include <furi.h>
#include "confirm_view.h"
#include <stdlib.h>
#include <gui/view_dispatcher.h>
#include <stdlib.h>
#include <gui/elements.h>
#include <gui/view_i.h>




typedef struct {
    char* confirmText;
    char* denyText;
} ConfirmModel;

struct Confirm {
    View* view;
    BackCallback back_callback;
    void* callback_context;
};

void confirm_view_draw_callback(Canvas* canvas, void* ctx){
    ConfirmModel* model = ctx;
    UNUSED(model);
    // with_view_model(instance->view, (ConfirmModel* model){
        canvas_draw_str(canvas, 64, 32, "Are you sure you want to quit?");
        elements_button_left(canvas, "confirm");
        elements_button_right(canvas, "deny");
        // canvas_draw_str(canvas, 20, 20, "Test");
        // canvas_draw_str(canvas, 20, 20, "test");
        // return true;
    // })
    // return true;
}

void confirm_set_back_callback(Confirm* instance, BackCallback cb, void* ctx){
    instance->back_callback = cb;
    instance->callback_context = ctx;
}

bool confirm_view_input_callback(InputEvent* event, void* ctx){
    Confirm* instance = ctx;
    bool consumed = false;
    with_view_model(instance->view, (ConfirmModel* model){
        if(event->type==InputTypePress){
            switch(event->key){
                case InputKeyLeft:
                    view_previous(instance->view);
                    consumed = true;
                case InputKeyRight:
                    instance->back_callback(instance->callback_context);
                    consumed = true;
                default:
                    break;
            }
        }
        return consumed;
    })
    return consumed;
}

View* confirm_get_view(Confirm* instance){
    return instance->view;
}

Confirm* confirm_view_alloc(){
    Confirm* instance = malloc(sizeof(Confirm));

    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLockFree, sizeof(ConfirmModel));
    // with_view_model(instance->view, (ConfirmModel* model){
    //     strncpy(model->confirmText, "Confirm", 128);
    //     strncpy(model->denyText, "Deny", 128);
    //     return true;
    // })

    view_set_draw_callback(instance->view, confirm_view_draw_callback);
    view_set_input_callback(instance->view, confirm_view_input_callback);
    // view_set_exit_callback()
    return instance;
}

void confirm_view_free(Confirm* instance){
    view_free_model(instance->view);
    view_free(instance->view);
    free(instance);
}