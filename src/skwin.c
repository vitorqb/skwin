#include <stdio.h>
#include <gtk/gtk.h>

#define BUF_SIZE 1024

static char * read_from_stdin()
{
  char buffer[BUF_SIZE];
  size_t contentSize = 1;
  char *content = malloc(sizeof(char) * BUF_SIZE);

  if (content == NULL) {
    perror("Failed to allocate content");
    exit(1);
  }

  content[0] = '\0';

  while (fgets(buffer, BUF_SIZE, stdin)) {
    char *old = content;
    contentSize += strlen(buffer);
    content = realloc(content, contentSize);

    if (content == NULL) {
      perror("Failed to reallocate content");
      free(old);
      exit(1);
    }

    strcat(content, buffer);
  }

  return content;
}

static void set_can_focus_false(GObject* obj)
{
  GValue val = G_VALUE_INIT;
  g_value_init(&val, G_TYPE_BOOLEAN);
  g_value_set_boolean(&val, FALSE);
  g_object_set_property(obj, "can-focus", &val);
}

static gboolean key_press(GtkWidget *widget, GdkEventKey *event)
{
  g_print("%c", event->keyval);
  fflush(stdout);
  gtk_widget_destroy(GTK_WIDGET(widget));
  return FALSE;
}

static void activate (GtkApplication* app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *view;
  GtkTextBuffer *buffer;

  /* text view and buffer */
  view = gtk_text_view_new();
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  gtk_text_buffer_set_text(buffer, read_from_stdin(), -1);
  set_can_focus_false(G_OBJECT(view));

  /* Window */
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DIALOG);

  /* Adds */
  gtk_container_add(GTK_CONTAINER(window), view);

  gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
  g_signal_connect(GTK_WINDOW(window), "key_press_event", G_CALLBACK(key_press), NULL);
  gtk_widget_show_all(window);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("vitorqb.skwin", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
