#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_my_event(lv_event_t * e);
extern void action_slider_changed(lv_event_t * e);
extern void action_button_pressed(lv_event_t * e);
extern void action_switch_changed(lv_event_t * e);
extern void action_screen_loaded(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/