// Applet for displaying current keyboard layout

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


GtkBuilder *builder;
GtkWidget *winTray1;
GtkImage *imgTray1;
GtkLabel *labTray1;

static gboolean
update(GtkWidget *widget) {
  // update tray label
  //printf("update: \n");
  
  // in GDK there is function gdk_keymap_get_caps_lock_state, but not
  // gdk_keymap_get_scroll_lock_state since they decided nobody should 
  // use scroll lock (which in fact is another hack to detect current
  // keyboard layout which was original intention, but there is no way
  // to (normally) detect it either)
  GdkDisplay *dpy = gdk_display_get_default();
  unsigned int indicators = 0;
  XkbGetIndicatorState(GDK_DISPLAY_XDISPLAY(dpy),XkbUseCoreKbd,&indicators);
  //printf("  ind=%d\n",indicators);

  if (indicators < 4000)
    gtk_label_set_markup(labTray1,"US");
  else
    gtk_label_set_markup(labTray1,"SK");
  
  return TRUE;
}

int main (int argc, char *argv[]) {
  // initialize gtk application
  gtk_init (&argc, &argv);

  // glade builder
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "traykeymap.glade", NULL);
  gtk_builder_add_from_file (builder, PREFIX"/share/jwmtools/traykeymap.glade", NULL);

  // main window
  winTray1 = GTK_WIDGET (gtk_builder_get_object (builder, "winTray1"));
  gtk_builder_connect_signals (builder, NULL);          
  
  // tray components
  labTray1 = (GtkLabel*)gtk_builder_get_object(builder, "labTray1");
  imgTray1 = (GtkImage*)gtk_builder_get_object(builder, "imgTray1");

  // 1s timer for updating tray label with keyboard layout
  g_timeout_add(1000, (GSourceFunc) update, (gpointer) winTray1);
  //update(winTray1);

  // main loop
  //g_object_unref (G_OBJECT (builder));
  gtk_widget_show (winTray1);       
  gtk_main ();
  
  return 0;
}
