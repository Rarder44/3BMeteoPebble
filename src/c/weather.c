#include "pebble.h"



static Window *s_main_window;

static TextLayer *s_temperature_layer;
static TextLayer *s_city_layer;
static BitmapLayer *s_icon_layer;
static GBitmap *s_icon_bitmap = NULL;
static SimpleMenuLayer* s_simple_menu_layer=0;
static SimpleMenuSection  s_menu_sections[1];
static SimpleMenuItem* s_city_menu_items=0;


static AppSync s_sync;
static uint8_t s_sync_buffer[64];

enum MessageKey {
  MESSAGE_REQUEST_LIST_CITY = 0x0,         
  MESSAGE_REQUEST_LIST_DAY = 0x1,
  MESSAGE_REQUEST_LIST_HOUR = 0x2,        
	MESSAGE_REQUEST_FORCE_UPDATE = 0x3,
	MESSAGE_REQUEST_ADD_CITY= 0x4,
	MESSAGE_REQUEST_REMOVE_CITY = 0x5,
	MESSAGE_INBOX_LIST_CITY =0x6,
	MESSAGE_INBOX_LIST_DAY= 0x7,
	MESSAGE_INBOX_LIST_HOUR = 0x8,
};

static const uint32_t WEATHER_ICONS[] = {
  RESOURCE_ID_METEO_0,
	/*RESOURCE_ID_METEO_1,
	RESOURCE_ID_METEO_2,
	RESOURCE_ID_METEO_3,
	RESOURCE_ID_METEO_4,
	RESOURCE_ID_METEO_5,
	RESOURCE_ID_METEO_6,
	RESOURCE_ID_METEO_7,
	RESOURCE_ID_METEO_8,
	RESOURCE_ID_METEO_9,
	RESOURCE_ID_METEO_10,
	RESOURCE_ID_METEO_11,
	RESOURCE_ID_METEO_12,
	RESOURCE_ID_METEO_13,
	RESOURCE_ID_METEO_14,
	RESOURCE_ID_METEO_15,
	RESOURCE_ID_METEO_16,
	RESOURCE_ID_METEO_17,
	RESOURCE_ID_METEO_18,
	RESOURCE_ID_METEO_19,
	RESOURCE_ID_METEO_20,
	RESOURCE_ID_METEO_21,
	RESOURCE_ID_METEO_22,
	RESOURCE_ID_METEO_23,
	RESOURCE_ID_METEO_24,
	RESOURCE_ID_METEO_25,
	RESOURCE_ID_METEO_26,
	RESOURCE_ID_METEO_27,
	RESOURCE_ID_METEO_28,
	RESOURCE_ID_METEO_29,
	RESOURCE_ID_METEO_30,
	RESOURCE_ID_METEO_31,
	RESOURCE_ID_METEO_32,
	RESOURCE_ID_METEO_33,
	RESOURCE_ID_METEO_34,
	RESOURCE_ID_METEO_35,
	RESOURCE_ID_METEO_36,
	RESOURCE_ID_METEO_37,
	RESOURCE_ID_METEO_38,
	RESOURCE_ID_METEO_39,
	RESOURCE_ID_METEO_40,
	RESOURCE_ID_METEO_41,
	RESOURCE_ID_METEO_42,
	RESOURCE_ID_METEO_43,
	RESOURCE_ID_METEO_44,
	RESOURCE_ID_METEO_45,
	RESOURCE_ID_METEO_46,
	RESOURCE_ID_METEO_47,
	RESOURCE_ID_METEO_48,
	RESOURCE_ID_METEO_49,
	RESOURCE_ID_METEO_50,
	RESOURCE_ID_METEO_51,
	RESOURCE_ID_METEO_52,
	RESOURCE_ID_METEO_53,
	RESOURCE_ID_METEO_54,
	RESOURCE_ID_METEO_55,
	RESOURCE_ID_METEO_56,
	RESOURCE_ID_METEO_57,
	RESOURCE_ID_METEO_58,
	RESOURCE_ID_METEO_59,
	RESOURCE_ID_METEO_60,
	RESOURCE_ID_METEO_61,
	RESOURCE_ID_METEO_62,
	RESOURCE_ID_METEO_63,
	RESOURCE_ID_METEO_64,
	RESOURCE_ID_METEO_65,
	RESOURCE_ID_METEO_66,
	RESOURCE_ID_METEO_67,
	RESOURCE_ID_METEO_68,
	RESOURCE_ID_METEO_69,
	RESOURCE_ID_METEO_70,
	RESOURCE_ID_METEO_71,
	RESOURCE_ID_METEO_72,
	RESOURCE_ID_METEO_73,
	RESOURCE_ID_METEO_74,
	RESOURCE_ID_METEO_75,
	RESOURCE_ID_METEO_76,
	RESOURCE_ID_METEO_77,
	RESOURCE_ID_METEO_78,
	RESOURCE_ID_METEO_79,
	RESOURCE_ID_METEO_80,
	RESOURCE_ID_METEO_81,
	RESOURCE_ID_METEO_82,
	RESOURCE_ID_METEO_83,
	RESOURCE_ID_METEO_84,
	RESOURCE_ID_METEO_85,
	RESOURCE_ID_METEO_86,
	RESOURCE_ID_METEO_87,
	RESOURCE_ID_METEO_88,
	RESOURCE_ID_METEO_89,
	RESOURCE_ID_METEO_90,
	RESOURCE_ID_METEO_91,
	RESOURCE_ID_METEO_92,
	RESOURCE_ID_METEO_93,
	RESOURCE_ID_METEO_94,
	RESOURCE_ID_METEO_95,
	RESOURCE_ID_METEO_96,
	RESOURCE_ID_METEO_97,
	RESOURCE_ID_METEO_98,
	RESOURCE_ID_METEO_99,
	RESOURCE_ID_METEO_100,
	RESOURCE_ID_METEO_101,
	RESOURCE_ID_METEO_102*/
};


static void FreeCities();
static void LoadMenuLayer();
static void LoadCityLayer();
static void LoadDaysLayer();
static void LoadHoursLayer();

/*static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case WEATHER_ICON_KEY:
      if (s_icon_bitmap) {
        gbitmap_destroy(s_icon_bitmap);
      }

      s_icon_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[new_tuple->value->uint8]);
      bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
      bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
      break;

    case WEATHER_TEMPERATURE_KEY:
      // App Sync keeps new_tuple in s_sync_buffer, so we may use it directly
      text_layer_set_text(s_temperature_layer, new_tuple->value->cstring);
      break;

    case WEATHER_CITY_KEY:
      text_layer_set_text(s_city_layer, new_tuple->value->cstring);
      break;
  }
}
*/
static char **Cities=0;
int NCities=0;
static void inbox_received_callback(DictionaryIterator *iter, void *context) {
		
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Messaggio ricevuto");
  // nell'1 c'è sempre il messaggio 
	 	Tuple *t = dict_find(iter, 1);
		if(t) {
			int num=t->value->uint8;
			APP_LOG(APP_LOG_LEVEL_DEBUG, "mex n: %d", num);
			
			//stampare una variabile
			char* snum=malloc(sizeof(char)*10);
			snprintf(snum, 10, "%d", num);
			
			 switch (num) {
				case MESSAGE_INBOX_LIST_CITY:;	
				 		FreeCities();
				 		//nel 2 c'è il numero di città nella lista
				 		APP_LOG(APP_LOG_LEVEL_DEBUG, "Lista città");
				 
				 		Tuple *n = dict_find(iter, 2);
				 		if(n){
							NCities=n->value->uint8;
							APP_LOG(APP_LOG_LEVEL_DEBUG, "Numero città: %d",NCities);
							
							Cities = (char**)malloc(sizeof(char*) * NCities);
							for(int i=0;i<NCities;i++)
							{
								Tuple *c =dict_find(iter, 3+i);
								if(c){
									APP_LOG(APP_LOG_LEVEL_DEBUG, "città %d trovata",i);
									APP_LOG(APP_LOG_LEVEL_DEBUG, "Nome: %s",c->value->cstring);
									
									
									int asd=strlen(c->value->cstring)+1;
									APP_LOG(APP_LOG_LEVEL_DEBUG, "L: %d",asd);
									
									
									Cities[i]=(char*)malloc(asd * sizeof(char));
									APP_LOG(APP_LOG_LEVEL_DEBUG, "allocato");
									
									strcpy(Cities[i], c->value->cstring);
									
									APP_LOG(APP_LOG_LEVEL_DEBUG, "Nome: %s",Cities[i]);
								}
							}
						}
				 		LoadCityLayer();
				break;

				case MESSAGE_INBOX_LIST_DAY:
						
				break;

				case MESSAGE_INBOX_LIST_HOUR:
						
				break;
			}
		}
}

static void RequestListCity(void) 
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore invio richiesta lista città");
    return;
  }

	int value=MESSAGE_REQUEST_LIST_CITY;
	//l'1 la uso come posizione per il messaggio da inviare
  dict_write_int(iter, 1 , &value, sizeof(int), true);
  dict_write_end(iter);
  app_message_outbox_send();
}

static void RequestCitySmall(int City) 
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore invio richiesta dati small");
    return;
  }

	int value=MESSAGE_INBOX_LIST_DAY;
	//l'1 la uso come posizione per il messaggio da inviare
	//il 2 lo uso x l'ID della città
  dict_write_int(iter, 1 , &value, sizeof(int), true);
	dict_write_int(iter, 2 , &City, sizeof(int), true);
  dict_write_end(iter);
  app_message_outbox_send();
}


static void RequestCityHour(int City,int Hour) 
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore invio richiesta dati orari");
    return;
  }

	int value=MESSAGE_INBOX_LIST_HOUR;
	//l'1 la uso come posizione per il messaggio da inviare
	//il 2 lo uso x l'ID della città
	//il 3 lo uso x l'ID dell'ora
  dict_write_int(iter, 1 , &value, sizeof(int), true);
	dict_write_int(iter, 2 , &City, sizeof(int), true);
	dict_write_int(iter, 3 , &Hour, sizeof(int), true);
  dict_write_end(iter);
  app_message_outbox_send();
}


static void RequestForceUpdate(void)
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
		text_layer_set_text(s_city_layer, "Error r");
    return;
  }

	int value=MESSAGE_REQUEST_FORCE_UPDATE;
	//l'1 la uso come posizione per il messaggio da inviare
  dict_write_int(iter, 1 , &value, sizeof(int), true);
  dict_write_end(iter);
  app_message_outbox_send();
}


static void FreeCities()
{
	if(Cities!=0)
	{
		for (int i = 0; i <NCities; i++)
		{
				free(Cities[i]);
		}	
		free(Cities);
		Cities=0;
	}
}	


static void LoadMenuLayer()
{
	//GBitmap *s_menu_icon_image = gbitmap_create_with_resource(RESOURCE_ID_METEO_0);
	if(Cities==0)
		return;
	
	if(s_city_menu_items!=0)
		free(s_city_menu_items);
 s_city_menu_items = (SimpleMenuItem*)malloc(sizeof(SimpleMenuItem)*NCities);


	//s_menu_icon_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_1);
	for (int i = 0; i <NCities; i++)
	{
		s_city_menu_items[i] = (SimpleMenuItem){
		  .title = Cities[i],
			//.subtitle = "100%-6 SEE-100%",
    	//.callback = menu_select_callback,
    	//.icon = s_menu_icon_image,
		};
	}

  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NCities,
    .items = s_city_menu_items,
  };

  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_frame(window_layer);

	
	if(s_simple_menu_layer!=0)
		free(s_simple_menu_layer);
	
	s_simple_menu_layer = simple_menu_layer_create(bounds, s_main_window, s_menu_sections, 1, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
 
	
}
static void LoadSmallInfoLayer()
{
	Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);

  s_icon_layer = bitmap_layer_create(GRect(0, 10, bounds.size.w, 80));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));

  s_temperature_layer = text_layer_create(GRect(0, 90, bounds.size.w, 32));
  text_layer_set_text_color(s_temperature_layer, GColorWhite);
  text_layer_set_background_color(s_temperature_layer, GColorClear);
  text_layer_set_font(s_temperature_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_temperature_layer));

  s_city_layer = text_layer_create(GRect(0, 122, bounds.size.w, 32));
  text_layer_set_text_color(s_city_layer, GColorWhite);
  text_layer_set_background_color(s_city_layer, GColorClear);
  text_layer_set_font(s_city_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_city_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_city_layer));
	
}

static void menu_select_callback(int index, void *context)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "index click: %d", index);
}
static void LoadCityLayer()
{

	//resetto le variabili gia inizializzate
	if(Cities==0)
		return;

	
	if(s_city_menu_items!=0)
		free(s_city_menu_items);

	
 	s_city_menu_items = (SimpleMenuItem*)malloc(sizeof(SimpleMenuItem)*NCities);

	
	if(s_simple_menu_layer!=0)
	{

		layer_remove_from_parent((Layer *)s_simple_menu_layer);

		simple_menu_layer_destroy(s_simple_menu_layer);

		s_simple_menu_layer=0;
	}

	
	//creo gli item del menu contenenti il nome delle città
	for (int i = 0; i <NCities; i++)
	{
		s_city_menu_items[i] = (SimpleMenuItem){
		  .title = Cities[i],
			.callback = menu_select_callback,
		};
	}


	//creo il numeu e gli associo gli item
  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NCities,
    .items = s_city_menu_items,
  };


	
	//ottengo il layer su cui aggiungere il mio menu
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_frame(window_layer);
	

	//creo il nuovo layer
	s_simple_menu_layer = simple_menu_layer_create(bounds, s_main_window, s_menu_sections, 1, NULL);

	
	//lo aggiungo al layer principale
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
	
}

static void LoadDaysLayer()
{
	
	
}

static void LoadHoursLayer()
{
	
	
}



static void window_load(Window *window) {
 
	
  
  //app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer),      initial_values, ARRAY_LENGTH(initial_values),      sync_tuple_changed_callback, sync_error_callback, NULL);
	app_message_register_inbox_received(inbox_received_callback);
	
	

  //RequestForceUpdate();
}

static void window_unload(Window *window) {
  if (s_icon_bitmap) {
    gbitmap_destroy(s_icon_bitmap);
  }

  text_layer_destroy(s_city_layer);
  text_layer_destroy(s_temperature_layer);
  bitmap_layer_destroy(s_icon_layer);
}

static void init(void) {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorFromHEX(0x1461A1));
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_main_window, true);

  app_message_open(64, 64);
}

static void deinit(void) {
  window_destroy(s_main_window);

  app_sync_deinit(&s_sync);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
