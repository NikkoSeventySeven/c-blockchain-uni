#include "ui.h"
#include "miner.h"
#include <stdlib.h>

#include <gtk/gtk.h>

#define APP_ID "nikko77.io3.blockchain"

static GtkWidget *window, *controlPanel;
static GtkApplication* app;

static GtkWidget *txt_block, *txt_nonce, *txt_hash;
static GtkTextBuffer* txtbuf_data;

static GtkWidget *scale_difficulty;

static void hashAll()
{
    GtkTextIter iterStart, iterEnd;
    gtk_text_buffer_get_start_iter( GTK_TEXT_BUFFER( txtbuf_data ), &iterStart );
    gtk_text_buffer_get_end_iter( GTK_TEXT_BUFFER( txtbuf_data ), &iterEnd );
    char* textData = gtk_text_buffer_get_text( txtbuf_data, &iterStart, &iterEnd, gtk_true() );
    unsigned char* hashData = hash(
        strtol( gtk_entry_get_text( GTK_ENTRY(txt_block) ), 0, 10 ),
        strtol( gtk_entry_get_text( GTK_ENTRY(txt_nonce) ), 0, 10 ),
        textData );
    char hashString[65];
    sprintf( hashString,
        "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx"
        "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx"
        "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx"
        "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
        hashData[0], hashData[1], hashData[2], hashData[3],
        hashData[4], hashData[5], hashData[6], hashData[7],
        hashData[8], hashData[9], hashData[10], hashData[11],
        hashData[12], hashData[13], hashData[14], hashData[15],
        hashData[16], hashData[17], hashData[18], hashData[19],
        hashData[20], hashData[21], hashData[22], hashData[23],
        hashData[24], hashData[25], hashData[26], hashData[27],
        hashData[28], hashData[29], hashData[30], hashData[31]);

    hashString[64] = '\0';
    gtk_entry_set_text( GTK_ENTRY( txt_hash ), hashString );

    free( textData );
    free( hashData );
}

static void onDataChanged( GtkWidget* widget, GdkEvent* event, gpointer data )
{
    hashAll();
}

static void minar()
{
    int nonce;

    GtkTextIter iterStart, iterEnd;
    gtk_text_buffer_get_start_iter( GTK_TEXT_BUFFER( txtbuf_data ), &iterStart );
    gtk_text_buffer_get_end_iter( GTK_TEXT_BUFFER( txtbuf_data ), &iterEnd );
    char* textData = gtk_text_buffer_get_text( txtbuf_data, &iterStart, &iterEnd, gtk_true() );
    nonce = mine(
        strtol( gtk_entry_get_text( GTK_ENTRY(txt_block) ), 0, 10 ),
        textData, gtk_range_get_value( GTK_RANGE( scale_difficulty ) ) );

    char nonceString[16];
    sprintf( nonceString, "%d", nonce );
    gtk_entry_set_text( GTK_ENTRY( txt_nonce ), nonceString );

    free( textData );

    hashAll();
}

static void onMinar( GtkButton* button, gpointer data )
{
    minar();
}

static void onClose( GtkWidget* widget, gpointer data )
{
    gtk_window_close( GTK_WINDOW(controlPanel) );
    gtk_window_close( GTK_WINDOW(window) );
}

static void buildWindow()
{
    // Create Window UI

    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing( GTK_GRID(grid), 6 );
    gtk_grid_set_column_spacing( GTK_GRID(grid), 6 );
    gtk_container_add( GTK_CONTAINER(window), grid );

    GtkWidget* label1 = gtk_label_new("Bloque N°: ");
    gtk_label_set_xalign(GTK_LABEL(label1), 1.0f);

    txt_block = gtk_entry_new();
    gtk_entry_set_text( GTK_ENTRY(txt_block), "1");

    GtkWidget* label2 = gtk_label_new("Nonce: ");
    gtk_label_set_xalign(GTK_LABEL(label2), 1.0f);

    txt_nonce = gtk_entry_new();
    gtk_entry_set_text( GTK_ENTRY(txt_nonce), "0");

    GtkWidget* label3 = gtk_label_new("Datos: ");
    gtk_label_set_xalign(GTK_LABEL(label3), 0.0f);

    GtkWidget* txtview_data = gtk_text_view_new();
    txtbuf_data = gtk_text_view_get_buffer( GTK_TEXT_VIEW(txtview_data) );
    gtk_widget_set_size_request( txtview_data, 550, 200 );
    gtk_container_set_border_width( GTK_CONTAINER(txtview_data), 4 );
    gtk_text_view_set_wrap_mode( GTK_TEXT_VIEW(txtview_data), GTK_WRAP_WORD_CHAR );

    GtkWidget* label4 = gtk_label_new("Hash: ");
    gtk_label_set_xalign(GTK_LABEL(label4), 0.0f);

    txt_hash = gtk_entry_new();
    static GValue tmp_false = G_VALUE_INIT;
    g_value_init( &tmp_false, G_TYPE_INT );
    g_value_set_int( &tmp_false, 0 );
    g_object_set_property( G_OBJECT(txt_hash), "editable", &tmp_false );
    g_value_unset( &tmp_false );

    GtkWidget* btn_minar = gtk_button_new_with_label("Minar");

    g_signal_connect( G_OBJECT(btn_minar), "clicked", G_CALLBACK(onMinar), NULL );

    gtk_grid_attach( GTK_GRID(grid), label1, 0, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID(grid), label2, 0, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID(grid), label3, 0, 2, 2, 1 );
    gtk_grid_attach( GTK_GRID(grid), label4, 0, 4, 2, 1 );

    gtk_grid_attach( GTK_GRID(grid), txt_block, 1, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID(grid), txt_nonce, 1, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID(grid), txtview_data, 0, 3, 2, 1 );
    gtk_grid_attach( GTK_GRID(grid), txt_hash, 0, 5, 2, 1 );

    gtk_grid_attach( GTK_GRID(grid), btn_minar, 0, 6, 2, 1 );

    g_signal_connect( txt_block, "key-release-event", G_CALLBACK(onDataChanged), NULL );
    g_signal_connect( txt_hash, "key-release-event", G_CALLBACK(onDataChanged), NULL );
    g_signal_connect( txt_nonce, "key-release-event", G_CALLBACK(onDataChanged), NULL );
    g_signal_connect( txtview_data, "key-release-event", G_CALLBACK(onDataChanged), NULL );

    // Create Control Panel UI

    GtkWidget *grid2 = gtk_grid_new();
    gtk_grid_set_row_spacing( GTK_GRID(grid2), 6 );
    gtk_grid_set_column_spacing( GTK_GRID(grid2), 6 );
    gtk_container_add( GTK_CONTAINER(controlPanel), grid2 );

    GtkWidget *clabel1 = gtk_label_new("Dificultad: ");
    gtk_label_set_xalign( GTK_LABEL(clabel1), 1.0f );

    scale_difficulty = gtk_scale_new_with_range( GTK_ORIENTATION_HORIZONTAL, 1, 8, 1 );
    gtk_range_set_value( GTK_RANGE(scale_difficulty), 4 );
    gtk_widget_set_size_request( scale_difficulty, 150, 10 );

    gtk_grid_attach( GTK_GRID(grid2), clabel1, 0,0,1,1 );
    gtk_grid_attach( GTK_GRID(grid2), scale_difficulty, 1,0,1,1 );

    gtk_widget_grab_focus( btn_minar );
}

static void windowActivate( GtkApplication* local_app, gpointer data )
{
    window = gtk_application_window_new( local_app );
    gtk_window_set_resizable( GTK_WINDOW(window), gtk_false() );
    gtk_container_set_border_width( GTK_CONTAINER(window), 12 );

    controlPanel = gtk_application_window_new( local_app );
    gtk_window_set_resizable( GTK_WINDOW(controlPanel), gtk_false() );
    gtk_container_set_border_width( GTK_CONTAINER(controlPanel), 12 );
    gtk_window_set_title( GTK_WINDOW( controlPanel ), "Panel de Configuración" );

    buildWindow();

    g_signal_connect( window, "destroy", G_CALLBACK( onClose ), NULL );
    g_signal_connect( controlPanel, "destroy", G_CALLBACK( onClose ), NULL );

    gtk_widget_show_all( window );
    gtk_widget_show_all( controlPanel );

    hashAll();
}

void initUI()
{
    app = gtk_application_new( APP_ID, G_APPLICATION_FLAGS_NONE );
    g_signal_connect( app, "activate", G_CALLBACK( windowActivate ), NULL );

}

int runApp( int argc, char** argv )
{
    int status;
    status = g_application_run( G_APPLICATION( app ), argc, argv );
    g_object_unref( app );
    return status;
}
