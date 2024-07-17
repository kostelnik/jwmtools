// Applet for changing volume with mouse wheel (pulse audio version)

#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <values.h>
#include <pulseaudio_volume.c>

GtkBuilder *builder;
GtkWidget *winTray1;
GtkImage *imgTray1;

void update_icon(long int v);
G_MODULE_EXPORT void on_eventbox1_scroll_event (GtkWidget *object, GdkEventScroll *scroll);

void update_icon(long int v) {
  // update icon depending on current volume
  gtk_image_set_from_icon_name(imgTray1,"audio-volume-muted",GTK_ICON_SIZE_SMALL_TOOLBAR);
  if (v > 10000) gtk_image_set_from_icon_name(imgTray1,"audio-volume-low",GTK_ICON_SIZE_SMALL_TOOLBAR);
  if (v > 20000) gtk_image_set_from_icon_name(imgTray1,"audio-volume-medium",GTK_ICON_SIZE_SMALL_TOOLBAR);
  if (v > 30000) gtk_image_set_from_icon_name(imgTray1,"audio-volume-high",GTK_ICON_SIZE_SMALL_TOOLBAR);
}

G_MODULE_EXPORT void
on_eventbox1_scroll_event (GtkWidget *object, GdkEventScroll *scroll) {
  // eventbox scroll event (1=down, 0=up)
  long int v = pa_get_volume();
  printf("eventbox scroll: volume=%li\n",v);
  if (scroll->direction)
    v -= 2000;
  else
    v += 2000;
  pa_set_volume(v);
  update_icon(v);
}

int main (int argc, char *argv[]) {
  fprintf (stdout, "glade file: "MAIN_GLADE_FILE"\n");
  // initialize gtk application
  gtk_init (&argc, &argv);
  fprintf (stdout, "foo\n");

  // glade builder
  builder = gtk_builder_new ();
  if (g_file_test(MAIN_GLADE_FILE, G_FILE_TEST_EXISTS)) {
    gtk_builder_add_from_file (builder, MAIN_GLADE_FILE, NULL);
  } else {
    gtk_builder_add_from_file (builder, PREFIX"/share/jwmtools/"MAIN_GLADE_FILE, NULL);
  }

  // main window
  winTray1 = GTK_WIDGET (gtk_builder_get_object (builder, "winTray1"));
  gtk_builder_connect_signals (builder, NULL);          
  
  // tray components
  imgTray1 = (GtkImage*)gtk_builder_get_object(builder, "imgTray1");
  //gtk_image_set_from_icon_name(imgClouds1,"gtk-dialog-error",64);

  // initial volume settings
  update_icon(pa_get_volume());

  // main loop
  //g_object_unref (G_OBJECT (builder));
  gtk_widget_show (winTray1);       
  gtk_main ();
  
  return 0;
}
