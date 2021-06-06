#include <gtk/gtk.h>
#include "inc/btree.h"
#include <stdio.h>
#include <stdlib.h>



//Luu dinh vao Btree
BTA *btfile;


//Khai bao cac widget
GtkWidget *g_from;
GtkWidget *g_to;
GtkWidget *g_search_button;
GtkWidget *g_view;
GtkTextBuffer *text_buffer;
GObject *completion, *list_store;

//Cac ham lien quan den cac widget
void seach_button_clicked(){

}

int main(int argc, char *argv[])
{
    char str[100000];
    int i=0;
    //Khoi tao Btree
    btinit();
    //Doc het tat ca ID vao
    btfile=btopn("ID_data",0,0);
    if (btfile==NULL){
        btfile=btcrt("ID_data",0,0);
        FILE *f=fopen("id.txt","r");
        while (i<555){
            fgets(str,10000,f);
            if (str[strlen(str)-1]=='\n') str[strlen(str)-1]='\0';
            btins(btfile,strdup(str),"",1);
            i++;
        }
        fclose(f);
    }



    GtkBuilder      *builder;
    GtkWidget       *window;

    //Khoi tao giao dien
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/window_main.glade");

    g_from=GTK_WIDGET(gtk_builder_get_object(builder,"g_from"));
    g_to=GTK_WIDGET(gtk_builder_get_object(builder,"g_to"));
    g_search_button=GTK_WIDGET(gtk_builder_get_object(builder,"search_button"));
    g_view=GTK_WIDGET(gtk_builder_get_object(builder,"view"));
    text_buffer=GTK_TEXT_BUFFER(gtk_builder_get_object(builder,"textbuffer"));

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    //Tao ra cai goi y va hoan thien tu tren thanh tim kiem
    completion = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(completion, 0);
    list_store = gtk_list_store_new(10, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(list_store));
    gtk_entry_set_completion(GTK_ENTRY(g_to), completion);
    gtk_entry_set_completion(GTK_ENTRY(g_from), completion);

    //Ket noi widget vs function
    g_signal_connect(g_search_button,"clicked",G_CALLBACK(seach_button_clicked),NULL);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();
    btcls(btfile);
    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
