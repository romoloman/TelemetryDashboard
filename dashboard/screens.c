#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            // rssibar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.rssibar = obj;
            lv_obj_set_pos(obj, 37, 9);
            lv_obj_set_size(obj, 80, 10);
            lv_bar_set_range(obj, 0, 120);
        }
        {
            // rxbatt
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.rxbatt = obj;
            lv_obj_set_pos(obj, 236, 9);
            lv_obj_set_size(obj, 80, 10);
            lv_bar_set_range(obj, 0, 8.4);
        }
        {
            // rssitext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.rssitext = obj;
            lv_obj_set_pos(obj, 2, 5);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "RSSI");
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff212121), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // rxbatttext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.rxbatttext = obj;
            lv_obj_set_pos(obj, 179, 5);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "RX Batt");
        }
        {
            // rssivalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.rssivalue = obj;
            lv_obj_set_pos(obj, 119, 5);
            lv_obj_set_size(obj, 25, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // rssvaluelabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.rssvaluelabel = obj;
            lv_obj_set_pos(obj, 144, 5);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Db");
        }
        {
            // topline
            lv_obj_t *obj = lv_line_create(parent_obj);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 319, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            objects.topline = obj;
            lv_obj_set_pos(obj, 0, 25);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xff008dff), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 3, 28);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "RSSI");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // rssibigtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.rssibigtext = obj;
            lv_obj_set_pos(obj, 92, 28);
            lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_bg_grad_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff001bff), LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 171, 67);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "RBat");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // rxbattbigtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.rxbattbigtext = obj;
            lv_obj_set_pos(obj, 225, 67);
            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 288, 67);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "V");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // SWR
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.swr = obj;
            lv_obj_set_pos(obj, 171, 30);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SWR");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // swrbigtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.swrbigtext = obj;
            lv_obj_set_pos(obj, 248, 29);
            lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_bg_grad_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff001bff), LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // A1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.a1 = obj;
            lv_obj_set_pos(obj, 4, 115);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "A1");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // a1bigtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.a1bigtext = obj;
            lv_obj_set_pos(obj, 59, 115);
            lv_obj_set_size(obj, 74, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // a1units
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.a1units = obj;
            lv_obj_set_pos(obj, 135, 115);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "V");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // A2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.a2 = obj;
            lv_obj_set_pos(obj, 172, 115);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "A2");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // a2bigtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.a2bigtext = obj;
            lv_obj_set_pos(obj, 225, 115);
            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // a2units
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.a2units = obj;
            lv_obj_set_pos(obj, 288, 115);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "V");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // txbatt
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.txbatt = obj;
            lv_obj_set_pos(obj, 4, 67);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "TBat");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // txbattbigtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.txbattbigtext = obj;
            lv_obj_set_pos(obj, 59, 67);
            lv_obj_set_size(obj, 74, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // txbattunits
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.txbattunits = obj;
            lv_obj_set_pos(obj, 135, 67);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "V");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // line2
            lv_obj_t *obj = lv_line_create(parent_obj);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 0, 200 }
            };
            lv_line_set_points(obj, line_points, 2);
            objects.line2 = obj;
            lv_obj_set_pos(obj, 165, 30);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xff0f1bd9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // alertlabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.alertlabel = obj;
            lv_obj_set_pos(obj, 45, 102);
            lv_obj_set_size(obj, 230, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Telemetria persa");
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffed7474), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_dash_gap(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Alt
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.alt = obj;
            lv_obj_set_pos(obj, 4, 158);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Alt");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // vspd
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.vspd = obj;
            lv_obj_set_pos(obj, 172, 158);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Vsp");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // altbigtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.altbigtext = obj;
            lv_obj_set_pos(obj, 59, 158);
            lv_obj_set_size(obj, 75, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_bg_grad_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_line_width(obj, 1, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff001bff), LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // altunits
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.altunits = obj;
            lv_obj_set_pos(obj, 133, 158);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "m");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // vspdbigtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.vspdbigtext = obj;
            lv_obj_set_pos(obj, 225, 158);
            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // vspdunits
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.vspdunits = obj;
            lv_obj_set_pos(obj, 285, 169);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "m/s");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // varioswitch
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.varioswitch = obj;
            lv_obj_set_pos(obj, 263, 207);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // Vario
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.vario = obj;
            lv_obj_set_pos(obj, 172, 205);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Vario");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_SCROLLED);
        }
        {
            // altreset
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.altreset = obj;
            lv_obj_set_pos(obj, 104, 207);
            lv_obj_set_size(obj, 57, 25);
            lv_obj_add_event_cb(obj, action_button_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "RESET");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_gps() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.gps = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    {
        lv_obj_t *parent_obj = obj;
        {
            // latlabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.latlabel = obj;
            lv_obj_set_pos(obj, 2, 62);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Lat");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // longlabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.longlabel = obj;
            lv_obj_set_pos(obj, 162, 62);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Long");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // altlabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.altlabel = obj;
            lv_obj_set_pos(obj, 2, 122);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Altitude");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // speedlabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.speedlabel = obj;
            lv_obj_set_pos(obj, 162, 122);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Speed");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // latlabel_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.latlabel_1 = obj;
            lv_obj_set_pos(obj, 247, 2);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Course");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // latvalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.latvalue = obj;
            lv_obj_set_pos(obj, 4, 85);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lonvalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lonvalue = obj;
            lv_obj_set_pos(obj, 160, 85);
            lv_obj_set_size(obj, 158, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // altvalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.altvalue = obj;
            lv_obj_set_pos(obj, 4, 145);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // speedvalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.speedvalue = obj;
            lv_obj_set_pos(obj, 160, 145);
            lv_obj_set_size(obj, 158, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // headvalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.headvalue = obj;
            lv_obj_set_pos(obj, 187, 25);
            lv_obj_set_size(obj, 123, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // satlabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.satlabel = obj;
            lv_obj_set_pos(obj, 2, 182);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Satellites");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // satvalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.satvalue = obj;
            lv_obj_set_pos(obj, 4, 205);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // datavalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.datavalue = obj;
            lv_obj_set_pos(obj, 2, 2);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // oravalue
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.oravalue = obj;
            lv_obj_set_pos(obj, 2, 28);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 0, 319 }
            };
            lv_line_set_points(obj, line_points, 2);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 0, 120);
            lv_obj_set_size(obj, 320, 1);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4085f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 0, 319 }
            };
            lv_line_set_points(obj, line_points, 2);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 0, 180);
            lv_obj_set_size(obj, 320, 1);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4085f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 199, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 159, 2);
            lv_obj_set_size(obj, 2, 240);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4085f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 0, 319 }
            };
            lv_line_set_points(obj, line_points, 2);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, -2, 60);
            lv_obj_set_size(obj, 320, 1);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4085f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_gps();
}

void tick_screen_gps() {
}

void create_screen_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_screen_loaded, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_add_event_cb(obj, action_my_event, LV_EVENT_GESTURE, (void *)0);
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // container
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.container = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 408);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // lux
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lux = obj;
                    lv_obj_set_pos(obj, 3, 12);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Light");
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_letter_space(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // luxvalue
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.luxvalue = obj;
                    lv_obj_set_pos(obj, 153, 18);
                    lv_obj_set_size(obj, 130, 10);
                    lv_slider_set_range(obj, 10, 255);
                    lv_slider_set_value(obj, 100, LV_ANIM_OFF);
                    lv_obj_add_event_cb(obj, action_slider_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    // vol
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.vol = obj;
                    lv_obj_set_pos(obj, 3, 48);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Volume");
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // volvalue
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.volvalue = obj;
                    lv_obj_set_pos(obj, 153, 54);
                    lv_obj_set_size(obj, 130, 10);
                    lv_slider_set_value(obj, 100, LV_ANIM_OFF);
                    lv_obj_add_event_cb(obj, action_slider_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    // alarmlabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alarmlabel = obj;
                    lv_obj_set_pos(obj, 3, 81);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Alarm levels");
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // batteria
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.batteria = obj;
                    lv_obj_set_pos(obj, 3, 112);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "RX Battery");
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // alarmvalue
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.alarmvalue = obj;
                    lv_obj_set_pos(obj, 154, 118);
                    lv_obj_set_size(obj, 129, 10);
                    lv_slider_set_range(obj, 40, 120);
                    lv_slider_set_value(obj, 50, LV_ANIM_ON);
                    lv_obj_add_event_cb(obj, action_slider_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    // alarmvaluelabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alarmvaluelabel = obj;
                    lv_obj_set_pos(obj, 248, 134);
                    lv_obj_set_size(obj, 55, 16);
                    lv_label_set_text(obj, "5.0");
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // batteriatx
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.batteriatx = obj;
                    lv_obj_set_pos(obj, 3, 152);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "TX Battery");
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // alarmvaluetx
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.alarmvaluetx = obj;
                    lv_obj_set_pos(obj, 154, 158);
                    lv_obj_set_size(obj, 129, 10);
                    lv_slider_set_range(obj, 50, 120);
                    lv_slider_set_value(obj, 72, LV_ANIM_ON);
                    lv_obj_add_event_cb(obj, action_slider_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    // alarmvaluetxlabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alarmvaluetxlabel = obj;
                    lv_obj_set_pos(obj, 248, 174);
                    lv_obj_set_size(obj, 55, 16);
                    lv_label_set_text(obj, "7.2");
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // rssi
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.rssi = obj;
                    lv_obj_set_pos(obj, 3, 192);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "RSSI Low");
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // alarmrssilow
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.alarmrssilow = obj;
                    lv_obj_set_pos(obj, 154, 198);
                    lv_obj_set_size(obj, 129, 10);
                    lv_slider_set_range(obj, 32, 50);
                    lv_slider_set_value(obj, 42, LV_ANIM_ON);
                    lv_obj_add_event_cb(obj, action_slider_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    // alarmrssilowlabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alarmrssilowlabel = obj;
                    lv_obj_set_pos(obj, 248, 214);
                    lv_obj_set_size(obj, 55, 16);
                    lv_label_set_text(obj, "42");
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // rssi1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.rssi1 = obj;
                    lv_obj_set_pos(obj, 3, 232);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "RSSI Crit");
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // alarmrssicrit
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.alarmrssicrit = obj;
                    lv_obj_set_pos(obj, 154, 238);
                    lv_obj_set_size(obj, 129, 10);
                    lv_slider_set_range(obj, 32, 50);
                    lv_slider_set_value(obj, 39, LV_ANIM_ON);
                    lv_obj_add_event_cb(obj, action_slider_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    // alarmrssicritlabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alarmrssicritlabel = obj;
                    lv_obj_set_pos(obj, 248, 254);
                    lv_obj_set_size(obj, 55, 16);
                    lv_label_set_text(obj, "39");
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // protolabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.protolabel = obj;
                    lv_obj_set_pos(obj, 3, 274);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Protocol");
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // proto
                    lv_obj_t *obj = lv_dropdown_create(parent_obj);
                    objects.proto = obj;
                    lv_obj_set_pos(obj, 38, 301);
                    lv_obj_set_size(obj, 245, LV_SIZE_CONTENT);
                    lv_dropdown_set_options(obj, "FrSky HUB\nSmart Port");
                }
                {
                    // save
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.save = obj;
                    lv_obj_set_pos(obj, 110, 362);
                    lv_obj_set_size(obj, 100, 34);
                    lv_obj_add_event_cb(obj, action_button_pressed, LV_EVENT_PRESSED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "SAVE");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_settings();
}

void tick_screen_settings() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_gps,
    tick_screen_settings,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_gps();
    create_screen_settings();
}
