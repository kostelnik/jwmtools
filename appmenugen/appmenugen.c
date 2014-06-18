// Applications menu generator for JWM main menu
/*
Example:

    <!-- application menu -->
    <RootMenu height="24" onroot="6">
        <Include>exec:/opt/jwmtools/0.1/bin/appmenugen</Include>
    </RootMenu>
*/

#include <stdio.h>
#include <glib.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


// item category for apps menu
enum 
  ItemCategory {
    icOther,
    icAccessibility,
    icGames,
    icUtility,
    icScienceEducation,
    icAudioVideo,
    icOffice,
    icDevelopment,
    icInternet,
    icGraphics,
    icSettings
  };

// structure to hold menu items
typedef struct {
  enum ItemCategory category;
  char * name;
  char * exec;
  char * icon;
} Item;

Item * item_new(const enum ItemCategory category, const gchar* name, const gchar* exec, const gchar* icon);
void item_free(Item * item);
gint item_compare(gconstpointer a, gconstpointer b);
void item_print(gpointer ptr, gpointer user_data);
void jwm_menu_begin(const gchar * label, const gchar * icon);
void jwm_menu_end();
void jwm_program(gpointer ptr, gpointer user_data);
void jwm_category(GPtrArray *items, const gchar * category, int id);

Item * item_new(const enum ItemCategory category, const gchar* name, const gchar* exec, const gchar* icon) {
  // create one item
  Item *i = (Item*)malloc(sizeof(Item));
  i->category = category;
  i->name = g_strdup(name);
  i->exec = g_strdup(exec);
  i->icon = g_strdup(icon);
  return i;
}

void item_free(Item * item) {
  // release item
  g_free(item->name);
  g_free(item->exec);
  g_free(item->icon);
  g_free(item);
}

gint item_compare(gconstpointer a, gconstpointer b) {
  // sort 2 items by category and by names within same category
  Item *aa = *((Item**)a);
  Item *bb = *((Item**)b);
  // category
  int c = aa->category - bb->category;
  if (c!=0) 
    return c;
  // names - strcmp(aa->name,bb->name)
  gchar * sa = g_utf8_casefold(aa->name,-1);
  gchar * sb = g_utf8_casefold(bb->name,-1);
  c = strcmp(sa,sb);
  g_free(sa);
  g_free(sb);
  return c;
}

void item_print(gpointer ptr, gpointer user_data) {
  // print item for debuging purposes
  Item *item = (Item*)ptr;
  printf("c=%2d  n=%-20s  e=%-15s  i=%-15s ud=%d\n",
    item->category,
    item->name,
    item->exec,
    item->icon,
    (int)(long)user_data);
}

void jwm_menu_begin(const gchar * label, const gchar * icon) {
  // begin of submenu
  printf("  <Menu icon=\"%s\" label=\"%s\">\n",icon,label);
}

void jwm_menu_end() {
  // end of submenu
  printf("  </Menu>\n");
}

void jwm_program(gpointer ptr, gpointer user_data) {
  // print item for JWM menu
  Item *item = (Item*)ptr;
  if (item->category == (int)(long)user_data) {
    if (item->icon==NULL) {
      printf("item->name = %s\n",item->name);
      g_error("item->icon is null");
    }
    // FIXME: convert all .svg suffix to .png suffix because JWM does not suppoer SVG
    // FIXME: some better icon finding system
    if ( (g_str_has_suffix(item->icon,".png"))
      || (g_str_has_suffix(item->icon,".xpm"))
      || (g_str_has_suffix(item->icon,".svg"))
      || (g_str_has_suffix(item->icon,".jpg")) )
      printf("    <Program icon=\"%s\" label=\"%s\">%s</Program>\n",item->icon,item->name,item->exec);
    else {
      // test if there is xpm in pixmaps but not png (e.g. aumix has only xpm)
      char *s = (char*)malloc(sizeof(char)*1000);
      sprintf(s,"/usr/share/pixmaps/%s.xpm",item->icon);
      if (g_file_test(s,G_FILE_TEST_EXISTS))
        printf("    <Program icon=\"%s.xpm\" label=\"%s\">%s</Program>\n",item->icon,item->name,item->exec);
      else
        printf("    <Program icon=\"%s.png\" label=\"%s\">%s</Program>\n",item->icon,item->name,item->exec);
      g_free(s);
      
    }
  }
}

void jwm_category(GPtrArray *items, const gchar * category, int id) {
  // print one submenu
  jwm_menu_begin(category,"folder.png");
  g_ptr_array_foreach(items,jwm_program,(gpointer)(long)id);
  jwm_menu_end();
}

int main(int argc, char * argv[]) {
  ///g_warning("Sample warning");
  ///g_message("Loading *.desktop files");
  
  // pointer array for storing menu items
  GPtrArray *items = g_ptr_array_new ();
  
  // read all files from applications directory
  gchar *path = g_strdup("/usr/share/applications");
  GDir *d = g_dir_open(path,0,NULL);
  const gchar *n;
  while ( (n = g_dir_read_name(d)) ) {
    // get filename from dir content
    gchar *fn = g_build_filename(path,n,NULL);
    //printf("--> fn=%s\n",fn);
    // is it .desktop file
    if (!g_str_has_suffix(fn,".desktop"))
      continue;
    // load .desktop file
    GKeyFile *kf = g_key_file_new();
    gboolean b = g_key_file_load_from_file(kf, fn, G_KEY_FILE_NONE, NULL);
    g_assert(b);
    // load categories
    gsize length = 0;
    char **cats = g_key_file_get_string_list(kf,"Desktop Entry","Categories",&length,NULL);
    gchar *name = g_key_file_get_value(kf,"Desktop Entry","Name",NULL);
    gchar *exec = g_key_file_get_value(kf,"Desktop Entry","Exec",NULL);
    gchar *icon = g_key_file_get_value(kf,"Desktop Entry","Icon",NULL);
    gchar *type = g_key_file_get_value(kf,"Desktop Entry","Type",NULL);
    gboolean no_display = g_key_file_get_boolean(kf,"Desktop Entry","NoDisplay",NULL);
    // sort it to categories in application menu

    // determine best matching category
    enum ItemCategory c = icOther;
    for (guint i=0; i<length; i++) {
      //printf("  Category[%d]: %s\n",i,cats[i]);
      if (strstr("DesktopSettings, HardwareSettings, Monitor, PackageManager, Settings, System, X-GNOME-NetworkSettings, X-GNOME-PersonalSettings, X-LXDE-Settings, X-Red-Hat-Base, X-SuSE-ControlCenter-System, X-XFCE",cats[i]))
        c = icSettings;
      if (strstr("ArcadeGame, BlocksGame, BoardGame, CardGame, Game, LogicGame",cats[i]))
        c = icGames;
      if (strstr("Archiving, Compression, Core, DiscBurning, Documentation, FileManager, TerminalEmulator, Utility",cats[i]))
        c = icUtility;
      if (strstr("Astronomy, Dictionary, Education, Math, Science, Translation",cats[i]))
        c = icScienceEducation;
      if (strstr("Audio, AudioVideo, AudioVideoEditing, Mixer, Player, Recorder, Video",cats[i]))
        c = icAudioVideo;
      if (strstr("Calculator, Calendar, ContactManagement, Office, Presentation, Printing, TextEditor, Spreadsheet, WordProcessor",cats[i]))
        c = icOffice;
      if (strstr("Development, Emulator, Engineering, GTK, GUIDesigner, IDE, Qt",cats[i]))
        c = icDevelopment;
      if (strstr("Email, FileTransfer, Chat, InstantMessaging, Internet, IRCClient, Network, P2P, RemoteAccess, Security, WebBrowser",cats[i]))
        c = icInternet;
      if (strstr("Graphics, Photography, RasterGraphics, Scanning, VectorGraphics, Viewer, 2DGraphics, 3DGraphics",cats[i]))
        c = icGraphics;
    }
    
    // add items to array
    if ((!no_display)&&(name)&&(exec)&&(icon))
      g_ptr_array_add (items, item_new(c, name, exec, icon));

    // free temp vars
    g_strfreev(cats);
    g_free(name);
    g_free(exec);
    g_free(icon);
    g_free(type);
    g_key_file_free(kf);
  }
  // close dir
  g_dir_close(d);

  // sort items by categories and by name
  g_ptr_array_sort(items,&item_compare);
  
  // print what we get
  printf("<?xml version=\"1.0\"?>\n");
  printf("<JWM>\n");
  //jwm_category(items,"Accessibility",icAccessibility);
  jwm_category(items,"Audio and video",icAudioVideo);
  jwm_category(items,"Development",icDevelopment);
  jwm_category(items,"Games",icGames);
  jwm_category(items,"Graphics",icGraphics);
  jwm_category(items,"Internet",icInternet);
  jwm_category(items,"Office",icOffice);
  jwm_category(items,"Other",icOther);
  jwm_category(items,"Science and education",icScienceEducation);
  jwm_category(items,"Settings",icSettings);
  jwm_category(items,"Utility",icUtility);
  printf("</JWM>\n");
    
  ///g_error("dont know what to do now");
  return 0;
}
