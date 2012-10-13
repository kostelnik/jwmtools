// Applet for displaying system load information (cpu, memory, disk, net)

#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <values.h>
#include <sys/stat.h>
#include <assert.h>
#include <glib.h>

#define HEIGHT 19

GtkBuilder *builder;
GtkWidget *winTray1;
GtkDrawingArea *d_cpu, *d_load, *d_mem, *d_swap, *d_net, *d_disk;
GArray *a_cpu, *a_load, *a_mem, *a_swap, *a_net, *a_disk;

unsigned long long cpu_idle, cpu_idle_old;
int cores;
int load_1, load_5, load_15;
int mem_used, mem_cached, mem_swap;
unsigned long long net_recv, net_tran, net_recv_old, net_tran_old;
unsigned long long disk, disk_old;
int ccc = 0;

void draw_monitor(GtkDrawingArea *d, GArray *a, gfloat red, gfloat green, gfloat blue);

G_MODULE_EXPORT gboolean
expose_event_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data);

G_MODULE_EXPORT gboolean
expose_event_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data) {
  // redraw when drawing area is exposed
  draw_monitor(d_cpu, a_cpu, 1,  1,  0);
  draw_monitor(d_load,a_load,0,  1,  0);
  draw_monitor(d_mem, a_mem, 0.5,0.5,1);
  draw_monitor(d_swap,a_swap,1,  0,  1);
  draw_monitor(d_net, a_net, 0,  1,  1);
  draw_monitor(d_disk,a_disk,1,  0,  0);
  return TRUE;
}

void draw_monitor(GtkDrawingArea *d, GArray *a, gfloat red, gfloat green, gfloat blue) {
  // draw single monitor
  int i,y;
  
  // cairo context
  cairo_t *cr = gdk_cairo_create(GTK_WIDGET(d)->window);  
  
  // clear background with black color
  cairo_set_source_rgb(cr,0,0,0);
  cairo_rectangle(cr,0,0,HEIGHT,HEIGHT);
  cairo_fill(cr);
  
  // draw all lines from array
  cairo_set_source_rgb(cr,red,green,blue);
  cairo_set_line_width (cr, 1.0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_SQUARE);
  for (i=0; i<a->len; i++) {
    y = HEIGHT*g_array_index(a,guint,i)/100;
    cairo_move_to (cr, i+0.5, HEIGHT);
    cairo_line_to (cr, i+0.5, HEIGHT-y+0.5);
  }
  cairo_stroke (cr);
  
  // release cairo context
  cairo_destroy(cr);
}

static gboolean
update(GtkWidget *widget) {
  // get sysmonitor data and update drawing areas
  //printf("update: \n");

  // execute backend sysmonitor
  FILE *f;
  if (g_file_test(PREFIX"/bin/sysmonitor",G_FILE_TEST_EXISTS))
    f = popen(PREFIX"/bin/sysmonitor","r");
  if (g_file_test("../sysmonitor/sysmonitor",G_FILE_TEST_EXISTS))
    f = popen("../sysmonitor/sysmonitor","r");
  assert(f);
    
  if (f) {
    // parse sysmonitor output
    // E.g.:  cpu 1818274 2 load 0.35 0.48 0.54 mem 54 34 0 net 233514297 7743205 disk 99736
    fscanf(f,"cpu %Ld %d load %d %d %d mem %d %d %d net %Ld %Ld disk %Ld\n",
      &cpu_idle, &cores,
      &load_1, &load_5, &load_15,
      &mem_used, &mem_cached, &mem_swap,
      &net_recv, &net_tran,
      &disk);
    fclose(f);
    //printf("raw: cpu=%Ld  cores=%d  l1=%d  l5=%d  l15=%d\n",cpu_idle,cores,load_1,load_5,load_15);
    // percent calculations
    int per_cpu  = 100-(cpu_idle-cpu_idle_old)/cores;
    int per_load = load_1;
    int per_net  = ((net_recv+net_tran)-(net_recv_old+net_tran_old))/1000;
    int per_disk = (disk-disk_old)/10;
    //printf("cpu=%d %%, load=%d %%, net=%d %%, disk=%d %%\n",per_cpu,per_load,per_net,per_disk);
    // push new values to arrays
    g_array_prepend_val(a_cpu,per_cpu);   g_array_set_size(a_cpu,22);
    g_array_prepend_val(a_load,per_load); g_array_set_size(a_load,22);
    g_array_prepend_val(a_mem,mem_used);  g_array_set_size(a_mem,22);
    g_array_prepend_val(a_swap,mem_swap); g_array_set_size(a_swap,22);
    g_array_prepend_val(a_net,per_net);   g_array_set_size(a_net,22);
    g_array_prepend_val(a_disk,per_disk); g_array_set_size(a_disk,22);
    // remember old values (only where required)
    cpu_idle_old = cpu_idle;
    net_recv_old = net_recv;
    net_tran_old = net_tran;
    disk_old = disk;
    // redraw all drawing areas
    draw_monitor(d_cpu, a_cpu, 1,  1,  0);
    draw_monitor(d_load,a_load,0,  1,  0);
    draw_monitor(d_mem, a_mem, 0.5,0.5,1);
    draw_monitor(d_swap,a_swap,1,  0,  1);
    draw_monitor(d_net, a_net, 0,  1,  1);
    draw_monitor(d_disk,a_disk,1,  0,  0);
  }
  return TRUE;
}

int main (int argc, char *argv[]) {
  // initialize gtk application
  gtk_init (&argc, &argv);

  // glade builder
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "traymonitor.glade", NULL);
  gtk_builder_add_from_file (builder, PREFIX"/share/jwmtools/traymonitor.glade", NULL);

  // main window
  winTray1 = GTK_WIDGET (gtk_builder_get_object (builder, "winTray1"));
  gtk_builder_connect_signals (builder, NULL);

  // drawing components
  d_cpu  = (GtkDrawingArea*) gtk_builder_get_object (builder, "drawingarea1");
  d_load = (GtkDrawingArea*) gtk_builder_get_object (builder, "drawingarea2");
  d_mem  = (GtkDrawingArea*) gtk_builder_get_object (builder, "drawingarea3");
  d_swap = (GtkDrawingArea*) gtk_builder_get_object (builder, "drawingarea4");
  d_net  = (GtkDrawingArea*) gtk_builder_get_object (builder, "drawingarea5");
  d_disk = (GtkDrawingArea*) gtk_builder_get_object (builder, "drawingarea6");
  
  // set tooltips
  gtk_widget_set_tooltip_text((GtkWidget*)d_cpu, "CPU");
  gtk_widget_set_tooltip_text((GtkWidget*)d_load,"Load");
  gtk_widget_set_tooltip_text((GtkWidget*)d_mem, "Memory");
  gtk_widget_set_tooltip_text((GtkWidget*)d_swap,"Swap");
  gtk_widget_set_tooltip_text((GtkWidget*)d_net, "Network");
  gtk_widget_set_tooltip_text((GtkWidget*)d_disk,"Disk");
  
  // arrays for storing recent monitored values
  a_cpu = g_array_new(FALSE,TRUE,sizeof(guint));
  a_load = g_array_new(FALSE,TRUE,sizeof(guint));
  a_mem = g_array_new(FALSE,TRUE,sizeof(guint));
  a_swap = g_array_new(FALSE,TRUE,sizeof(guint));
  a_net = g_array_new(FALSE,TRUE,sizeof(guint));
  a_disk = g_array_new(FALSE,TRUE,sizeof(guint)); 
  
  // update every second
  g_timeout_add(1000, (GSourceFunc) update, (gpointer) winTray1);
  
  // main loop
  //g_object_unref (G_OBJECT (builder));
  gtk_widget_show (winTray1);
  gtk_main ();
  
  return 0;
}
