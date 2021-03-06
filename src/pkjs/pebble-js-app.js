if (!Date.now) {
    Date.now = function() { return new Date().getTime(); };
}
if (!Date.nowSecond) {
    Date.nowSecond=function() { return Date.now()/1000;};
}

Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};





/*
enum MessageKey {
  MESSAGE_REQUEST_LIST_CITIES = 0x0,         
  MESSAGE_REQUEST_CITY_SMALL = 0x1,
  MESSAGE_REQUEST_CITY_HOUR = 0x2,        
	MESSAGE_REQUEST_FORCE_UPDATE = 0x3,
	MESSAGE_INBOX_LIST_CITIES = 0x4,
	MESSAGE_INBOX_DATA_SMALL = 0x5,
	MESSAGE_INBOX_DATA_HOUR = 0x6,
};

*/
var Messages= {
	IncomingRequestListCity:0,
	IncomingRequestListDay:1,
	IncomingRequestListHour:2,
  IncomingRequestForceUpdate:3,
	IncomingRequestAddCity:4,
	IncomingRequestRemoveCity:5,
	SendListCity:6,
	SendListDay:7,
	SendListHour:8,	
	/*IncomingRequestAllData:9,
	SendAllData:10,
	IncomingRequestParametri:11,
	SendParametri:12,*/
};

var LastUpdate;
function SaveLastUpdate()
{
	localStorage.setItem('LastUpdate',LastUpdate);
	console.log("salvato LastUpdate: "+LastUpdate);
}
function LoadLastUpdate()
{
	try {
       LastUpdate=JSON.parse(localStorage.getItem('LastUpdate'));
			if(LastUpdate==null)
				LastUpdate=0;
    } catch (e) {
       LastUpdate=0;
    }
}


var MeteoArray;
function SaveMeteoArray()
{
	var v=JSON.stringify(MeteoArray);
	localStorage.setItem('MeteoArray',v);
	console.log("salvate MeteoArray: "+v);
}
function LoadMeteoArray()
{
	try {
       MeteoArray=JSON.parse(localStorage.getItem('MeteoArray'));
			if(MeteoArray==null)
				MeteoArray={};
    } catch (e) {
       MeteoArray={};
    }
}

var MeteoArraySmall;
function SaveMeteoArraySmall()
{
	var v=JSON.stringify(MeteoArraySmall);
	localStorage.setItem('MeteoArraySmall',v);
	console.log("salvate MeteoArraySmall: "+v);
}
function LoadMeteoArraySmall()
{
	try {
       MeteoArraySmall=JSON.parse(localStorage.getItem('MeteoArraySmall'));
			if(MeteoArraySmall==null)
				MeteoArraySmall={};
    } catch (e) {
       MeteoArraySmall={};
    }
}

var ListaCity;
function SaveListaCity()
{
	localStorage.setItem('ListaCity', JSON.stringify(ListaCity));
	console.log("salvate: "+ListaCity);
}
function LoadListaCity()
{
	try {
			
      ListaCity=JSON.parse(localStorage.getItem('ListaCity'));
			console.log(ListaCity);
			if(ListaCity==null)
				ListaCity=[];
    } catch (e) {
	
       ListaCity=[]
    }
	
}

/*function iconFromWeatherDesc(Desc) {
	
  if (Desc =="Sereno" || Desc =="quasi sereno") {
		//pioggia
    return 2;
  } else if  (Desc =="variabile" || Desc =="nuvoloso") {
		//neve
    return 3;
  } else if (Desc == "800") {
		//nuvole
    return 1;
  } else {
		//sole
    return 0;
  }
}
*/





function SendListCity()
{

	UpdateAll(false,function(){
		console.log('Inizio invio Lista città');
		var v={1: Messages.SendListCity}; //numero messaggio 
		v[2]=ListaCity.length;	//numero elementi
		console.log('Numero città :'+v[2]);
		var n=3;
		console.log(ListaCity);
		ListaCity.forEach(function (item) {	//inseriesco gli elementi nell'array
			v[n]=item;
			n++;
		});
		console.log("Citta inviate");
		Pebble.sendAppMessage(v); //invio
	});
}

/*
	1:messaggio
	2:numero giorni
	3:id citta
	4:Titolo G 1
	5:Sottotitolo G 1
	6:desc G 1
*/
function SendListDay(City)
{
	console.log('Inizio invio Lista giorni della citta: '+City+" -> "+ListaCity[City]);
	
	
	UpdateAll(false,function(){
		var v={1: Messages.SendListDay}; //numero messaggio 
		v[2]=MeteoArraySmall[ListaCity[City]].length; //numero giorni ( x ogni giorno ci sono 3 elementi -> Ngiorni*3=ToTEl  )
		v[3]=City;
		var n=4;
		
		//prendo tutti i dati di ogni giorno ( small ) e li invio
		MeteoArraySmall[ListaCity[City]].forEach(function (item) {
			v[n++]=item[0]+" "+item[1];		
			v[n++]=item[2]+" "+item[3];		
			v[n++]=item[4];
		});
		
		console.log(JSON.stringify(v));
		//var v={1:Messages.SendListDay,2:1,3:0,4:"mer 24",5:"25 27",6:0};
		Pebble.sendAppMessage(v);
		console.log('Lista giorni inviata');
	});
}

/*
1:messaggio
2:numero ore
3:id citta
4:id giorno
5:Titolo O 1
6:Sottotitolo O 1
7:desc O 1
*/
function SendListHour(City,Day)
{
	UpdateAll(false,function(){
		var v={1: Messages.SendListHour}; //numero messaggio 
		 //numero di orari ( x ogni orario ci sono 7 elementi + orario -> 8 ->   Ngiorni*8=ToTEl  )
		v[2]=Object.keys(MeteoArray[ListaCity[City]][Day]).length-3; //-3 perche: index 0 è il nome del giorno| 1 numero giorno| "S" small data
		v[3]=City;
		v[4]=Day;
		
		
		var n=5;
		
		var target=MeteoArray[ListaCity[City]][Day];
		console.log(JSON.stringify(target));
		for (var k in target){
			if (target.hasOwnProperty(k)) {
				if(k!=0 && k!=1 && k!="S")
				{
					v[n++]=k+" "+target[k][1]+"-"+target[k][2]; //titolo
					v[n++]=target[k][3]+"/"+target[k][4]+" "+target[k][5]+"/"+target[k][6];	
					v[n++]=target[k][0];//descrizione
				}
			}
		}
		console.log(City+" "+Day);
		console.log(JSON.stringify(MeteoArray[ListaCity[City]]));
		console.log(JSON.stringify(v));
		Pebble.sendAppMessage(v);
	});
}



function UpdateAll(forceUpdate,callback)
{
		//forceUpdate=true;
		//se è passata piu di un ora o l'aggiornamento è forzato
		if((Date.nowSecond()-LastUpdate)>3600 || forceUpdate)
		{
			//ottengo il numero delle città e inizializzo un contatore
			var Len=ListaCity.length;
			console.log("Numero Di citta: "+Len);
			var current=-1;
			MeteoArray={};
			MeteoArraySmall={};
			var LoopUpdate=function(){
				
				//	aumento il contatore
				current++;
				console.log("Contatore "+current);
				//se è ancora nella lunghezza del vettore
				if(current<Len)
				{
					//prendo i dati della città dal sito; aspetto che finisca e richiamo la funzione stessa
					FetchWeather(ListaCity[current],LoopUpdate);
				}	
				else //altrimenti richiamo la callback
				{
					LastUpdate=Date.nowSecond();
					SaveLastUpdate();
					callback();
				}	
				
			};
		
			//richiamo il loop e la callback va a settare la data di aggiornamento dei dati 
			LoopUpdate();				
		}	
		else
			callback();
}

















var FetchWeatherEnd=true;
function FetchWeather(luogo,callback) {
	if(!FetchWeatherEnd)
		return false;
	FetchWeatherEnd=false;
  var req = new XMLHttpRequest();
	console.log('http://lpozzi.it/3bmeteo/?luogo='+luogo);
  req.open('GET', 'http://lpozzi.it/3bmeteo/?luogo='+luogo, true);
  req.onload = function () {
		console.log(req.readyState);
		console.log(req.status);
    if (req.readyState === 4) {
      if (req.status === 200) {
        console.log(req.responseText);
        var response = JSON.parse(req.responseText);
				MeteoArray[luogo]=response;
				MeteoArraySmall[luogo]=[];
				response.forEach(function (item) {
					//MeteoArraySmall[luogo].push(item[0]);
					//MeteoArraySmall[luogo].push(item[1]);
					var temp=[];
					item["S"].forEach(function (item1) {
						temp.push(item1);
						});
					MeteoArraySmall[luogo].push(temp);
				});
				SaveMeteoArray();
				SaveMeteoArraySmall();
				FetchWeatherEnd=true;
				console.log("fine fetch");
				callback();
        /*Pebble.sendAppMessage({
					1: "ASD" + '\xB0C',
          2: icon,         
          3: city
        });*/
      } else {
        console.log('Error');
      }
    }
  };
  req.send(null);
	return true;
}



function AddCity(City,callback)
{
	if(typeof ListaCity ==="undefined" || ListaCity.constructor !== Array)
	  ListaCity={};
	
	ListaCity.push(City);
 	SaveListaCity();
	FetchWeather(City,callback);
}

function RemoveCity(City)
{
	if(typeof ListaCity ==="undefined" || ListaCity.constructor !== Array)
	  ListaCity={};
 	else
	{
		var indice=ListaCity.indexOf(City);
		if(indice!=-1)
			ListaCity.splice(indice,1);
		
		delete MeteoArray[City];
		delete MeteoArraySmall[City];
		
	}
	SaveListaCity();
}
function RemoveCityFromIndex(CityIndex)
{
	if(typeof ListaCity ==="undefined" || ListaCity.constructor !== Array)
	  ListaCity={};
 	else
	{
		if(CityIndex!=-1)
			ListaCity.splice(CityIndex,1);
	}
	SaveListaCity();
}


function ManageIncomingMessage(dict)
{
	console.log("messaggio ricevuto: "+JSON.stringify(dict));
	if(dict[1]==Messages.IncomingRequestListCity)
	{
		SendListCity();
	}
	else if(dict[1]==Messages.IncomingRequestListDay)
	{
		SendListDay(dict[2]);
	}
	else if(dict[1]==Messages.IncomingRequestListHour)
	{
		SendListHour(dict[2],dict[3]);
	}
	else if(dict[1]==Messages.IncomingRequestForceUpdate)
	{
		UpdateAll(true,function(){
			SendListCity();
		});
	}
	else if(dict[1]==Messages.IncomingRequestAddCity)
	{
		AddCity(dict[2],function(){
			SendListCity();
		});
	}
	else if(dict[1]==Messages.IncomingRequestRemoveCity)
	{
		RemoveCityFromIndex(dict[2]);
		SendListCity();
	}
}


Pebble.addEventListener('ready', function (e) {
  //console.log('connect!' + e.ready);
  console.log("Pronto");
	LoadMeteoArray();
	LoadMeteoArraySmall();
	LoadListaCity();
	LoadLastUpdate();
	
	
	
	LastUpdate=0;
	

	
	console.log('Inizio Aggiornamento');
	UpdateAll(false,function(){
		console.log('Aggiornamento finito');
		SendListCity();
	});
});

Pebble.addEventListener('appmessage', function (e) {

	//struttura messaggi in arrivo:
	//1: messaggio
	//2: altri dati ( citta numero o nome città )
	//3: altri dati ( data )
	ManageIncomingMessage(e.payload);
		
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed');
	try {
			
			var temp=JSON.parse(decodeURIComponent(e.response));
			console.log(temp);
			if(temp!=null)
				ListaCity=temp;
			
			SaveListaCity();
			UpdateAll(true,function(){
				SendListCity();
			});
			
    } catch (ee) {
    }
	
	
});

Pebble.addEventListener('showConfiguration', function() {
	var url = 'http://lpozzi.it/3bmeteo/config.php?jsn='+encodeURIComponent(JSON.stringify(ListaCity));

  Pebble.openURL(url);
});





