#pragma once
#include <gui/view.h>

typedef struct Confirm Confirm;

typedef void (*BackCallback)(void* ctx);

Confirm* confirm_view_alloc();

void confirm_view_free();

View* confirm_get_view();

void confirm_set_back_callback(Confirm* instance, BackCallback, void* ctx);