#include "pebble.h"



static Window *window_city;
static SimpleMenuLayer* s_simple_menu_layer_city=0;
static SimpleMenuSection  s_menu_sections_city[1];
static SimpleMenuItem* s_menu_items_city=0;
static int s_menu_items_city_count=0;

static Window *window_day=0;
static SimpleMenuLayer* s_simple_menu_layer_day=0;
static SimpleMenuSection  s_menu_sections_day[1];
static SimpleMenuItem* s_menu_items_day=0;
static int s_menu_items_day_count=0;


static Window *window_hour=0;
static SimpleMenuLayer* s_simple_menu_layer_hour=0;
static SimpleMenuSection  s_menu_sections_hour[1];
static SimpleMenuItem* s_menu_items_hour=0;
static int s_menu_items_hour_count=0;

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
  RESOURCE_ID_IMAGE_0,
RESOURCE_ID_IMAGE_1,
RESOURCE_ID_IMAGE_2,
RESOURCE_ID_IMAGE_3_42,
RESOURCE_ID_IMAGE_4,
RESOURCE_ID_IMAGE_5,
RESOURCE_ID_IMAGE_6,
RESOURCE_ID_IMAGE_7,
RESOURCE_ID_IMAGE_8,
RESOURCE_ID_IMAGE_9,
RESOURCE_ID_IMAGE_10,
RESOURCE_ID_IMAGE_11,
RESOURCE_ID_IMAGE_12,
RESOURCE_ID_IMAGE_13,
RESOURCE_ID_IMAGE_14,
RESOURCE_ID_IMAGE_15_66,
RESOURCE_ID_IMAGE_16_67,
RESOURCE_ID_IMAGE_17_68,
RESOURCE_ID_IMAGE_18_69,
RESOURCE_ID_IMAGE_19_70,
RESOURCE_ID_IMAGE_20_71,
RESOURCE_ID_IMAGE_21_41,
RESOURCE_ID_IMAGE_22_25_73_76,
RESOURCE_ID_IMAGE_23_24_74_75,
RESOURCE_ID_IMAGE_23_24_74_75,//24
RESOURCE_ID_IMAGE_22_25_73_76,//25
RESOURCE_ID_IMAGE_26_28_77_79,
RESOURCE_ID_IMAGE_27_78,
RESOURCE_ID_IMAGE_26_28_77_79,//28
RESOURCE_ID_IMAGE_29_80,
RESOURCE_ID_IMAGE_30_47_81_98,
RESOURCE_ID_IMAGE_31_82,
RESOURCE_ID_IMAGE_32_50_83_101,
RESOURCE_ID_IMAGE_33_46_48_84_97_99,
RESOURCE_ID_IMAGE_34_35_36_85_86_87,
RESOURCE_ID_IMAGE_34_35_36_85_86_87,//35
RESOURCE_ID_IMAGE_34_35_36_85_86_87,//36
RESOURCE_ID_IMAGE_37,
RESOURCE_ID_IMAGE_38_89,
RESOURCE_ID_IMAGE_39,
RESOURCE_ID_IMAGE_40_91,
RESOURCE_ID_IMAGE_21_41, //41
RESOURCE_ID_IMAGE_3_42,//42
RESOURCE_ID_IMAGE_43,
RESOURCE_ID_IMAGE_44,
RESOURCE_ID_IMAGE_45,
RESOURCE_ID_IMAGE_33_46_48_84_97_99,//46
RESOURCE_ID_IMAGE_30_47_81_98,//47
RESOURCE_ID_IMAGE_33_46_48_84_97_99,//48
RESOURCE_ID_IMAGE_49,
RESOURCE_ID_IMAGE_32_50_83_101,//50
RESOURCE_ID_IMAGE_51,
RESOURCE_ID_IMAGE_52,
RESOURCE_ID_IMAGE_53,
RESOURCE_ID_IMAGE_54_93,
RESOURCE_ID_IMAGE_55,
RESOURCE_ID_IMAGE_56,
RESOURCE_ID_IMAGE_57,
RESOURCE_ID_IMAGE_58,
RESOURCE_ID_IMAGE_59,
RESOURCE_ID_IMAGE_60,
RESOURCE_ID_IMAGE_61,
RESOURCE_ID_IMAGE_62,
RESOURCE_ID_IMAGE_63,
RESOURCE_ID_IMAGE_64,
RESOURCE_ID_IMAGE_65,
RESOURCE_ID_IMAGE_15_66,//66
RESOURCE_ID_IMAGE_16_67,//67
RESOURCE_ID_IMAGE_17_68,//68
RESOURCE_ID_IMAGE_18_69,//69
RESOURCE_ID_IMAGE_19_70,//70
RESOURCE_ID_IMAGE_20_71,//71
RESOURCE_ID_IMAGE_72_92,
RESOURCE_ID_IMAGE_22_25_73_76,//73
RESOURCE_ID_IMAGE_23_24_74_75,//74
RESOURCE_ID_IMAGE_23_24_74_75,//75
RESOURCE_ID_IMAGE_22_25_73_76,//76
RESOURCE_ID_IMAGE_26_28_77_79,//77
RESOURCE_ID_IMAGE_27_78,//78
RESOURCE_ID_IMAGE_26_28_77_79,//79
RESOURCE_ID_IMAGE_29_80,//80
RESOURCE_ID_IMAGE_30_47_81_98,//81
RESOURCE_ID_IMAGE_31_82,//82
RESOURCE_ID_IMAGE_32_50_83_101,//83
RESOURCE_ID_IMAGE_33_46_48_84_97_99,//84
RESOURCE_ID_IMAGE_34_35_36_85_86_87,//85
RESOURCE_ID_IMAGE_34_35_36_85_86_87,//86
RESOURCE_ID_IMAGE_34_35_36_85_86_87,//87
RESOURCE_ID_IMAGE_88,
RESOURCE_ID_IMAGE_38_89, //89
RESOURCE_ID_IMAGE_90,
RESOURCE_ID_IMAGE_40_91,//91
RESOURCE_ID_IMAGE_72_92,//92
RESOURCE_ID_IMAGE_54_93,//93
RESOURCE_ID_IMAGE_94,
RESOURCE_ID_IMAGE_95,
RESOURCE_ID_IMAGE_96,
RESOURCE_ID_IMAGE_33_46_48_84_97_99,//97
RESOURCE_ID_IMAGE_30_47_81_98,//98
RESOURCE_ID_IMAGE_33_46_48_84_97_99,//99
RESOURCE_ID_IMAGE_100,
RESOURCE_ID_IMAGE_32_50_83_101,//101
RESOURCE_ID_IMAGE_102,
RESOURCE_ID_IMAGE_103,
};
static int IconNum=103;




static void LoadCityLayer();
static void LoadDaysLayer();
static void LoadHoursLayer();
static void menu_select_callback(int index, void *context);
static GBitmap * GetBitmapFromID(int id);
static void FreeLayerCity();
static void FreeLayerDay();
static void FreeLayerHour();


static void inbox_received_callback(DictionaryIterator *iter, void *context) {
		

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Messaggio ricevuto");
  // nell'1 c'è sempre il messaggio 
	 	Tuple *t = dict_find(iter, 1);
		if(t) {
			int num=t->value->uint32;
			APP_LOG(APP_LOG_LEVEL_DEBUG, "mex n: %d", num);
			
			//stampare una variabile
			/*char* snum=malloc(sizeof(char)*10);
			snprintf(snum, 10, "%d", num);*/
			
			 switch (num) {
				case MESSAGE_INBOX_LIST_CITY:;	

				 		APP_LOG(APP_LOG_LEVEL_DEBUG, "Lista città");
				 		LoadCityLayer(iter);
				break;

				case MESSAGE_INBOX_LIST_DAY:
						APP_LOG(APP_LOG_LEVEL_DEBUG, "Lista gioni");
						LoadDaysLayer(iter);
				break;

				case MESSAGE_INBOX_LIST_HOUR:
				 APP_LOG(APP_LOG_LEVEL_DEBUG, "Lista ore");
						LoadHoursLayer(iter);
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

static void RequestListDay(int City) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Richiesta lista giorno");
  DictionaryIterator *iter;
	
  app_message_outbox_begin(&iter);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Dizionario inizializzato sull'output");
	
  if (!iter) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore invio richiesta dati small");
    return;
  }
	
	int value= MESSAGE_REQUEST_LIST_DAY;
	//l'1 la uso come posizione per il messaggio da inviare
	//il 2 lo uso x l'ID della città
  dict_write_int(iter, 1 , &value, sizeof(int), true);
	dict_write_int(iter, 2 , &City, sizeof(int), true);
  dict_write_end(iter);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "messaggio con valore %d",value);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "ID città %d",City);
	app_message_outbox_send();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Messaggio inviato");
}

static void RequestListHours(int City,int Hour)
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore invio richiesta dati orari");
    return;
  }

	int value= MESSAGE_REQUEST_LIST_HOUR;
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
    return;
  }

	int value=MESSAGE_REQUEST_FORCE_UPDATE;
	//l'1 la uso come posizione per il messaggio da inviare
  dict_write_int(iter, 1 , &value, sizeof(int), true);
  dict_write_end(iter);
  app_message_outbox_send();
}




































static int CurrentCity=-1;
static int CurrentDay=-1;




static void window_unload(Window *window) {

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Window distrutta");
	if( window == window_hour)
	{
		CurrentDay=-1;
		
		FreeLayerHour();
		window_hour=0;
	}
	else if( window == window_day)
	{
		CurrentDay=-1;
		CurrentCity=-1;
		
		FreeLayerDay();
		window_day=0;
		
	}
	else if( window == window_city)
	{
		FreeLayerCity();
		window_city=0;
	}
	
	window_destroy(window);
	
	
}





static void FreeLayerCity()
{
	if (s_menu_items_city != 0)
	{
		for(int i=0;i<s_menu_items_city_count;i++)
				free((void*)s_menu_items_city[i].title);
		free(s_menu_items_city);
		s_menu_items_city=0;
	}
	if (s_simple_menu_layer_city != 0)
	{
		layer_remove_from_parent((Layer *)s_simple_menu_layer_city);
		simple_menu_layer_destroy(s_simple_menu_layer_city);
		s_simple_menu_layer_city=0;
	}
}

static void FreeLayerDay()
{
	if (s_menu_items_day != 0)
	{
		for(int i=0;i<s_menu_items_day_count;i++)
		{
				gbitmap_destroy(s_menu_items_day[i].icon);
				free((void*)s_menu_items_day[i].title);
				free((void*)s_menu_items_day[i].subtitle);
		}
		free(s_menu_items_day);
		s_menu_items_day=0;
	}
	if (s_simple_menu_layer_day != 0)
	{
		layer_remove_from_parent((Layer *)s_simple_menu_layer_day);
		simple_menu_layer_destroy(s_simple_menu_layer_day);
		s_simple_menu_layer_day=0;
	}
}

static void FreeLayerHour()
{
	if (s_menu_items_hour != 0)
	{
		for(int i=0;i<s_menu_items_hour_count;i++)
		{
				gbitmap_destroy(s_menu_items_hour[i].icon);
				free((void*)s_menu_items_hour[i].title);
				free((void*)s_menu_items_hour[i].subtitle);
		}
		free(s_menu_items_hour);
		s_menu_items_hour=0;
	}
	
	if (s_simple_menu_layer_hour != 0)
	{
		layer_remove_from_parent((Layer *)s_simple_menu_layer_hour);
		simple_menu_layer_destroy(s_simple_menu_layer_hour);
		s_simple_menu_layer_hour=0;
	}
}






static bool PopReachWindows(Window* window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "controllo se la win esiste");
	if(!window_stack_contains_window(window))
		return false;
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Esiste! inizio ciclo pop win");
	while(window_stack_get_top_window()!=window)
	{
		Window* removed=window_stack_pop(false);
	}
	return true;
}





static Window* Crea3BWindow()
{
	Window* temp= window_create();		
	window_set_background_color(temp, GColorFromHEX(0x1461A1));
	window_set_window_handlers(temp, (WindowHandlers) {
		.unload = window_unload
	});
	return temp;
}



static SimpleMenuLayer* SimpleMenuLateyCreateAndBind(Window* window,SimpleMenuSection* section)
{
	//ottengo il window layer
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);

	//creo il nuovo layer (simple menu layer)
	SimpleMenuLayer* temp = simple_menu_layer_create(bounds, window, section, 1, NULL);

	//lo aggiungo al window layer
	layer_add_child(window_layer, simple_menu_layer_get_layer(temp));

	return temp;
}




//static int CurrentHour=-1;

/*
	Dizionario:
	
	1:messaggio
	2:numero citta
	3:nome citta 1
	4:nome citta 2...
*/
static void LoadCityLayer(DictionaryIterator *iter)
{
	
	/*if (Cities == 0)
		return;*/
	


	//ottengo il numero di citta
	Tuple *n = dict_find(iter, 2);
	if (n) {
		int NCities = n->value->uint8;
		
		
		//resetto le variabili gia inizializzate
		FreeLayerCity();
		
		s_menu_items_city = (SimpleMenuItem*)malloc(sizeof(SimpleMenuItem)*NCities);
		s_menu_items_city_count=NCities;
		
		CurrentCity=-1;
		CurrentDay=-1;
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Numero città: %d", NCities);
		
		
		
		
		for (int i = 0;i < NCities;i++)
		{
			Tuple *c = dict_find(iter, 3 + i);
			if (c) {
				char* Titolo=malloc(c->length);
				strcpy ( Titolo,  c->value->cstring );
				
				s_menu_items_city[i] = (SimpleMenuItem) {
					.title = Titolo,
					.callback = menu_select_callback,
				};
			}
		}
		
		
		
		//creo il numeu e gli associo gli item
		s_menu_sections_city[0] = (SimpleMenuSection) {
			.num_items = NCities,
				.items = s_menu_items_city,
		};

		APP_LOG(APP_LOG_LEVEL_DEBUG, "Controllo se la win");
		if(!PopReachWindows(window_city))
		{
			window_city = Crea3BWindow();
			window_stack_push(window_city, true);
		}
		
		//creo il nuovo layer e lo aggiungo al layer window
		s_simple_menu_layer_city=SimpleMenuLateyCreateAndBind(window_city,s_menu_sections_city);
		
		
	}
}





/*
	Dizionario:
	
	1:messaggio
	2:numero giorni
	3:id citta
	4:Titolo G 1
	5:Sottotitolo G 1
	6:desc G 1
*/
static void LoadDaysLayer(DictionaryIterator *iter)
{

	
	//ottengo il numero di giorni
	Tuple *n = dict_find(iter, 2);
	if (n) {
		int NDays = n->value->uint8;
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Numero giorni: %d", NDays);
		
		//ottengo l'id della citta selezionata
		n = dict_find(iter, 3);
		if (n) {
			CurrentCity=n->value->uint8;
		}
		else 
			return;
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "id città: %d", 	CurrentCity);
		
		
		
		
		//resetto le variabili gia inizializzate
		FreeLayerDay();
		
		s_menu_items_day = (SimpleMenuItem*)malloc(sizeof(SimpleMenuItem)*NDays);
		s_menu_items_day_count=NDays;
		
		CurrentDay=-1;
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Old var liberate");
		
		

		
		for (int i = 0;i < NDays;i++)
		{
			Tuple *c = dict_find(iter, 4 + i*3 );
			char* Titolo=malloc(c->length);
			strcpy ( Titolo,  c->value->cstring );
			
			c = dict_find(iter, 4 + i*3 +1 );
			char* Sottotitolo=malloc(c->length);
			strcpy ( Sottotitolo,  c->value->cstring );
			
			c = dict_find(iter, 4 + i*3 +2 );
			int desc= c->value->uint32;
			GBitmap *s_menu_icon_image = GetBitmapFromID(desc);
			
			s_menu_items_day[i] = (SimpleMenuItem) {
				.title = Titolo,
				.subtitle= Sottotitolo,
				.callback = menu_select_callback,
				.icon = s_menu_icon_image,
			};
			
			//gbitmap_destroy(s_menu_icon_image);
		}
		
		
		
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Menu items creati");

		//creo il numeu e gli associo gli item
		s_menu_sections_day[0] = (SimpleMenuSection) {
			.num_items = NDays,
			.items = s_menu_items_day,
		};
		APP_LOG(APP_LOG_LEVEL_DEBUG, "items associati alla sezione");

		bool WinJustCreated=false;
		if(!PopReachWindows(window_day))
		{
			window_day = Crea3BWindow();
			WinJustCreated=true;
			APP_LOG(APP_LOG_LEVEL_DEBUG, "window_day creata");
		}
		
		
		
		
		s_simple_menu_layer_day=SimpleMenuLateyCreateAndBind(window_day,s_menu_sections_day);
		
	
		
		if(WinJustCreated)
			window_stack_push(window_day, true);
	}	
	
}






/*
Dizionario:

1:messaggio
2:numero ore
3:id citta
4:id giorno
5:Titolo O 1
6:Sottotitolo O 1
7:desc O 1
*/
static void LoadHoursLayer(DictionaryIterator *iter)
{

	//ottengo il numero di giorni
	Tuple *n = dict_find(iter, 2);
	if (n) {
		int NHours = n->value->uint8;

		//ottengo l'id della citta selezionata
		n = dict_find(iter, 3);
		if (n) {
			int ncitta = n->value->uint8;
			CurrentCity = ncitta;
		}
		else
			return;


		//ottengo l'id del giorno selezionato
		n = dict_find(iter, 4);
		if (n) {
			int nday = n->value->uint8;
			CurrentDay = nday;
		}
		else
			return;

		
		
		//resetto le variabili gia inizializzate
		FreeLayerHour();
		

		s_menu_items_hour = (SimpleMenuItem*)malloc(sizeof(SimpleMenuItem)*NHours);
		s_menu_items_hour_count=NHours;
		

		APP_LOG(APP_LOG_LEVEL_DEBUG, "Numero fasce orarie: %d", NHours);

		
		

		for (int i = 0;i < NHours;i++)
		{
			Tuple *c = dict_find(iter, 5 + i * 3);
			char* Titolo = malloc(c->length);
			strcpy(Titolo, c->value->cstring);

			c = dict_find(iter,5 + i * 3 + 1);
			char* Sottotitolo = malloc(c->length);
			strcpy(Sottotitolo, c->value->cstring);


			c = dict_find(iter, 5 + i * 3 + 2);
			int desc = c->value->uint32;
			GBitmap *s_menu_icon_image = GetBitmapFromID(desc);

			s_menu_items_hour[i] = (SimpleMenuItem) {
				.title = Titolo,
				.subtitle = Sottotitolo,
				.callback = menu_select_callback,
				.icon = s_menu_icon_image,
			};
		}

		

		//creo il numeu e gli associo gli item
		s_menu_sections_hour[0] = (SimpleMenuSection) {
			.num_items = NHours,
				.items = s_menu_items_hour,
		};

		
		
		bool WinJustCreated=false;
		if(!PopReachWindows(window_hour))
		{
			window_hour = Crea3BWindow();
			WinJustCreated=true;
			APP_LOG(APP_LOG_LEVEL_DEBUG, "window_hour creata");
		}
		
		
		
		s_simple_menu_layer_hour=SimpleMenuLateyCreateAndBind(window_hour,s_menu_sections_hour);
		

		
		if(WinJustCreated)
			window_stack_push(window_hour, true);
		
		
	}

}
/*
static void LoadSmallInfoLayer()
{
	Layer *window_layer = window_get_root_layer(window_city);
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

}*/





static GBitmap * GetBitmapFromID(int id)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "id img %d",id);
	if(id>IconNum)
	{
		return gbitmap_create_with_resource(RESOURCE_ID_DOMANDA);
	}
	else
		return gbitmap_create_with_resource(WEATHER_ICONS[id]);
}













static void menu_select_callback(int index, void *context)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "index click: %d", index);
	if(CurrentCity== -1 && CurrentDay==-1)
	{
		//cliccato sulla citta, carico la lista dei giorni
		RequestListDay(index);
		//s_menu_items_city[index].title="click";
		
	}
	else if(CurrentCity!= -1 && CurrentDay==-1)
	{
		//cliccato sul giorno, carico la lista delle ore
		RequestListHours(CurrentCity,index);
		//s_menu_items_day[index].title="click";
	}else
	{
		//s_menu_items_hour[index].title="click";
	}
}

static void window_load(Window *window) {
 	app_message_register_inbox_received(inbox_received_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}



static void init(void) {
  window_city = window_create();
  window_set_background_color(window_city, GColorFromHEX(0x1461A1));
  window_set_window_handlers(window_city, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(window_city, true);

}

static void deinit(void) {
  window_destroy(window_city);
	window_destroy(window_day);
	window_destroy(window_hour);

  /*app_sync_deinit(&s_sync);*/
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
