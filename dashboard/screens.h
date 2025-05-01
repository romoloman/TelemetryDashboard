#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *gps;
    lv_obj_t *settings;
    lv_obj_t *rssibar;
    lv_obj_t *rxbatt;
    lv_obj_t *rssitext;
    lv_obj_t *rxbatttext;
    lv_obj_t *rssivalue;
    lv_obj_t *rssvaluelabel;
    lv_obj_t *topline;
    lv_obj_t *rssibigtext;
    lv_obj_t *rxbattbigtext;
    lv_obj_t *swr;
    lv_obj_t *swrbigtext;
    lv_obj_t *a1;
    lv_obj_t *a1bigtext;
    lv_obj_t *a1units;
    lv_obj_t *a2;
    lv_obj_t *a2bigtext;
    lv_obj_t *a2units;
    lv_obj_t *txbatt;
    lv_obj_t *txbattbigtext;
    lv_obj_t *txbattunits;
    lv_obj_t *line2;
    lv_obj_t *alertlabel;
    lv_obj_t *alt;
    lv_obj_t *vspd;
    lv_obj_t *altbigtext;
    lv_obj_t *altunits;
    lv_obj_t *vspdbigtext;
    lv_obj_t *vspdunits;
    lv_obj_t *varioswitch;
    lv_obj_t *vario;
    lv_obj_t *altreset;
    lv_obj_t *latlabel;
    lv_obj_t *longlabel;
    lv_obj_t *altlabel;
    lv_obj_t *speedlabel;
    lv_obj_t *latlabel_1;
    lv_obj_t *latvalue;
    lv_obj_t *lonvalue;
    lv_obj_t *altvalue;
    lv_obj_t *speedvalue;
    lv_obj_t *headvalue;
    lv_obj_t *satlabel;
    lv_obj_t *satvalue;
    lv_obj_t *datavalue;
    lv_obj_t *oravalue;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *container;
    lv_obj_t *lux;
    lv_obj_t *luxvalue;
    lv_obj_t *vol;
    lv_obj_t *volvalue;
    lv_obj_t *alarmlabel;
    lv_obj_t *batteria;
    lv_obj_t *alarmvalue;
    lv_obj_t *alarmvaluelabel;
    lv_obj_t *batteriatx;
    lv_obj_t *alarmvaluetx;
    lv_obj_t *alarmvaluetxlabel;
    lv_obj_t *rssi;
    lv_obj_t *alarmrssilow;
    lv_obj_t *alarmrssilowlabel;
    lv_obj_t *rssi1;
    lv_obj_t *alarmrssicrit;
    lv_obj_t *alarmrssicritlabel;
    lv_obj_t *protolabel;
    lv_obj_t *proto;
    lv_obj_t *save;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_GPS = 2,
    SCREEN_ID_SETTINGS = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_gps();
void tick_screen_gps();

void create_screen_settings();
void tick_screen_settings();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/