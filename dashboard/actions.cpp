#include "Arduino.h"
#include "actions.h"
#include "screens.h"
#include <lvgl.h>

//extern objects_t objects;; // Dichiarazione degli oggetti generati da EEZ Studio

extern int telemetryprotocol;
extern uint luminosity;
extern uint volume;
extern bool vario;
extern float battrxalarm;
extern float batttxalarm;
extern int saveconfig;
extern int needrestart;
extern uint rssilowvalue;
extern uint rssicritvalue;
extern float altitude;
extern float altitudeoffset;

int currentScreenIndex = 0;  // 0: main, 1: settings

// Funzione per caricare uno schermo in base all'indice
void loadScreen(int screenIndex) {
  if (screenIndex == 0) {
    lv_scr_load(objects.main);
    currentScreenIndex = 0;
  } else if (screenIndex == 1) {
    lv_scr_load(objects.gps);
    currentScreenIndex = 1;
  } else if (screenIndex == 2) {
    lv_scr_load(objects.settings);
    currentScreenIndex = 2;
  } else {
    LV_LOG_WARN("Indice schermo non valido");
  }
}

void action_my_event(lv_event_t *e) {
  lv_indev_t *indev = lv_event_get_indev(e);
  lv_dir_t dir = lv_indev_get_gesture_dir(indev);
  switch (dir) {
    case LV_DIR_RIGHT:
      if (currentScreenIndex == 0) {
        loadScreen(1);  // Vai a gps
      } else if (currentScreenIndex == 1) {
        loadScreen(2);  // Vai a settings
      }
      break;
    case LV_DIR_LEFT:
      if (currentScreenIndex == 1) {
        loadScreen(0);  // Vai a main
      } else if (currentScreenIndex == 2) {
        loadScreen(1);  // Vai a gps
      }
      break;
    default:
      break;
  }
}

void action_slider_changed(lv_event_t *e) {
  lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e);  // Cast esplicito
  if (slider == objects.luxvalue) {
    luminosity = lv_slider_get_value(slider);
    analogWrite(21, luminosity);
  }
  if (slider == objects.volvalue) {
    volume = lv_slider_get_value(slider);
  }
  if (slider == objects.alarmvalue) {
    battrxalarm = (lv_slider_get_value(slider) / 10.0);
    char buffer[8];
    sprintf(buffer, "%0.1f", battrxalarm);
    lv_label_set_text(objects.alarmvaluelabel, buffer);
  }
  if (slider == objects.alarmvaluetx) {
    batttxalarm = (lv_slider_get_value(slider) / 10.0);
    char buffer[8];
    sprintf(buffer, "%0.1f", batttxalarm);
    lv_label_set_text(objects.alarmvaluetxlabel, buffer);
  }
  if (slider == objects.alarmrssilow) {
    rssilowvalue = lv_slider_get_value(slider);
    char buffer[8];
    sprintf(buffer, "%d", rssilowvalue);
    lv_label_set_text(objects.alarmrssilowlabel, buffer);
  }
  if (slider == objects.alarmrssicrit) {
    rssicritvalue = lv_slider_get_value(slider);
    char buffer[8];
    sprintf(buffer, "%d", rssicritvalue);
    lv_label_set_text(objects.alarmrssicritlabel, buffer);
  }

}

void action_switch_changed(lv_event_t *e) {
  lv_obj_t *sw = (lv_obj_t *)lv_event_get_target(e);
  if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
    vario = true;
    saveconfig = 1;
  } else {
    vario = false;
    saveconfig = 1;
  }
}

void action_button_pressed(lv_event_t *e) {
  lv_obj_t *button = (lv_obj_t *)lv_event_get_target(e);
  lv_event_code_t code = lv_event_get_code(e);
  uint16_t selected_index = lv_dropdown_get_selected(objects.proto);
  if (button==objects.altreset) {
    altitudeoffset=altitude;
  } else if (button==objects.save) {
    if (telemetryprotocol != selected_index) {
      telemetryprotocol = selected_index;
      saveconfig = 1;
      needrestart = 1;
    } else {
      saveconfig = 1;
      loadScreen(0);
    }
  }
}

void action_screen_loaded(lv_event_t *e) {
  lv_obj_t *screen = (lv_obj_t *)lv_event_get_target(e);
  if (screen==objects.settings) {
    lv_obj_scroll_to_y(objects.settings, 0, LV_ANIM_OFF);
  }
}