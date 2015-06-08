#include "pebble.h"

#define PERSONALIZED_TEXT_INPUT 0
#define DATE_STYLE 1

Window *window;


TextLayer *text_dayname_layer;
TextLayer *text_date_layer;
TextLayer *text_time_layer;
TextLayer *text_battery_layer;

static GBitmap     *image;
static BitmapLayer *image_layer;

Layer *line_layer;
Layer       *BTLayer1;

GFont        fontHelvNewLight20;
GFont		     fontRobotoCondensed21;
GFont        fontRobotoBoldSubset45;

GPoint     Linepoint;
static int BTConnected = 1;
static int BTVibesDone = 0;

static int batterychargepct;
static int BatteryVibesDone = 0;
static int batterycharging = 0;

static int FirstTime = 0;
static int SecsInt = 0;

static char date_type[]="us  ";
static char dayname_text[] = "XXXXXXXXXX";
static char time_text[] = "00:00";
static char date_text[] = "Xxx 00       0000";
static char date_format[]="%b %e, %Y";


GColor TextColorHold;
GColor BGColorHold;

void handle_battery(BatteryChargeState charge_state) {
  static char BatteryPctTxt[] = "+100%";

  batterychargepct = charge_state.charge_percent;

  if (charge_state.is_charging) {
    batterycharging = 1;
  } else {
    batterycharging = 0;
  }

  // Reset if Battery > 20% ********************************
  if (batterychargepct > 20) {
     if (BatteryVibesDone == 1) {     //OK Reset to normal
         BatteryVibesDone = 0;
     }
     #ifdef PBL_COLOR
         text_layer_set_background_color(text_battery_layer, BGColorHold);
         text_layer_set_text_color(text_battery_layer, TextColorHold);
      #endif
  }

  //
  if (batterychargepct < 30) {
     if (BatteryVibesDone == 0) {            // Do Once
         BatteryVibesDone = 1;
         vibes_long_pulse();
         #ifdef PBL_COLOR
           text_layer_set_text_color(text_battery_layer, GColorFashionMagenta);
         #endif
      }
  }

   if (charge_state.is_charging) {
     strcpy(BatteryPctTxt, "Chrg");
  } else {
     snprintf(BatteryPctTxt, 5, "%d%%", charge_state.charge_percent);
  }
   text_layer_set_text(text_battery_layer, BatteryPctTxt);

  layer_mark_dirty(line_layer);
}

void line_layer_update_callback(Layer *LineLayer, GContext* ctx) {

     graphics_context_set_fill_color(ctx, TextColorHold);
     graphics_fill_rect(ctx, layer_get_bounds(LineLayer), 3, GCornersAll);

     if (batterycharging == 1) {
       #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorBlue);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif

       graphics_fill_rect(ctx, GRect(2, 1, 100, 4), 3, GCornersAll);

     } else if (batterychargepct > 20) {
       #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorGreen);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif

       graphics_fill_rect(ctx, GRect(2, 1, batterychargepct, 4), 3, GCornersAll);

     } else {
      #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorRed);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif

       graphics_fill_rect(ctx, GRect(2, 1, batterychargepct, 4), 3, GCornersAll);
     }
}


void handle_bluetooth(bool connected) {

    if (connected) {

         BTConnected = 1;     // Connected
         BTVibesDone = 0;

    } else {
         BTConnected = 0;      // Not Connected

         if (BTVibesDone == 0) {    //Do First Vibe
             BTVibesDone = 1;

             vibes_long_pulse();
         }
    }
    layer_mark_dirty(BTLayer1);
}

//BT Logo Callback;
void BTLine_update_callback(Layer *BTLayer1, GContext* BT1ctx) {

       GPoint BTLinePointStart;
       GPoint BTLinePointEnd;

      graphics_context_set_stroke_color(BT1ctx, TextColorHold);

      if (BTConnected == 0) {

        #ifdef PBL_COLOR
            graphics_context_set_stroke_color(BT1ctx, GColorRed);
            graphics_context_set_fill_color(BT1ctx, GColorWhite);
            graphics_fill_rect(BT1ctx, layer_get_bounds(BTLayer1), 0, GCornerNone);
        #endif

        // "X"" Line 1
          BTLinePointStart.x = 1;
          BTLinePointStart.y = 1;

          BTLinePointEnd.x = 20;
          BTLinePointEnd.y = 20;
          graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

         // "X"" Line 2
          BTLinePointStart.x = 1;
          BTLinePointStart.y =20;

          BTLinePointEnd.x = 20;
          BTLinePointEnd.y = 1;
          graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

      } else {

       //Line 1
       BTLinePointStart.x = 10;
       BTLinePointStart.y = 1;

       BTLinePointEnd.x = 10;
       BTLinePointEnd.y = 20;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 1a
       BTLinePointStart.x = 11;
       BTLinePointStart.y = 1;

       BTLinePointEnd.x = 11;
       BTLinePointEnd.y = 20;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 2
       BTLinePointStart.x = 10;
       BTLinePointStart.y = 1;

       BTLinePointEnd.x = 17;
       BTLinePointEnd.y = 6;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 2a
       BTLinePointStart.x = 11;
       BTLinePointStart.y = 1;

       BTLinePointEnd.x = 17;
       BTLinePointEnd.y = 5;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 3
       BTLinePointStart.x = 4;
       BTLinePointStart.y = 5;

       BTLinePointEnd.x = 17;
       BTLinePointEnd.y = 15;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 3a
       BTLinePointStart.x = 4;
       BTLinePointStart.y = 6;

       BTLinePointEnd.x = 17;
       BTLinePointEnd.y = 16;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 4
       BTLinePointStart.x = 4;
       BTLinePointStart.y = 15;

       BTLinePointEnd.x = 17;
       BTLinePointEnd.y = 5;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 4a
       BTLinePointStart.x = 4;
       BTLinePointStart.y = 16;

       BTLinePointEnd.x = 17;
       BTLinePointEnd.y = 6;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 5
       BTLinePointStart.x = 10;
       BTLinePointStart.y = 20;

       BTLinePointEnd.x = 17;
       BTLinePointEnd.y = 15;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 5a
       BTLinePointStart.x = 11;
       BTLinePointStart.y = 20;

       BTLinePointEnd.x = 17;
       BTLinePointEnd.y = 16;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);
      }
}


void handle_appfocus(bool in_focus){
    if (in_focus) {
        handle_bluetooth(bluetooth_connection_service_peek());
    }
}

void handle_tick(struct tm *tick_time, TimeUnits units_changed) {

  if(SecsInt == 0) {
  char *time_format;

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  //handle_battery(battery_state_service_peek());

  //handle_bluetooth(bluetooth_connection_service_peek());

  // Set Time of Day
  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  // Set day and date
  strftime(dayname_text, sizeof(dayname_text), "%A",        tick_time);
  strftime(date_text,    sizeof(date_text), date_format, tick_time);

 //Initialize
if (FirstTime == 0) {
    text_layer_set_text(text_dayname_layer, dayname_text);
    text_layer_set_text(text_date_layer, date_text);
    FirstTime = 1;
    }

if (units_changed & DAY_UNIT) {
   // Only update the day name & date when it's changed.
    text_layer_set_text(text_dayname_layer, dayname_text);
    text_layer_set_text(text_date_layer, date_text);
  }
  //Always set time
  text_layer_set_text(text_time_layer, time_text);
  }
}

//Receive Input from Config html page:
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "In Inbox received callback");

  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {

    // Which key was received?
    switch(t->key) {

    case 0:
      strcpy(date_type, t->value->cstring);

      if (strcmp(date_type, "us") == 0) {
         strcpy(date_format, "%b %e, %Y");
      } else {
         strcpy(date_format, "%e %b %Y");
      }
      text_layer_set_text(text_date_layer, date_text);
      text_layer_set_text(text_date_layer, date_text);
      break;

    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Inbox Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void handle_deinit(void) {
  persist_write_string(DATE_STYLE, date_type);
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  app_focus_service_unsubscribe();

  text_layer_destroy(text_time_layer);
  text_layer_destroy(text_date_layer);
  text_layer_destroy(text_dayname_layer);
  text_layer_destroy(text_battery_layer);

  layer_destroy(line_layer);
  layer_destroy(BTLayer1);

  gbitmap_destroy(image);
  bitmap_layer_destroy(image_layer);

  fonts_unload_custom_font(fontHelvNewLight20);
  fonts_unload_custom_font(fontRobotoBoldSubset45);

  window_destroy(window);
}

void handle_init(void) {

  GColor BGCOLOR   = COLOR_FALLBACK(GColorDukeBlue, GColorBlack);
  BGColorHold = BGCOLOR;

  GColor TEXTCOLOR = COLOR_FALLBACK(GColorWhite, GColorWhite);
  TextColorHold = TEXTCOLOR;

  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, BGCOLOR);

  fontHelvNewLight20     = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HELV_NEW_LIGHT_20));
  fontRobotoBoldSubset45 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_45));
  Layer *window_layer = window_get_root_layer(window);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

// Date
  text_date_layer = text_layer_create(GRect(1, 94, 144, 28));
  text_layer_set_text_color(text_date_layer, TEXTCOLOR);
  text_layer_set_background_color(text_date_layer, BGCOLOR);
  text_layer_set_text_alignment(text_date_layer, GTextAlignmentCenter);;
  text_layer_set_font(text_date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  //Persistent Value Date Format:
  if (persist_exists(DATE_STYLE)) {
     persist_read_string(DATE_STYLE, date_type, sizeof(date_type));
     APP_LOG(APP_LOG_LEVEL_WARNING, "persistent exists");
     APP_LOG(APP_LOG_LEVEL_WARNING, date_type);
  }  else {
     strcpy(date_type, "us");
  }// Time of Day

  text_time_layer = text_layer_create(GRect(1, 120, 144, 45));
  text_layer_set_text_color(text_time_layer, TEXTCOLOR);
  text_layer_set_background_color(text_time_layer, BGCOLOR);
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  text_layer_set_font(text_time_layer,fontRobotoBoldSubset45);
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

   // Dayname
  text_dayname_layer = text_layer_create(GRect(1, 74, 144, 26));
  text_layer_set_text_color(text_dayname_layer, TEXTCOLOR);
  text_layer_set_text_alignment(text_dayname_layer, GTextAlignmentCenter);
  text_layer_set_background_color(text_dayname_layer, BGCOLOR);
  text_layer_set_font(text_dayname_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_layer, text_layer_get_layer(text_dayname_layer));

  // RICE Logo
  #if PBL_COLOR
      image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RICE_LOGO);
  #else
      image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RICE_LOGO_BW);
  #endif

  image_layer = bitmap_layer_create(GRect(1, 29, 144, 49)); //49 was 45
  bitmap_layer_set_bitmap(image_layer, image);
  bitmap_layer_set_alignment(image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));

   // Line
  GRect line_frame = GRect(22, 122, 104, 6);
  line_layer = layer_create(line_frame);
  layer_set_update_proc(line_layer, line_layer_update_callback);
  layer_add_child(window_layer, line_layer);

  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);

  //Bluetooth Logo Setup area
  GRect BTArea = GRect(1, 5, 20, 20);
  BTLayer1 = layer_create(BTArea);

  layer_add_child(window_layer, BTLayer1);

  layer_set_update_proc(BTLayer1, BTLine_update_callback);

  bluetooth_connection_service_subscribe(&handle_bluetooth);


  //Battery Text
  text_battery_layer = text_layer_create(GRect(85,2,55,28));
  text_layer_set_text_color(text_battery_layer, TEXTCOLOR);
  text_layer_set_background_color(text_battery_layer, BGCOLOR);
  text_layer_set_font(text_battery_layer,  fontHelvNewLight20 );
  text_layer_set_text_alignment(text_battery_layer, GTextAlignmentRight);

  layer_add_child(window_layer, text_layer_get_layer(text_battery_layer));

  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  app_focus_service_subscribe(&handle_appfocus);

  handle_battery(battery_state_service_peek());
  handle_bluetooth(bluetooth_connection_service_peek());

}



int main(void) {
   handle_init();

   app_event_loop();

   handle_deinit();
}
