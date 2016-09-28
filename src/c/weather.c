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
	/*MESSAGE_REQUEST_ALL_DATA = 0x9,
	MESSAGE_INBOX_ALL_DATA = 0x10,
	MESSAGE_REQUEST_PARAMETRI = 0x11,
	MESSAGE_INBOX_PARAMETRI = 0x12,*/
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




//static void FreeCities();
static void LoadCityLayer();
static void LoadDaysLayer();
static void LoadHoursLayer();
static void menu_select_callback(int index, void *context);
static GBitmap * GetBitmapFromID(int id);


//Parte per memorizzare dati offline
/*

//questi parametri devono essere settati dal javascript
#define VERSIONE_PARAMETRI 1
static int Parametri[5];
static int NumeroParametri = 5;


//static int NUMERO_DATI_ORA = 8;
//static int NUMERO_FASCIE_ORARIE = 24;
//static int NUMERO_DATI_GIORNALIERI_ADDIZIONALI = 5;
//static int NUMERO_DI_GIORNI = 15;
//static int NUMERO_DATI_CITTA_ADDIZIONALI = 1;
//
//
//
//	Struttura storage:
//
//	VERSIONE_PARAMETRI
//	NUMERO_DATI_ORA
//	NUMERO_FASCIE_ORARIE
//	NUMERO_DATI_GIORNALIERI_ADDIZIONALI
//	NUMERO_DI_GIORNI
//	NUMERO_DATI_CITTA_ADDIZIONALI
//	numero città
//
//	Nome citta
//	{
//		nome giorno
//		numero giorno
//		temp min
//		temp max
//		desc giorno
//		numero fascie orarie
//		{
//			ora
//			desc
//			temp
//			temp percepita
//			precipitazioni
//			vento intesita
//			vento direzione
//			umidita %
//		}
//		{...}
//	}{...}
//	Nome Citta2
//	{...}
//
//





static bool CheckVersionStorage()
{
	if (persist_exists(0) && persist_read_int(0)== VERSIONE_PARAMETRI)
		return true;
	return false;
}
static bool LoadParametriDaStorage()
{
	if (persist_exists(1) && persist_exists(2) && persist_exists(3) && persist_exists(4) && persist_exists(5))
	{
		NUMERO_DATI_ORA = persist_read_int(1);
		NUMERO_FASCIE_ORARIE = persist_read_int(2);
		NUMERO_DATI_GIORNALIERI_ADDIZIONALI = persist_read_int(3);
		NUMERO_DI_GIORNI = persist_read_int(4);
		NUMERO_DATI_CITTA_ADDIZIONALI = persist_read_int(5);
		return true;
	}
	return false;
}
static void RequestParametriDaJS()
{
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	if (!iter) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore invio richiesta dati orari");
		return;
	}

	int value = MESSAGE_REQUEST_PARAMETRI;
	dict_write_int(iter, 1, &value, sizeof(int), true);
	dict_write_end(iter);
	app_message_outbox_send();
}
static bool SetParametriFromJS(DictionaryIterator *iter)
{
	//numero di parametri
	Tuple *n = dict_find(iter, 2);
	if (n) {
		int nparam = n->value->uint8;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Numero parametri: %d", NCities);
		if (nparam == 5)
		{
			for (int i = 0;i < nparam;i++)
			{
				Tuple *c = dict_find(iter, 3+i);
				if (c) {
					Parametri[i] = c->value->uint8;
				}
				else
					return false;
			}
			return true;
		}
	}
	return false;
}
static void LiteClearStorage()
{
	//semplicemente cancello la versione dello storage in modo che tutti i dati all'interno vengono invalidati e sovrascritti
	if (persist_exists(0))
		persist_delete(0);
}
static void HeavyClearStorage()
{
	//cancello tutti i dati
	for (int i = 0;i < 4096;i++)
		if (persist_exists(i))
			persist_delete(i);
}

//Struttura storage:
//
//VERSIONE_PARAMETRI
//NUMERO_DATI_ORA
//NUMERO_FASCIE_ORARIE
//NUMERO_DATI_GIORNALIERI_ADDIZIONALI
//NUMERO_DI_GIORNI
//NUMERO_DATI_CITTA_ADDIZIONALI
//numero città
//
//Nome citta
//numero giorni
//{
//	nome giorno
//	temp min
//	temp max
//	desc giorno
//	numero fascie orarie
//	{
//		ora
//		desc
//		temp
//		temp percepita
//		precipitazioni
//		vento intesita
//		vento direzione
//		umidita %
//	}
//	{...}
//}{...}
//	
//Nome Citta2
//{...}



static bool SetAllDataStorage(DictionaryIterator *iter)
{
	Tuple *n = dict_find(iter, 2);
	if (n) {
		int IndirizzoPartenzaStorage = 6;

		int ncitta = n->value->uint32;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Numero citta: %d", ncitta);

		int PosCorrenteDict = 3;
		for (int icitta = 0;icitta < ncitta;i++)
		{

			Tuple *c = dict_find(iter, ++PosCorrenteDict); //nome città
			if (c)
				persist_write_string(GetID_Citta(icitta)+1, c->value->cstring)
			else
				return false;

			c = dict_find(iter, ++PosCorrenteDict);	//numero giorni
			if (c)
			{
				int ngiorni = c->value->uint32;
				for (int igiorni = 0;igiorni < ngiorni;i++)
				{
					c = dict_find(iter, ++PosCorrenteDict); //nome città
					if (c)
						persist_write_string(GetID_Giorno(icitta, igiorni) + 1, c->value->cstring)
					else
						return false;

					c = dict_find(iter, ++PosCorrenteDict); //temp min
					if (c)
						persist_write_data(GetID_Giorno(icitta, igiorni) + 2, c->value->uint8,sizeof(uint8_t))
					else
						return false;

					c = dict_find(iter, ++PosCorrenteDict); //temp max
					if (c)
						persist_write_data(GetID_Giorno(icitta, igiorni) + 3, c->value->uint8, sizeof(uint8_t))
					else
						return false;

					c = dict_find(iter, ++PosCorrenteDict); //desc giorno
					if (c)
						persist_write_data(GetID_Giorno(icitta, igiorni) + 4, c->value->uint8, sizeof(uint8_t))
					else
						return false;

					c = dict_find(iter, ++PosCorrenteDict); //desc giorno
					if (c)
						int nfascie = c->value->uint32;
						for (int ifascie = 0;ifascie < nfascie;i++)
						{
							c = dict_find(iter, ++PosCorrenteDict); //ora
							if (c)
								persist_write_string(GetID_Ore(icitta, igiorni, ifascie) + 1, c->value->cstring)
							else
								return false;

							c = dict_find(iter, ++PosCorrenteDict); //desc
							if (c)
								persist_write_data(GetID_Ore(icitta, igiorni, ifascie) + 2, c->value->uint8, sizeof(uint8_t))
							else
								return false;

							c = dict_find(iter, ++PosCorrenteDict); //temp
							if (c)
								persist_write_data(GetID_Ore(icitta, igiorni, ifascie) + 3, c->value->uint8, sizeof(uint8_t))
							else
								return false;

							c = dict_find(iter, ++PosCorrenteDict); //tempperc
							if (c)
								persist_write_data(GetID_Ore(icitta, igiorni, ifascie) + 4, c->value->uint8, sizeof(uint8_t))
							else
								return false;

							c = dict_find(iter, ++PosCorrenteDict); //prec
							if (c)
								persist_write_data(GetID_Ore(icitta, igiorni, ifascie) + 5, c->value->uint8, sizeof(uint8_t))
							else
								return false;

							c = dict_find(iter, ++PosCorrenteDict); //vent intes
							if (c)
								persist_write_data(GetID_Ore(icitta, igiorni, ifascie) + 6, c->value->uint8, sizeof(uint8_t))
							else
								return false;

							c = dict_find(iter, ++PosCorrenteDict); //vent dir
							if (c)
								persist_write_string(GetID_Ore(icitta, igiorni, ifascie) + 7, c->value->cstring)
							else
								return false;

							c = dict_find(iter, ++PosCorrenteDict); //umid
							if (c)
								persist_write_data(GetID_Ore(icitta, igiorni, ifascie) + 8, c->value->uint8, sizeof(uint8_t))
							else
								return false;
						}
					else
						return false;
				}
			}
			else
				return false;

			//DA COMPLETARE
		}

	}
	return false;
}


static int NumeroDatiInGiorno()
{
	return NUMERO_FASCIE_ORARIE*NUMERO_DATI_ORA + NUMERO_DATI_GIORNALIERI_ADDIZIONALI;
}
static int NumeroDatiInCitta()
{
	return NUMERO_DI_GIORNI* NumeroDatiInGiorno();
}

static int GetID_Ore(int NCitta,int NGiorno,int Fascia)
{
	return GetID_Giorno(NCitta, NGiorno) + NUMERO_DATI_GIORNALIERI_ADDIZIONALI + 1 + NUMERO_DATI_ORA*Fascia;
}
static int GetID_Giorno(int NCitta, int NGiorno)
{
	return GetID_Citta(NCitta) + NUMERO_DATI_CITTA_ADDIZIONALI +1+ NumeroDatiInGiorno()*NGiorno ;
}
static int GetID_Citta(int NCitta)
{
	return NumeroDatiInCitta()*NCitta + NumeroParametri;
}








static void RequestAllData()
{
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	if (!iter) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore invio richiesta dati orari");
		return;
	}

	int value = MESSAGE_REQUEST_ALL_DATA;
	dict_write_int(iter, 1, &value, sizeof(int), true);
	dict_write_end(iter);
	app_message_outbox_send();
}
*/



/*static char **Cities=0;
int NCities=0;*/



static void inbox_received_callback(DictionaryIterator *iter, void *context) {
		
	//TODO
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
				/*case MESSAGE_INBOX_PARAMETRI:
					APP_LOG(APP_LOG_LEVEL_DEBUG, "Parametri Arrivati");
					//se non riesce a leggere il dizionario ( o ci sono problemi ) 
					if (!SetParametriFromJS(iter))
					{
						APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore nei dati letti dal JS");
						//resetto la versione dello storage corrente
						LiteClearStorage();
						APP_LOG(APP_LOG_LEVEL_DEBUG, "Richiedo i parametri al JS");
						//richiedo da JS i parametri
						RequestParametriDaJS();
					}
				break;
				case MESSAGE_REQUEST_ALL_DATA:
					if (SetAllDataStorage(iter))
					{
						APP_LOG(APP_LOG_LEVEL_DEBUG, "Errore nel settagio di tutti i dati meteo");
					}
				break;
				*/

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
		window_hour=0;
		
		for(int i=0;i<s_menu_items_hour_count;i++)
				gbitmap_destroy(s_menu_items_hour[i].icon);
		
	}
	else if( window == window_day)
	{
		CurrentDay=-1;
		CurrentCity=-1;
		window_day=0;
		
		for(int i=0;i<s_menu_items_day_count;i++)
				gbitmap_destroy(s_menu_items_day[i].icon);
	}
	else if( window == window_city)
	{
		window_city=0;
	}
	
	window_destroy(window);
	
	
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
		if (s_menu_items_city != 0)
			free(s_menu_items_city);
		s_menu_items_city = (SimpleMenuItem*)malloc(sizeof(SimpleMenuItem)*NCities);
		s_menu_items_city_count=NCities;
		if (s_simple_menu_layer_city != 0)
		{
			layer_remove_from_parent((Layer *)s_simple_menu_layer_city);
			simple_menu_layer_destroy(s_simple_menu_layer_city);
			s_simple_menu_layer_city = 0;
		}
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

		//ottengo il layer su cui aggiungere il mio menu
		Layer *window_layer = window_get_root_layer(window_city);
		GRect bounds = layer_get_frame(window_layer);

		//creo il nuovo layer
		s_simple_menu_layer_city = simple_menu_layer_create(bounds, window_city, s_menu_sections_city, 1, NULL);

		//lo aggiungo al layer principale
		layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer_city));
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
		if (s_menu_items_day != 0)
			free(s_menu_items_day);
		
		s_menu_items_day = (SimpleMenuItem*)malloc(sizeof(SimpleMenuItem)*NDays);
		s_menu_items_day_count=NDays;
		if (s_simple_menu_layer_day != 0)
		{
			layer_remove_from_parent((Layer *)s_simple_menu_layer_day);
			simple_menu_layer_destroy(s_simple_menu_layer_day);
			s_simple_menu_layer_day = 0;
		}
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

		//window_hour
		if(window_day!=0)
		{
			APP_LOG(APP_LOG_LEVEL_DEBUG, "trovata vecchia window_day");
			window_destroy(window_day);
			APP_LOG(APP_LOG_LEVEL_DEBUG, "vecchia window distrutta");
		}
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "controllo win completato");
		
		window_day = window_create();
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Window creata");
		
		window_set_background_color(window_day, GColorFromHEX(0x1461A1));
		window_set_window_handlers(window_day, (WindowHandlers) {
			.unload = window_unload
		});
		
		
		
		//ottengo il layer su cui aggiungere il mio menu
		Layer *window_layer = window_get_root_layer(window_day);
		GRect bounds = layer_get_frame(window_layer);

		//creo il nuovo layer
		s_simple_menu_layer_day = simple_menu_layer_create(bounds, window_day, s_menu_sections_day, 1, NULL);

		//lo aggiungo al layer principale
		layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer_day));

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
		if (s_menu_items_hour != 0)
			free(s_menu_items_hour);
		s_menu_items_hour = (SimpleMenuItem*)malloc(sizeof(SimpleMenuItem)*NHours);
		s_menu_items_hour_count=NHours;
		if (s_simple_menu_layer_hour != 0)
		{
			layer_remove_from_parent((Layer *)s_simple_menu_layer_hour);
			simple_menu_layer_destroy(s_simple_menu_layer_hour);
			s_simple_menu_layer_hour = 0;
		}



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

			//gbitmap_destroy(s_menu_icon_image);
		}


		//creo il numeu e gli associo gli item
		s_menu_sections_hour[0] = (SimpleMenuSection) {
			.num_items = NHours,
				.items = s_menu_items_hour,
		};

			//window_hour
		if(window_hour!=0)
		{
			APP_LOG(APP_LOG_LEVEL_DEBUG, "trovata vecchia window_day");
			window_destroy(window_hour);
			APP_LOG(APP_LOG_LEVEL_DEBUG, "vecchia window distrutta");
		}
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "controllo win completato");
		
		window_hour = window_create();
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Window creata");
		
		window_set_background_color(window_hour, GColorFromHEX(0x1461A1));
		window_set_window_handlers(window_hour, (WindowHandlers) {
			.unload = window_unload
		});
		
		
		
		//ottengo il layer su cui aggiungere il mio menu
		Layer *window_layer = window_get_root_layer(window_hour);
		GRect bounds = layer_get_frame(window_layer);

		//creo il nuovo layer
		s_simple_menu_layer_hour = simple_menu_layer_create(bounds, window_hour, s_menu_sections_hour, 1, NULL);

		//lo aggiungo al layer principale
		layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer_hour));

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
