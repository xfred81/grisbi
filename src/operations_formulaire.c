 /* Ce fichier s'occupe de la gestion du formulaire de saisie des op�rations */
 /* formulaire.c */

 /*     Copyright (C) 2000-2001  C�dric Auger */
 /* 			cedric@grisbi.org */
 /* 			http://www.grisbi.org */

 /*     This program is free software; you can redistribute it and/or modify */
 /*     it under the terms of the GNU General Public License as published by */
 /*     the Free Software Foundation; either version 2 of the License, or */
 /*     (at your option) any later version. */

 /*     This program is distributed in the hope that it will be useful, */
 /*     but WITHOUT ANY WARRANTY; without even the implied warranty of */
 /*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
 /*     GNU General Public License for more details. */

 /*     You should have received a copy of the GNU General Public License */
 /*     along with this program; if not, write to the Free Software */
 /*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */


 #include "include.h"
 #include "structures.h"
 #include "variables-extern.c"
 #include "en_tete.h"


 /***********************************************************************************************************/
 /*  Routine qui cr�e le formulaire et le renvoie */
 /***********************************************************************************************************/

 GtkWidget *creation_formulaire ( void )
 {
  GtkWidget *table;
  GtkWidget *menu;
  GtkWidget *bouton;
  GdkColor couleur_normale;
  GdkColor couleur_grise;
  GtkTooltips *tips;

  /*   on cr�e tout de suite les styles qui seront appliqu�s aux entr�es du formulaire : */
  /*     style_entree[0] sera la couleur noire, normale */
  /*     Style_entree[1] sera une couleur att�nu�e quand le formulaire est vide */

  couleur_normale.red = COULEUR_NOIRE_RED;
  couleur_normale.green = COULEUR_NOIRE_GREEN;
  couleur_normale.blue = COULEUR_NOIRE_BLUE;

  couleur_grise.red = COULEUR_GRISE_RED;
  couleur_grise.green = COULEUR_GRISE_GREEN;
  couleur_grise.blue = COULEUR_GRISE_BLUE;

  style_entree_formulaire [0] = gtk_style_copy ( gtk_widget_get_style (GTK_WIDGET (window)) );
  style_entree_formulaire [0]->fg[GTK_STATE_NORMAL] = couleur_normale;

  style_entree_formulaire [1] = gtk_style_copy ( gtk_widget_get_style (GTK_WIDGET (window)) );
  style_entree_formulaire [1]->fg[GTK_STATE_NORMAL] = couleur_grise;


  /* on cr�e le tooltips */

  tips = gtk_tooltips_new ();

  /*   le formulaire est une vbox avec en haut un tableau de 6 colonnes, et */
  /* en bas si demand� les boutons valider et annuler */

  formulaire = gtk_vbox_new ( FALSE,
			      5 );

  /* le formulaire est une table de 6 colonnes  sur 4 */

 /*  table = gtk_table_new (4, 7, FALSE);*/
   /* GDC : 7 colonnes (1 ajoutee pour la date_reelle
      Le commentaire d'origine est passe de 7 colonnes (3.1.1)
      � 6 (3.1.2) mais l'appel de fonction est rest� le m�me !
      Je ne suis pas sur de ce qu'il faut faire... Je laisse 7
      pour le moment comme dans la 3.1.2 */
  table = gtk_table_new (4, 7, FALSE);

  gtk_table_set_col_spacings ( GTK_TABLE ( table ),
			       10 );
  gtk_box_pack_start ( GTK_BOX ( formulaire ),
		       table,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( table );

  /*   met la taille du formulaire � 1 au d�but pour �viter un agrandissement automatique de la fen�tre d� aux */
  /* tailles par d�faut des entr�es */
  /* cette taille sera modifi�e automatiquement � l'affichage */

  gtk_widget_set_usize ( GTK_WIDGET ( table ),
			 1,
			 FALSE );

  /* no d'op� */

  widget_formulaire_operations[0] = gtk_label_new ( "" );
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[0],
		     0, 1, 0,1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  if ( etat.affiche_no_operation )
    gtk_widget_show ( widget_formulaire_operations[0] );


  /* entr�e de la date */

  widget_formulaire_operations[1] = gtk_entry_new_with_max_length (10);
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[1],
		     1, 2, 0,1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[1]),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(1) );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[1]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(1) );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[1]),
 		       "focus_in_event",
		       GTK_SIGNAL_FUNC (entree_prend_focus),
		       NULL );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[1]),
 		       "focus_out_event",
		       GTK_SIGNAL_FUNC (entree_perd_focus),
		       GINT_TO_POINTER(1) );
  gtk_widget_show (widget_formulaire_operations[1]);


  /*  entr�e du tiers : c'est une combofix */

  creation_liste_tiers_combofix();
  widget_formulaire_operations[2] = gtk_combofix_new ( liste_tiers_combofix,
						       FALSE,
						       TRUE,
						       TRUE );
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[2],
		     2,3, 0,1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX ( widget_formulaire_operations[2] ) -> entry),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(2) );
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX ( widget_formulaire_operations[2] ) -> arrow ),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(2) );
  gtk_signal_connect_object ( GTK_OBJECT (GTK_COMBOFIX ( widget_formulaire_operations[2]) -> entry),
			      "focus_in_event",
			      GTK_SIGNAL_FUNC (entree_prend_focus),
			      GTK_OBJECT ( widget_formulaire_operations[2] ));
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX ( widget_formulaire_operations[2]) -> entry),
		       "focus_out_event",
		       GTK_SIGNAL_FUNC (entree_perd_focus),
		       GINT_TO_POINTER(2) );
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX ( widget_formulaire_operations[2] ) -> entry),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(2) );
  gtk_widget_show (widget_formulaire_operations[2]);



  /*  Affiche le d�bit */

  widget_formulaire_operations[3] = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[3],
		     3, 4, 0,1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[3]),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(3) );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[3]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(3) );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[3]),
			     "focus_in_event",
			     GTK_SIGNAL_FUNC (entree_prend_focus),
			     NULL );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[3]),
			     "focus_out_event",
			     GTK_SIGNAL_FUNC (entree_perd_focus),
			     GINT_TO_POINTER (3) );
  gtk_widget_show (widget_formulaire_operations[3]);


  /*  Affiche le cr�dit */

  widget_formulaire_operations[4] = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[4],
		     4,5,0,1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[4]),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(4) );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[4]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(4) );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[4]),
			     "focus_in_event",
			     GTK_SIGNAL_FUNC (entree_prend_focus),
			     NULL );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[4]),
			     "focus_out_event",
			     GTK_SIGNAL_FUNC (entree_perd_focus),
			     GINT_TO_POINTER (4) );
  gtk_widget_show (widget_formulaire_operations[4]);


  /* met l'option menu des devises */

  widget_formulaire_operations[5] = gtk_option_menu_new ();
  gtk_tooltips_set_tip ( GTK_TOOLTIPS ( tips ),
			 widget_formulaire_operations[5],
			 "Choix de la devise",
			 "Choix de la devise" );
  menu = creation_option_menu_devises ( -1,
					liste_struct_devises );
  gtk_option_menu_set_menu ( GTK_OPTION_MENU ( widget_formulaire_operations[5] ),
			     menu );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[5]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(5) );
  gtk_table_attach ( GTK_TABLE ( table ),
		     widget_formulaire_operations[5],
		     5, 6, 0,1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_widget_show ( widget_formulaire_operations[5] );


  /* mise en forme du bouton change */

  widget_formulaire_operations[6] = gtk_button_new_with_label ( "Change ..." );
  gtk_button_set_relief ( GTK_BUTTON ( widget_formulaire_operations[6] ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect ( GTK_OBJECT (  widget_formulaire_operations[6] ),
 		       "clicked",
		       GTK_SIGNAL_FUNC ( click_sur_bouton_voir_change ),
		       NULL );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[6]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(6) );
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[6],
		     6,7, 0, 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_widget_show ( widget_formulaire_operations[6] );


  /* met l'option menu des comptes */

/*   widget_formulaire_operations[7] = gtk_option_menu_new (); */
/*   gtk_tooltips_set_tip ( GTK_TOOLTIPS ( tips ), */
/* 			 widget_formulaire_operations[7], */
/* 			 "Choix du compte", */
/* 			 "Choix du compte" ); */

/*   gtk_option_menu_set_menu ( GTK_OPTION_MENU ( widget_formulaire_operations[7] ), */
/* 			     creation_option_menu_comptes () ); */
/*   gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[7]), */
/*  		       "key_press_event", */
/* 		       GTK_SIGNAL_FUNC (touches_champ_formulaire), */
/* 		       GINT_TO_POINTER(7) ); */
/*   gtk_table_attach ( GTK_TABLE ( table ), */
/* 		     widget_formulaire_operations[7], */
/* 		     0, 2, 1, 2, */
/* 		     GTK_SHRINK | GTK_FILL, */
/* 		     GTK_SHRINK | GTK_FILL, */
/* 		     0,0); */
/*   gtk_widget_show ( widget_formulaire_operations[7] ); */


   /* GDC : entr�e de la date reelle */
  /*   mise � la place du choix du compte, verra plus tard o� le mettre */

   widget_formulaire_operations[7] = gtk_entry_new_with_max_length (13);
   gtk_table_attach ( GTK_TABLE (table),
 		     widget_formulaire_operations[7],
 		     1, 2, 1, 2,
 		     GTK_SHRINK | GTK_FILL,
 		     GTK_SHRINK | GTK_FILL,
 		     0,0);
   gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[7]),
  		       "button_press_event",
 		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
 		       GINT_TO_POINTER(7)  );
   gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[7]),
  		       "key_press_event",
 		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
 		       GINT_TO_POINTER(7)  );
   gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[7]),
  		       "focus_in_event",
 		       GTK_SIGNAL_FUNC (entree_prend_focus),
 		       NULL );
   gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[7]),
  		       "focus_out_event",
 		       GTK_SIGNAL_FUNC (entree_perd_focus),
 		       GINT_TO_POINTER(7) );
	/* L'utilisation (l'affichage) de la date r�elle est d�sactivable */
   if ( etat.affiche_date_bancaire )
     gtk_widget_show ( widget_formulaire_operations[7] );
	/* FinGDC */


  /*  Affiche les cat�gories / sous-cat�gories */

  widget_formulaire_operations[8] = gtk_combofix_new_complex ( liste_categories_combofix,
							       FALSE,
							       TRUE,
							       TRUE );
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[8],
		     2, 3, 1,2,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[8]) -> entry),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(8) );
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[8]) -> arrow),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(8) );
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[8]) -> entry),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(8) );
  gtk_signal_connect_object ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[8]) -> entry),
			      "focus_in_event",
			      GTK_SIGNAL_FUNC (entree_prend_focus),
			      GTK_OBJECT ( widget_formulaire_operations[8] ));
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[8]) -> entry),
		       "focus_out_event",
		       GTK_SIGNAL_FUNC (entree_perd_focus),
		       GINT_TO_POINTER (8) );
  gtk_widget_show (widget_formulaire_operations[8]);


  /*   cr�ation de l'entr�e du ch�que, non affich�e pour le moment */
  /* � cr�er avant l'option menu du type d'op� */

  widget_formulaire_operations[10] = gtk_entry_new();
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[10]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(10) );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[10]),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(10) );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[10]),
			     "focus_in_event",
			     GTK_SIGNAL_FUNC (entree_prend_focus),
			     NULL );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[10]),
			     "focus_out_event",
			     GTK_SIGNAL_FUNC (entree_perd_focus),
			     GINT_TO_POINTER (10) );
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[10],
		     5, 7, 1, 2,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);

  /*  Affiche l'option menu des types */

  widget_formulaire_operations[9] = gtk_option_menu_new ();
  gtk_tooltips_set_tip ( GTK_TOOLTIPS ( tips ),
			 widget_formulaire_operations[9],
			 "Choix du type d'op�ration",
			 "Choix du type d'op�ration" );
  gtk_signal_connect ( GTK_OBJECT ( widget_formulaire_operations[9] ),
		       "key_press_event",
		       GTK_SIGNAL_FUNC ( touches_champ_formulaire ),
		       GINT_TO_POINTER(9) );
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[9],
		     3, 5, 1,2,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);

  /* le menu par d�faut est celui des d�bits */

  if ( (menu = creation_menu_types ( 1, compte_courant, 0  )))
    {
      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

      gtk_option_menu_set_menu ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
				 menu );
      gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
				    cherche_no_menu_type ( TYPE_DEFAUT_DEBIT ) );
      gtk_widget_show ( widget_formulaire_operations[9] );
    }


  /* met l'option menu de l'exercice */


  widget_formulaire_operations[11] = gtk_option_menu_new ();
  gtk_tooltips_set_tip ( GTK_TOOLTIPS ( tips ),
			 widget_formulaire_operations[11],
			 "Choix de l'exercice",
			 "Choix de l'exercice" );
  menu = gtk_menu_new ();
  gtk_option_menu_set_menu ( GTK_OPTION_MENU ( widget_formulaire_operations[11] ),
			     creation_menu_exercices () );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[11]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(11) );
  gtk_table_attach ( GTK_TABLE ( table ),
		     widget_formulaire_operations[11],
		     0, 2, 2, 3,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  if ( etat.utilise_exercice )
    gtk_widget_show ( widget_formulaire_operations[11] );



  /*  Affiche l'imputation budg�taire */

  widget_formulaire_operations[12] = gtk_combofix_new_complex ( liste_imputations_combofix,
								FALSE,
								TRUE,
								TRUE );
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[12],
		     2, 3, 2, 3,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[12]) -> entry),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(12) );
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[12]) -> arrow),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(12) );
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[12]) -> entry),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(12) );
  gtk_signal_connect_object ( GTK_OBJECT ( GTK_COMBOFIX (widget_formulaire_operations[12]) -> entry ),
		      "focus_in_event",
		      GTK_SIGNAL_FUNC (entree_prend_focus),
		      GTK_OBJECT ( widget_formulaire_operations[12] ) );
  gtk_signal_connect ( GTK_OBJECT (GTK_COMBOFIX (widget_formulaire_operations[12]) -> entry),
		       "focus_out_event",
		       GTK_SIGNAL_FUNC (entree_perd_focus),
		       GINT_TO_POINTER (12) );

  if ( etat.utilise_imputation_budgetaire )
    gtk_widget_show (widget_formulaire_operations[12]);


  /*   cr�ation de l'entr�e du no de pi�ce comptable */

  widget_formulaire_operations[13] = gtk_entry_new();
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[13],
		     5, 7, 2, 3,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[13]),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(13) );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[13]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(13) );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[13]),
			     "focus_in_event",
			     GTK_SIGNAL_FUNC (entree_prend_focus),
			     NULL );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[13]),
			     "focus_out_event",
			     GTK_SIGNAL_FUNC (entree_perd_focus),
			     GINT_TO_POINTER (13) );
  if ( etat.utilise_piece_comptable )
    gtk_widget_show ( widget_formulaire_operations[13] );


  /* mise en forme du bouton ventilation */

  widget_formulaire_operations[14] = gtk_button_new_with_label ( "Ventilation ..." );
  gtk_button_set_relief ( GTK_BUTTON ( widget_formulaire_operations[14] ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect ( GTK_OBJECT ( widget_formulaire_operations[14] ),
   		       "clicked",
   		       GTK_SIGNAL_FUNC ( basculer_vers_ventilation ),
   		       NULL );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[14]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(14) );
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[14],
		     0, 2, 3, 4,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_widget_show ( widget_formulaire_operations[14] );


  /*  Affiche les notes */

  widget_formulaire_operations[15] = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[15],
		     2, 3, 3, 4,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[15]),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(15) );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[15]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(15) );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[15]),
			     "focus_in_event",
			     GTK_SIGNAL_FUNC (entree_prend_focus),
			     NULL );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[15]),
			     "focus_out_event",
			     GTK_SIGNAL_FUNC (entree_perd_focus),
			     GINT_TO_POINTER (15) );
  gtk_widget_show (widget_formulaire_operations[15]);



  /*  Affiche les infos banque/guichet */

  widget_formulaire_operations[16] = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[16],
		     3, 6, 3, 4,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[16]),
 		       "button_press_event",
		       GTK_SIGNAL_FUNC (clique_champ_formulaire),
		       GINT_TO_POINTER(16) );
  gtk_signal_connect ( GTK_OBJECT (widget_formulaire_operations[16]),
 		       "key_press_event",
		       GTK_SIGNAL_FUNC (touches_champ_formulaire),
		       GINT_TO_POINTER(16) );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[16]),
			     "focus_in_event",
			     GTK_SIGNAL_FUNC (entree_prend_focus),
			     NULL );
  gtk_signal_connect_after ( GTK_OBJECT (widget_formulaire_operations[16]),
			     "focus_out_event",
			     GTK_SIGNAL_FUNC (entree_perd_focus),
			     GINT_TO_POINTER (16) );
  if ( etat.utilise_info_banque_guichet )
    gtk_widget_show (widget_formulaire_operations[16]);


  /*  Affiche le mode automatique / manuel  */

  widget_formulaire_operations[17] = gtk_label_new ("auto");
  gtk_table_attach ( GTK_TABLE (table),
		     widget_formulaire_operations[17],
		     6, 7, 3, 4,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0,0);
  gtk_widget_show (widget_formulaire_operations[17]);


  /* s�paration d'avec les boutons */

  separateur_formulaire_operations = gtk_hseparator_new ();
  gtk_box_pack_start ( GTK_BOX ( formulaire ),
		       separateur_formulaire_operations,
		       FALSE,
		       FALSE,
		       0 );
  if ( etat.affiche_boutons_valider_annuler )
    gtk_widget_show ( separateur_formulaire_operations );

  /* mise en place des boutons */

  hbox_valider_annuler_ope = gtk_hbox_new ( FALSE,
					    5 );
  gtk_box_pack_start ( GTK_BOX ( formulaire ),
		       hbox_valider_annuler_ope,
		       FALSE,
		       FALSE,
		       0 );
  if ( etat.affiche_boutons_valider_annuler )
    gtk_widget_show ( hbox_valider_annuler_ope );

  bouton = gnome_stock_button ( GNOME_STOCK_BUTTON_CANCEL );
  gtk_button_set_relief ( GTK_BUTTON ( bouton ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect ( GTK_OBJECT ( bouton ),
		       "clicked",
		       GTK_SIGNAL_FUNC ( echap_formulaire ),
		       NULL );
  gtk_box_pack_end ( GTK_BOX ( hbox_valider_annuler_ope ),
		     bouton,
		     FALSE,
		     FALSE,
		     0 );
  gtk_widget_show ( bouton );

  bouton = gnome_stock_button ( GNOME_STOCK_BUTTON_OK );
  gtk_button_set_relief ( GTK_BUTTON ( bouton ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect ( GTK_OBJECT ( bouton ),
		       "clicked",
		       GTK_SIGNAL_FUNC ( fin_edition ),
		       NULL );
  gtk_box_pack_end ( GTK_BOX ( hbox_valider_annuler_ope ),
		     bouton,
		     FALSE,
		     FALSE,
		     0 );
  gtk_widget_show ( bouton );


  /* aucune op�ration associ�e au formulaire */

  gtk_object_set_data ( GTK_OBJECT ( table ),
			"adr_struct_ope",
			NULL );
  gtk_object_set_data ( GTK_OBJECT ( table ),
			"liste_adr_ventilation",
			NULL );

  return ( formulaire );

 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 void echap_formulaire ( void )
 {
  GSList *liste_tmp;

  /* si c'est une nouvelle op� ventil�e et qu'on a utilis� la compl�tion */
  /* il faut effacer les op�s de ventilation automatiquement cr��es */
  /* celles ci sont dans la liste dans "liste_adr_ventilation" */

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  liste_tmp = gtk_object_get_data ( GTK_OBJECT ( formulaire ),
				    "liste_adr_ventilation" );

  if ( liste_tmp
       &&
       !gtk_object_get_data ( GTK_OBJECT ( formulaire ),
			      "adr_struct_ope" )
       &&
       liste_tmp != GINT_TO_POINTER ( -1 ))
    {
      while ( liste_tmp )
	{
	  ligne_selectionnee_ventilation = liste_tmp -> data;
	  supprime_operation_ventilation ();
	  liste_tmp = liste_tmp -> next;
	}
    }

  etat.formulaire_en_cours = 0;

  formulaire_a_zero();

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;
  gtk_widget_grab_focus ( CLIST_OPERATIONS );

  if ( !etat.formulaire_toujours_affiche )
    gtk_widget_hide ( frame_droite_bas );

 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 /* Fonction appel�e quand une entry prend le focus */
 /* si elle contient encore des �l�ments gris�s, on les enl�ve */
 /***********************************************************************************************************/

 void entree_prend_focus ( GtkWidget *entree )
 {

  /* si le style est le gris, on efface le contenu de l'entr�e, sinon on fait rien */

  if ( GTK_IS_COMBOFIX ( entree ))
    {
      if ( gtk_widget_get_style ( GTK_COMBOFIX (entree)->entry ) == style_entree_formulaire[1] )
	{
	  gtk_combofix_set_text ( GTK_COMBOFIX ( entree ),
				  "" );
	  gtk_widget_set_style ( GTK_COMBOFIX (entree)->entry,
				 style_entree_formulaire[0] );
	}
    }
  else
    {
      if ( gtk_widget_get_style ( entree ) == style_entree_formulaire[1] )
	{
	  gtk_entry_set_text ( GTK_ENTRY ( entree ),
			       "" );
	  gtk_widget_set_style ( entree,
				 style_entree_formulaire[0] );
	}
    }
 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 /* Fonction appel�e quand une entry perd le focus */
 /* si elle ne contient rien, on remet la fonction en gris */
 /***********************************************************************************************************/

 void entree_perd_focus ( GtkWidget *entree,
			 GdkEventFocus *ev,
			 gint *no_origine )
 {
  gchar *texte;

  texte = NULL;
  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  switch ( GPOINTER_TO_INT ( no_origine ))
    {
      /* on sort de la date, soit c'est vide, soit on la v�rifie, la compl�te si n�cessaire et met � jour l'exercice */
    case 1:
      if ( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	{
	  modifie_date ( entree );

	  /* 	  si c'est une modif d'op�, on ne change pas l'exercice */

	  if ( !gtk_object_get_data ( GTK_OBJECT ( formulaire ),
				      "adr_struct_ope" ))
	    affiche_exercice_par_date( widget_formulaire_operations[1],
				       widget_formulaire_operations[11] );
	}
      else
	texte = "Date";
      break;

      /*       on sort du tiers : soit vide soit compl�te le reste de l'op� */

    case 2:
      if ( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	completion_operation_par_tiers ();
      else
	texte = "Tiers";
      break;

      /*       on sort du d�bit : soit vide, soit change le menu des types s'il ne correspond pas */

    case 3:

      if ( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	{
	  /* on  commence par virer ce qu'il y avait dans les cr�dits */

	  if ( gtk_widget_get_style ( widget_formulaire_operations[4] ) == style_entree_formulaire[0] )
	    {
	      gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[4] ),
				   "" );
	      entree_perd_focus ( widget_formulaire_operations[4],
				  NULL,
				  GINT_TO_POINTER (4));
	    }

	  if ( GTK_WIDGET_VISIBLE ( widget_formulaire_operations[9] )
	       &&
	       GPOINTER_TO_INT ( gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ) -> menu ),
						       "signe_menu" ))
	       ==
	       2 )
	    {
	      if ( etat.affiche_tous_les_types )
		{
		  /* on ne modifie que le d�faut */
		  gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
						cherche_no_menu_type ( TYPE_DEFAUT_DEBIT ) );
		  gtk_object_set_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ) -> menu ),
					"signe_menu",
					GINT_TO_POINTER ( 1 ));
		}
	      else
		{
		  GtkWidget *menu;

		  if ( (menu = creation_menu_types ( 1, compte_courant, 0  )))
		    {
		      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

		      gtk_option_menu_set_menu ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
						 menu );
		      gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
						    cherche_no_menu_type ( TYPE_DEFAUT_DEBIT ) );
		      gtk_widget_show ( widget_formulaire_operations[9] );
		    }
		  else
		    gtk_widget_hide ( widget_formulaire_operations[9] );
		}	     
	    }
	}
      else
	texte = "D�bit";
      break;

      /*       on sort du cr�dit : soit vide, soit change le menu des types s'il n'y a aucun tiers ( <=> nouveau tiers ) */

    case 4:
      if ( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	{
	  /* on  commence par virer ce qu'il y avait dans les d�bits */

	  if ( gtk_widget_get_style ( widget_formulaire_operations[3] ) == style_entree_formulaire[0] )
	    {
	      gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[3] ),
				   "" );
	      entree_perd_focus ( widget_formulaire_operations[3],
				  NULL,
				  GINT_TO_POINTER (3));
	    }

	  if ( GTK_WIDGET_VISIBLE ( widget_formulaire_operations[9] )
	       &&
	       GPOINTER_TO_INT ( gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ) -> menu ),
						       "signe_menu" ))
	       ==
	       1 )
	    {
	      if ( etat.affiche_tous_les_types )
		/* on ne modifie que le d�faut */
	      {
		gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
					      cherche_no_menu_type ( TYPE_DEFAUT_CREDIT ) );
		gtk_object_set_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ) -> menu ),
				      "signe_menu",
				      GINT_TO_POINTER ( 2 ));
	      }

	      else
		{
		  GtkWidget *menu;

		  if ( (menu = creation_menu_types ( 2, compte_courant, 0  )))
		    {
		      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

		      gtk_option_menu_set_menu ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
						 menu );
		      gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
						    cherche_no_menu_type ( TYPE_DEFAUT_CREDIT ) );
		      gtk_widget_show ( widget_formulaire_operations[9] );
		    }
		  else
		    gtk_widget_hide ( widget_formulaire_operations[9] );
		}
	    }
	}
      else
	texte = "Cr�dit";
      break;

    case 7:
      if ( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	modifie_date ( entree );
      else
	texte = "Date de valeur";
      break;


      /*       sort des cat�gories : si c'est une op� ventil�e, affiche le bouton */

    case 8:
      if ( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	{
	  if ( strcmp ( g_strstrip ( gtk_combofix_get_text ( GTK_COMBOFIX ( widget_formulaire_operations[8]))),
			"Op�ration ventil�e" ))
	    gtk_widget_hide ( widget_formulaire_operations[14] );
	  else
	    gtk_widget_show ( widget_formulaire_operations[14] );
	}
      else
	texte = "Cat�gories : Sous-cat�gories";

      break;

    case 10:
      if ( !strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	texte = "n� Ch�que/Virement";
      break;

    case 12:
      if ( !strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	texte = "Imputation budg�taire";
      break;

    case 13:
      if ( !strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	texte = "Pi�ce comptable";

      break;

    case 15:
      if ( !strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	texte = "Notes";
      break;

    case 16:
      if ( !strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
	texte = "Informations banque/guichet";
      break;

       /* on sort de la date r�elle , soit c'est vide, soit on la v�rifie et la compl�te si n�cessaire  */
	case 18:
		if ( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree )))))
		{
			modifie_date ( entree );
		}
		else
		 	texte = "Date de valeur";
	break;

    default :
    }


  /* l'entr�e �tait vide, on remet le d�faut */
  /* si l'origine �tait un combofix, il faut remettre le texte */
  /* avec le gtk_combofix (sinon risque de compl�tion), donc utiliser l'origine */

  if ( texte )
    {
      switch ( GPOINTER_TO_INT ( no_origine ))
	{
	case 2:
	case 8:
	case 12:
	  gtk_combofix_set_text ( GTK_COMBOFIX ( widget_formulaire_operations[GPOINTER_TO_INT ( no_origine )] ),
				  texte );
	  break;

	default:

	  gtk_entry_set_text ( GTK_ENTRY ( entree ),
			       texte );
	}
      gtk_widget_set_style ( entree,
			     style_entree_formulaire[1] );
    }
 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 void clique_champ_formulaire ( GtkWidget *entree,
			       GdkEventButton *ev,
			       gint *no_origine )
 {
  etat.formulaire_en_cours = 1;

  /* on rend sensitif tout ce qui ne l'�tait pas sur le formulaire */

  degrise_formulaire_operations ();

   /* si l'entr�e de la date est grise, on met la date courante */
	/* seulement si la date r�elle est grise aussi. Dans le cas contraire, c'est elle qui prend le focus */
   if ( (gtk_widget_get_style ( widget_formulaire_operations[1] ) == style_entree_formulaire[1])
	&& (gtk_widget_get_style ( widget_formulaire_operations[7] ) == style_entree_formulaire[1]) )
    {
      entree_prend_focus ( widget_formulaire_operations[1] );

      gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[1] ),
			   date_jour() );

      /* si le click est sur l'entr�e de la date, on la s�lectionne et elle prend le focus */

      if ( GPOINTER_TO_INT ( no_origine ) == 1 )
	{
	  if ( ev )
	    gtk_signal_emit_stop_by_name ( GTK_OBJECT ( entree ),
					   "button_press_event");
	  gtk_entry_select_region ( GTK_ENTRY ( entree ),
				    0,
				    -1);
	  gtk_widget_grab_focus ( GTK_WIDGET ( entree ));
	}
    }
else if (gtk_widget_get_style ( widget_formulaire_operations[1] ) == style_entree_formulaire[1])
{
       entree_prend_focus ( widget_formulaire_operations[7] );
       /* si le click est sur l'entr�e de la date, on la s�lectionne et elle prend le focus */

       if ( GPOINTER_TO_INT ( no_origine ) == 1 )
 	{
 	  if ( ev )
 	    gtk_signal_emit_stop_by_name ( GTK_OBJECT ( entree ),
 					   "button_press_event");
 	  gtk_entry_select_region ( GTK_ENTRY ( entree ),
 				    0,
 				    -1);
 	  gtk_widget_grab_focus ( GTK_WIDGET ( entree ));
 	}
}

  /*   si ev est null ( cad que �a ne vient pas d'un click mais appel� par ex � la fin */
  /* de fin_edition ), on se barre */

  if ( !ev )
    return;

  /* �num�ration suivant l'entr�e o� on clique */

  switch ( GPOINTER_TO_INT ( no_origine ))
    {
    case 1:
      /* click sur l'entr�e de la date */

      /* si double click, on popup le calendrier */

      if ( ev->type == GDK_2BUTTON_PRESS )
	{
	  GtkWidget *popup;
	  GtkWidget *popup_boxv;
	  GtkRequisition *taille_entree;
	  gint x, y;
	  GtkWidget *calendrier;
	  int cal_jour, cal_mois, cal_annee;
	  GtkWidget *bouton;
	  GtkWidget *frame;

	  /* cherche la position o� l'on va mettre la popup */

	  taille_entree = malloc ( sizeof ( GtkRequisition ));

	  gdk_window_get_origin ( GTK_WIDGET ( entree ) -> window,
				  &x,
				  &y );
	  gtk_widget_size_request ( GTK_WIDGET ( entree ),
				    taille_entree );
  
	  y = y + taille_entree->height;


	  /* cr�ation de la popup */

	  popup = gtk_window_new ( GTK_WINDOW_POPUP );
	  gtk_window_set_modal ( GTK_WINDOW (popup),
				 TRUE);
	  gtk_widget_set_uposition ( GTK_WIDGET ( popup ),
				     x,
				     y );


	  /* cr�ation de l'int�rieur de la popup */

	  frame = gtk_frame_new ( NULL );
	  gtk_container_add ( GTK_CONTAINER (popup),
			      frame);
	  gtk_widget_show ( frame );

	  popup_boxv = gtk_vbox_new ( FALSE,
				      5 );
	  gtk_container_set_border_width ( GTK_CONTAINER ( popup_boxv ),
					   5 );

	  gtk_container_add ( GTK_CONTAINER ( frame ),
			      popup_boxv);
	  gtk_widget_show ( popup_boxv );

	  if ( !( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree ))))
		  &&
		  sscanf ( gtk_entry_get_text ( GTK_ENTRY ( entree )),
			   "%d/%d/%d",
			   &cal_jour,
			   &cal_mois,
			   &cal_annee)))
	    sscanf ( date_jour(),
		     "%d/%d/%d",
		     &cal_jour,
		     &cal_mois,
		     &cal_annee);
      
	  calendrier = gtk_calendar_new();
	  gtk_calendar_select_month ( GTK_CALENDAR ( calendrier ),
				      cal_mois-1,
				      cal_annee);
	  gtk_calendar_select_day  ( GTK_CALENDAR ( calendrier ),
				     cal_jour);

	  gtk_calendar_display_options ( GTK_CALENDAR ( calendrier ),
					 GTK_CALENDAR_SHOW_HEADING |
					 GTK_CALENDAR_SHOW_DAY_NAMES |
					 GTK_CALENDAR_WEEK_START_MONDAY );

	  gtk_signal_connect ( GTK_OBJECT ( calendrier),
			       "day_selected_double_click",
			       GTK_SIGNAL_FUNC ( date_selectionnee ),
			       popup );
	  gtk_signal_connect ( GTK_OBJECT ( popup ),
			       "key_press_event",
			       GTK_SIGNAL_FUNC ( touche_calendrier ),
			       NULL );
	  gtk_signal_connect_object ( GTK_OBJECT ( popup ),
				      "destroy",
				      GTK_SIGNAL_FUNC ( gdk_pointer_ungrab ),
				      GDK_CURRENT_TIME );
	  gtk_box_pack_start ( GTK_BOX ( popup_boxv ),
			       calendrier,
			       TRUE,
			       TRUE,
			       0 );
	  gtk_widget_show ( calendrier );


	  /* ajoute le bouton annuler */

	  bouton = gtk_button_new_with_label ( "Annuler" );
	  gtk_signal_connect_object ( GTK_OBJECT ( bouton ),
				      "clicked",
				      GTK_SIGNAL_FUNC ( gtk_widget_destroy ),
				      GTK_OBJECT ( popup ));
	  gtk_box_pack_start ( GTK_BOX ( popup_boxv ),
			       bouton,
			       TRUE,
			       TRUE,
			       0 );
	  gtk_widget_show ( bouton );

	  gtk_widget_show (popup);
      
	  gdk_pointer_grab ( popup -> window, 
			     TRUE,
			     GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
			     GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK |
			     GDK_POINTER_MOTION_MASK,
			     NULL, 
			     NULL, 
			     GDK_CURRENT_TIME );

	  gtk_widget_grab_focus ( GTK_WIDGET ( popup ));
	}
      break;
    /* GDC : Gestion de la date r�elle */
     case 18:
       /* click sur l'entr�e de la date r�elle*/

       /* si double click, on popup le calendrier */

       if ( ev->type == GDK_2BUTTON_PRESS )
 	{
 	  GtkWidget *popup;
 	  GtkWidget *popup_boxv;
 	  GtkRequisition *taille_entree;
 	  gint x, y;
 	  GtkWidget *calendrier;
 	  int cal_jour, cal_mois, cal_annee;
 	  GtkWidget *bouton;
 	  GtkWidget *frame;

  	  /* cherche la position o� l'on va mettre la popup */

 	  taille_entree = malloc ( sizeof ( GtkRequisition ));

 	  gdk_window_get_origin ( GTK_WIDGET ( entree ) -> window,
 				  &x,
 				  &y );
 	  gtk_widget_size_request ( GTK_WIDGET ( entree ),
 				    taille_entree );

 	  y = y + taille_entree->height;


 	  /* cr�ation de la popup */

 	  popup = gtk_window_new ( GTK_WINDOW_POPUP );
 	  gtk_window_set_modal ( GTK_WINDOW (popup),
 				 TRUE);
 	  gtk_widget_set_uposition ( GTK_WIDGET ( popup ),
 				     x,
 				     y );


 	  /* cr�ation de l'int�rieur de la popup */

 	  frame = gtk_frame_new ( NULL );
 	  gtk_container_add ( GTK_CONTAINER (popup),
 			      frame);
 	  gtk_widget_show ( frame );

 	  popup_boxv = gtk_vbox_new ( FALSE,
 				      5 );
 	  gtk_container_set_border_width ( GTK_CONTAINER ( popup_boxv ),
 					   5 );

 	  gtk_container_add ( GTK_CONTAINER ( frame ),
 			      popup_boxv);
 	  gtk_widget_show ( popup_boxv );

 	  if ( !( strlen ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( entree ))))
 		  &&
 		  sscanf ( gtk_entry_get_text ( GTK_ENTRY ( entree )),
 			   "%d/%d/%d",
 			   &cal_jour,
 			   &cal_mois,
 			   &cal_annee)))
 	    sscanf ( date_jour(),
 		     "%d/%d/%d",
 		     &cal_jour,
 		     &cal_mois,
 		     &cal_annee);

 	  calendrier = gtk_calendar_new();
 	  gtk_calendar_select_month ( GTK_CALENDAR ( calendrier ),
 				      cal_mois-1,
 				      cal_annee);
 	  gtk_calendar_select_day  ( GTK_CALENDAR ( calendrier ),
 				     cal_jour);

 	  gtk_calendar_display_options ( GTK_CALENDAR ( calendrier ),
 					 GTK_CALENDAR_SHOW_HEADING |
 					 GTK_CALENDAR_SHOW_DAY_NAMES |
 					 GTK_CALENDAR_WEEK_START_MONDAY );

 	  gtk_signal_connect ( GTK_OBJECT ( calendrier),
 			       "day_selected_double_click",
 			       GTK_SIGNAL_FUNC ( date_bancaire_selectionnee ),
 			       popup );
 	  gtk_signal_connect ( GTK_OBJECT ( popup ),
 			       "key_press_event",
 			       GTK_SIGNAL_FUNC ( touche_calendrier ),
 			       NULL );
 	  gtk_signal_connect_object ( GTK_OBJECT ( popup ),
 				      "destroy",
 				      GTK_SIGNAL_FUNC ( gdk_pointer_ungrab ),
 				      GDK_CURRENT_TIME );
 	  gtk_box_pack_start ( GTK_BOX ( popup_boxv ),
 			       calendrier,
 			       TRUE,
 			       TRUE,
 			       0 );
 	  gtk_widget_show ( calendrier );


 	  /* ajoute le bouton annuler */

 	  bouton = gtk_button_new_with_label ( "Annuler" );
 	  gtk_signal_connect_object ( GTK_OBJECT ( bouton ),
 				      "clicked",
 				      GTK_SIGNAL_FUNC ( gtk_widget_destroy ),
 				      GTK_OBJECT ( popup ));
 	  gtk_box_pack_start ( GTK_BOX ( popup_boxv ),
 			       bouton,
 			       TRUE,
 			       TRUE,
 			       0 );
 	  gtk_widget_show ( bouton );

 	  gtk_widget_show (popup);

 	  gdk_pointer_grab ( popup -> window,
 			     TRUE,
 			     GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
 			     GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK |
 			     GDK_POINTER_MOTION_MASK,
 			     NULL,
 			     NULL,
 			     GDK_CURRENT_TIME );

 	  gtk_widget_grab_focus ( GTK_WIDGET ( popup ));
 	}
       break;
/* GDCFin */

    default:
    }
 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 void touches_champ_formulaire ( GtkWidget *widget,
				GdkEventKey *ev,
				gint *no_origine )
 {
  gint origine;

  origine = GPOINTER_TO_INT ( no_origine );

  /*   si etat.entree = 1, la touche entr�e finit l'op�ration ( fonction par d�faut ) */
  /* sinon elle fait comme tab */

  if ( !etat.entree
       &&
       ( ev->keyval == 65293
	 ||
	 ev->keyval == 65421 ))
    ev->keyval = 65289;


  switch (ev->keyval)
    {
      /* �chap */

    case 65307:

      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;
      gtk_widget_grab_focus ( CLIST_OPERATIONS );
      echap_formulaire();
      break;


      /*       tabulation */

    case 65289:

      /* une tabulation passe au widget affich� suivant */
      /* et retourne � la date ou enregistre l'op� s'il est � la fin */

      gtk_signal_emit_stop_by_name ( GTK_OBJECT ( widget ),
				     "key_press_event");

      /* on efface la s�lection en cours si c'est une entr�e ou un combofix */

      if ( GTK_IS_ENTRY ( widget ))
	gtk_entry_select_region ( GTK_ENTRY ( widget ),
				  0,
				  0);
      else
	if ( GTK_IS_COMBOFIX ( widget ))
	  gtk_entry_select_region ( GTK_ENTRY ( GTK_COMBOFIX ( widget ) -> entry ),
				    0,
				    0);

      /* on fait perdre le focus au widget courant pour faire les changements automatiques si n�cessaire */

      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;
      gtk_widget_grab_focus ( CLIST_OPERATIONS );

		/* GDC : si l'origine est la date r�elle, on fait comme si c'�tait la date */
		if (origine == 18) origine = 1;
		/* FinGDC */

      /* on donne le focus au widget suivant */

      origine = (origine + 1 ) % 17;

      while ( !(GTK_WIDGET_VISIBLE ( widget_formulaire_operations[origine] )
		&&
		GTK_WIDGET_SENSITIVE ( widget_formulaire_operations[origine] )
		&&
		( GTK_IS_COMBOFIX (widget_formulaire_operations[origine] )
		  ||
		  GTK_IS_ENTRY ( widget_formulaire_operations[origine] )
		  ||
		  GTK_IS_BUTTON ( widget_formulaire_operations[origine] ) )))
	origine = (origine + 1 ) % 17;


      /*       si on se retrouve sur la date et que etat.entree = 0, on enregistre l'op�rations */

      if ( origine == 1 && !etat.entree )
	{
	  fin_edition();
	  return;
	}

      /* si le prochain est le d�bit, on v�rifie s'il n'y a rien dans cette entr�e et s'il y a quelque chose dans l'entr�e du cr�dit */

      if ( origine == 3 )
	{
	  /* si le d�bit est gris et le cr�dit est noir, on met sur le cr�dit */

	  if ( gtk_widget_get_style ( widget_formulaire_operations[origine] ) == style_entree_formulaire[1]
	       &&
	       gtk_widget_get_style ( widget_formulaire_operations[origine+1] ) == style_entree_formulaire[0] )
	    origine++;
	}

      /*       si le prochain est le cr�dit et que le d�bit a �t� rempli, on se met sur la devise et on efface le cr�dit */

      if ( origine == 4 )
	if ( gtk_widget_get_style ( widget_formulaire_operations[origine-1] ) == style_entree_formulaire[0] )
	  {
	    origine++;
	    gtk_widget_set_style (widget_formulaire_operations[4],
				  style_entree_formulaire[1] );
	    gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[4]),
				 "Cr�dit" );
	  }

      /* on s�lectionne le contenu de la nouvelle entr�e */

      if ( GTK_IS_COMBOFIX ( widget_formulaire_operations[origine] ) )
	{
	  gtk_widget_grab_focus ( GTK_COMBOFIX ( widget_formulaire_operations[origine] ) -> entry );  
	  gtk_entry_select_region ( GTK_ENTRY ( GTK_COMBOFIX ( widget_formulaire_operations[origine] ) -> entry ),
				    0,
				    -1 );
	}
      else
	{
	  if ( GTK_IS_ENTRY ( widget_formulaire_operations[origine] ) )
	    gtk_entry_select_region ( GTK_ENTRY ( widget_formulaire_operations[origine] ),
				      0,
				      -1 );

	  gtk_widget_grab_focus ( widget_formulaire_operations[origine]  );
	}

      break;


      /* entree */
    case 65293 :
    case 65421 :
      gtk_signal_emit_stop_by_name ( GTK_OBJECT (widget),
				     "key_press_event");

      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;
      gtk_widget_grab_focus ( CLIST_OPERATIONS );

      fin_edition();
      break;

      /* touches + */

    case 65451:
    case 61:
      /*       si on est dans une entree de date, on augmente d'un jour la date */

		/* GDC : prise en compte de la date r�elle (18) FinGDC */
       if ( origine == 1 || origine == 18)
	{
	  gtk_signal_emit_stop_by_name ( GTK_OBJECT ( widget ),
					 "key_press_event");
	  incremente_decremente_date ( widget_formulaire_operations[origine],
				       1 );
	}
      break;

      /* touches - */

    case 65453:
    case 45:
      /*       si on est dans une entree de date, on diminue d'un jour la date */

		/* GDC : prise en compte de la date r�elle (18) FinGDC */
       if ( origine == 1 || origine == 18)
	{
	  gtk_signal_emit_stop_by_name ( GTK_OBJECT ( widget ),
					 "key_press_event");
	  incremente_decremente_date ( widget_formulaire_operations[origine],
				       -1 );
	}
      break;


    default:
    }

 }
 /***********************************************************************************************************/


 /***********************************************************************************************************/
 /* Fonction touche_calendrier */
 /* supprime le calendrier si on appuie sur �chap */
 /***********************************************************************************************************/

 void touche_calendrier ( GtkWidget *popup,
			 GdkEventKey *ev,
			 gpointer null )
 {
  if ( ev->keyval == 65307 )
    gtk_widget_destroy ( GTK_WIDGET (popup));

 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 /* Fonction date_selectionnee */
 /* appel�e lorsqu'on a click� 2 fois sur une date du calendrier */
 /***********************************************************************************************************/

 void date_selectionnee ( GtkCalendar *calendrier,
			 GtkWidget *popup )
 {
  guint annee, mois, jour;

  gtk_calendar_get_date ( calendrier,
			  &annee,
			  &mois,
			  &jour);

  gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[1] ),
		       g_strdup_printf ( "%02d/%02d/%d",
					 jour,
					 mois + 1,
					 annee));
  gtk_widget_destroy ( popup );

  gtk_widget_grab_focus ( GTK_COMBOFIX ( widget_formulaire_operations[2] ) -> entry );
	
 }
 /***********************************************************************************************************/



/***********************************************************************************************************/
/* Fonction date_relle_selectionnee */
/* appel�e lorsqu'on a click� 2 fois sur une date du calendrier pour la date reelle */
/***********************************************************************************************************/

void date_bancaire_selectionnee ( GtkCalendar *calendrier, GtkWidget *popup )
{
	guint annee, mois, jour;

	gtk_calendar_get_date ( calendrier, &annee, &mois, &jour);

	gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[7] ),
 		g_strdup_printf ( "%02d/%02d/%d", jour, mois + 1, annee));
	gtk_widget_destroy ( popup );

	gtk_widget_grab_focus ( GTK_COMBOFIX ( widget_formulaire_operations[2] ) -> entry );
}
/***********************************************************************************************************/






 /*********************************************************************************************/
 /* Fonction modifie_date */
 /* prend en argument une entr�e contenant une date */
 /*  v�rifie la validit� et la modifie si seulement une partie est donn�e */
 /* met la date du jour si l'entr�e est vide */
 /* renvoie TRUE si la date est correcte */
 /*********************************************************************************************/
 /* GDC : modif pour ne forcer une date vide � la date du jour que si la date r�elle est vide */
 /* aussi. Pour le moment, j'acc�de directement au widget de la date r�elle. Il faudra        */
 /* g�n�raliser en ajoutant un param�tre � la fonction, la "date-condition"                   */
gboolean modifie_date ( GtkWidget *entree )
{
  gchar *pointeur_entry;
	/* GDC */
	gchar *pointeur_entry_date;
	gchar *pointeur_entry_date_bancaire;
	/* FinGDC */
  int jour, mois, annee;
  GDate *date;
  gchar **tab_date;
  /* si l'entr�e est grise, on se barre */
	/* GDC : et si l'entr�e de la date r�elle est elle  aussi gris�e  si on l'utilise */
	if (( gtk_widget_get_style ( entree ) == style_entree_formulaire[1] )  &&
		(( gtk_widget_get_style ( widget_formulaire_operations[7] ) == style_entree_formulaire[1] )
		|| (!etat.affiche_date_bancaire))
)
    return ( FALSE );

  pointeur_entry = g_strstrip ( gtk_entry_get_text ( GTK_ENTRY (entree)) );


	/* GDC : si pas d'affichage de la date r�elle, m�me comportement */
  if ( (entree == widget_formulaire_operations[1]) &&(!etat.affiche_date_bancaire ) && (!strlen ( pointeur_entry )) )
    gtk_entry_set_text ( GTK_ENTRY ( entree ),
			 date_jour() );
	/* GDC : on ne force une date r�elle vide (ou �gale � la valeur par d�faut */
	/* "Date r�elle") � la date du jour que si la date est vide aussi ou */
	/* �gale � sa valeur par d�faut */
else if ((entree == widget_formulaire_operations[7])
		&& ((!strlen ( pointeur_entry )) || (!strcmp (pointeur_entry, "Date de valeur"))))
{
		pointeur_entry_date = g_strstrip ( gtk_entry_get_text ( GTK_ENTRY (widget_formulaire_operations[1])) );
		if ((!strlen ( pointeur_entry_date )) ||
				(!strcmp (gtk_entry_get_text ( GTK_ENTRY (widget_formulaire_operations[1])), "Date")) )
{
printf ("Blop\n");
fflush(0);
			gtk_entry_set_text ( GTK_ENTRY ( entree ), date_jour() );
}
/* Eviter les boucles infinies par appels crois�s */
		/* GDC : si la date n'est pas vide, on retourne son statut � elle.
		else return ( modifie_date ( widget_formulaire_operations[7] ) ); */
}
	/* GDC : on ne force une date vide (ou �gale � la valeur par d�faut */
	/* "Date") � la date du jour que si la date r�elle est vide aussi ou */
	/* �gale � sa valeur par d�faut */
	else if ( (entree == widget_formulaire_operations[1])
			&& ((!strlen ( pointeur_entry )) || (!strcmp (pointeur_entry, "Date"))))
	{
		pointeur_entry_date_bancaire = g_strstrip ( gtk_entry_get_text ( GTK_ENTRY (widget_formulaire_operations[7])) );
		if ((!strlen ( pointeur_entry_date_bancaire )) ||
				(!strcmp (gtk_entry_get_text ( GTK_ENTRY (widget_formulaire_operations[7])), "Date de valeur")) )
			gtk_entry_set_text ( GTK_ENTRY ( entree ), date_jour() );
		/* GDC : si la date r�elle n'est pas vide, on retourne son statut � elle. */
		else return ( modifie_date ( widget_formulaire_operations[7] ) );
	}
  else
    {
      date = g_date_new ();
		g_date_set_time ( date, time(NULL));

		tab_date = g_strsplit ( pointeur_entry, "/", 3 );

      if ( tab_date[2] && tab_date[1] )
	{
	  /*       on a rentr� les 3 chiffres de la date */

			jour = strtod ( tab_date[0],  NULL );
			mois = strtod ( tab_date[1], NULL );
			annee = strtod ( tab_date[2], NULL );
 
	  if ( annee < 100 )
	    {
				if ( annee < 80 ) annee = annee + 2000;
				else annee = annee + 1900;
	    }
	}
      else
	if ( tab_date[1] )
	  {
	    /* 	on a rentr� la date sous la forme xx/xx , il suffit de mettre l'ann�e courante */

			jour = strtod ( tab_date[0], NULL );
			mois = strtod ( tab_date[1], NULL );
	    annee = g_date_year ( date );
	  }
	else
	  {
	    /* 	on a rentr� que le jour de la date, il faut mettre le mois et l'ann�e courante */
	    /* ou bien on a rentr� la date sous forme jjmm ou jjmmaa ou jjmmaaaa */

	    gchar buffer[3];

	    switch ( strlen ( tab_date[0] ))
	      {
		/* 	      forme jj ou j */
	      case 1:
	      case 2:
					jour = strtod ( tab_date[0], 	NULL );
		mois = g_date_month ( date );
		annee = g_date_year ( date );
		break;

		/* form jjmm */

	      case 4 :
		buffer[0] = tab_date[0][0];
		buffer[1] = tab_date[0][1];
		buffer[2] = 0;

					jour = strtod ( buffer, NULL );
					mois = strtod ( tab_date[0] + 2, NULL );
		annee = g_date_year ( date );
		break;

		/* forme jjmmaa */

	      case 6:
		buffer[0] = tab_date[0][0];
		buffer[1] = tab_date[0][1];
		buffer[2] = 0;

					jour = strtod ( buffer, NULL );
		buffer[0] = tab_date[0][2];
		buffer[1] = tab_date[0][3];

					mois = strtod ( buffer, NULL );
					annee = strtod ( tab_date[0] + 4, NULL ) + 2000;
		break;

		/* forme jjmmaaaa */

	      case 8:
		buffer[0] = tab_date[0][0];
		buffer[1] = tab_date[0][1];
		buffer[2] = 0;

					jour = strtod ( buffer, NULL );
		buffer[0] = tab_date[0][2];
		buffer[1] = tab_date[0][3];

					mois = strtod ( buffer, NULL );
					annee = strtod ( tab_date[0] + 4, NULL );
		break;

	      default :
		jour = 0;
		mois = 0;
		annee = 0;
	      }
	  }
      g_strfreev ( tab_date );

		if ( g_date_valid_dmy ( jour, mois, annee) )
	gtk_entry_set_text ( GTK_ENTRY ( entree ),
					g_strdup_printf ( "%02d/%02d/%d", jour, mois, annee ));
      else
	return ( FALSE );
    }
  return ( TRUE );
}
/***********************************************************************************************************/



 /***********************************************************************************************************/
 /* Fonction completion_operation_par_tiers */
 /* appel�e lorsque le tiers perd le focus */
 /* r�cup�re le tiers, et recherche la derni�re op� associ�e � ce tiers */
 /* remplit le reste de l'op�ration avec les derni�res donn�es */
 /***********************************************************************************************************/

 void completion_operation_par_tiers ( void )
 {
  GSList *liste_tmp;
  struct struct_tiers *tiers;
  struct structure_operation *operation;
  GSList *pointeur_ope;

  /* s'il y a quelque chose dans les entr�es d�bit/cr�dit ou cat�gories, on se barre */

  if ( gtk_widget_get_style ( widget_formulaire_operations[3] ) == style_entree_formulaire[0]
       ||
       gtk_widget_get_style ( widget_formulaire_operations[4] ) == style_entree_formulaire[0]
       ||
       gtk_widget_get_style ( GTK_COMBOFIX (widget_formulaire_operations[8])->entry ) == style_entree_formulaire[0] )
    return;


  /* recherche le tiers demand� */

  liste_tmp = g_slist_find_custom ( liste_struct_tiers,
				    g_strstrip ( gtk_combofix_get_text ( GTK_COMBOFIX ( widget_formulaire_operations[2] ))),
				    ( GCompareFunc ) recherche_tiers_par_nom );


  /*   si nouveau tiers,  on s'en va simplement */

  if ( !liste_tmp )
    return;

  tiers = liste_tmp -> data;

  /* on fait d'abord le tour du compte courant pour recherche une op� avec ce tiers */
  /* s'il n'y a aucune op� correspondante, on fait le tour de tous les comptes */

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  operation = NULL;
  pointeur_ope = LISTE_OPERATIONS;

  while ( pointeur_ope )
    {
      struct structure_operation *ope_test;

      ope_test = pointeur_ope -> data;

      if ( ope_test -> tiers == tiers -> no_tiers )
	{
	  if ( operation )
	    {
	      if ( g_date_compare ( ope_test -> date,
				    operation -> date ) >= 0 )
		operation = ope_test;
	    }
	  else
	    operation = ope_test;
	}

      pointeur_ope = pointeur_ope -> next;
    }


  if ( !operation )
    {
      /* aucune op�ration correspondant � ce tiers n'a �t� trouv�e dans le compte courant */
      /*       on recherche dans les autres comptes, la premi�re trouv�e fera l'affaire */

      gint i;

      p_tab_nom_de_compte_variable = p_tab_nom_de_compte;

      for ( i = 0 ; i < nb_comptes ; i++ )
	{
	  if ( i != compte_courant )
	    {
	      pointeur_ope = LISTE_OPERATIONS;

	      while ( pointeur_ope )
		{
		  struct structure_operation *ope_test;

		  ope_test = pointeur_ope -> data;

		  if ( ope_test -> tiers == tiers -> no_tiers )
		    {
		      operation = ope_test;
		      pointeur_ope = NULL;
		      i = nb_comptes;
		    }
		  else
		    pointeur_ope = pointeur_ope -> next;
		}
	    }
	  p_tab_nom_de_compte_variable++;
	}
    }

  /* si on n'a trouv� aucune op�ration, on se tire */

  if ( !operation )
    return;


  /* remplit les diff�rentes entr�es du formulaire */

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;


  /* remplit les montant */

  if ( operation -> montant < 0 )
    {
      entree_prend_focus ( widget_formulaire_operations[3] );
      gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[3] ),
			   g_strdup_printf ( "%4.2f",
					     -operation -> montant ));

      /* met le menu des types d�bits */

      if ( !etat.affiche_tous_les_types )
	{
	  GtkWidget *menu;

	  if ( (menu = creation_menu_types ( 1, compte_courant, 0  )))
	    {
	      gtk_option_menu_set_menu ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
					 menu );
	      gtk_widget_show ( widget_formulaire_operations[9] );
	    }
	  else
	    gtk_widget_hide ( widget_formulaire_operations[9] );
	}
    }
  else
    {
      entree_prend_focus ( widget_formulaire_operations[4] );
      gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[4] ),
			   g_strdup_printf ( "%4.2f",
					     operation -> montant ));

      /* met le menu des types cr�dits */

      if ( !etat.affiche_tous_les_types )
	{
	  GtkWidget *menu;

	  if ( (menu = creation_menu_types ( 2, compte_courant, 0 )))
	    {
	      gtk_option_menu_set_menu ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
					 menu );
	      gtk_widget_show ( widget_formulaire_operations[9] );
	    }
	  else
	    gtk_widget_hide ( widget_formulaire_operations[9] );
	}
    }

  /* met la devise */

  gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[5] ),
				g_slist_position ( liste_struct_devises,
						   g_slist_find_custom ( liste_struct_devises,
									 GINT_TO_POINTER ( operation -> devise ),
									 ( GCompareFunc ) recherche_devise_par_no )));
  /* mise en forme des cat�gories */

  if ( operation -> operation_ventilee )
    {
      GtkWidget *dialog;
      GtkWidget *label;
      gint result;

      entree_prend_focus ( widget_formulaire_operations[8] );

      /* c'est une op� ventil�e, la cat�gorie est Op�ration ventil�e */
      /* on demande si on veut r�cup�rer les anciennes op�s de ventilation */

      gtk_combofix_set_text ( GTK_COMBOFIX ( widget_formulaire_operations[8] ),
			      "Op�ration ventil�e" );
      gtk_widget_show ( widget_formulaire_operations[14] );

      /* affiche la question de r�cup�ration */

      dialog = gnome_dialog_new ( "R�cup�ration des ventilations ?",
				  GNOME_STOCK_BUTTON_YES,
				  GNOME_STOCK_BUTTON_NO,
				  NULL );
      gtk_window_set_transient_for ( GTK_WINDOW ( dialog ),
				     GTK_WINDOW ( window ));

      label = gtk_label_new ( "Voulez-vous r�cup�rer aussi les op�rations de la derni�re ventilation ?" );
      gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
			   label,
			   FALSE,
			   FALSE,
			   5 );
      gtk_widget_show ( label );

      result = gnome_dialog_run_and_close ( GNOME_DIALOG ( dialog ));

      if ( !result )
	{
	  /* r�cup�ration des anciennes op�s de la ventilation */

	  GSList *liste_des_opes_de_ventilation;

	  liste_des_opes_de_ventilation = NULL;
	  pointeur_ope = LISTE_OPERATIONS;

	  while ( pointeur_ope )
	    {
	      struct structure_operation *ope_test;

	      ope_test = pointeur_ope -> data;

	      if ( ope_test -> no_operation_ventilee_associee == operation -> no_operation )
		{
		  struct structure_operation *nouvelle_operation;

		  nouvelle_operation = calloc ( 1,
						sizeof ( struct structure_operation ));

		  nouvelle_operation -> no_compte = ope_test -> no_compte;
		  nouvelle_operation -> montant = ope_test -> montant;
		  nouvelle_operation -> categorie = ope_test -> categorie;
		  nouvelle_operation -> sous_categorie = ope_test -> sous_categorie;

		  if ( ope_test -> notes )
		    nouvelle_operation -> notes = g_strdup ( ope_test -> notes );
		  else
		    nouvelle_operation -> notes = NULL;

		  nouvelle_operation -> type_ope = ope_test -> type_ope;

		  if ( ope_test -> contenu_type )
		    nouvelle_operation -> contenu_type = g_strdup ( ope_test -> contenu_type );
		  else
		    nouvelle_operation -> contenu_type = NULL;

		  nouvelle_operation -> no_exercice = ope_test -> no_exercice;
		  nouvelle_operation -> imputation = ope_test -> imputation;
		  nouvelle_operation -> sous_imputation = ope_test -> sous_imputation;

		  if ( ope_test -> no_piece_comptable )
		    nouvelle_operation -> no_piece_comptable = g_strdup ( ope_test -> no_piece_comptable );
		  else
		    nouvelle_operation -> no_piece_comptable = NULL;

		  if ( ope_test -> info_banque_guichet )
		    nouvelle_operation -> info_banque_guichet = g_strdup ( ope_test -> info_banque_guichet );
		  else
		    nouvelle_operation -> info_banque_guichet = NULL;


		  nouvelle_operation -> devise = 1;
		  nouvelle_operation -> jour = 1;
		  nouvelle_operation -> mois = 1;
		  nouvelle_operation -> annee = 2000;

		  /* on met la relation � -1 pour qu'elle puisse etre retrouv�e lors de l'�dition des ventilations */

		  nouvelle_operation -> no_operation_ventilee_associee = -1;

		  nouvelle_operation -> date = g_date_new_dmy ( nouvelle_operation -> jour,
								nouvelle_operation -> mois,
								nouvelle_operation -> annee );

		  ajout_operation ( nouvelle_operation );

		  if ( ope_test -> relation_no_operation )
		    {
		      /* c'est un virement, on cr�e la contre op�ration */

		      struct structure_operation *contre_nouvelle_operation;

		      contre_nouvelle_operation = calloc ( 1,
							   sizeof ( struct structure_operation ));

		      contre_nouvelle_operation -> no_compte = ope_test -> relation_no_compte;
		      contre_nouvelle_operation -> montant = -ope_test -> montant;

		      if ( ope_test -> notes )
			contre_nouvelle_operation -> notes = g_strdup ( ope_test -> notes );
		      else
			contre_nouvelle_operation -> notes = NULL;

		      contre_nouvelle_operation -> type_ope = ope_test -> type_ope;

		      if ( ope_test -> contenu_type )
			contre_nouvelle_operation -> contenu_type = g_strdup ( ope_test -> contenu_type );
		      else
			contre_nouvelle_operation -> contenu_type = NULL;

		      contre_nouvelle_operation -> no_exercice = ope_test -> no_exercice;
		      contre_nouvelle_operation -> imputation = ope_test -> imputation;
		      contre_nouvelle_operation -> sous_imputation = ope_test -> sous_imputation;

		      if ( ope_test -> no_piece_comptable )
			contre_nouvelle_operation -> no_piece_comptable = g_strdup ( ope_test -> no_piece_comptable );
		      else
			contre_nouvelle_operation -> no_piece_comptable = NULL;

		      if ( ope_test -> info_banque_guichet )
			contre_nouvelle_operation -> info_banque_guichet = g_strdup ( ope_test -> info_banque_guichet );
		      else
			contre_nouvelle_operation -> info_banque_guichet = NULL;

		      contre_nouvelle_operation -> devise = 1;
		      contre_nouvelle_operation -> jour = 1;
		      contre_nouvelle_operation -> mois = 1;
		      contre_nouvelle_operation -> annee = 2000;

		      contre_nouvelle_operation -> date = g_date_new_dmy ( contre_nouvelle_operation -> jour,
									   contre_nouvelle_operation -> mois,
									   contre_nouvelle_operation -> annee );

		      ajout_operation ( contre_nouvelle_operation );

		      contre_nouvelle_operation -> relation_no_compte = compte_courant;
		      contre_nouvelle_operation -> relation_no_operation = nouvelle_operation -> no_operation;
		      nouvelle_operation -> relation_no_compte = ope_test -> relation_no_compte;
		      nouvelle_operation -> relation_no_operation = contre_nouvelle_operation -> no_operation;

		    }

		  liste_des_opes_de_ventilation = g_slist_append ( liste_des_opes_de_ventilation,
								   nouvelle_operation );
		}
	      pointeur_ope = pointeur_ope -> next;
	    }

	  if ( liste_des_opes_de_ventilation )
	    gtk_object_set_data ( GTK_OBJECT ( formulaire ),
				  "liste_adr_ventilation",
				  liste_des_opes_de_ventilation );
	  else
	    gtk_object_set_data ( GTK_OBJECT ( formulaire ),
				  "liste_adr_ventilation",
				  GINT_TO_POINTER ( -1 ) );
	}
    }
  else
    if ( operation -> relation_no_operation )
      {
	/* c'est un virement */

	entree_prend_focus ( widget_formulaire_operations[8] );

	p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> relation_no_compte;

	gtk_combofix_set_text ( GTK_COMBOFIX ( widget_formulaire_operations[8] ),
				g_strconcat ( "Virement : ",
					      NOM_DU_COMPTE,
					      NULL ));

	p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;
      }
    else
      {
	/* c'est des cat�gories normales */

	liste_tmp = g_slist_find_custom ( liste_struct_categories,
					  GINT_TO_POINTER ( operation -> categorie ),
					  ( GCompareFunc ) recherche_categorie_par_no );

	if ( liste_tmp )
	  {
	    GSList *liste_tmp_2;

	    entree_prend_focus ( widget_formulaire_operations[8]);

	    liste_tmp_2 = g_slist_find_custom ( (( struct struct_categ * )( liste_tmp -> data )) -> liste_sous_categ,
						GINT_TO_POINTER ( operation -> sous_categorie ),
						( GCompareFunc ) recherche_sous_categorie_par_no );
	    if ( liste_tmp_2 )
	      gtk_combofix_set_text ( GTK_COMBOFIX ( widget_formulaire_operations[8] ),
				      g_strconcat ( (( struct struct_categ * )( liste_tmp -> data )) -> nom_categ,
						    " : ",
						    (( struct struct_sous_categ * )( liste_tmp_2 -> data )) -> nom_sous_categ,
						    NULL ));
	    else
	      gtk_combofix_set_text ( GTK_COMBOFIX ( widget_formulaire_operations[8] ),
				      (( struct struct_categ * )( liste_tmp -> data )) -> nom_categ );
	  }
      }



  /* met l'option menu du type d'op� */

  if ( GTK_WIDGET_VISIBLE ( widget_formulaire_operations[9] ))
    {
      gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
				    cherche_no_menu_type ( operation -> type_ope ));
      if ( operation -> type_ope )
	{
	  struct struct_type_ope *type;

	  type = g_slist_find_custom ( TYPES_OPES,
				       GINT_TO_POINTER ( operation -> type_ope ),
				       (GCompareFunc) recherche_type_ope_par_no ) -> data;

	  if ( type -> affiche_entree )
	    {
	      if ( type -> numerotation_auto )
		{
		  entree_prend_focus ( widget_formulaire_operations[10] );
		  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[10] ),
				       itoa ( type -> no_en_cours + 1 ));
		}
	      else
		if ( operation -> contenu_type )
		  {
		    entree_prend_focus ( widget_formulaire_operations[10] );
		    gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[10] ),
					 operation -> contenu_type );
		  }
	    }
	}
    }


  /* met en place l'imputation budg�taire */


  liste_tmp = g_slist_find_custom ( liste_struct_imputation,
				    GINT_TO_POINTER ( operation -> imputation ),
				    ( GCompareFunc ) recherche_imputation_par_no );

  if ( liste_tmp )
    {
      GSList *liste_tmp_2;

      entree_prend_focus ( widget_formulaire_operations[12]);

      liste_tmp_2 = g_slist_find_custom ( (( struct struct_imputation * )( liste_tmp -> data )) -> liste_sous_imputation,
					  GINT_TO_POINTER ( operation -> sous_imputation ),
					  ( GCompareFunc ) recherche_sous_categorie_par_no );
      if ( liste_tmp_2 )
	gtk_combofix_set_text ( GTK_COMBOFIX ( widget_formulaire_operations[12] ),
				g_strconcat ( (( struct struct_imputation * )( liste_tmp -> data )) -> nom_imputation,
					      " : ",
					      (( struct struct_sous_imputation * )( liste_tmp_2 -> data )) -> nom_sous_imputation,
					      NULL ));
      else
	gtk_combofix_set_text ( GTK_COMBOFIX ( widget_formulaire_operations[12] ),
				(( struct struct_imputation * )( liste_tmp -> data )) -> nom_imputation );
    }


  /* mise en place de la pi�ce comptable */

  if ( operation -> no_piece_comptable )
    {
      entree_prend_focus ( widget_formulaire_operations[13] );
      gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[13] ),
			   operation -> no_piece_comptable );
    }


  /*   remplit les notes */

  if ( operation -> notes )
    {
      entree_prend_focus ( widget_formulaire_operations[15] );
      gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[15] ),
			   operation -> notes );
    }

  /*   remplit les infos guichet / banque */

  if ( operation -> info_banque_guichet )
    {
      entree_prend_focus ( widget_formulaire_operations[16] );
      gtk_entry_set_text ( GTK_ENTRY ( widget_formulaire_operations[16] ),
			   operation -> info_banque_guichet );
    }
 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 /* proc�dure appel�e lors de la fin de l'�dition d'op�ration */
 /* cr�e une nouvelle op�ration � partir du formulaire */
 /* puis appelle ajout_operation pour la sauver */
 /***********************************************************************************************************/

 void fin_edition ( void )
 {
  struct structure_operation *operation;
  struct structure_operation *operation_2;
  gchar *pointeur_char;
  GSList *pointeur_liste;
  gchar **tableau_char;
  gint virement;
  gint compte_virement = 0;
  struct struct_devise *devise;
  struct struct_devise *devise_compte;


  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;


  /* on v�rifie qu'il y a bien une date */

  if ( gtk_widget_get_style ( widget_formulaire_operations[1] ) != style_entree_formulaire[0] )
    {
      dialogue ( " Erreur : il faut obligatoirement entrer une date.");
      return;
    }

  /* v�rifie que la date est correcte */

  if ( !modifie_date ( widget_formulaire_operations[1] ))
    {
      dialogue ( " Erreur : La date est invalide" );
      gtk_widget_grab_focus ( widget_formulaire_operations[1] );
      gtk_entry_select_region ( GTK_ENTRY (  widget_formulaire_operations[1]),
				0,
				-1);
      return;
    }

   /* v�rifie que la date bancaire est correcte */

   if ( gtk_widget_get_style ( widget_formulaire_operations[7] ) == style_entree_formulaire[0]
	&&
	!modifie_date ( widget_formulaire_operations[7] ))
     {
       dialogue ( " Erreur : La date bancaire est invalide" );
       gtk_widget_grab_focus ( widget_formulaire_operations[7] );
       gtk_entry_select_region ( GTK_ENTRY (  widget_formulaire_operations[7]),
 				0,
				 -1);
      return;
    }

  /* v�rification que ce n'est pas un virement sur lui-m�me */

  if ( !g_strcasecmp ( g_strstrip ( gtk_combofix_get_text ( GTK_COMBOFIX (widget_formulaire_operations[8]))),
		       g_strconcat ( "Virement : ", NOM_DU_COMPTE, NULL ) ) )
    {
      dialogue ( " Erreur : impossibilit� de virer un compte   \n    sur lui-m�me");
      return;
    }



  /*   r�cup�ration de l'op�ration : soit l'adr de la struct, soit NULL si nouvelle */

  operation = gtk_object_get_data ( GTK_OBJECT ( formulaire ),
				    "adr_struct_ope" );

  /*   pour les types qui sont � incr�mentation automatique ( surtout les ch�ques ) */
  /* on fait le tour des operations pour voir si le no n'a pas d�j� �t� utilis� */
  /* si operation n'est pas nul, c'est une modif donc on ne fait pas ce test */

  if ( GTK_WIDGET_VISIBLE ( widget_formulaire_operations[10] ))
    {
      struct struct_type_ope *type;

      type = gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ) -> menu_item ),
				   "adr_type" );

      if ( type -> numerotation_auto )
	{
	  /* v�rifie s'il y a quelque chose */

	  if ( gtk_widget_get_style ( widget_formulaire_operations[10] ) == style_entree_formulaire[1] )
	    {
	      if ( question ( "Le type d'op�ration choisi est � num�rotation automatique mais ne contient aucun num�ro.\nSouhaitez-vous continuer ?" ) )
		goto sort_test_cheques;
	      else
		return;
	    }

	  if ( !operation )
	    {
	      GSList *liste_tmp;
	      gchar *no_cheque;

	      liste_tmp = LISTE_OPERATIONS;
	      no_cheque = g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[10] )));

	      while ( liste_tmp )
		{
		  struct structure_operation *operation_tmp;

		  operation_tmp = liste_tmp -> data;

		  if ( operation_tmp -> type_ope == type -> no_type
		       &&
		       !strcmp ( operation_tmp -> contenu_type,
				 no_cheque ))
		    {
		      if ( question ( "Attention, le num�ro du ch�que est d�j� utilis�.\nSouhaitez-vous continuer ?" ))
			goto sort_test_cheques;
		      else
			return;
		    }
		  liste_tmp = liste_tmp -> next;
		}
	    }
	}
    }

  sort_test_cheques :


    /*   v�rifie tout de suite si c'est une op�ration ventil�e, */
    /*si c'est le cas, si la liste des ventilation existe, on va l'enregistrer plus tard */
    /* sinon on va ventiler tout de suite */

    if ( !strcmp ( g_strstrip ( gtk_combofix_get_text ( GTK_COMBOFIX ( widget_formulaire_operations[8] ))),
		   "Op�ration ventil�e" )
	 &&
	 !gtk_object_get_data ( GTK_OBJECT ( formulaire ),
				"liste_adr_ventilation" ))
      {
	enregistre_ope_au_retour = 1;

	if ( gtk_widget_get_style ( widget_formulaire_operations[3] ) == style_entree_formulaire[0] )
	  ventiler_operation ( g_strtod ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[3] ))),
					  NULL ),
			       0 );
	else
	  ventiler_operation ( g_strtod ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[4] ))),
					  NULL ),
			       1 );

	return;
      }


  /* si c'est une nouvelle op�, on la cr�e, met son num�ro � 0, le compte courant, et manuelle */

  if ( !operation )
    {
      operation = calloc ( 1,
			   sizeof ( struct structure_operation ) );
      operation -> no_compte = compte_courant;
    }


  /* traitement de la date */

    pointeur_char = g_strstrip ( gtk_entry_get_text ( GTK_ENTRY (widget_formulaire_operations[1])));

      tableau_char = g_strsplit ( pointeur_char,
				  "/",
				  3 );

      operation -> jour = strtod ( tableau_char[0],
				   NULL );
      operation -> mois = strtod ( tableau_char[1],
				   NULL );
      operation -> annee = strtod (tableau_char[2],
				   NULL );

      operation->date = g_date_new_dmy ( operation->jour,
					 operation->mois,
					 operation->annee);

      strcpy ( date_courante,
	       pointeur_char );


  if ( etat.affiche_date_bancaire
       &&
       gtk_widget_get_style ( widget_formulaire_operations[7] ) == style_entree_formulaire[0] )
    {
      pointeur_char = g_strstrip ( gtk_entry_get_text ( GTK_ENTRY (widget_formulaire_operations[7])));

      tableau_char = g_strsplit ( pointeur_char,
				  "/",
				  3 );

      operation -> jour_bancaire = strtod ( tableau_char[0],
					    NULL );
      operation -> mois_bancaire = strtod ( tableau_char[1],
					    NULL );
      operation -> annee_bancaire = strtod (tableau_char[2],
					    NULL );

      operation->date_bancaire = g_date_new_dmy ( operation->jour_bancaire,
						  operation->mois_bancaire,
						  operation->annee_bancaire);
    }
  else
    {
      operation -> jour_bancaire = 0;
      operation -> mois_bancaire = 0;
      operation -> annee_bancaire = 0;

      operation->date_bancaire = NULL;
    }

  /* r�cup�ration du tiers, s'il n'existe pas, on le cr�e */

  if ( gtk_widget_get_style ( GTK_COMBOFIX ( widget_formulaire_operations[2] ) -> entry ) == style_entree_formulaire[0] )
    {
      pointeur_char = g_strstrip ( gtk_combofix_get_text ( GTK_COMBOFIX ( widget_formulaire_operations[2] )));

      if ( ( pointeur_liste = g_slist_find_custom ( liste_struct_tiers,
						    pointeur_char,
						    ( GCompareFunc ) recherche_tiers_par_nom )) )
	operation -> tiers = (( struct struct_tiers * )( pointeur_liste -> data )) -> no_tiers;
      else
	operation -> tiers = (( struct struct_tiers * )( ajoute_nouveau_tiers ( pointeur_char ))) -> no_tiers;
    }
  else
    operation -> tiers = 0;



  /* r�cup�ration du montant */

  if ( gtk_widget_get_style ( widget_formulaire_operations[3] ) == style_entree_formulaire[0] )
    operation -> montant = -g_strtod ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[3] ))),
				       NULL );
  else
    operation -> montant = g_strtod ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[4] ))),
				      NULL );



  /* r�cup�ration de la devise */
 
  devise = gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[5] ) -> menu_item ),
				 "adr_devise" );


  /* si c'est la devise du compte ou */
  /* si c'est un compte qui doit passer � l'euro ( la transfo se fait au niveau de l'affichage de la liste ) */
  /* ou si c'est un compte en euro et l'op� est dans une devise qui doit passer � l'euro -> pas de change � demander */

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  devise_compte = g_slist_find_custom ( liste_struct_devises,
					GINT_TO_POINTER ( DEVISE ),
					( GCompareFunc ) recherche_devise_par_no ) -> data;

  operation -> devise = devise -> no_devise;

  if ( !( operation -> no_operation
	  ||
	  devise -> no_devise == DEVISE
	  ||
	  ( devise_compte -> passage_euro && !strcmp ( devise -> nom_devise, "Euro" ))
	  ||
	  ( !strcmp ( devise_compte -> nom_devise, "Euro" ) && devise -> passage_euro )))
    {
      /* c'est une devise �trang�re, on demande le taux de change et les frais de change */

      demande_taux_de_change ( devise_compte,
			       devise,
			       1,
			       (gdouble ) 0,
			       (gdouble ) 0 );

      operation -> taux_change = taux_de_change[0];
      operation -> frais_change = taux_de_change[1];

      if ( operation -> taux_change < 0 )
	{
	  operation -> taux_change = -operation -> taux_change;
	  operation -> une_devise_compte_egale_x_devise_ope = 1;
	}
    }


  /* r�cup�ration du type d'op�ration */

  if ( GTK_WIDGET_VISIBLE ( widget_formulaire_operations[9] ))
    {
      operation -> type_ope = GPOINTER_TO_INT ( gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ) -> menu_item ),
								      "no_type" ));
      if ( GTK_WIDGET_VISIBLE ( widget_formulaire_operations[10] )
	   &&
	   gtk_widget_get_style ( widget_formulaire_operations[10] ) == style_entree_formulaire[0] )
	{
	  struct struct_type_ope *type;

	  type = gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ) -> menu_item ),
				       "adr_type" );

	  operation -> contenu_type = g_strdup ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[10] ))));

	  if ( type -> numerotation_auto )
	    type -> no_en_cours = ( atoi ( operation -> contenu_type ));
	}
      else
	operation -> contenu_type = NULL;
    }
  else
    {
      operation -> type_ope = 0;
      operation -> contenu_type = NULL;
    }


  /* r�cup�ration du no d'exercice */

  operation -> no_exercice = GPOINTER_TO_INT ( gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( widget_formulaire_operations[11] ) -> menu_item ),
								     "no_exercice" ));


  /* r�cup�ration de l'imputation budg�taire */

  if ( gtk_widget_get_style ( GTK_COMBOFIX ( widget_formulaire_operations[12] ) -> entry ) == style_entree_formulaire[0] )
    {
      struct struct_imputation *imputation;
      
      pointeur_char = gtk_combofix_get_text ( GTK_COMBOFIX ( widget_formulaire_operations[12] ));

      tableau_char = g_strsplit ( pointeur_char,
				  ":",
				  2 );
      
      tableau_char[0] = g_strstrip ( tableau_char[0] );

      if ( tableau_char[1] )
	tableau_char[1] = g_strstrip ( tableau_char[1] );

      pointeur_liste = g_slist_find_custom ( liste_struct_imputation,
					     tableau_char[0],
					     ( GCompareFunc ) recherche_imputation_par_nom );

      if ( pointeur_liste )
	imputation = pointeur_liste -> data;
      else
	{
	  imputation = ajoute_nouvelle_imputation ( tableau_char[0] );

	  if ( operation -> montant < 0 )
	    imputation -> type_imputation = 1;
	  else
	    imputation -> type_imputation = 0;
	}

      operation -> imputation = imputation -> no_imputation;
	  
      if ( tableau_char[1] && strlen (tableau_char[1]) )
	{
	  struct struct_sous_imputation *sous_imputation;
		  
	  pointeur_liste = g_slist_find_custom ( imputation -> liste_sous_imputation,
						 tableau_char[1],
						 ( GCompareFunc ) recherche_sous_imputation_par_nom );
	      
	  if ( pointeur_liste )
	    sous_imputation = pointeur_liste -> data;
	  else
	    sous_imputation = ajoute_nouvelle_sous_imputation ( tableau_char[1],
								imputation );
		  
	  operation -> sous_imputation = sous_imputation -> no_sous_imputation;
	}
      else
	operation -> sous_imputation = 0;

      g_strfreev ( tableau_char );
    }
  else
    {
      operation -> imputation = 0;
      operation -> sous_imputation = 0;
    }

 /* r�cup�ration du no de pi�ce comptable */

  if ( gtk_widget_get_style ( widget_formulaire_operations[13] ) == style_entree_formulaire[0] )
    operation -> no_piece_comptable = g_strdup ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[13] ))));
  else
    operation -> no_piece_comptable = NULL;


  /* r�cup�ration des notes */

  if ( gtk_widget_get_style ( widget_formulaire_operations[15] ) == style_entree_formulaire[0] )
    operation -> notes = g_strdup ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[15] ))));
  else
    operation -> notes = NULL;

  /* r�cup�ration de l'info banque/guichet */

  if ( gtk_widget_get_style ( widget_formulaire_operations[16] ) == style_entree_formulaire[0] )
    operation -> info_banque_guichet = g_strdup ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[16] ))));
  else
    operation -> info_banque_guichet = NULL;



  /*   r�cup�ration des cat�gories / sous-cat�g, s'ils n'existent pas, on les cr�e */
  /* � mettre en dernier car si c'est une op� ventil�e, chaque op� de ventil va r�cup�rer les donn�es du dessus */

  virement = 0;

  if ( gtk_widget_get_style ( GTK_COMBOFIX ( widget_formulaire_operations[8] ) -> entry ) == style_entree_formulaire[0] )
    {
      struct struct_categ *categ;
      
      pointeur_char = gtk_combofix_get_text ( GTK_COMBOFIX ( widget_formulaire_operations[8] ));

      /* r�cup�ration de la ventilation si n�cessaire */

      if ( !strcmp ( g_strstrip ( pointeur_char ),
		     "Op�ration ventil�e" ))
	{
	  /* c'est une op�ration ventil�e : bcp de choses � faire : */
	  /* -faire le tour de chaque op� de ventilation et remplir les champs qu'on n'avait pu remplir avant (date,tiers,devise... ) */
	  /* -si une des op� de ventilation est un virement, il faut modifier aussi l'op�ration vir�e sur l'autre compte */

	  GSList *pointeur_tmp;

	  operation -> operation_ventilee = 1;
	  operation -> categorie = 0;
	  operation -> sous_categorie = 0;

	  /* fait le tour des op�s de ventilation pour modifier ce qui reste */

	  pointeur_tmp = gtk_object_get_data ( GTK_OBJECT ( formulaire ),
					       "liste_adr_ventilation" );

	  if ( pointeur_tmp != GINT_TO_POINTER ( -1 ))
	    {
	      while ( pointeur_tmp )
		{
		  struct structure_operation *ope_de_ventilation;

		  ope_de_ventilation = pointeur_tmp -> data;

		  /* on commence par mettre le no de l'op�ration en rapport qui lui sera attribu�e */

		  if ( operation -> no_operation )
		    ope_de_ventilation -> no_operation_ventilee_associee = operation -> no_operation;
		  else
		    ope_de_ventilation -> no_operation_ventilee_associee = no_derniere_operation + 1;


		  ope_de_ventilation -> jour = operation -> jour;
		  ope_de_ventilation -> mois = operation -> mois;
		  ope_de_ventilation -> annee = operation -> annee;
		  ope_de_ventilation -> date = g_date_new_dmy ( ope_de_ventilation -> jour,
								ope_de_ventilation -> mois,
								ope_de_ventilation -> annee );

		  ope_de_ventilation -> devise = operation -> devise;
		  ope_de_ventilation -> une_devise_compte_egale_x_devise_ope = operation -> une_devise_compte_egale_x_devise_ope;
		  ope_de_ventilation -> taux_change = operation -> taux_change;
		  ope_de_ventilation -> frais_change = operation -> frais_change;

		  ope_de_ventilation -> tiers = operation -> tiers;
		  ope_de_ventilation -> type_ope = operation -> type_ope;
		  ope_de_ventilation -> pointe = operation -> pointe;
		  ope_de_ventilation -> auto_man = operation -> auto_man;

		  /*   maintenant, si c'est un virement, on doit mettre � jour l'op� en relation */

		  if ( ope_de_ventilation -> relation_no_operation )
		    {
		      struct structure_operation *ope_virement;

		      p_tab_nom_de_compte_variable = p_tab_nom_de_compte + ope_de_ventilation -> relation_no_compte;

		      ope_virement = g_slist_find_custom ( LISTE_OPERATIONS,
							   GINT_TO_POINTER ( ope_de_ventilation -> relation_no_operation),
							   (GCompareFunc) recherche_operation_par_no ) -> data;

		      ope_virement -> jour = ope_de_ventilation -> jour;
		      ope_virement -> mois = ope_de_ventilation -> mois;
		      ope_virement -> annee = ope_de_ventilation -> annee;
		      ope_virement -> date = g_date_new_dmy ( ope_virement -> jour,
							      ope_virement -> mois,
							      ope_virement -> annee );

		      ope_virement -> devise = ope_de_ventilation -> devise;
		      ope_virement -> une_devise_compte_egale_x_devise_ope = operation -> une_devise_compte_egale_x_devise_ope;
		      ope_virement -> taux_change = operation -> taux_change;
		      ope_virement -> frais_change = operation -> frais_change;
		      ope_virement -> tiers = ope_de_ventilation -> tiers;
		      ope_virement -> auto_man = ope_de_ventilation -> auto_man;
		      ope_virement -> type_ope = ope_de_ventilation -> type_ope;

		      MISE_A_JOUR = 1;

		      mise_a_jour_solde ( ope_virement -> no_compte );

		      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;
		    }
		  pointeur_tmp = pointeur_tmp -> next;
		}
	    }
	}
      else
	{
	  tableau_char = g_strsplit ( pointeur_char,
				      ":",
				      2 );
      
	  tableau_char[0] = g_strstrip ( tableau_char[0] );

	  if ( tableau_char[1] )
	    tableau_char[1] = g_strstrip ( tableau_char[1] );


	  if ( strlen ( tableau_char[0] ) )
	    {
	      if ( !strcmp ( tableau_char[0],
			     "Virement" )
		   && tableau_char[1]
		   && strlen ( tableau_char[1]) )
		{
		  /* c'est un virement, il n'y a donc aucune cat�torie */

		  gint i;

		  operation -> categorie = 0;
		  operation -> sous_categorie = 0;

		  /* s�pare entre virement vers un compte et virement vers un compte supprim� */

		  if ( strcmp ( tableau_char[1],
				"Compte supprim�" ) )
		    {
		      virement = 1;

		      /* recherche le no de compte du virement */

		      p_tab_nom_de_compte_variable = p_tab_nom_de_compte;

		      compte_virement = -1;

		      for ( i = 0 ; i < nb_comptes ; i++ )
			{
			  if ( !g_strcasecmp ( NOM_DU_COMPTE,
					       tableau_char[1] ) )
			    compte_virement = i;
			  p_tab_nom_de_compte_variable++;
			}

		      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

		      if ( compte_virement == -1 )
			{
			  dialogue ( "Erreur : le compte associ� au virement est invalide" );
			  if ( !operation -> no_operation )
			    free ( operation );
			  return;
			}
		    }
		  else
		    {
		      /* c'est un virement vers un compte supprim� */

		      operation -> relation_no_compte = -1;
		      operation -> relation_no_operation = 1;
		    }
		}
	      else
		{
		  /* c'est une cat�g normale, si c'est une modif d'op�, v�rifier si ce n'�tait pas une ventilation */

		  if ( operation -> no_operation
		       &&
		       operation -> operation_ventilee )
		    {
		      /* c'�tait une op� ventil�e et cela ne l'est plus, il faut virer toutes les */
		      /* 	op�s de ventilations associ�es � cette op� */

		      GSList *liste_tmp;

		      liste_tmp = LISTE_OPERATIONS;

		      while ( liste_tmp )
			{
			  struct structure_operation *ope_tmp;
		      
			  ope_tmp = liste_tmp -> data;

			  if ( ope_tmp -> no_operation_ventilee_associee == operation -> no_operation )
			    {
			      liste_tmp = liste_tmp -> next;
			      supprime_operation ( ope_tmp );
			    }
			  else
			    liste_tmp = liste_tmp -> next;
			}
		      operation -> operation_ventilee = 0;
		    }


		  pointeur_liste = g_slist_find_custom ( liste_struct_categories,
							 tableau_char[0],
							 ( GCompareFunc ) recherche_categorie_par_nom );

		  if ( pointeur_liste )
		    categ = pointeur_liste -> data;
		  else
		    {
		      categ = ajoute_nouvelle_categorie ( tableau_char[0] );
		      if ( operation -> montant < 0 )
			categ -> type_categ = 1;
		      else
			categ -> type_categ = 0;
		    }

		  operation -> categorie = categ -> no_categ;
	  
		  if ( tableau_char[1] && strlen (tableau_char[1]) )
		    {
		      struct struct_sous_categ *sous_categ;
		  
		      pointeur_liste = g_slist_find_custom ( categ -> liste_sous_categ,
							     tableau_char[1],
							     ( GCompareFunc ) recherche_sous_categorie_par_nom );
	      
		      if ( pointeur_liste )
			sous_categ = pointeur_liste -> data;
		      else
			sous_categ = ajoute_nouvelle_sous_categorie ( tableau_char[1],
								      categ );
		  
		      operation -> sous_categorie = sous_categ -> no_sous_categ;
		    }
		  else
		    operation -> sous_categorie = 0;
		}
	    }
	  else
	    {
	      operation -> categorie = 0;
	      operation -> sous_categorie = 0;
	    }
	  g_strfreev ( tableau_char );
	}
    }
  else
    {
      operation -> categorie = 0;
      operation -> sous_categorie = 0;
    }
 

  /*   on a fini de remplir l'op�, on peut l'ajouter � la liste */

  ajout_operation ( operation );


  /* si on est en train d'�quilibrer => recalcule le total point� */

  if ( etat.equilibrage )
    {
      GSList *pointeur_liste_ope;
      struct struct_devise *devise_compte;
      struct struct_devise *devise_operation;

      p_tab_nom_de_compte_variable =  p_tab_nom_de_compte_courant;

      /* r�cup�re l'adr de la devise du compte */

      devise_compte = g_slist_find_custom ( liste_struct_devises,
					    GINT_TO_POINTER ( DEVISE ),
					    ( GCompareFunc ) recherche_devise_par_no) -> data;

      pointeur_liste_ope = LISTE_OPERATIONS;
      operations_pointees = 0;

      while ( pointeur_liste_ope )
	{
	  struct structure_operation *operation;

	  operation = pointeur_liste_ope -> data;

	  if ( operation -> pointe == 1 )
	    {
	      gdouble montant;

	      if ( operation -> devise == DEVISE )
		montant = operation -> montant;
	      else
		{
		  /* ce n'est pas la devise du compte, si le compte passe � l'euro et que la devise est l'euro, utilise la conversion du compte, */
		  /* si c'est une devise qui passe � l'euro et que la devise du compte est l'euro, utilise la conversion du compte */
		  /* sinon utilise la conversion stock�e dans l'op� */
	      
		  devise_operation = g_slist_find_custom ( liste_struct_devises,
							   GINT_TO_POINTER ( operation -> devise ),
							   ( GCompareFunc ) recherche_devise_par_no ) -> data;
	      
		  if ( devise_compte -> passage_euro
		       &&
		       !strcmp ( devise_operation -> nom_devise, "Euro" ) )
		    montant = operation -> montant * devise_compte -> change - operation -> frais_change;
		  else
		    if ( devise_operation -> passage_euro
			 &&
			 !strcmp ( devise_compte -> nom_devise, "Euro" ))
		      montant = operation -> montant / devise_operation -> change;
		    else
		      if ( operation -> une_devise_compte_egale_x_devise_ope )
			montant = operation -> montant / operation -> taux_change - operation -> frais_change;
		      else
			montant = operation -> montant * operation -> taux_change - operation -> frais_change;
		  montant = ( rint (montant * 100 )) / 100;
		}
	      operations_pointees = operations_pointees + montant;
	    }

	  pointeur_liste_ope = pointeur_liste_ope -> next;
	}

      gtk_label_set_text ( GTK_LABEL ( label_equilibrage_pointe ),
			   g_strdup_printf ( "%4.2f", 
					     operations_pointees ));

      if ( fabs ( solde_final - solde_initial - operations_pointees ) < 0.01 )
	{
	  gtk_label_set_text ( GTK_LABEL ( label_equilibrage_ecart ),
			       g_strdup_printf ( "%4.2f",
						 0.0 ));
	  gtk_widget_set_sensitive ( GTK_WIDGET ( bouton_ok_equilibrage ),
				     TRUE );
	}
      else
	{
	  gtk_label_set_text ( GTK_LABEL ( label_equilibrage_ecart ),
			       g_strdup_printf ( "%4.2f",
						 solde_final - solde_initial - operations_pointees ));
	  gtk_widget_set_sensitive ( GTK_WIDGET ( bouton_ok_equilibrage ),
				     FALSE );
	}
    }


  /*   si c'�tait un virement, on cr�e une copie de l'op�, on l'ajout � la liste puis on remplit les relations */

  if ( virement )
    {    
      /* si c'�tait une nvelle op�, les relations sont nulles, sinon r�cup�re l'op� correspondante */

      struct struct_devise *devise_compte_2;

      /* 	si en modifiant le virement on a modifi� le compte associ�, on supprime l'op�ration associ�e */
      /* et en recr�e une nouvelle */

      if ( operation -> relation_no_operation )
	{
	  /* c'est une modif de virement, recherche l'op� associ�e */

	  p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> relation_no_compte;

	  operation_2 = g_slist_find_custom ( LISTE_OPERATIONS,
					      GINT_TO_POINTER ( operation -> relation_no_operation ),
					      ( GCompareFunc ) recherche_operation_par_no ) -> data;

	  /* 	si en modifiant le virement on a modifi� le compte associ�, on supprime l'op�ration associ�e */
	  /* et en recr�e une nouvelle */

	  if ( operation -> relation_no_compte != compte_virement )
	    {
	      operation_2 -> relation_no_operation = 0;
	      supprime_operation ( operation_2 );

	      operation_2 = calloc ( 1,
				     sizeof ( struct structure_operation ) );
	      operation_2 -> no_compte = compte_virement;

	      /* il faudra r�afficher la liste en cours */

	      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;
	      MISE_A_JOUR = 1;
	    }
	}
      else
	{
	  operation_2 = calloc ( 1,
				 sizeof ( struct structure_operation ) );
	  operation_2 -> no_compte = compte_virement;
	}
      
      p_tab_nom_de_compte_variable = p_tab_nom_de_compte + compte_virement;

      /* remplit la nouvelle op� */
      
      operation_2 -> jour = operation -> jour;
      operation_2 -> mois = operation -> mois;
      operation_2 -> annee = operation -> annee;
      operation_2 ->date = g_date_new_dmy ( operation_2->jour,
					    operation_2->mois,
					    operation_2->annee);
      operation_2 -> montant = -operation -> montant;

      /* si c'est la devise du compte ou si c'est un compte qui doit passer � l'euro ( la transfo se fait au niveau */
      /* de l'affichage de la liste ) ou si c'est un compte en euro et l'op� est dans une devise qui doit passer � l'euro -> ok */

      devise_compte_2 = g_slist_find_custom ( liste_struct_devises,
					      GINT_TO_POINTER ( DEVISE ),
					      ( GCompareFunc ) recherche_devise_par_no ) -> data;

      operation_2 -> devise = operation -> devise;

      if ( !( operation_2-> no_operation
	      ||
	      devise -> no_devise == DEVISE
	      ||
	      ( devise_compte_2 -> passage_euro && !strcmp ( devise -> nom_devise, "Euro" ))
	      ||
	      ( !strcmp ( devise_compte_2 -> nom_devise, "Euro" ) && devise -> passage_euro )))
	{
	  /* c'est une devise �trang�re, on demande le taux de change et les frais de change */
	  
	  demande_taux_de_change ( devise_compte_2,
				   devise,
				   1,
				   (gdouble ) 0,
				   (gdouble ) 0 );

	  operation_2 -> taux_change = taux_de_change[0];
	  operation_2 -> frais_change = taux_de_change[1];

	  if ( operation_2 -> taux_change < 0 )
	    {
	      operation_2 -> taux_change = -operation_2 -> taux_change;
	      operation_2 -> une_devise_compte_egale_x_devise_ope = 1;
	    }
	}
      else
	{
	  operation_2 -> taux_change = 0;
	  operation_2 -> frais_change = 0;
	}

      operation_2 -> tiers = operation -> tiers;
      operation_2 -> categorie = operation -> categorie;
      operation_2 -> sous_categorie = operation -> sous_categorie;

      if ( operation -> notes )
	operation_2 -> notes = g_strdup ( operation -> notes);
      operation_2 -> auto_man = operation -> auto_man;
      operation_2 -> type_ope = operation -> type_ope;

      if ( operation -> contenu_type )
	operation_2 -> contenu_type = g_strdup ( operation -> contenu_type );

      operation_2 -> no_exercice = operation -> no_exercice;
      operation_2 -> imputation = operation -> imputation;
      operation_2 -> sous_imputation = operation -> sous_imputation;

      if ( operation -> no_piece_comptable )
      operation_2 -> no_piece_comptable = g_strdup ( operation -> no_piece_comptable );

      if ( operation -> info_banque_guichet )
      operation_2 -> info_banque_guichet = g_strdup ( operation -> info_banque_guichet );

      /*   on a fini de remplir l'op�, on peut l'ajouter � la liste */

      ajout_operation ( operation_2 );


      /* on met maintenant les relations entre les diff�rentes op� */

      operation -> relation_no_operation = operation_2 -> no_operation;
      operation -> relation_no_compte = operation_2 -> no_compte;
      operation_2 -> relation_no_operation = operation -> no_operation;
      operation_2 -> relation_no_compte = operation -> no_compte;


      verification_mise_a_jour_liste ();


      /* on met � jour le compte courant pour le virement ( il a �t� mis � jour avec ajout op�ration, mais sans les liens de virement ) */

      p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> no_compte;

      MISE_A_JOUR = 1;
      verification_mise_a_jour_liste ();
    }
  else
    {
      if ( operation -> relation_no_operation )
	{
	  /* c'�tait un virement, et ce ne l'est plus, donc on efface l'op� en relation */

	  p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> relation_no_compte;

	  operation_2 = g_slist_find_custom ( LISTE_OPERATIONS,
					      GINT_TO_POINTER ( operation -> relation_no_operation ),
					      ( GCompareFunc ) recherche_operation_par_no ) -> data;
	  
	  operation_2 -> relation_no_operation = 0;
	  MISE_A_JOUR = 1;

	  supprime_operation ( operation_2 );


	  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

	  operation -> relation_no_operation = 0;
	  operation -> relation_no_compte = 0;
	}
    }


  /* si c'�tait une nouvelle op�ration, on efface le formulaire, on remet la date pour la suivante, */
  /* si c'�tait une modification, on redonne le focus � la liste */

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> no_compte;

  etat.formulaire_en_cours = 0;

  if ( gtk_object_get_data ( GTK_OBJECT ( formulaire ),
			     "adr_struct_ope" ) )
    {
      gtk_widget_grab_focus ( GTK_WIDGET ( CLIST_OPERATIONS ));

      formulaire_a_zero ();

      if ( !etat.formulaire_toujours_affiche )
	gtk_widget_hide ( frame_droite_bas );
    }
  else
    {
      formulaire_a_zero ();

      clique_champ_formulaire ( widget_formulaire_operations[1],
				NULL,
				GINT_TO_POINTER ( 1 ));
      gtk_entry_select_region ( GTK_ENTRY (  widget_formulaire_operations[1]),
				0,
				-1);
      gtk_widget_grab_focus ( GTK_WIDGET ( widget_formulaire_operations[1] ));

      etat.formulaire_en_cours = 1;
    }


  /*   met � jour les listes ( nouvelle op�ration associ�e au tiers et � la cat�gorie ) */

  mise_a_jour_tiers ();
  mise_a_jour_categ ();
  mise_a_jour_imputation ();

  modification_fichier ( TRUE );
 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 /* Fonction ajout_operation */
 /* ajoute l'op�ration donn�e en argument � la liste des op�s, trie la liste */
 /* et r�affiche la clist */
 /* remplit si n�cessaire le no d'op�ration */
 /***********************************************************************************************************/

 void ajout_operation ( struct structure_operation *operation )
 {
  gpointer **save_ptab;

  save_ptab = p_tab_nom_de_compte_variable;

  /* si c'est une nouvelle op�ration, on lui met son no d'op�ration */

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> no_compte;

  if ( !operation -> no_operation )
    {
      operation -> no_operation = ++no_derniere_operation;

      LISTE_OPERATIONS = g_slist_append ( LISTE_OPERATIONS,
					  operation );
      NB_OPE_COMPTE++;
    }

  /*   on r�affiche(ra) la liste des op�s */

  MISE_A_JOUR = 1;

  if ( operation -> no_compte == compte_courant )
    verification_mise_a_jour_liste ();

  mise_a_jour_solde ( operation -> no_compte );

  /* on r�affiche les comptes de l'accueil */

  update_liste_comptes_accueil();
  mise_a_jour_soldes_minimaux();
  mise_a_jour_fin_comptes_passifs ();

  p_tab_nom_de_compte_variable = save_ptab;
 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 /* cette proc�dure compare 2 struct de compte entre elles au niveau de la date */
 /* pour le classement, si la d�te est �quivalente, elle les classe par le no d'op� */
 /***********************************************************************************************************/

 gint comparaison_date_list_ope ( struct structure_operation *ope_1,
				 struct structure_operation *ope_2)
 {
  gint retour;

  if ( !( retour = g_date_compare ( ope_1 -> date, ope_2 -> date) ) )
    retour = ope_1 -> no_operation - ope_2 -> no_operation;

  return ( retour );
 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 /* efface le contenu du formulaire */
 /***********************************************************************************************************/

 void formulaire_a_zero (void)
 {

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;


  /* on met les styles des entr�es au gris */

  gtk_widget_set_style (widget_formulaire_operations[1],
			style_entree_formulaire[1] );
  gtk_widget_set_style (GTK_COMBOFIX ( widget_formulaire_operations[2])->entry,
			style_entree_formulaire[1] );
  gtk_widget_set_style (widget_formulaire_operations[3],
			style_entree_formulaire[1] );
  gtk_widget_set_style (widget_formulaire_operations[4],
			style_entree_formulaire[1] );
  gtk_widget_set_style (widget_formulaire_operations[7],
			style_entree_formulaire[1] );
  gtk_widget_set_style (GTK_COMBOFIX ( widget_formulaire_operations[8])->entry,
			style_entree_formulaire[1] );
  gtk_widget_set_style (widget_formulaire_operations[10],
			style_entree_formulaire[1] );
  gtk_widget_set_style (GTK_COMBOFIX ( widget_formulaire_operations[12])->entry,
			style_entree_formulaire[1] );
  gtk_widget_set_style (widget_formulaire_operations[13],
			style_entree_formulaire[1] );
  gtk_widget_set_style (widget_formulaire_operations[15],
			style_entree_formulaire[1] );
  gtk_widget_set_style (widget_formulaire_operations[16],
			style_entree_formulaire[1] );

  gtk_label_set_text ( GTK_LABEL ( widget_formulaire_operations[0]),
		       "" );

  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[1]),
		       "Date" );

  gtk_combofix_set_text ( GTK_COMBOFIX ( widget_formulaire_operations[2] ),
			  "Tiers" );
  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[3]),
		       "D�bit" );
  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[4]),
		       "Cr�dit" );

  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[7]),
		       "Date de valeur" );
  gtk_combofix_set_text ( GTK_COMBOFIX (widget_formulaire_operations[8]),
			  "Cat�gories : Sous-cat�gories" );
  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[10]),
		       "n� Ch�que/Virement" );

  gtk_combofix_set_text ( GTK_COMBOFIX (widget_formulaire_operations[12]),
			  "Imputation budg�taire" );
  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[13]),
		       "Pi�ce comptable" );

  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[15]),
		       "Notes" );
  gtk_entry_set_text ( GTK_ENTRY (widget_formulaire_operations[16]),
		       "Informations banque/guichet" );

  gtk_widget_hide ( widget_formulaire_operations[6] );
/*   gtk_widget_hide ( widget_formulaire_operations[7] ); */
  gtk_widget_hide ( widget_formulaire_operations[14] );
  gtk_widget_hide ( widget_formulaire_operations[17] );

  gtk_widget_set_sensitive ( widget_formulaire_operations[4],
			     TRUE );
  gtk_widget_set_sensitive ( widget_formulaire_operations[3],
			     TRUE );

  gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[9] ),
				cherche_no_menu_type ( TYPE_DEFAUT_DEBIT ) );
  gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[5] ),
				g_slist_position ( liste_struct_devises,
						   g_slist_find_custom ( liste_struct_devises,
									 GINT_TO_POINTER ( DEVISE ),
									 ( GCompareFunc ) recherche_devise_par_no )));
/*   gtk_option_menu_set_history ( GTK_OPTION_MENU ( widget_formulaire_operations[7] ), */
/* 				compte_courant ); */

  gtk_widget_set_sensitive ( GTK_WIDGET ( widget_formulaire_operations[9] ),
			     FALSE );
  gtk_widget_set_sensitive ( GTK_WIDGET ( widget_formulaire_operations[5] ),
			     FALSE );
/*   gtk_widget_set_sensitive ( GTK_WIDGET ( widget_formulaire_operations[7] ), */
/* 			     FALSE ); */
  gtk_widget_set_sensitive ( GTK_WIDGET ( widget_formulaire_operations[11] ),
			     FALSE );
  gtk_widget_set_sensitive ( GTK_WIDGET ( hbox_valider_annuler_ope ),
			     FALSE );


  gtk_object_set_data ( GTK_OBJECT ( formulaire ),
			"adr_struct_ope",
			NULL );
  gtk_object_set_data ( GTK_OBJECT ( formulaire ),
			"liste_adr_ventilation",
			NULL );

  gtk_widget_set_sensitive ( bouton_affiche_cache_formulaire,
			     TRUE );

 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 /* proc�dure qui renvoie soit la d�te du jour, soit l'ancienne date tap�e sous forme de string */
 /***********************************************************************************************************/

 gchar *date_jour ( void )
 {
  time_t temps;

  if ( !etat.ancienne_date )
    {
      time ( &temps );

      strftime ( date_courante,
		 11,
		 "%d/%m/%Y",
		 localtime ( &temps ) );
      etat.ancienne_date = 1; 
    }

  return ( g_strdup ( date_courante ) );

 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 /* Fonction affiche_cache_le_formulaire */
 /* si le formulaire �tait affich�, le cache et vice-versa */
 /***********************************************************************************************************/

 void affiche_cache_le_formulaire ( void )
 {
  gpointer **save_ptab;

  save_ptab = p_tab_nom_de_compte_variable;
  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;



  if ( etat.formulaire_toujours_affiche )
    {
      gtk_widget_hide ( fleche_bas );
      gtk_widget_show ( fleche_haut );

      gtk_widget_hide ( frame_droite_bas );
      etat.formulaire_toujours_affiche = 0;

    }
  else
    {
      gtk_widget_hide ( fleche_haut );
      gtk_widget_show ( fleche_bas );

      gtk_widget_show ( frame_droite_bas );
      etat.formulaire_toujours_affiche = 1;

    }

  gtk_widget_grab_focus ( CLIST_OPERATIONS );

  p_tab_nom_de_compte_variable = save_ptab;

 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 /* Fonction  allocation_taille_formulaire */
 /* appel�e quand le formulaire est affich�*/
 /* modifie la liste pour qu'il n'y ai pas de changement */
 /***********************************************************************************************************/

 void allocation_taille_formulaire ( GtkWidget *widget,
				    gpointer null )
 {
  GtkAdjustment *ajustement;
  gpointer **save_ptab;
  return;
  save_ptab = p_tab_nom_de_compte_variable;

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  if ( !GTK_WIDGET_VISIBLE ( notebook_general ))
    return;

  ajustement = gtk_clist_get_vadjustment ( GTK_CLIST ( CLIST_OPERATIONS ) );

  /*   if ( ajustement -> value == ( ajustement -> upper - ajustement -> page_size )) */
  /*        return; */

  gtk_adjustment_set_value ( GTK_ADJUSTMENT ( ajustement ),
			     ajustement -> value + GTK_WIDGET ( frame_droite_bas ) -> allocation.height + 10 );

  p_tab_nom_de_compte_variable = save_ptab;

 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 /* Fonction efface_formulaire */
 /* appel�e quand le formulaire est effac�*/
 /* modifie la liste pour qu'il n'y ai pas de changement */
 /***********************************************************************************************************/

 void efface_formulaire ( GtkWidget *widget,
			 gpointer null )
 {
  GtkAdjustment *ajustement;
  gpointer **save_ptab;

  save_ptab = p_tab_nom_de_compte_variable;
  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  ajustement = gtk_clist_get_vadjustment ( GTK_CLIST ( CLIST_OPERATIONS ) );

  gtk_adjustment_set_value ( GTK_ADJUSTMENT ( ajustement ),
			     ajustement -> value - GTK_WIDGET ( frame_droite_bas ) -> allocation.height - 10 );


  p_tab_nom_de_compte_variable = save_ptab;

 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 /* Fonction basculer_vers_ventilation */
 /* appel�e par l'appui du bouton Ventilation... */
 /* permet de voir les op�s ventil�es d'une ventilation */
 /***********************************************************************************************************/

 void basculer_vers_ventilation ( GtkWidget *bouton,
				 gpointer null )
 {
  enregistre_ope_au_retour = 0;

  if ( gtk_widget_get_style ( widget_formulaire_operations[3] ) == style_entree_formulaire[0] )
    ventiler_operation ( g_strtod ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[3] ))),
				    NULL ),
			 0 );
  else
    ventiler_operation ( g_strtod ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( widget_formulaire_operations[4] ))),
				    NULL ),
			 1 );
 }
 /***********************************************************************************************************/



 /***********************************************************************************************************/
 /* Fonction click_sur_bouton_voir_change  */
 /* permet de modifier un change �tabli pour une op�ration */
 /***********************************************************************************************************/

 void click_sur_bouton_voir_change ( void )
 {
  struct structure_operation *operation;
  struct struct_devise *devise_compte;
  struct struct_devise *devise;

  gtk_widget_grab_focus ( widget_formulaire_operations[1] );

  operation = gtk_object_get_data ( GTK_OBJECT ( formulaire ),
				    "adr_struct_ope" );


  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  devise_compte = g_slist_find_custom ( liste_struct_devises,
					GINT_TO_POINTER ( DEVISE ),
					( GCompareFunc ) recherche_devise_par_no ) -> data;
  devise = g_slist_find_custom ( liste_struct_devises,
				 GINT_TO_POINTER ( operation -> devise ),
				 ( GCompareFunc ) recherche_devise_par_no ) -> data;

  demande_taux_de_change ( devise_compte,
			   devise,
			   operation -> une_devise_compte_egale_x_devise_ope,
			   operation -> taux_change,
			   operation -> frais_change );
      
  if ( taux_de_change[0] ||  taux_de_change[1] )
    {
      operation -> taux_change = taux_de_change[0];
      operation -> frais_change = taux_de_change[1];
	  
      if ( operation -> taux_change < 0 )
	{
	  operation -> taux_change = -operation -> taux_change;
	  operation -> une_devise_compte_egale_x_devise_ope = 1;
	}
      else
	operation -> une_devise_compte_egale_x_devise_ope = 0;
    }

 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 void degrise_formulaire_operations ( void )
 {

  gtk_widget_set_sensitive ( GTK_WIDGET ( widget_formulaire_operations[9] ),
			     TRUE );
/*   gtk_widget_set_sensitive ( GTK_WIDGET ( widget_formulaire_operations[7] ), */
/* 			     TRUE ); */
  gtk_widget_set_sensitive ( GTK_WIDGET ( widget_formulaire_operations[11] ),
			     TRUE );
  gtk_widget_set_sensitive ( GTK_WIDGET ( widget_formulaire_operations[5] ),
			     TRUE );
  gtk_widget_set_sensitive ( GTK_WIDGET ( hbox_valider_annuler_ope ),
			     TRUE );
 }
 /***********************************************************************************************************/




 /***********************************************************************************************************/
 /* appel�e lors de l'appui des touche + ou - sur les formulaires */
 /* augmente ou diminue la date entr�e de 1 */
 /***********************************************************************************************************/

 void incremente_decremente_date ( GtkWidget *entree,
				  gint demande )
 {
  gchar **tableau_char;
  GDate *date;
  gint jour, mois, annee;

  /* on commence par v�rifier que la date est valide */

  if ( !modifie_date ( entree ))
    return;

  tableau_char = g_strsplit ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY (entree))),
			      "/",
			      3 );

  jour = strtod ( tableau_char[0],
		  NULL );
  mois = strtod ( tableau_char[1],
		  NULL );
  annee = strtod (tableau_char[2],
		  NULL );

  date = g_date_new_dmy ( jour,
			  mois,
			  annee);

  g_date_add_days ( date,
		    demande );

  gtk_entry_set_text ( GTK_ENTRY (entree),
		       g_strdup_printf ( "%d/%d/%d",
					 g_date_day ( date ),
					 g_date_month ( date ),
					 g_date_year ( date )));

 }
 /***********************************************************************************************************/
