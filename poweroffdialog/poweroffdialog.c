// simple dialog for poweroff, reboot, logout, etc...

#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <values.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>
#include <gtk-2.0/gdk/gdkkeysyms-compat.h>

GtkBuilder *builder;
GtkWidget *window1;

// button callbacks

G_MODULE_EXPORT void on_poweroff (GtkWidget *object, gpointer user_data);
G_MODULE_EXPORT void on_restart (GtkWidget *object, gpointer user_data);
G_MODULE_EXPORT void on_lock (GtkWidget *object, gpointer user_data);
G_MODULE_EXPORT void on_switch (GtkWidget *object, gpointer user_data);
G_MODULE_EXPORT void on_logout (GtkWidget *object, gpointer user_data);
G_MODULE_EXPORT void on_goodnight (GtkWidget *object, gpointer user_data);
G_MODULE_EXPORT int on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data);

int enabled = 1;

G_MODULE_EXPORT void
on_poweroff (GtkWidget *object, gpointer user_data) {
  printf("on_poweroff\n");
  system("poweroff");
  gtk_main_quit();
}

G_MODULE_EXPORT void
on_restart (GtkWidget *object, gpointer user_data) {
  printf("on_restart\n");
  system("reboot");
  gtk_main_quit();
}

G_MODULE_EXPORT void
on_lock (GtkWidget *object, gpointer user_data) {
  // prevent button being pressed by enter when you hit enter in slock after typing password
  if (!enabled)
    return;
  printf("on_lock\n");
  gtk_widget_hide (window1);
  system("/opt/jwmtools/front/bin/slock");
  gtk_main_quit();
  enabled = 0;
}

G_MODULE_EXPORT void
on_switch (GtkWidget *object, gpointer user_data) {
  printf("on_switch\n");
  system("dm-tool switch-to-greeter");
  gtk_main_quit();
}

G_MODULE_EXPORT void
on_logout (GtkWidget *object, gpointer user_data) {
  printf("on_logout\n");
  system("jwm -exit");
  gtk_main_quit();
}

G_MODULE_EXPORT void
on_goodnight (GtkWidget *object, gpointer user_data) {
  printf("on_goodnight\n");
  system("~/bin/goodnight");
  gtk_main_quit();
}

G_MODULE_EXPORT int on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
  printf("on_key_press %d\n",event->keyval);
  if (event->keyval == GDK_Escape)
    gtk_main_quit();
  return 0;
}

int main (int argc, char *argv[]) {
  // initialize gtk application
  gtk_init (&argc, &argv);

  // glade builder
  builder = gtk_builder_new ();
  if (g_file_test(MAIN_GLADE_FILE, G_FILE_TEST_EXISTS)) {
    gtk_builder_add_from_file (builder, MAIN_GLADE_FILE, NULL);
  } else {
    gtk_builder_add_from_file (builder, PREFIX"/share/jwmtools/"MAIN_GLADE_FILE, NULL);
  }

  // main window
  window1 = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
  gtk_builder_connect_signals (builder, NULL);          

  // main loop
  //g_object_unref (G_OBJECT (builder));
  gtk_widget_show (window1);
  gtk_main ();
  return 0;
}
