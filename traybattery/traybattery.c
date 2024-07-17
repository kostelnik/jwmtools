// Applet for displaying current battery state

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "s.h"
#include "s.c"

// Here are some sample data for debuging, uncomment whichever you want
//#define SYS_BATTERY_PATH "data/1/"
//#define SYS_BATTERY_PATH "data/2/"
//#define SYS_BATTERY_PATH "data/3/"
//#define SYS_BATTERY_PATH "data/4/"

GtkBuilder  *builder;
GtkWidget   *winTray1;
GtkImage    *imgTray1;
GtkLabel    *labTray1;
char        *batteryPath;

int read_int(char * file);
char * read_word(char * file);

int read_int(char * file) {
  // read integer from single /sys/... file
  int i = 0;
  FILE *f;
  char *fn = SCreateAppend(batteryPath,file);
  if ((f = fopen(fn,"r"))) {
    fscanf(f,"%d",&i);
    fclose(f);
  }
  SFree(fn);
  return i;
}

char * read_word(char * file) {
  // read word from single /sys/... file
  // FIXME: this actually reads entire file! including EOL so SEqual is not working
  char *fn = SCreateAppend(batteryPath,file);
  char *s = SCreateFromFile(fn);
  SFree(fn);
  return s;
}

static gboolean
update(GtkWidget *widget) {
  // update tray label
  // read charge_now
  int charge_now = read_int("charge_now");
  int charge_full = read_int("charge_full");
  int charge = (int)((100.0*charge_now)/charge_full);
  char *status = read_word("status");
  //printf("charge_now=%d charge_full=%d == %d\n",charge_now, charge_full, charge);

  // show percents in label
  char *p = SCreateInt(charge,0);
  char *s = SCreateAppend(p,"%");
  if (labTray1)
    gtk_label_set_markup(labTray1,s);
  SFree(p);
  SFree(s);
  
  // change appropriate icon
  /*
  battery
  battery-caution
  battery-caution-charging
  battery-empty
  battery-full
  battery-full-charged
  battery-full-charging
  battery-good
  battery-good-charging
  battery-low
  battery-low-charging
  battery-missing  
  */

  // capacity
  char *capacity = NULL;
  if ( (charge > 90)&&(charge <= 100) ) capacity = "full";
  if ( (charge > 70)&&(charge <= 90) )  capacity = "good";
  if ( (charge > 50)&&(charge <= 70) )  capacity = "low";
  if ( (charge > 0) &&(charge <= 50) )  capacity = "empty";

  // is it charging now?  
  char *charging = NULL;
  if (SPos(status,"Charging") == 0) charging = "-charging";
  SFree(status);

  // create icon name depending on capacity and charging state
  char *icon = SCreateAppend("battery-",capacity);
  char *icon2 = SCreateAppend(icon,charging);
  SFree(icon);
  if (imgTray1)
    gtk_image_set_from_icon_name(imgTray1,icon2,GTK_ICON_SIZE_SMALL_TOOLBAR);
  SFree(icon2);

  return TRUE;
}

int main (int argc, char *argv[]) {
  batteryPath = getenv ("SYS_BATTERY_PATH");
  if (batteryPath == NULL ) {
    batteryPath = SYS_BATTERY_PATH;
  }
  // test if battery file exists
  char * tmpPath = g_strconcat(batteryPath, "charge_now", NULL);
  if (!g_file_test (tmpPath, G_FILE_TEST_EXISTS)) {
    fprintf(stderr, "%szzz\n", batteryPath);
    fprintf(stderr, "error: battery file '%s' does not exist\n"
            "       Please change SYS_BATTERY_PATH directive in config.mk and recompile it.\n"
            "       OR: set SYS_BATTERY_PATH\n",
            tmpPath);
    return 1;
  }
  g_free (tmpPath);
  tmpPath = g_strconcat(batteryPath, "charge_full", NULL);
  if (!g_file_test (tmpPath, G_FILE_TEST_EXISTS)) {
    fprintf(stderr, "error: battery file '%s' does not exist\n"
            "       Please change SYS_BATTERY_PATH directive in config.mk and recompile it.\n"
            "       OR: set SYS_BATTERY_PATH\n",
            tmpPath);
   return 2;
  }
  g_free (tmpPath);
  tmpPath = g_strconcat (batteryPath, "status", NULL);
  if (!g_file_test(tmpPath, G_FILE_TEST_EXISTS)) {
    fprintf(stderr, "error: battery file '%s' does not exist\n"
            "       Please change SYS_BATTERY_PATH directive in config.mk and recompile it.\n"
            "       OR: set SYS_BATTERY_PATH\n",
            tmpPath);
    return 3;
  }
  g_free (tmpPath);

  // initialize gtk application
  gtk_init (&argc, &argv);

  // glade builder
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "traybattery.glade", NULL);
  gtk_builder_add_from_file (builder, PREFIX"/share/jwmtools/traybattery.glade", NULL);

  // main window
  winTray1 = GTK_WIDGET (gtk_builder_get_object (builder, "winTray1"));
  gtk_builder_connect_signals (builder, NULL);          
  
  // tray components
  labTray1 = (GtkLabel*)gtk_builder_get_object(builder, "labTray1");
  imgTray1 = (GtkImage*)gtk_builder_get_object(builder, "imgTray1");

  // 1s timer for updating tray label 
  g_timeout_add(5000, (GSourceFunc) update, (gpointer) winTray1);
  //update(winTray1);

  // first update early
  update(NULL);

  // main loop
  //g_object_unref (G_OBJECT (builder));
  gtk_widget_show (winTray1);       
  gtk_main ();
  g_object_unref (builder);
  
  return 0;
}
