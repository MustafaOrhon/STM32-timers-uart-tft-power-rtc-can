
#include "lvgl/lvgl.h"

enum {SLIDER_R = 0,SLIDER_G,SLIDER_B};
typedef struct{
    uint8_t slider_type;
    lv_obj_t* label;

}rgb_mixer_t;
lv_obj_t * rect;
void event_callback(lv_event_t* e)
{
    static uint8_t r, g, b;
    lv_obj_t* slider = lv_event_get_target(e);

    rgb_mixer_t* user_data = lv_event_get_user_data(e);
    int32_t value = lv_slider_get_value(slider);
    lv_label_set_text_fmt(user_data->label, "%d", value);

    if (user_data->slider_type == SLIDER_R)
    {
        r = value;
    }
    else if (user_data->slider_type == SLIDER_G)
    {
        g = value;
    }
    else if (user_data->slider_type == SLIDER_B)
    {
        b = value;
    }
    lv_obj_set_style_bg_color(rect, lv_color_make(r, g, b), LV_PART_MAIN);

}
void rgb_mixer_create_ui(void)
{
    static rgb_mixer_t r, g, b;
    r.slider_type = SLIDER_R;
    g.slider_type = SLIDER_G;
    b.slider_type = SLIDER_B;
      
       /*  lv_obj_t* base_obj = lv_obj_create(act_scr); //
    lv_obj_set_width(base_obj,300);
    lv_obj_set_height(base_obj,50);
    lv_obj_align(base_obj, LV_ALIGN_CENTER, 0,0);
     */
     /*Create a slider_r in the center of the display*/
    lv_obj_t* slider_r = lv_slider_create(lv_scr_act());
    lv_obj_t* slider_g = lv_slider_create(lv_scr_act());
    lv_obj_t* slider_b = lv_slider_create(lv_scr_act());
    /*Set Min Max Values for Sliders*/
    lv_slider_set_range(slider_r, 0, 255);
    lv_obj_set_size(slider_r, 200, 15);
    lv_slider_set_range(slider_g, 0, 255);
    lv_obj_set_size(slider_b, 200, 15);
    lv_slider_set_range(slider_b, 0, 255);
    lv_obj_set_size(slider_g, 200, 15);
    /* ALIGN SLIDERS*/
    lv_obj_align(slider_r, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_align_to(slider_g, slider_r, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_align_to(slider_b, slider_g, LV_ALIGN_TOP_MID, 0, 50);
    /*CONFIG FOR RECT*/
    rect = lv_obj_create(lv_scr_act());
    lv_obj_set_size(rect, 300, 60);
    lv_obj_set_style_border_color(rect, lv_color_black(),LV_PART_MAIN);
    lv_obj_align_to(rect,slider_b, LV_ALIGN_TOP_MID, 0,30);

    /*SET COLORS FOR SLIDERS*/
    lv_obj_set_style_bg_color(slider_r,lv_palette_main(LV_PALETTE_RED),LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider_r, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider_r, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);

    lv_obj_t* heading = lv_label_create(lv_scr_act());
    lv_obj_align(heading, LV_ALIGN_TOP_MID, 0,2);
    lv_label_set_recolor(heading, true);
    lv_label_set_text(heading, "#000000 RGB MIXER# ");


    r.label = lv_label_create(lv_scr_act());
    lv_label_set_text(r.label, "0");
    lv_obj_align_to(r.label, slider_r, LV_ALIGN_TOP_MID, 0,-20);

    g.label = lv_label_create(lv_scr_act());
    lv_label_set_text(g.label, "0");
    lv_obj_align_to(g.label, slider_g, LV_ALIGN_TOP_MID, 0, -20);

    b.label = lv_label_create(lv_scr_act());
    lv_label_set_text(b.label, "0");
    lv_obj_align_to(b.label, slider_b, LV_ALIGN_TOP_MID, 0, -20);
 
    lv_obj_add_event_cb(slider_r, event_callback, LV_EVENT_VALUE_CHANGED,&r);
    lv_obj_add_event_cb(slider_g, event_callback, LV_EVENT_VALUE_CHANGED,&g);
    lv_obj_add_event_cb(slider_b, event_callback, LV_EVENT_VALUE_CHANGED,&b);


}

