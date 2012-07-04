#ifndef __GRISBI_WINDOW_H__
#define __GRISBI_WINDOW_H__

#include <gtk/gtk.h>

/*START_INCLUDE*/
#include "structures.h"
/*END_INCLUDE*/


G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define GRISBI_TYPE_WINDOW              (grisbi_window_get_type())
#define GRISBI_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj),  GRISBI_TYPE_WINDOW, GrisbiWindow))
#define GRISBI_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass),   GRISBI_TYPE_WINDOW, GrisbiWindowClass))
#define GRISBI_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj),  GRISBI_TYPE_WINDOW))
#define GRISBI_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass),  GRISBI_TYPE_WINDOW))
#define GRISBI_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj),   GRISBI_TYPE_WINDOW, GrisbiWindowClass))

/* Private structure type */
typedef struct _GrisbiWindowPrivate GrisbiWindowPrivate;


/* Main object structure */
typedef struct _GrisbiWindow GrisbiWindow;

struct _GrisbiWindow
{
    GtkWindow window;

    /*< private > */
    GrisbiWindowPrivate *priv;
};

/* Class definition */
typedef struct _GrisbiWindowClass GrisbiWindowClass;

struct _GrisbiWindowClass
{
    GtkWindowClass parent_class;
};


/* START_DECLARATION */
GType grisbi_window_get_type ( void ) G_GNUC_CONST;

GtkActionGroup *grisbi_window_get_action_group ( GrisbiWindow *window,
                        const gchar *action_group_name );
void grisbi_window_etat_mutex_lock ( void );
void grisbi_window_etat_mutex_unlock ( void );
void grisbi_window_free_priv_file ( GrisbiWindow *window );
void grisbi_window_free_list_accounts ( GrisbiWindow *window );
GtkWidget *grisbi_window_get_accueil_page ( GrisbiWindow *window );
const gchar *grisbi_window_get_filename ( GrisbiWindow *window );
const gchar *grisbi_window_get_file_title ( GrisbiWindow *window );
GtkWidget *grisbi_window_get_headings_eb ( GrisbiWindow *window );
gboolean grisbi_window_get_is_closed_account ( void );
GSList *grisbi_window_get_list_accounts ( GrisbiWindow *window );
GtkWidget *grisbi_window_get_navigation_tree_view ( void );
GtkWidget *grisbi_window_get_scheduler_calendar ( GrisbiWindow *window );
guint grisbi_window_get_sub_menu_merge_id ( GrisbiWindow *window,
                        const gchar *sub_menu );
GrisbiWindowEtat *grisbi_window_get_struct_etat ( void );
GrisbiWindowRun *grisbi_window_get_struct_run ( GrisbiWindow *window );
GtkWidget *grisbi_window_get_widget_by_name (  const gchar *name );
GtkUIManager *grisbi_window_get_ui_manager ( GrisbiWindow *window );
GtkWidget *grisbi_window_new_general_widget ( void );
void grisbi_window_headings_update_label_markup ( gchar *label_name,
                        const gchar *text,
                        gboolean escape_text );
void grisbi_window_init_struct_etat ( GrisbiWindow *window );
gboolean grisbi_window_set_active_title ( gint account_number );
gboolean grisbi_window_set_filename ( GrisbiWindow *window,
                        const gchar *filename );
gboolean grisbi_window_set_file_title ( GrisbiWindow *window,
                        const gchar *file_title );
gboolean grisbi_window_set_is_closed_account ( gint is_closed_account );
gboolean grisbi_window_set_list_accounts ( GrisbiWindow *window,
                        GSList *list_accounts );
gboolean grisbi_window_set_navigation_tree_view ( GtkWidget *navigation_tree_view  );
gboolean grisbi_window_set_scheduler_calendar ( GrisbiWindow *window,
                        GtkWidget *scheduler_calendar );
void grisbi_window_set_sub_menu_merge_id ( GrisbiWindow *window,
                        guint merge_id,
                        const gchar *sub_menu );
void grisbi_window_set_window_title ( GrisbiWindow *window,
                        const gchar *title );
void grisbi_window_statusbar_remove ( GrisbiWindow *window );
void grisbi_window_statusbar_push ( GrisbiWindow *window,
                        const gchar *msg );
/* END_DECLARATION */

G_END_DECLS

#endif  /* __GRISBI_APP_H__ */
