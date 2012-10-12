// Applet that executes specified bash script every few seconds and shows it output

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// widgets
GtkBuilder *builder;
GtkWidget *winTray1, *winSetup1;
GtkLabel *labTray1;

// input parameters
int interval = 30;
char * name = NULL;
char * script = NULL;

G_MODULE_EXPORT void
on_winTray1_delete_event (GtkObject *object, gpointer user_data) {
  // correctly terminate application
  gtk_main_quit();
}

gboolean b = TRUE;

G_MODULE_EXPORT void
on_eventbox1_button_press_event (GtkObject *object, gpointer user_data) {
  // click on eventbox show/hide second window
  printf("eventbox1 button press\n");
  /*
  b = !b;
  if (b)
    gtk_widget_hide (winSetup1);
  else {
    gtk_widget_show (winSetup1);
    int x,y;
    gtk_window_get_position (winSetup1, &x, &y);
    if (y < 25)
      y = 25;
    gtk_window_move (winSetup1, x, y);
  }
  */
}

static gboolean
time_handler(GtkWidget *widget) {
  // periodical weather update
  printf("trayscript: update\n");

  // alocate read buffer
  size_t buffer_size = 5000;
  char * buffer = (char*)malloc(sizeof(char)*buffer_size+1);

  // execute script and show output
  FILE *f = popen(script,"r");  
  int q = getline(&buffer,&buffer_size,f);
  pclose(f);
  printf("script output: %s q=%d\n",buffer,q);
    
  // set label
  gtk_label_set_markup(labTray1,buffer);
    
  // done
  free(buffer);
  return TRUE;
}

int main (int argc, char *argv[]) {
  // parse input parameters
  int i;
  for (i=0; i<argc; i++) {
    //printf("arg[%d] = %s\n",i,argv[i]);
    if (strcmp(argv[i],"-name")==0) name = argv[i+1];
    if (strcmp(argv[i],"-script")==0) script = argv[i+1];
    if (strcmp(argv[i],"-interval")==0) sscanf(argv[i+1],"%d",&interval);
  }
  printf("name=%s\n",name);
  printf("script=%s\n",script);
  printf("interval=%d\n",interval);

  // check input parameters
  if ( (name==NULL) || (script==NULL) || (interval<=0) ) {
    //return die(1,"error: some parameters are missing");
    printf("usage: trayscript -name <UNIQUE_WINDOW_ID> -script <FILE_NAME_TO_EXECUTE> -interval <SECONDS>\n");
    printf("error: some parameters are missing\n");
    return 1;
  }
  
  // initialize gtk application
  gtk_init (&argc, &argv);

  // glade builder
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "trayscript.glade", NULL);
  gtk_builder_add_from_file (builder, PREFIX"/share/jwmtools/trayscript.glade", NULL);
  gtk_builder_connect_signals (builder, NULL);
  
  // windows
  winTray1 = GTK_WIDGET (gtk_builder_get_object (builder, "winTray1"));
  winSetup1 = GTK_WIDGET (gtk_builder_get_object (builder, "winSetup1"));

  // unique name
  gtk_window_set_title(winTray1,name);

  // widgets
  labTray1 = (GtkLabel*)gtk_builder_get_object(builder, "labTray1");
  
  // timer + its first run (every 30s)
  g_timeout_add(interval*1000, (GSourceFunc) time_handler, (gpointer) winTray1);
  time_handler(winTray1);

  // main loop
  //g_object_unref (G_OBJECT (builder));
  gtk_widget_show (winTray1);
  gtk_main ();
  
  return 0;
}
