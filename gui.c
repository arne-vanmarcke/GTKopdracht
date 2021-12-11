#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "PJ_RPI.h"
typedef struct _Gpio{
    int nr;
    char role[10];
    int state;
}_gpio;

typedef struct info{
    GtkWidget *SpinBtn;
    GtkWidget *guiItem;
}infoToSend;

void endProgram (GtkWidget *wid, gpointer ptr){
    gtk_main_quit();
}

void sendOutput(GtkWidget *wid, gpointer ptr){
    char buff[30];
    infoToSend *info=ptr;
    gint pinnr = gtk_spin_button_get_value_as_int((GtkSpinButton*)info->SpinBtn);
    int state=GPIO_READ((int)pinnr);
    state^=1;
    if(state)
        GPIO_SET = 1 << pinnr;
    else
        GPIO_CLR = 1 << pinnr;
    sprintf(buff, "Set to: %d", state);
    gtk_label_set_text(GTK_LABEL(info->guiItem),buff);
}

int setupPins(_gpio *_gpio, int gpioAmount){
    if(map_peripheral(&gpio)==-1){
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }
    
    for(int i=0; i<gpioAmount; i++){
        if(!strcmp((_gpio+i)->role, "OUTPUT"))
            OUT_GPIO((_gpio+i)->nr);
        else
            INP_GPIO((_gpio+i)->nr);
    }
    return 0;
}

int main (int argc, char *argv[])
{
    _gpio gIn[2]={{23,"INPUT",0},{24,"INTPUT",0}};
    _gpio gOut[2]={{17,"OUTPUT",0},{27,"OUTPUT",0}};

    if(setupPins(&gIn[0], sizeof(gIn)/sizeof(gIn[0]))==-1)
        return 0;
    if(setupPins(&gOut[0], sizeof(gOut)/sizeof(gOut[0]))==-1)
        return 0;

    gtk_init (&argc, &argv);
    GtkObject *adj = gtk_adjustment_new(0,0,10,1,1,1);
    GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *btn1 = gtk_button_new_with_label ("Toggle output");
    GtkWidget *btnSpin = gtk_spin_button_new(GTK_ADJUSTMENT (adj), 0, 0);
    GtkWidget *lbl = gtk_label_new("Gpio state: 0");
    GtkWidget *box = gtk_hbox_new(FALSE, 5);
    infoToSend info={btnSpin,lbl};
    g_signal_connect(btn1, "clicked", G_CALLBACK(sendOutput), &info);
    g_signal_connect(win, "delete_event", G_CALLBACK(endProgram), NULL);
    gtk_box_pack_start(GTK_BOX(box), lbl, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), btnSpin, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (win), box);
    gtk_widget_show_all (win);
    gtk_main ();
}