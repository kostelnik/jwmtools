// Applet for displaying metar weather data for given location

#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <values.h>
#include <metar.h>

GtkBuilder *builder;
GtkWidget *window1, *window2;
GtkLabel *labTitle1;
GtkLabel *labDateTime1;
GtkLabel *labPressureVisibility1;
GtkLabel *labTemp1;
GtkLabel *labWind1;
GtkImage *imgClouds1;
GtkImage *imgWind1;

GtkLabel *labTray1;
GtkImage *imgTray1;

char station[5] = "LZKZ";
char station_name[50] = "Košice, Barca (Slovensko)";

G_MODULE_EXPORT void
on_winWeatherTray1_delete_event (GtkObject *object, gpointer user_data);

G_MODULE_EXPORT void
on_eventbox1_button_press_event (GtkObject *object, gpointer user_data);

G_MODULE_EXPORT void
on_winWeatherTray1_delete_event (GtkObject *object, gpointer user_data) {
  // correctly terminate application
  gtk_main_quit();
}

gboolean b = TRUE;

G_MODULE_EXPORT void
on_eventbox1_button_press_event (GtkObject *object, gpointer user_data) {
  // click on eventbox (image + label)
  printf("eventbox zzzzzz\n");
  b = !b;
  if (b)
    gtk_widget_hide (window2);
  else {
    gtk_widget_show (window2);
    int x,y;
    gtk_window_get_position ((GtkWindow*)window2, &x, &y);
    if (y < 25)
      y = 25;
    gtk_window_move ((GtkWindow*)window2, x, y);
  }
  //window2 = GTK_WIDGET (gtk_builder_get_object (builder, "winMain1"));
}

static gboolean
time_handler(GtkWidget *widget) {
  // periodical weather update
  printf("weather: update\n");

  // download observation (2 lines (datetime + metar data) from weather.noaa.gov)
  // FIXME: perhaps I could use libcurl or something instead of wget pipe
  char * cmd = (char*)malloc(sizeof(char)*256);
  //sprintf(cmd,"wget -qO - http://weather.noaa.gov/pub/data/observations/metar/stations/%s.TXT",station);
  sprintf(cmd,"wget -qO - http://tgftp.nws.noaa.gov/data/observations/metar/decoded/%s.TXT",station);
  char * datetime = NULL;
  char * observation = NULL;
  FILE *f = (FILE*)popen(cmd,"r");
  if (f) {
    size_t len = 0;
    ssize_t i = getline(&datetime,&len,f);
	if (i <= 0)
    	printf("wget: dt = '%s'\n",datetime);
    i = getline(&observation,&len,f);
	if (i <= 0)
    	printf("wget: o  = '%s'\n",observation);
    fclose(f);
  } else {
    // failed
    printf("error: cannot download metar data\n");
    return TRUE;
  }

  // decode metar data
  Decoded_METAR MetarStruct, *Mptr = &MetarStruct;
  if( decode_metar( observation, Mptr) != 0 ) {
    printf("metar: decoding error - %s\n",observation);             
    return TRUE;
  }
  
  // update labels
  char * title = (char*)malloc(sizeof(char)*256);
  char * temp = (char*)malloc(sizeof(char)*256);
  char * temp_b = (char*)malloc(sizeof(char)*256);
  char * visi = (char*)malloc(sizeof(char)*256);
  char * pres_visi = (char*)malloc(sizeof(char)*256);
  char * wind = (char*)malloc(sizeof(char)*256);
  char * clouds = (char*)malloc(sizeof(char)*256);

  // prepare strings
  sprintf(title,"<b>%s</b>",station_name);
  sprintf(temp,"%d °C ",Mptr->temp);
  sprintf(temp_b,"<b>%s</b>",temp);

  //                   111111 
  //         0123456789012345
  // convert 2011/02/13 12:34\n to "13. Month 2011, HH:MM"
  // FIXME: holy shit, this datetime to datetime conversion is insane
  // FIXME: learn and use glib
  // equivalent pascal code: datetime := FormatDateTime('dd. mmmm yyyy HH:MM',StrToDateTime(s))
  char year[4+1];
  year[0] = datetime[0];
  year[1] = datetime[1];
  year[2] = datetime[2];
  year[3] = datetime[3];
  year[5] = 0;
  char day[2+1];
  day[0] = datetime[8];
  day[1] = datetime[9];
  day[2] = 0;
  int mm = 10*(datetime[5]-'0')+(datetime[6]-'0');
  //printf("mm = %d\n",mm);
  char month[20+1];
  switch (mm) {
    case 1:  strcpy(month,"Január"); break;
    case 2:  strcpy(month,"Február"); break;
    case 3:  strcpy(month,"Marec"); break;
    case 4:  strcpy(month,"Apríl"); break;
    case 5:  strcpy(month,"Máj"); break;
    case 6:  strcpy(month,"Jún"); break;
    case 7:  strcpy(month,"Júl"); break;
    case 8:  strcpy(month,"August"); break;
    case 9:  strcpy(month,"September"); break;
    case 10: strcpy(month,"Október"); break;
    case 11: strcpy(month,"November"); break;
    case 12: strcpy(month,"December"); break;
  }
  char hour[2+1];
  hour[0] = datetime[11];
  hour[1] = datetime[12];
  hour[2] = 0;
  char minute[2+1];
  minute[0] = datetime[14];
  minute[1] = datetime[15];
  minute[2] = 0;
  sprintf(datetime,"%s. %s %s, %s:%s",day,month,year,hour,minute);
  
  // viditelnost
  strcpy(visi,"???");
  if (Mptr->CAVOK)
    strcpy(visi,"výborná");
  if ( (Mptr->prevail_vsbyM > 0)&&(Mptr->prevail_vsbyM < 99000) )
    sprintf(visi,"%1.0f m",Mptr->prevail_vsbyM);
  if ( (Mptr->prevail_vsbyKM > 0)&&(Mptr->prevail_vsbyKM < 200) )
    sprintf(visi,"%1.1f km",Mptr->prevail_vsbyKM);
    
  // tlak a viditelnost
  sprintf(pres_visi,"Tlak %d kPa, viditeľnosť %s",Mptr->hectoPasc_altstng,visi);

  // convert knots to m/s
  int ws = Mptr->winData.windSpeed;
  if (strcmp(Mptr->winData.windUnits,"KT")==0) 
    ws = ws / 0.514444 / 3.6;
  sprintf(wind,"<b>%d m/s</b>",ws);

  // debug
  printf("  title     : %s\n",title);
  printf("  temp      : %s\n",temp);
  printf("  pres_visi : %s\n",pres_visi);

  // tray
  gtk_label_set_markup(labTray1,temp);
  //gtk_image_set_from_icon_name(imgClouds1,"weather-few-clouds",GTK_ICON_SIZE_LARGE_TOOLBAR);

  // dialog
  gtk_label_set_markup(labTitle1,title);
  gtk_label_set_markup(labDateTime1,datetime);
  gtk_label_set_markup(labTemp1,temp_b);
  gtk_label_set_markup(labWind1,wind);
  gtk_label_set_markup(labPressureVisibility1,pres_visi);
  //gtk_image_set_from_icon_name(imgClouds1,"weather-few-clouds",GTK_ICON_SIZE_LARGE_TOOLBAR);
  //gtk_image_set_from_icon_name(imgWind1,"gtk-dialog-error",GTK_ICON_SIZE_LARGE_TOOLBAR);

  // clouds
  sprintf(clouds,"gtk-image-missing");
  printf("Cloud type: %s\n",Mptr->cloudGroup[0].cloud_type);
  if (
      (strcmp("SKC",Mptr->cloudGroup[0].cloud_type)==0)
   || (strcmp("CLR",Mptr->cloudGroup[0].cloud_type)==0) 
   || (strcmp("NSC",Mptr->cloudGroup[0].cloud_type)==0)
   || (strcmp("",Mptr->cloudGroup[0].cloud_type)==0) 
     )
    sprintf(clouds,"weather-clear");

  if ( 
     (strcmp("FEW",Mptr->cloudGroup[0].cloud_type)==0)
  || (strcmp("SCT",Mptr->cloudGroup[0].cloud_type)==0) )
    sprintf(clouds,"weather-few-clouds");

  if (
     (strcmp("BKN",Mptr->cloudGroup[0].cloud_type)==0) 
  || (strcmp("OVC",Mptr->cloudGroup[0].cloud_type)==0) 
     )
    sprintf(clouds,"weather-overcast");
  // is it night already?
#if defined VERY_OLD_GLIB && (VERY_OLD_GLIB > 0)
  gint h = 12;
#else
  GDateTime *dt = g_date_time_new_now_local();
  gint h = g_date_time_get_hour(dt);
  g_date_time_unref(dt);
#endif
  gboolean night = ((h>=0)&&(h<=6)) || (h >= 20);
  
  printf("h=%d night=%d\n",h,night);
  if (night)
    if ( (!(strcmp("OVC",Mptr->cloudGroup[0].cloud_type)==0)) && (!(strcmp("BKN",Mptr->cloudGroup[0].cloud_type)==0)) )
      sprintf(clouds,"%s-night",clouds);
  // set icons
  gtk_image_set_from_icon_name(imgClouds1,clouds,GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_image_set_from_icon_name(imgTray1,clouds,GTK_ICON_SIZE_LARGE_TOOLBAR);
  
  printf("obstruct = %s\n",Mptr->WxObstruct[0]);
  
  // snow?
  if ( (strcmp(Mptr->WxObstruct[0],"SN")==0)
    || (strcmp(Mptr->WxObstruct[0],"-SN")==0)
    || (strcmp(Mptr->WxObstruct[0],"+SN")==0)
    || (strcmp(Mptr->WxObstruct[0],"SS")==0)
    || (strcmp(Mptr->WxObstruct[0],"-SS")==0)
    || (strcmp(Mptr->WxObstruct[0],"+SS")==0)
    || (strcmp(Mptr->WxObstruct[0],"SG")==0) 
    || (strcmp(Mptr->WxObstruct[0],"-SG")==0) 
    || (strcmp(Mptr->WxObstruct[0],"+SG")==0) ) {
    gtk_image_set_from_icon_name(imgClouds1,"weather-snow",GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_image_set_from_icon_name(imgTray1,"weather-snow",GTK_ICON_SIZE_LARGE_TOOLBAR);
  }

  // rain?
  if ( (strcmp(Mptr->WxObstruct[0],"RA")==0) 
    || (strcmp(Mptr->WxObstruct[0],"-RA")==0)
    || (strcmp(Mptr->WxObstruct[0],"+RA")==0) ) {
    gtk_image_set_from_icon_name(imgClouds1,"weather-snow",GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_image_set_from_icon_name(imgTray1,"weather-snow",GTK_ICON_SIZE_LARGE_TOOLBAR);
  }

  // thunderstorm?
  // FIXME: i'm not sure if TS in in obstruct field
  if (strcmp(Mptr->WxObstruct[0],"TS")==0) {
    gtk_image_set_from_icon_name(imgClouds1,"weather-storm",GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_image_set_from_icon_name(imgTray1,"weather-storm",GTK_ICON_SIZE_LARGE_TOOLBAR);
  }

  // wind direction
  // FIXME: for now only 4 directions, add 8
  int wd = Mptr->winData.windDir;
  // north
  if ( ((wd >= 0)&&(wd < 45)) || (wd >= 315) ) 
    gtk_image_set_from_icon_name(imgWind1,"go-down",GTK_ICON_SIZE_LARGE_TOOLBAR);
  // east
  if ((wd >= 45)&&(wd < 135)) 
    gtk_image_set_from_icon_name(imgWind1,"go-previous",GTK_ICON_SIZE_LARGE_TOOLBAR);
  // south
  if ((wd >= 135)&&(wd < 225)) 
    gtk_image_set_from_icon_name(imgWind1,"go-up",GTK_ICON_SIZE_LARGE_TOOLBAR);
  // west
  if ((wd >= 225)&&(wd < 315)) 
    gtk_image_set_from_icon_name(imgWind1,"go-next",GTK_ICON_SIZE_LARGE_TOOLBAR);
  // no wind, no icon
  if (ws <= 0)
    gtk_image_set_from_icon_name(imgWind1,"face-smile",GTK_ICON_SIZE_LARGE_TOOLBAR);


  // release temporary variables
  free(datetime);
  free(observation);
  free(cmd);

  free(title);
  free(temp);
  free(temp_b);
  free(pres_visi);
  free(wind);
  free(visi);
  free(clouds);
  

/*
/usr/share/icons/Tango/16x16/status/weather-severe-alert.png
/usr/share/icons/Tango/16x16/status/weather-showers.png
/usr/share/icons/Tango/16x16/status/weather-showers-scattered.png
/usr/share/icons/Tango/16x16/status/weather-snow.png
/usr/share/icons/Tango/16x16/status/weather-storm.png
*/









/*

  char * time = (char*)malloc(sizeof(char)*256);
  char * temperature = (char*)malloc(sizeof(char)*256);
  char * wind = (char*)malloc(sizeof(char)*256);
  char * pressure = (char*)malloc(sizeof(char)*256);
  char * clouds = (char*)malloc(sizeof(char)*256);
  
  weather_data_get("LZKZ",time,temperature,wind,pressure,clouds);

  // tray
  gtk_label_set_markup(labTray1,temperature);
  //gtk_image_set_from_icon_name(imgClouds1,"weather-few-clouds",GTK_ICON_SIZE_LARGE_TOOLBAR);

  // dialog
  //gtk_label_set_markup(labTitle1,"<b>Košice, Barca</b>");
  gtk_label_set_markup(labDateTime1,time);
  gtk_label_set_markup(labTemp1,temperature);
  gtk_label_set_markup(labWind1,wind);
  gtk_label_set_markup(labPressureVisibility1,pressure);
  //gtk_image_set_from_icon_name(imgClouds1,"weather-few-clouds",GTK_ICON_SIZE_LARGE_TOOLBAR);
  //gtk_image_set_from_icon_name(imgWind1,"gtk-dialog-error",GTK_ICON_SIZE_LARGE_TOOLBAR);
  
  free(time);
  free(temperature);
  free(wind);
  free(pressure);
  free(clouds);
  
  */
  
  return TRUE;
}

int main (int argc, char *argv[]) {
  // initialize gtk application
  gtk_init (&argc, &argv);

  // glade builder
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "trayweather.glade", NULL);
  gtk_builder_add_from_file (builder, PREFIX"/share/jwmtools/trayweather.glade", NULL);

  // main window
  window1 = GTK_WIDGET (gtk_builder_get_object (builder, "winWeatherTray1"));
  window2 = GTK_WIDGET (gtk_builder_get_object (builder, "winMain1"));
  gtk_builder_connect_signals (builder, NULL);          
  
  // tray components
  imgTray1 = (GtkImage*)gtk_builder_get_object(builder, "imgTray1");
  labTray1 = (GtkLabel*)gtk_builder_get_object(builder, "labTray1");
  
  // get components
  labTitle1 = (GtkLabel*)gtk_builder_get_object(builder, "labTitle1");
  labDateTime1 = (GtkLabel*)gtk_builder_get_object(builder, "labDateTime1");
  labPressureVisibility1 = (GtkLabel*)gtk_builder_get_object(builder, "labPressureVisibility1");
  labTemp1 = (GtkLabel*)gtk_builder_get_object(builder, "labTemp1");
  labWind1 = (GtkLabel*)gtk_builder_get_object(builder, "labWind1");
  imgClouds1 = (GtkImage*)gtk_builder_get_object(builder, "imgClouds1");
  imgWind1 = (GtkImage*)gtk_builder_get_object(builder, "imgWind1");
  
  // set component values
  gtk_label_set_markup(labTitle1,"<b>??????, ????? ( ?????? ??? )</b>");
  gtk_label_set_markup(labDateTime1,"??. ??????? 20??, ??:??");
  gtk_label_set_markup(labPressureVisibility1,"Tlak ???? kPa, viditeľnosť ?? km");
  gtk_label_set_markup(labTemp1,"<b>? °C</b>");
  gtk_label_set_markup(labWind1,"<b>? m/s</b>");
  gtk_image_set_from_icon_name(imgClouds1,"gtk-dialog-error",GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_image_set_from_icon_name(imgWind1,"gtk-dialog-error",GTK_ICON_SIZE_LARGE_TOOLBAR);

  // timer + its first run (time is in ms, 600'000 is 10 minutes)
  g_timeout_add(600000, (GSourceFunc) time_handler, (gpointer) window1);
  time_handler(window1);

  // main loop
  //g_object_unref (G_OBJECT (builder));
  gtk_widget_show (window1);       
  gtk_main ();
  
  return 0;
}
