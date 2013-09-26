// Applet for displaying current battery state

#include <gtk/gtk.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include "s.h"
#include "s.c"
//#include <sys/types.h>
//#include <values.h>
//#include <gdk/gdkx.h>

// FIXME: automatically detect best path or at least let user decide from commandline which path is the best
//#define SYS_BATERY_PATH "/sys/bus/acpi/drivers/battery/PNP0C0A:00/power_supply/C16D/"

// Here are some sample data for debuging, uncomment whichever you want
#define SYS_BATERY_PATH "data/1/"
//#define SYS_BATERY_PATH "data/2/"
//#define SYS_BATERY_PATH "data/3/"
//#define SYS_BATERY_PATH "data/4/"

GtkBuilder *builder;
GtkWidget *winTray1;
GtkImage *imgTray1;
GtkLabel *labTray1;

int read_int(char * file);
char * read_word(char * file);

int read_int(char * file) {
  // read integer from single /sys/... file
  int i = 0;
  FILE *f;
  char *fn = SCreateAppend(SYS_BATERY_PATH,file);
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
  char *fn = SCreateAppend(SYS_BATERY_PATH,file);
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
  // test if battery file exists
  if (!g_file_test(SYS_BATERY_PATH"charge_now", G_FILE_TEST_EXISTS)) {
    fprintf(stderr, "error: battery file '%scharge_now' does not exist\n       Please change SYS_BATERY_PATH directive in traybattery.c and recompile it\n", SYS_BATERY_PATH);
    return 1;
  }  
  if (!g_file_test(SYS_BATERY_PATH"charge_full", G_FILE_TEST_EXISTS)) {
    fprintf(stderr, "error: battery file '%scharge_full' does not exist\n       Please change SYS_BATERY_PATH directive in traybattery.c and recompile it\n", SYS_BATERY_PATH);
    return 2;
  }  
  if (!g_file_test(SYS_BATERY_PATH"status", G_FILE_TEST_EXISTS)) {
    fprintf(stderr, "error: battery file '%sstatus' does not exist\n       Please change SYS_BATERY_PATH directive in traybattery.c and recompile it\n", SYS_BATERY_PATH);
    return 3;
  }  

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
  
  return 0;
}
