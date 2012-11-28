// Applet for mounting and unmounting flash drives without dbus or udev
/*
It require pmount, pumount, blkid.
This applet is NOT for mounting CDROMs.
This applet only shows devices that have label (use e2label for ext filesystem, mlabel for vfat)
FIXME: it behave bit strange when user do not safely umount vfat usb key. media dirs stays there but empty and I cannot mount again or something like that
FIXME: Currently it is limited to 10 devices
FIXME: It reads content of /proc/mounts every 5s, perhaps there is other way, but /proc is not real files so maybe I shouldnt bother
TODO: Check exit code from system('umount ...') and if it is nonzero, change tray_label to something or show error message!!!
      Unmount failed: Cannot unmount because file system on device is busy
*/

#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <values.h>
#include "blkid.c"
#include <scandirstr.h>
#include <scandirstr.c>
//#include "../trayumount/proc_mounts.c"

GtkBuilder *builder;
GtkWidget *winTray1;
GtkWidget *winMedia1;
GtkWidget *labTray1;
GtkWidget *labInfo1;
GtkImage *imgTray1;
GtkButton * button[10];

void update_icon(void);
G_MODULE_EXPORT void on_eventbox1_scroll_event (GtkObject *object, GdkEventScroll *scroll);
G_MODULE_EXPORT void on_button_clicked (GtkObject *object, gpointer user_data);
G_MODULE_EXPORT void show_winMedia1 (GtkObject *object);
void update_media_button(GtkButton *button, char **captions, int index, int count);
int get_button_index(GtkObject *object);

void update_media_button(GtkButton *button, char **captions, int index, int count) {
  // set caption for single button and make it visible
  if (index < count)
    gtk_button_set_label(button,captions[index]);
  gtk_widget_set_visible((GtkWidget*)button,index < count);
}

char * sys_block_dir = NULL;
char * proc_mounts_str = NULL;

void update_icon(void) {
  // update icon depending on number of mounted media devices
    
  int i;
  // set button labels
  // FIXME: make it for more than 10 buttons and more universally
 
  // only if /sys/block changed since last time
  int a = scandirstrchanged("/sys/block",&sys_block_dir);
  int b = proc_mounts_changed(&proc_mounts_str);
  
  if ( (a == 1)||(b == 1) ) {
    //printf("/sys/block or /proc/mounts changed...\n");
    blkid_parse();
    //blkid_debug();

    for (i=0; i<10; i++) {
      // change label depending of if device is mounted or not  
      char *poms = (char*)malloc(sizeof(char)*300);
      if (blkid_mounted[i])
        sprintf(poms,"Odpojiť %s",blkid_labels[i]);
      else
        sprintf(poms,"Pripojiť %s",blkid_labels[i]);
      gtk_button_set_label(button[i],poms);
      free(poms);
    
      // hide unused labels
      gtk_widget_set_visible((GtkWidget*)button[i],strlen(blkid_labels[i])>0);
    }

    // display number of mounted devices
    // FIXME: i don't need blkid fo this, only use /proc/mounts and when user click on trayicon only then parse blkid
    char *cnt = (char*)malloc(sizeof(char)*30);
    int c = blkid_mounted_count();
    sprintf(cnt,"%dx",c);
    gtk_label_set_markup((GtkLabel*)labTray1,cnt);
    free(cnt);
    
    // change info label if there are no devices
    if (c == 0)
      gtk_label_set_markup((GtkLabel*)labInfo1,"Nie je nič pripojené");
    else
      gtk_label_set_markup((GtkLabel*)labInfo1,"Pripojiť alebo odpojiť?");
     
  } else {
    // printf("/sys/block NOT changed\n");
  }

}

G_MODULE_EXPORT void 
show_winMedia1 (GtkObject *object) {

  int ww, hh;

  // make sure it is up to date
  update_icon();
  
  if (gtk_widget_get_visible(winMedia1)) {
    // hide winMedia1 if it is visible
    gtk_widget_hide(winMedia1);
  } else {
    printf("blkcnt=%d height=%d\n",blkid_labels_count,blkid_labels_count*22+22);
/*    gtk_widget_set_size_request((GtkWidget*)button[0],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[1],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[2],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[3],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[4],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[5],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[6],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[7],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[8],-1,22);
    gtk_widget_set_size_request((GtkWidget*)button[9],-1,22);*/
//    gtk_widget_set_size_request((GtkWidget*)winMedia1,-1,blkid_labels_count*22+22); // buttons * height + label
    gtk_window_get_size((GtkWindow*)winMedia1,&ww,&hh);
    gtk_window_resize((GtkWindow*)winMedia1,ww,blkid_labels_count*22+16);
    // show winMedia1
    gtk_widget_show(winMedia1);
    // move it little bit lower to prevent obscuring top panel by window itself
    gint x,y;
    gtk_window_get_position((GtkWindow*)winMedia1,&x,&y);
    printf("winMedia1: x=%d y=%d\n",x,y);
    if (y < 24) y = 24;
    gtk_widget_set_uposition((GtkWidget*)winMedia1,x,y);
  }
}

G_MODULE_EXPORT void
on_eventbox1_scroll_event (GtkObject *object, GdkEventScroll *scroll) {
  // eventbox scroll event (1=down, 0=up)
  long int v = 10;
  //long int v = pa_get_volume();
  printf("eventbox scroll: volume=%li\n",v);
  if (scroll->direction)
    v -= 2000;
  else
    v += 2000;
  //pa_set_volume(v);
  update_icon();
}

int get_button_index(GtkObject *object) {
  // return which button did we clicked (lazarus equivalent: TButton(Sender).Tag)
  int i;
  for (i=0;i<10;i++)
    if (button[i]==(GtkButton*)object)
      return i;
  return -1;
}

G_MODULE_EXPORT void
on_button_clicked (GtkObject *object, gpointer user_data) {
  // click on umount button
  int button_index = get_button_index(object);
  printf("Click on button, button_index=%d\n",button_index);

  // get button label  
  gchar *label = (gchar*)gtk_button_get_label((GtkButton*)object);
  printf("  LABEL='%s' sensitive=%d\n",label,gtk_widget_get_sensitive((GtkWidget*)object));

  // guess mount directory
  char *cmd = (char*)malloc(sizeof(char)*256);
  int a = strpos(label,' ');
  
  // if it is mounted, umount it, if not mount it
  if (blkid_mounted[button_index]==0)
    sprintf(cmd,"pmount %s %s",blkid_devices[button_index],blkid_labels[button_index]);
  else
    sprintf(cmd,"pumount %s",blkid_devices[button_index]);

  // do actual umount depending on button clicked (e.g. caption or something)
  if (a > 0) {
    printf("cmd='%s'\n",cmd);
    system(cmd);
  }
  
  // hide winMedia1
  gtk_widget_hide(winMedia1);
  
  // refresh icon
  update_icon();
}

static gboolean
update_timer(GtkWidget *widget) {
  // periodically update icon using 5s timer
  update_icon();
  return TRUE;
}

int main (int argc, char *argv[]) {

  // initialize gtk application
  gtk_init (&argc, &argv);
  
  // initialize blkid parse
  blkid_allocate();

  // glade builder
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "trayusermount.glade", NULL);
  gtk_builder_add_from_file (builder, PREFIX"/share/jwmtools/trayusermount.glade", NULL);

  // main window
  winTray1 = GTK_WIDGET (gtk_builder_get_object (builder, "winTray1"));
  gtk_builder_connect_signals (builder, NULL);

  // popup window with media buttons
  winMedia1 = GTK_WIDGET (gtk_builder_get_object (builder, "winMedia1"));
  labInfo1 = (GtkWidget*)gtk_builder_get_object(builder, "labInfo1");
  
  // tray components
  imgTray1 = (GtkImage*)gtk_builder_get_object(builder, "imgTray1");
  labTray1 = (GtkWidget*)gtk_builder_get_object(builder, "labTray1");
  //gtk_image_set_from_icon_name(imgClouds1,"gtk-dialog-error",64);
  
  // media buttons
  // FIXME: do it universal for any number of buttons, e.g. generate those buttons on the fly
  button[0] = (GtkButton*)gtk_builder_get_object(builder, "button0");
  button[1] = (GtkButton*)gtk_builder_get_object(builder, "button1");
  button[2] = (GtkButton*)gtk_builder_get_object(builder, "button2");
  button[3] = (GtkButton*)gtk_builder_get_object(builder, "button3");
  button[4] = (GtkButton*)gtk_builder_get_object(builder, "button4");
  button[5] = (GtkButton*)gtk_builder_get_object(builder, "button5");
  button[6] = (GtkButton*)gtk_builder_get_object(builder, "button6");
  button[7] = (GtkButton*)gtk_builder_get_object(builder, "button7");
  button[8] = (GtkButton*)gtk_builder_get_object(builder, "button8");
  button[9] = (GtkButton*)gtk_builder_get_object(builder, "button9");
  
  // make buttons left aligned
  gtk_button_set_alignment(button[0], 0.0, 0.5);
  gtk_button_set_alignment(button[1], 0.0, 0.5);
  gtk_button_set_alignment(button[2], 0.0, 0.5);
  gtk_button_set_alignment(button[3], 0.0, 0.5);
  gtk_button_set_alignment(button[4], 0.0, 0.5);
  gtk_button_set_alignment(button[5], 0.0, 0.5);
  gtk_button_set_alignment(button[6], 0.0, 0.5);
  gtk_button_set_alignment(button[7], 0.0, 0.5);
  gtk_button_set_alignment(button[8], 0.0, 0.5);
  gtk_button_set_alignment(button[9], 0.0, 0.5);

  // initial icon settings
  update_icon();
  
  // 5s timer for updating tray icon
  g_timeout_add(5000, (GSourceFunc) update_timer, (gpointer) winTray1);

  // main loop
  //g_object_unref (G_OBJECT (builder));
  gtk_widget_show (winTray1);
  gtk_main ();

  // release blkid parser
  blkid_free();
  
  return 0;
}
