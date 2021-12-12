#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "PJ_RPI.h"
typedef struct info{
    GtkWidget *SpinBtn;
    GtkWidget *guiItem;
}infoToSend;

void endProgram (GtkWidget *wid, gpointer ptr){
    gtk_main_quit();
}

int sendOutput(GtkWidget *wid, gpointer ptr){
    char buff[30];
    infoToSend *info=ptr;
    gint pinnr = gtk_spin_button_get_value_as_int((GtkSpinButton*)info->SpinBtn);
    if(map_peripheral(&gpio)==-1){
		printf("Fout");
    	return -1;
    }
    OUT_GPIO((int)pinnr);
    int state=GPIO_READ((int)pinnr);
    if(state>=1)
	    state=1;
    state^=1;
    if(state)
        GPIO_SET = 1 << (int)pinnr;
    else
        GPIO_CLR = 1 << (int)pinnr;
    sprintf(buff, "Set to: %d", state);
    gtk_label_set_text(GTK_LABEL(info->guiItem),buff);
}

int GetInput(GtkWidget *wid, gpointer ptr){
	char buff[30];
    infoToSend *info=ptr;
    gint pinnr = gtk_spin_button_get_value_as_int((GtkSpinButton*)info->SpinBtn);
    if(map_peripheral(&gpio)==-1){
		printf("Fout");
    	return -1;
    }
    INP_GPIO((int)pinnr);
    int state=GPIO_READ((int)pinnr);
    if(state>=1)
	    state=1;
    sprintf(buff, "Gpio state: %d", state);
    gtk_label_set_text(GTK_LABEL(info->guiItem),buff);
}

int main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);
    GtkObject *adj = gtk_adjustment_new(0,0,27,1,1,1);
    GtkObject *adjIn = gtk_adjustment_new(0,0,27,1,1,1);
    GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *btn1 = gtk_button_new_with_label ("Toggle output");
    GtkWidget *btnSpin = gtk_spin_button_new(GTK_ADJUSTMENT (adj), 0, 0);
    GtkWidget *lbl = gtk_label_new("Gpio state: 0");
    GtkWidget *btnIn = gtk_button_new_with_label ("Read input");
    GtkWidget *btnSpinIn = gtk_spin_button_new(GTK_ADJUSTMENT (adjIn), 0, 0);
    GtkWidget *lblIn = gtk_label_new("Gpio state: 0");
    GtkWidget *box = gtk_hbox_new(FALSE, 6);
    infoToSend info={btnSpin,lbl};
    infoToSend infoIn={btnSpinIn,lblIn};
    g_signal_connect(btn1, "clicked", G_CALLBACK(sendOutput), &info);
    g_signal_connect(btnIn, "clicked", G_CALLBACK(GetInput), &infoIn);
    g_signal_connect(win, "delete_event", G_CALLBACK(endProgram), NULL);
    gtk_box_pack_start(GTK_BOX(box), lbl, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), btnSpin, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), btnSpinIn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), btnIn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), lblIn, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (win), box);
    gtk_widget_show_all (win);
    gtk_main ();
}
