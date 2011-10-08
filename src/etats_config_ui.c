/* ************************************************************************** */
/*                                                                            */
/*     Copyright (C)    2000-2008 Cédric Auger (cedric@grisbi.org)            */
/*                      2003-2008 Benjamin Drieu (bdrieu@april.org)           */
/*          2008-2011 Pierre Biava (grisbi@pierre.biava.name)                 */
/*          http://www.grisbi.org                                             */
/*                                                                            */
/*  This program is free software; you can redistribute it and/or modify      */
/*  it under the terms of the GNU General Public License as published by      */
/*  the Free Software Foundation; either version 2 of the License, or         */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program; if not, write to the Free Software               */
/*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                            */
/* ************************************************************************** */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>
#include <gtk/gtk.h>


/*START_INCLUDE*/
#include "etats_config_ui.h"
/* #include "dialog.h"  */
/* #include "etats_calculs.h"  */
/* #include "etats_config.h"  */
/* #include "fenetre_principale.h"  */
#include "gsb_calendar_entry.h"
/* #include "gsb_data_account.h"  */
/* #include "gsb_data_budget.h"  */
/* #include "gsb_data_category.h"  */
/* #include "gsb_data_fyear.h"  */
/* #include "gsb_data_payee.h"  */
/* #include "gsb_data_payment.h"  */
#include "gsb_etats_config.h"
/* #include "navigation.h"  */
#include "structures.h"
#include "utils.h"
#include "utils_buttons.h"
#include "utils_gtkbuilder.h"
/* #include "utils_str.h"  */
#include "erreur.h"
/*END_INCLUDE*/


/*START_STATIC*/
static GtkWidget *etats_config_ui_button_init_button_select_unselect_all ( gchar *name,
                        GtkWidget *tree_view );

static void etats_config_ui_left_panel_add_line ( GtkTreeStore *tree_model,
                        GtkTreeIter *iter,
                        GtkWidget *notebook,
                        GtkWidget *child,
                        const gchar *title,
                        gint page );
GtkWidget *etats_config_ui_left_panel_create_tree_view ( void );
static void etats_config_ui_left_panel_populate_tree_model ( GtkTreeStore *tree_model,
                        GtkWidget *notebook );
static gboolean etats_config_ui_left_panel_tree_view_selectable_func (GtkTreeSelection *selection,
                        GtkTreeModel *model,
                        GtkTreePath *path,
                        gboolean path_currently_selected,
                        gpointer data);
static gboolean etats_config_ui_left_panel_tree_view_selection_changed ( GtkTreeSelection *selection,
                        gpointer data );

static void etats_config_ui_onglet_comptes_init_buttons_choix_utilisation_virements ( void );
static void etats_config_ui_onglet_comptes_init_buttons_selection ( gchar *name,
                        GtkWidget *tree_view );
static void etats_config_ui_onglet_comptes_select_unselect ( GtkToggleButton *togglebutton,
                        GtkWidget *tree_view );
static GtkWidget *etats_config_ui_onglet_periode_create_page ( void );
static GtkTreeModel *etats_config_ui_onglet_periode_get_liste_dates ( void );
static GtkWidget *etats_config_ui_onglet_virements_create_page ( void );

static void etats_config_ui_tree_view_init ( const gchar *treeview_name,
                        GtkTreeModel *(*function) ( void ),
                        GtkSelectionMode type_selection,
                        GCallback selection_callback );
static GtkWidget *etats_config_ui_tree_view_new_with_model ( const gchar *treeview_name,
                        GtkTreeModel *model );
static gboolean etats_config_ui_onglet_periode_selection_dates_changed ( GtkTreeSelection *selection,
                        GtkWidget *widget );

/*END_STATIC*/


/*START_EXTERN*/
/*END_EXTERN*/


/*START_GLOBAL_VARIABLES*/
/* definition of the columns of model for the left panel  */
enum left_panel_tree_columns
{
    LEFT_PANEL_TREE_TEXT_COLUMN,
    LEFT_PANEL_TREE_PAGE_COLUMN,
    LEFT_PANEL_TREE_BOLD_COLUMN,
    LEFT_PANEL_TREE_ITALIC_COLUMN,
    LEFT_PANEL_TREE_NUM_COLUMNS,
};

/* liste des plages de date possibles */
static gchar *etats_config_liste_plages_dates[] =
{
    N_("All"),
    N_("Custom"),
    N_("Total to now"),
    N_("Current month"),
    N_("Current year"),
    N_("Current month to now"),
    N_("Current year to now"),
    N_("Previous month"),
    N_("Previous year"),
    N_("Last 30 days"),
    N_("Last 3 months"),
    N_("Last 6 months"),
    N_("Last 12 months"),
    NULL };


/* builder */
static GtkBuilder *etat_config_builder = NULL;

/* mémorisation du togglebutton utilisé servira à remplacer son label */
GtkToggleButton *prev_togglebutton = NULL;

/*END_GLOBAL_VARIABLES*/


/*START_PUBLIC_FUNCTIONS*/
/**
 * Création de la fenêtre de dialog
 *
 * \param
 *
 * \return dialog widget
 */
GtkWidget *etats_config_ui_create_dialog ( void )
{
    GtkWidget *dialog = NULL;
    GtkWidget *tree_view;

    devel_debug (NULL);

    /* Creation d'un nouveau GtkBuilder */
    etat_config_builder = gtk_builder_new ( );

    if ( etat_config_builder == NULL )
        return NULL;

    /* Chargement du XML dans etat_config_builder */
    if ( !utils_gtkbuilder_merge_ui_data_in_builder ( etat_config_builder, "etats_config.ui" ) )
        return NULL;

    /* Recuparation d'un pointeur sur la fenetre. */
    dialog = GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, "config_etats_dialog" ) );
    gtk_window_set_transient_for ( GTK_WINDOW ( dialog ), GTK_WINDOW ( run.window ) );

    /* Recupération d'un pointeur sur le gtk_tree_view. */
    tree_view = etats_config_ui_left_panel_create_tree_view ( );

    /* return widget */
    return dialog;
}


/**
 * free the gtk_builder
 *
 *
 *
 * */
void etats_config_ui_free_builder ( void )
{
    g_object_unref ( G_OBJECT ( etat_config_builder ) );
}


/*END_PUBLIC_FUNCTIONS*/


/*START_PRIVATE_FUNCTIONS*/
/*LEFT_PANEL*/
/**
 * création du tree_view qui liste les onglets de la fenêtre de dialogue
 *
 *
 *\return tree_view or NULL;
 * */
GtkWidget *etats_config_ui_left_panel_create_tree_view ( void )
{
    GtkWidget *tree_view = NULL;
    GtkWidget *notebook;
    GtkTreeStore *model = NULL;
    GtkTreeViewColumn *column;
    GtkCellRenderer *cell;
    GtkTreeSelection *selection;

    devel_debug (NULL);

    /* Création du model */
    model = gtk_tree_store_new ( LEFT_PANEL_TREE_NUM_COLUMNS,
                        G_TYPE_STRING,  /* LEFT_PANEL_TREE_TEXT_COLUMN */
                        G_TYPE_INT,     /* LEFT_PANEL_TREE_PAGE_COLUMN */
                        G_TYPE_INT,     /* LEFT_PANEL_TREE_BOLD_COLUMN */
                        G_TYPE_INT );   /* LEFT_PANEL_TREE_ITALIC_COLUMN */

    /* Create container + TreeView */
    tree_view = GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, "treeview_left_panel" ) );
    gtk_tree_view_set_model ( GTK_TREE_VIEW ( tree_view ), GTK_TREE_MODEL ( model ) );
    g_object_unref ( G_OBJECT ( model ) );

    /* set the color of selected row */
    utils_set_tree_view_selection_and_text_color ( tree_view );

    /* make column */
    cell = gtk_cell_renderer_text_new ( );
    column = gtk_tree_view_column_new_with_attributes ( "Categories",
                        cell,
                        "text", LEFT_PANEL_TREE_TEXT_COLUMN,
                        "weight", LEFT_PANEL_TREE_BOLD_COLUMN,
                        "style", LEFT_PANEL_TREE_ITALIC_COLUMN,
                        NULL );
    gtk_tree_view_column_set_sizing ( GTK_TREE_VIEW_COLUMN ( column ), GTK_TREE_VIEW_COLUMN_FIXED );
    gtk_tree_view_append_column ( GTK_TREE_VIEW ( tree_view ), GTK_TREE_VIEW_COLUMN ( column ) );

    /* Handle select */
    selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
    g_signal_connect ( selection,
                        "changed",
                        G_CALLBACK ( etats_config_ui_left_panel_tree_view_selection_changed ),
                        NULL );

    /* Choose which entries will be selectable */
    gtk_tree_selection_set_select_function ( selection,
                        etats_config_ui_left_panel_tree_view_selectable_func, NULL, NULL );

    /* expand all rows after the treeview widget has been realized */
    g_signal_connect ( tree_view,
                        "realize",
                        G_CALLBACK ( utils_tree_view_set_expand_all_and_select_path_realize ),
                        "0:0" );

    /* initialisation du notebook pour les pages de la configuration */
    notebook = GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, "notebook_config_etat" ) );
    gtk_notebook_set_show_tabs ( GTK_NOTEBOOK ( notebook ), FALSE );
    gtk_notebook_set_show_border ( GTK_NOTEBOOK ( notebook ), FALSE );
    gtk_container_set_border_width ( GTK_CONTAINER ( notebook ), 0 );

    /* remplissage du paned gauche */
    etats_config_ui_left_panel_populate_tree_model ( model, notebook );

    /* show all widgets */
    gtk_widget_show_all ( tree_view );

    return tree_view;
}


/**
 * remplit le model pour la configuration des états
 *
 * \param
 * \param
 *
 * \return
 * */
void etats_config_ui_left_panel_populate_tree_model ( GtkTreeStore *tree_model,
                        GtkWidget *notebook )
{
    GtkWidget *widget;
    GtkTreeIter iter;
    gint page = 0;

    /* append group page */
    etats_config_ui_left_panel_add_line ( tree_model, &iter, NULL, NULL, _("Data selection"), -1 );

    /* append page Dates */
    widget = etats_config_ui_onglet_periode_create_page ( );
    etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Dates"), page );
    page++;

    /* append page Transferts */
    widget = etats_config_ui_onglet_virements_create_page ( );
    etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Transfers"), page );
    page++;

    /* append page Accounts */
/*     widget = gsb_etats_config_onglet_etat_comptes ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Accounts"), page );
 *     page++;
 */

    /* append page Payee */
/*     widget = gsb_etats_config_onglet_etat_tiers ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Payee"), page );
 *     page++;
 */

    /* append page Categories */
/*     widget = gsb_etats_config_onglet_etat_categories ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Categories"), page );
 *     page++;
 */

    /* append page Budgetary lines */
/*     widget = gsb_etats_config_onglet_etat_ib ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Budgetary lines"), page );
 *     page++;
 */

    /* append page Texts */
/*     widget = gsb_etats_config_onglet_etat_texte ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Texts"), page );
 *     page++;
 */

    /* append page Amounts */
/*     widget = gsb_etats_config_onglet_etat_montant ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Amounts"), page );
 *     page++;
 */

    /* append page Payment methods */
/*     widget = gsb_etats_config_onglet_etat_mode_paiement ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Payment methods"), page );
 *     page++;
 */

    /* append page Misc. */
/*     widget = gsb_etats_config_onglet_etat_divers ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Miscellaneous"), page );
 *     page++;
 */

    /* remplissage de l'onglet d'organisation */
    etats_config_ui_left_panel_add_line ( tree_model, &iter, NULL, NULL, _("Data organization"), -1 );

    /* Data grouping */
/*     widget = gsb_etats_config_page_data_grouping ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Data grouping"), page );
 *     page++;
 */

    /* Data separation */
/*     widget = gsb_etats_config_page_data_separation ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Data separation"), page );
 *     page++;
 */

    /* remplissage de l'onglet d'affichage */
    etats_config_ui_left_panel_add_line ( tree_model, &iter, NULL, NULL, _("Data display"), -1 );

    /* append page Generalities */
/*     widget = gsb_etats_config_affichage_etat_generalites ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Generalities"), page );
 *     page++;
 */

    /* append page Titles */
/*     widget = gsb_etats_config_affichage_etat_titres ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Titles"), page );
 *     page++;
 */

    /* append page Transactions */
/*     widget = gsb_etats_config_affichage_etat_operations ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Transactions"), page );
 *     page++;
 */

    /* append page Currencies */
/*     widget = gsb_etats_config_affichage_etat_devises ( );
 *     etats_config_ui_left_panel_add_line ( tree_model, &iter, notebook, widget, _("Currencies"), page );
 */

    /* fin de fonction */
}


/**
 *
 *
 * \param
 *
 * \return
 */
gboolean etats_config_ui_left_panel_tree_view_selection_changed ( GtkTreeSelection *selection,
                        gpointer data )
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gint selected;

    if (! gtk_tree_selection_get_selected ( selection, &model, &iter ) )
        return(FALSE);

    gtk_tree_model_get ( model, &iter, 1, &selected, -1 );

    gtk_notebook_set_current_page ( GTK_NOTEBOOK (
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder, "notebook_config_etat", NULL ) ),
                        selected );

    /* return */
    return FALSE;
}


/**
 *
 *
 * \param
 *
 * \return
 */
gboolean etats_config_ui_left_panel_tree_view_selectable_func (GtkTreeSelection *selection,
                        GtkTreeModel *model,
                        GtkTreePath *path,
                        gboolean path_currently_selected,
                        gpointer data )
{
    GtkTreeIter iter;
    gint selectable;

    gtk_tree_model_get_iter ( model, &iter, path );
    gtk_tree_model_get ( model, &iter, 1, &selectable, -1 );

    return ( selectable != -1 );
}


/**
 * ajoute une ligne dans le tree_model du panel de gauche
 *
 *
 *
 * */
void etats_config_ui_left_panel_add_line ( GtkTreeStore *tree_model,
                        GtkTreeIter *iter,
                        GtkWidget *notebook,
                        GtkWidget *child,
                        const gchar *title,
                        gint page )
{
    GtkTreeIter iter2;

    if ( page == -1 )
    {
        /* append page groupe */
        gtk_tree_store_append ( GTK_TREE_STORE ( tree_model ), iter, NULL );
        gtk_tree_store_set (GTK_TREE_STORE ( tree_model ), iter,
                        LEFT_PANEL_TREE_TEXT_COLUMN, title,
                        LEFT_PANEL_TREE_PAGE_COLUMN, -1,
                        LEFT_PANEL_TREE_BOLD_COLUMN, 800,
                        -1 );
    }
    else
    {
        /* append page onglet*/
        gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook ),
                        child,
                        gtk_label_new ( title ) );

        gtk_tree_store_append (GTK_TREE_STORE ( tree_model ), &iter2, iter );
        gtk_tree_store_set (GTK_TREE_STORE ( tree_model ), &iter2,
                        LEFT_PANEL_TREE_TEXT_COLUMN, title,
                        LEFT_PANEL_TREE_PAGE_COLUMN, page,
                        LEFT_PANEL_TREE_BOLD_COLUMN, 400,
                        -1);
    }
}


/*RIGHT_PANEL : ONGLET_PERIODE*/
/**
 * Création de la page de détermination de la période de requête
 *
 * \param
 *
 * \return la page
 */
GtkWidget *etats_config_ui_onglet_periode_create_page ( void )
{
    GtkWidget *vbox_onglet;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *entree_date_init_etat;
    GtkWidget *entree_date_finale_etat;

    devel_debug (NULL);

    vbox_onglet =  GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, "onglet_etat_periode" ) );

    vbox = new_vbox_with_title_and_icon ( _("Date selection"), "scheduler.png" );

    gtk_box_pack_start ( GTK_BOX ( vbox_onglet ), vbox, FALSE, FALSE, 0 );
    gtk_box_reorder_child ( GTK_BOX ( vbox_onglet ), vbox, 0 );

    /* on traite la partie gauche de l'onglet dates */
    etats_config_ui_tree_view_init ( "treeview_dates",
                        etats_config_ui_onglet_periode_get_liste_dates,
                        GTK_SELECTION_SINGLE,
                        G_CALLBACK ( etats_config_ui_onglet_periode_selection_dates_changed ) );
    gtk_container_set_border_width ( GTK_CONTAINER (
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder, "vbox_utilisation_date", NULL ) ),
                        10 );

    hbox =  GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, "hbox_date_init" ) );
    entree_date_init_etat = gsb_calendar_entry_new ( FALSE );
    gtk_widget_set_size_request ( entree_date_init_etat, 100, -1 );
    g_object_set_data ( G_OBJECT ( hbox ), "entree_date_init_etat", entree_date_init_etat );
    gtk_box_pack_end ( GTK_BOX ( hbox ), entree_date_init_etat, FALSE, FALSE, 0 );

    hbox =  GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, "hbox_date_finale" ) );
    entree_date_finale_etat = gsb_calendar_entry_new ( FALSE );
    gtk_widget_set_size_request ( entree_date_finale_etat, 100, -1 );
    g_object_set_data ( G_OBJECT ( hbox ), "entree_date_finale_etat", entree_date_finale_etat );
    gtk_box_pack_end ( GTK_BOX ( hbox ), entree_date_finale_etat, FALSE, FALSE, 0 );

    etats_config_ui_onglet_periode_date_interval_sensitive ( FALSE );

    /* on traite la partie droite de l'onglet dates */
    etats_config_ui_tree_view_init ( "treeview_exer",
                        gsb_etats_config_onglet_get_liste_exercices,
                        GTK_SELECTION_MULTIPLE,
                        NULL );
    gtk_container_set_border_width ( GTK_CONTAINER (
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder, "vbox_utilisation_exo", NULL ) ),
                        10 );

    /* on met la connection pour rendre sensitif la frame vbox_utilisation_date */
    g_signal_connect ( G_OBJECT ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "radio_button_utilise_dates", NULL ) ),
                        "toggled",
                        G_CALLBACK ( sens_desensitive_pointeur ),
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder, "vbox_utilisation_date", NULL ) );

    /* on met la connection pour rendre sensitif la frame vbox_utilisation_exo */
    g_signal_connect ( G_OBJECT ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "radio_button_utilise_exo", NULL ) ),
                        "toggled",
                        G_CALLBACK ( sens_desensitive_pointeur ),
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder, "vbox_utilisation_exo", NULL ) );

    /* on connecte les signaux nécessaires pour gérer la sélection de l'exercice */
    g_signal_connect ( G_OBJECT ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "bouton_detaille_exo_etat", NULL ) ),
                        "toggled",
                        G_CALLBACK ( sens_desensitive_pointeur ),
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder, "sw_exer", NULL ) );

    gtk_widget_show_all ( vbox_onglet );

    return vbox_onglet;
}


/**
 *
 *
 * \param
 *
 * \return
 */
GtkTreeModel *etats_config_ui_onglet_periode_get_liste_dates ( void )
{
    GtkListStore *list_store;
    gchar **plages_dates;
    gint i;

    list_store = gtk_list_store_new ( 2, G_TYPE_STRING, G_TYPE_INT );

    /* on remplit la liste des dates */
    plages_dates = etats_config_liste_plages_dates;

    i = 0;

    while ( plages_dates[i] )
    {
        GtkTreeIter iter;
        gchar *plage = gettext ( plages_dates[i] );

        gtk_list_store_append ( list_store, &iter );
        gtk_list_store_set ( list_store, &iter, 0, plage, 1, i, -1 );
    
        i++;
    }

    /* return */
    return GTK_TREE_MODEL ( list_store );
}


/**
 *
 *
 * \param
 *
 * \return
 */
gboolean etats_config_ui_onglet_periode_selection_dates_changed ( GtkTreeSelection *selection,
                        GtkWidget *widget )
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gint selected;

    if ( !gtk_tree_selection_get_selected ( selection, &model, &iter ) )
        return FALSE;

    gtk_tree_model_get ( model, &iter, 1, &selected, -1 );
    etats_config_ui_onglet_periode_date_interval_sensitive ( selected );

    return TRUE;
}


/**
 * rend accessible ou nom l'ensemble des données de date configurables
 *
 * \param TRUE rend sensible FALSE rend insensible les données
 *
 * \return
 */
void etats_config_ui_onglet_periode_date_interval_sensitive ( gboolean show )
{
    if ( show > 1 )
        show = 0;

        gtk_widget_set_sensitive ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "hbox_select_dates", NULL ), show );
        gtk_widget_set_sensitive ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "hbox_date_init", "entree_date_init_etat" ), show );
        gtk_widget_set_sensitive ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "hbox_date_finale", "entree_date_finale_etat" ), show );
}


/*RIGHT_PANEL : ONGLET_VIREMENTS*/
/**
 * Création de l'onglet virements
 *
 * \param
 *
 * \return
 */
GtkWidget *etats_config_ui_onglet_virements_create_page ( void )
{
    GtkWidget *vbox_onglet;
    GtkWidget *vbox;
    GtkWidget *tree_view;

    devel_debug (NULL);

    vbox_onglet =  GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, "onglet_etat_virements" ) );

    vbox = new_vbox_with_title_and_icon ( _("Transfers"), "transfer.png" );

    gtk_box_pack_start ( GTK_BOX ( vbox_onglet ), vbox, FALSE, FALSE, 0 );
    gtk_box_reorder_child ( GTK_BOX ( vbox_onglet ), vbox, 0 );

    gtk_widget_set_sensitive ( utils_gtkbuilder_get_widget_by_name (etat_config_builder,
                        "hbox_liste_comptes_virements", NULL ), FALSE );

    /* on crée la liste des comptes */
    etats_config_ui_tree_view_init ( "treeview_virements",
                        gsb_etats_config_onglet_get_liste_comptes,
                        GTK_SELECTION_MULTIPLE,
                        NULL );

    tree_view = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, "treeview_virements", NULL );

    /* on initialise les boutons pour sensibiliser bouton_exclure_non_virements_etat et la liste des comptes */
    etats_config_ui_onglet_comptes_init_buttons_choix_utilisation_virements ( );

    /* on initialise les boutons pour sélectionner tout ou partie des comptes */
    etats_config_ui_onglet_comptes_init_buttons_selection ( "virements", tree_view );

    gtk_widget_show_all ( vbox_onglet );

    /* return */
    return vbox_onglet;
}


/**
 * Initialise les boutons pour sensibiliser bouton_exclure_non_virements_etat
 *  et la liste des comptes.
 *
 * \param
 *
 * \return
 */
void etats_config_ui_onglet_comptes_init_buttons_choix_utilisation_virements ( void )
{
    /* on connecte le signal pour gérer la sensibilité du bouton bouton_bouton_inclusion_virements_actifs_etat */
    g_signal_connect ( G_OBJECT ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "bouton_inclusion_virements_actifs_etat", NULL ) ),
                        "toggled",
                        G_CALLBACK ( sens_desensitive_pointeur ),
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "bouton_exclure_non_virements_etat", NULL ) );

    /* on connecte le signal pour gérer la sensibilité du bouton_inclusion_virements_hors_etat */
    g_signal_connect ( G_OBJECT ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "bouton_inclusion_virements_hors_etat", NULL ) ),
                        "toggled",
                        G_CALLBACK ( sens_desensitive_pointeur ),
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "bouton_exclure_non_virements_etat", NULL ) );

    /* on connecte le signal pour gérer la sensibilité du bouton_inclusion_virements_perso */
    g_signal_connect ( G_OBJECT ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "bouton_inclusion_virements_perso", NULL ) ),
                        "toggled",
                        G_CALLBACK ( sens_desensitive_pointeur ),
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "bouton_exclure_non_virements_etat", NULL ) );

    /* on connecte le signal pour gérer l'affichage de la liste des comptes */
    g_signal_connect ( G_OBJECT ( utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "bouton_inclusion_virements_perso", NULL ) ),
                        "toggled",
                        G_CALLBACK ( sens_desensitive_pointeur ),
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder,
                        "hbox_liste_comptes_virements", NULL ) );
}


/*RIGHT_PANEL : ONGLET_COMPTES*/
/**
 * Initialise les boutons de sélection des comptes
 *
 * \param suffixe du bouton
 * \param tree_view
 *
 * \return
 */
void etats_config_ui_onglet_comptes_init_buttons_selection ( gchar *name,
                        GtkWidget *tree_view )
{
    GtkWidget *button;
    gchar *tmp_str;

    /* bouton select unselect all*/
    tmp_str = g_strconcat ( "togglebutton_select_all_", name, NULL );
    button = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, tmp_str, NULL );
    gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button ), FALSE );
    g_object_set_data ( G_OBJECT ( button ), "type_compte", GINT_TO_POINTER ( -9 ) );
    g_free ( tmp_str );

    tmp_str = g_strconcat ( "hbox_toggle_select_all_", name, NULL );
    g_object_set_data ( G_OBJECT ( button ), "hbox_select_all",
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder, tmp_str, NULL ) );
    g_free ( tmp_str );

    tmp_str = g_strconcat ( "hbox_toggle_unselect_all_", name, NULL );
    g_object_set_data ( G_OBJECT ( button ), "hbox_unselect_all",
                        utils_gtkbuilder_get_widget_by_name ( etat_config_builder, tmp_str, NULL ) );
    g_signal_connect ( G_OBJECT ( button ),
                        "toggled",
                        G_CALLBACK ( etats_config_ui_onglet_comptes_select_unselect ),
                        tree_view );

    /* bouton_bank */
    tmp_str = g_strconcat ( "button_bank_", name, NULL );
    button = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, tmp_str, NULL );
    gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button ), FALSE );
    g_object_set_data ( G_OBJECT ( button ), "type_compte", GINT_TO_POINTER ( 0 ) );
    g_signal_connect ( G_OBJECT  ( button ),
                        "toggled",
                        G_CALLBACK ( etats_config_ui_onglet_comptes_select_unselect ),
                        tree_view );
    g_free ( tmp_str );

    /* bouton_cash */
    tmp_str = g_strconcat ( "button_cash_", name, NULL );
    button = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, tmp_str, NULL );
    gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button ), FALSE );
    g_object_set_data ( G_OBJECT ( button ), "type_compte", GINT_TO_POINTER ( 1 ) );
    g_signal_connect ( G_OBJECT  ( button ),
                        "toggled",
                        G_CALLBACK ( etats_config_ui_onglet_comptes_select_unselect ),
                        tree_view );
    g_free ( tmp_str );

    /* button_liabilities */
    tmp_str = g_strconcat ( "button_liabilities_", name, NULL );
    button = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, tmp_str, NULL );
    gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button ), FALSE );
    g_object_set_data ( G_OBJECT ( button ), "type_compte", GINT_TO_POINTER ( 2 ) );
    g_signal_connect ( G_OBJECT  ( button ),
                        "toggled",
                        G_CALLBACK ( etats_config_ui_onglet_comptes_select_unselect ),
                        tree_view );
    g_free ( tmp_str );

    /* "button_assets */
    tmp_str = g_strconcat ( "button_assets_", name, NULL );
    button = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, tmp_str, NULL );
    gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button ), FALSE );
    g_object_set_data ( G_OBJECT ( button ), "type_compte", GINT_TO_POINTER ( 3 ) );
    g_signal_connect ( G_OBJECT  ( button ),
                        "toggled",
                        G_CALLBACK ( etats_config_ui_onglet_comptes_select_unselect ),
                        tree_view );
    g_free ( tmp_str );
}


/**
 * select or unselect all the rows of treeview
 *
 * \param toggle_button
 * \param tree_view
 *
 * \return
 * */
void etats_config_ui_onglet_comptes_select_unselect ( GtkToggleButton *togglebutton,
                        GtkWidget *tree_view )
{
    GtkTreeSelection *selection;
    gboolean toggle;
    gint type_compte;

    type_compte = GPOINTER_TO_INT ( g_object_get_data ( G_OBJECT ( togglebutton ), "type_compte" ) );
    toggle = gtk_toggle_button_get_active ( togglebutton );
    selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );

    /* on mémorise l'état du bouton après changement d'état du bouton précédent */
    if ( prev_togglebutton && prev_togglebutton != togglebutton )
        utils_togglebutton_change_label_select_unselect ( GTK_TOGGLE_BUTTON ( prev_togglebutton ), FALSE );
    else
        prev_togglebutton = NULL;
    prev_togglebutton = togglebutton;

    switch ( type_compte )
    {
    case -9:
        {
            GtkTreeModel *model;

            model = gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) );

            if ( GTK_IS_TREE_STORE ( model ) )
                gsb_etats_config_tree_model_check_uncheck_all ( model, toggle );
            else
                    utils_togglebutton_select_unselect_all_rows ( togglebutton, tree_view );
            break;
        }
    default:
        {
            gtk_tree_selection_unselect_all ( selection );
            if ( toggle )
                gsb_etats_config_onglet_select_partie_liste_comptes ( tree_view, type_compte );
            utils_togglebutton_change_label_select_unselect ( GTK_TOGGLE_BUTTON ( togglebutton ), toggle );
        }
    }
}


/*FONCTIONS UTILITAIRES COMMUNES*/
/**
 * retourne l'index du radiobutton actif.
 *
 * \param nom du radio_button
 *
 * \return index bouton actif
 */
gint etats_config_ui_buttons_radio_get_active_index ( const gchar *button_name )
{
    GtkWidget *radio_button;
    GSList *liste;
    GSList *tmp_list;
    gint index = 0;

    radio_button = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, button_name, NULL );

    liste = g_slist_copy ( gtk_radio_button_get_group ( GTK_RADIO_BUTTON ( radio_button ) ) );
    tmp_list = g_slist_reverse ( liste );

    while ( tmp_list )
    {
        GtkWidget *button;

        button = tmp_list->data;
        if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON ( button ) ) )
            break;

        index++;
        tmp_list = tmp_list->next;
    }

    g_slist_free ( liste );

    return index;
}

/**
 * rend actif le button qui correspond à l'index passé en paramètre.
 *
 * \param nom du radio_button
 * \param index du bouton à rendre actif
 *
 * \return index bouton actif
 */
void etats_config_ui_buttons_radio_set_active_index ( const gchar *button_name,
                        gint index )
{
    GtkWidget *radio_button;
    GSList *liste;
    GSList *tmp_list;

    radio_button = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, button_name, NULL );

    liste = g_slist_copy ( gtk_radio_button_get_group ( GTK_RADIO_BUTTON ( radio_button ) ) );
    tmp_list = g_slist_reverse ( liste );

    gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( g_slist_nth_data ( tmp_list, index ) ), TRUE );

    g_slist_free ( liste );
}


/**
 * retourne l'état du togglebutton dont le nom est passé en paramètre.
 *
 * \param widget name
 *
 * \return activ
 */
gboolean etats_config_ui_widget_get_actif ( const gchar *button_name )
{
    GtkWidget *button = NULL;

    button = GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, button_name ) );
    if ( !button )
        return FALSE;

    /* return */
    return gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON ( button ) );
}


/**
 *  Positionne letogglebutton demandé par son nom en fonction de actif
 *
 *\param widget name
 *\param actif
 *
 * \return TRUE if success FALSE otherwise
 */
gboolean etats_config_ui_widget_set_actif ( const gchar *button_name,
                        gboolean actif )
{
    GtkWidget *button = NULL;

    button = GTK_WIDGET ( gtk_builder_get_object ( etat_config_builder, button_name ) );
    if ( !button )
        return FALSE;

    gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button ), actif );

    return TRUE;
}


/**
 * crée un nouveau tree_view initialisé avec model.
 * le modèle comporte deux colonnes : G_TYPE_STRING, G_TYPE_INT
 * le tree_view n'affiche que la colonne texte.
 *
 * \return the tree_wiew
 */
GtkWidget *etats_config_ui_tree_view_new_with_model ( const gchar *treeview_name,
                        GtkTreeModel *model )
{
    GtkWidget *tree_view;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;

    tree_view = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, treeview_name, NULL );
    if ( !tree_view )
        return NULL;

    gtk_tree_view_set_model ( GTK_TREE_VIEW ( tree_view ), GTK_TREE_MODEL ( model ) );
    g_object_unref ( G_OBJECT ( model ) );

    utils_set_tree_view_selection_and_text_color ( tree_view );

    /* set the column */
    cell = gtk_cell_renderer_text_new ( );

    column = gtk_tree_view_column_new_with_attributes ( NULL,
                        cell,
                        "text", 0,
                        NULL);
    gtk_tree_view_column_set_sizing ( GTK_TREE_VIEW_COLUMN ( column ), GTK_TREE_VIEW_COLUMN_FIXED );
    gtk_tree_view_append_column ( GTK_TREE_VIEW ( tree_view ),
                        GTK_TREE_VIEW_COLUMN ( column ) );
    gtk_tree_view_column_set_resizable ( column, TRUE );

    return tree_view;
}


/**
 * récupère l'index l'iter selectionné
 *
 * \param nom du tree_view
 *
 * \return numéro de la ligne sélectionnée
 */
gint etats_config_ui_tree_view_get_single_row_selected ( const gchar *treeview_name )
{
    GtkWidget *tree_view;
    GtkTreeModel *model;
    GtkTreeSelection *selection;
    GtkTreeIter iter;

    tree_view = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, treeview_name, NULL );
    if ( !tree_view )
        return -1;

    selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );

    if ( gtk_tree_selection_get_selected ( selection, &model, &iter ) )
    {
        gint index;

        gtk_tree_model_get ( GTK_TREE_MODEL ( model ), &iter, 1, &index, -1 );

        return index;
    }
    return -1;
}


/**
 *
 *
 * \param
 *
 * \return
 */
void etats_config_ui_tree_view_select_single_row ( const gchar *treeview_name,
                        gint numero )
{
    GtkWidget *tree_view;
    GtkTreeModel *model;
    GtkTreeSelection *selection;
    GtkTreeIter iter;

    tree_view = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, treeview_name, NULL );
    if ( !tree_view )
        return;

    model = gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) );
    selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );

    if ( gtk_tree_model_get_iter_first ( GTK_TREE_MODEL ( model ), &iter ) )
    {
        do
        {
            gint index;

            gtk_tree_model_get ( GTK_TREE_MODEL ( model ), &iter, 1, &index, -1 );

            if ( numero == index )
            {
                gtk_tree_selection_select_iter ( GTK_TREE_SELECTION ( selection ), &iter );
                break;
            }
        }
        while ( gtk_tree_model_iter_next ( GTK_TREE_MODEL ( model ), &iter ) );
    }
}


/**
 *
 *
 * \param
 *
 * \return
 */
GtkWidget *etats_config_ui_widget_get_widget_by_name ( const gchar *parent_name,
                        const gchar *child_name )
{
    return utils_gtkbuilder_get_widget_by_name ( etat_config_builder, parent_name, child_name );
}


/**
 * récupère l'index l'iter selectionné
 *
 * \param nom du tree_view
 *
 * \return numéro de la ligne sélectionnée
 */
GSList *etats_config_ui_tree_view_get_list_rows_selected ( const gchar *treeview_name )
{
    GtkWidget *tree_view;
    GtkTreeModel *model;
    GtkTreeSelection *selection;
    GtkTreeIter iter;
    GSList *tmp_list = NULL;
    GList *rows_list;

    tree_view = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, treeview_name, NULL );
    if ( !tree_view )
        return NULL;

    selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
    rows_list = gtk_tree_selection_get_selected_rows ( selection, &model );
    while ( rows_list )
    {
        GtkTreePath *path;
        gint index;

        path = rows_list->data;

        gtk_tree_model_get_iter ( model, &iter, path) ;
        gtk_tree_model_get ( GTK_TREE_MODEL ( model ), &iter, 1, &index, -1 );

        tmp_list = g_slist_append ( tmp_list, GINT_TO_POINTER ( index ) );
        
        gtk_tree_path_free ( path );
        rows_list = rows_list->next;
    }
    g_list_free ( rows_list );

    return tmp_list;
}


/**
 * initialise le tree_view avec son modèle et son type de sélection
 *
 * \param
 *
 * \return
 */
void etats_config_ui_tree_view_init ( const gchar *treeview_name,
                        GtkTreeModel *(*function) ( void ),
                        GtkSelectionMode type_selection,
                        GCallback selection_callback )
{
    GtkWidget *tree_view;
    GtkTreeModel *model;
    GtkTreeSelection *selection;

    /* on récupère le model par appel à function */
    model = function ( );

    tree_view = etats_config_ui_tree_view_new_with_model ( treeview_name, GTK_TREE_MODEL ( model ) );
    selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );
    gtk_tree_selection_set_mode ( selection, type_selection );

    if ( selection_callback )
        g_signal_connect ( G_OBJECT ( selection ),
                        "changed",
                        G_CALLBACK ( selection_callback ),
                        NULL );
}


void etats_config_ui_tree_view_select_rows_from_list ( GSList *liste,
                        gchar *treeview_name,
                        gint column )
{
    GtkWidget *tree_view;
    GtkTreeModel *model;
    GtkTreeSelection *selection;
    GtkTreeIter iter;
    GSList *tmp_list;

    if ( !liste )
        return;

    tree_view = utils_gtkbuilder_get_widget_by_name ( etat_config_builder, treeview_name, NULL );
    model = gtk_tree_view_get_model ( GTK_TREE_VIEW ( tree_view ) );
    selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW ( tree_view ) );

    if ( gtk_tree_model_get_iter_first ( GTK_TREE_MODEL ( model ), &iter ) )
    {
        do
        {
            gint fyear_number;

            gtk_tree_model_get ( GTK_TREE_MODEL ( model ), &iter, column, &fyear_number, -1 );

            tmp_list = liste;
            while ( tmp_list )
            {
                gint result;
                
                result = GPOINTER_TO_INT ( tmp_list -> data );

                if ( result == fyear_number )
                    gtk_tree_selection_select_iter ( GTK_TREE_SELECTION ( selection ), &iter );

                tmp_list = tmp_list -> next;
            }
        }
        while ( gtk_tree_model_iter_next ( GTK_TREE_MODEL ( model ), &iter ) );
    }
}


/*END_PRIVATE_FUNCTIONS*/
/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */

