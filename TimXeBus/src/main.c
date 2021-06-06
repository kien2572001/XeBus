#include <gtk/gtk.h>
#include "inc/btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"
#include "dllist.h"

#define nn 539

//Mang cac tuyen xe bus
int Line[10000][100];
int nLine=0;

//Luu duong di ngan nhat
int QuangDuongNganNhat[1000];
int nMin=0;

void chuanhoa(char text[]){
    //Xoa dau cach o dau va cuoi
    int i,l=strlen(text);
    if (text[0]==' '){
        for (i=0;i<l;i++){
            text[i]=text[i+1];
        }
    }
    l=strlen(text);
    if (text[l-1]==' ') text[l-1]='\0';
    if (text[l-1]=='\n') text[l-1]='\0';
}

void catString(char text[],int *n,char tramxebus[][200]){
    char temp[1000];
    int l=0,i=0,sotram=0;
    while (1){
        if (text[i]=='-'){
            temp[l]='\0';
            l=0;
            chuanhoa(temp);
            strcpy(tramxebus[sotram],temp);
            sotram++;
            i++;
        }
        else if (text[i]=='\0'){
            temp[l]='\0';
            l=0;
            chuanhoa(temp);
            strcpy(tramxebus[sotram],temp);
            sotram++;
            break;
        }
        else{
            temp[l]=text[i];
            l++;
            i++;
        }
    }
    *n=sotram;
}

char *intToChar(int n){
    char *out=(char *)malloc(100*sizeof(char));
    char temp[100];
    strcpy(temp,"");
    int k;
    while (n>0){
        k=n%10;
        if (k==0) strcat(temp,"0");
        else if (k==1) strcat(temp,"1");
        else if (k==2) strcat(temp,"2");
        else if (k==3) strcat(temp,"3");
        else if (k==4) strcat(temp,"4");
        else if (k==5) strcat(temp,"5");
        else if (k==6) strcat(temp,"6");
        else if (k==7) strcat(temp,"7");
        else if (k==8) strcat(temp,"8");
        else if (k==9) strcat(temp,"9");
        n=n/10;
    }
    int i;
    k=0;
    for (i=strlen(temp)-1;i>=0;i--){
        out[k++]=temp[i];
    }
    out[k]='\0';
    return out;
}

int getID(char name[],char tramxebus[][200],int sum){
   int i;
   for (i=0;i<sum;i++){
        if (strcmp(tramxebus[i],name)==0) return i;
   }
   return -1;
}

//Queue
typedef struct Queue{
	int data;
	struct Queue *next;
}Queue;

Queue *enQueue(int data,Queue *t){
	Queue *temp=(Queue *)malloc(sizeof(Queue));
	temp->data=data;
	temp->next=t;
	return temp;
}

Queue *deQueue(Queue *t,int *out){
	Queue *tmp=NULL;
	Queue *node=NULL;
	if (t==NULL){
		*out=-1;
		return NULL;
	}
	else if (t->next==NULL){
		*out=t->data;
		return NULL;
	}
	else{
		tmp=t;
		while (tmp->next->next!=NULL){
			tmp=tmp->next;
		}
		node=tmp->next;
		*out=node->data;
		tmp->next=NULL;
		return t;
	}
}

typedef struct {
    JRB edges;
    JRB vertices;
}Graph;

Graph createGraph(){
   Graph g;
   g.edges = make_jrb();
   g.vertices = make_jrb();
   return g;
}


void addVertex(Graph g,int id,char name[]){
    jrb_insert_int(g.vertices,id,new_jval_s(name));
}

char *getVertex(Graph g,int id){
    JRB temp=jrb_find_int(g.vertices,id);
    if (temp!=NULL){
        return strdup(jval_s(temp->val));
    }
    else return strdup("Loi roi\n");
}

void addEdge(Graph g,int v1,int v2){
    JRB temp,node,t1,t2;
    node = jrb_find_int(g.edges,v1);
    if (node ==NULL){
        temp=make_jrb();
        jrb_insert_int(g.edges,v1,new_jval_v(temp));
        jrb_insert_int(temp,v2,new_jval_i(nLine));
        nLine++;
    }
    else{
        temp=(JRB )jval_v(node->val);
        t1=jrb_find_int(temp,v2);
        if (t1!=NULL) return;
        jrb_insert_int(temp,v2,new_jval_i(nLine));
        nLine++;
    }
}

int ValueOfEdge(Graph g,int v1,int v2){
    JRB temp,node,t1,t2;
    node = jrb_find_int(g.edges,v1);
    if (node==NULL) return -1;
    else{
        temp=(JRB )jval_v(node->val);
        t1=jrb_find_int(temp,v2);
        if (t1==NULL) return -1;
        return jval_i(t1->val);
    }
}

int hasEdge(Graph graph, int v1, int v2){
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    if (jrb_find_int(tree, v2)==NULL)
       return 0;
    else
       return 1;
}

int outdegree(Graph g, int v, int *output){
    JRB temp=jrb_find_int(g.edges,v);
    if (temp==NULL) return 0;
    JRB con=jval_v(temp->val);
    int n=0;
    jrb_traverse(temp,con){
        output[n++]=jval_i(temp->key);
    }
    return n;
}

int indegree(Graph g, int v, int* output){
    int n=0;
    JRB temp,node,sub_g,search;
    jrb_traverse(temp,g.edges){
        sub_g=jval_v(temp->val);
        search=NULL;
        search=jrb_find_int(sub_g,v);
        if (search!=NULL){
            output[n++]=jval_i(temp->key);
        }
    }
    return n;
}

int in_deg(Graph g, int v, int* o) {
    JRB node, tree, n;
    node = jrb_find_int(g.edges, v);
    if(node == NULL) return 0;
    tree = g.edges;
    int total = 0;
    jrb_traverse(n, tree)
        if((JRB)jval_v(n->val) == node) o[total++] = jval_i(n->key);
    return total;
}

int* in_deg_arr(Graph g) {
	//Tra ve mang luu so dau vao cua moi node
    int* n = calloc(1000, sizeof(int));
    int out[1000];
    JRB node, tree = g.vertices;
    jrb_traverse(node, tree) {
        int key = jval_i(node->key);
        n[key] =indegree(g,key,out);
    }
    return n;
}


void printPath(Graph g,int p[],int diemcuoi){
    if (diemcuoi==-1) return;
    printPath(g,p,p[diemcuoi]);
    QuangDuongNganNhat[nMin++]=diemcuoi;
    //printf("%s - Bat cac xe: \n",getVertex(g,diemcuoi));
}


int KiemTraCoDuongHayKhong(Graph g,int diemdau,int diemcuoi){
    int d[1000],p[1000],visited[10000];
    int i,u,output[1000],k,v,dlen;
    for (i=0;i<10000;i++) visited[i]=0;
    //Khoi tao diem dau
    //Duyet
    Queue *l=NULL;
    for (i=0;i<1000;i++){
        visited[i]=0;
    }
    l=enQueue(diemdau,l);
    while (l!=NULL){
        l=deQueue(l,&u);
        if (u==diemcuoi) return 1;//CO duong
        if (visited[u]==0){
            visited[u]=1;
            k=outdegree(g,u,output);
            for (i=0;i<k;i++){
                l=enQueue(output[i],l);
            }
        }
    }
    return 0;//Khong co duong
}

void Dijkstra(Graph g,int diemdau,int diemcuoi){
    int d[10000],p[10000],visited[10000];
    int i,u,output[10000],k,v,dlen;
    //Khoi tao ban dau
    for (i=0;i<10000;i++){
        d[i]=1000;
        p[i]=-1;
    }
    //Khoi tao diem dau
    d[diemdau]=0;
    //Duyet
    Queue *l=NULL;
    for (i=0;i<10000;i++){
        visited[i]=0;
    }
    l=enQueue(diemdau,l);
    while (l!=NULL){
        l=deQueue(l,&u);
        if (visited[u]==0){
            visited[u]=1;
            k=outdegree(g,u,output);
            for (i=0;i<k;i++){
                v=output[i];
                l=enQueue(v,l);
                //dlen=jval_i(temp->val);
                dlen=1;
                //Tu cac lan sau tro di
                 if (dlen+d[u]<d[v]) {
                    d[v]=d[u]+1;
                    p[v]=u;
                }
            }
        }
    }
    printPath(g,p,diemcuoi);
}

//Khai bao bien
int i, n, output[1000],dinh=0,j,k;
Graph g;
char str[100000];
char temp[1000][200];
char tramxe[10000][200];
char tramxe_sort[10000][200];
int idxe=0;
FILE *id;
char line[100][200];//Ki hieu cua cac line tau

//Luu dinh vao Btree
BTA *btfile;
//Khai bao cac widget
GtkWidget *g_from;
GtkWidget *g_to;
GtkWidget *g_search_button;
GtkWidget *g_view;
GtkTextBuffer *text_buffer;
GObject *completion1, *list_store1;
GObject *completion2, *list_store2;

int prefix(const char *string1, const char *string2){
	int length1 = strlen(string1);
	int length2 = strlen(string2);
	int index;

	if (length1 > length2)
		return 0;

	for (index = 0; index < length1; index++)
	{
		if (string1[index] != string2[index])
			return 0;
	}

	return 1;
}


//Cac ham lien quan den cac widget
void seach_button_clicked(){
    char noidi[1000];
    char noiden[1000];
    int id1,id2;
    char DuongDi[100000]="";
    gtk_text_buffer_set_text(text_buffer,DuongDi,-1);
    strcpy(noidi,gtk_entry_get_text(GTK_ENTRY(g_from)));
    strcpy(noiden,gtk_entry_get_text(GTK_ENTRY(g_to)));
    printf("Noi di: %s\n",noidi);
    printf("Noi den: %s\n",noiden);
    id1=getID(noidi,tramxe,idxe);
    id2=getID(noiden,tramxe,idxe);
    printf("ID1: %d\nID2: %d\n",id1,id2);
    if (KiemTraCoDuongHayKhong(g,id1,id2)==0){
        gtk_text_buffer_set_text(text_buffer,"Không tồn tại đường đi giữa hai trạm này \n",-1);
        gtk_text_view_set_buffer(GTK_TEXT_VIEW(g_view),GTK_TEXT_BUFFER(text_buffer));
        gtk_text_view_set_editable(GTK_TEXT_VIEW(g_view), FALSE);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(g_view), FALSE);
        return ;
    }
    nMin=0;
    Dijkstra(g,id1,id2);
    int valueofedge=-1;
    char tmp[10000];
    for (i=0;i<nMin-1;i++){
        strcpy(tmp,"");
        sprintf(tmp,"Tram: %s \n",getVertex(g,QuangDuongNganNhat[i]));
        //printf("Tram: %s    Bat 1 trong cac tuyen sau: ",getVertex(g,QuangDuongNganNhat[i]));
        valueofedge=ValueOfEdge(g,QuangDuongNganNhat[i],QuangDuongNganNhat[i+1]);
        strcat(tmp,"Bat: ");
        for (j=1;j<=Line[valueofedge][0];j++){
            strcat(tmp,line[Line[valueofedge][j]]);
            strcat(tmp," ");
            //printf("%s || ",line[Line[valueofedge][j]]);
        }
        strcat(tmp,"\n\n");
        //printf("\n");
        strcat(DuongDi,tmp);
    }
    strcpy(tmp,"");
    sprintf(tmp,"Tram cuoi: %s\n", getVertex(g,QuangDuongNganNhat[nMin-1]));
    strcat(DuongDi,tmp);
    //printf("Tram cuoi: %s\n", getVertex(g,QuangDuongNganNhat[nMin-1]));
    gtk_text_buffer_set_text(text_buffer,DuongDi,-1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(g_view),GTK_TEXT_BUFFER(text_buffer));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(g_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(g_view), FALSE);
}


void show_completion1(char word[]){
    GtkTreeIter Iter;
    gtk_list_store_clear(list_store1);
    char tu_goi_y[30];
    int rsize;

    int i,number=10,value,check=-1;

    for (i=0;i<idxe;i++){
        if (prefix(word,tramxe_sort[i])==1){
            check=i;
            break;
        }
    }
    if (check!=-1){
        gtk_list_store_append(list_store1,&Iter);
        gtk_list_store_set(list_store1,&Iter,0,tramxe_sort[check],-1);
    }
    strcpy(tu_goi_y,word);
    for (i=1;i<number&&number<20;i++){
        if (prefix(word,tramxe_sort[i+check])==1){
            gtk_list_store_append(list_store1,&Iter);
            gtk_list_store_set(list_store1,&Iter,0,tramxe_sort[check+i],-1);
        }
        else number++;
    }
}

void show_completion2(char word[]){
    GtkTreeIter Iter;
    gtk_list_store_clear(list_store2);
    char tu_goi_y[30];
    int rsize;

    int i,number=10,value,check=-1;

    for (i=0;i<idxe;i++){
        if (prefix(word,tramxe_sort[i])==1){
            check=i;
            break;
        }
    }
    if (check!=-1){
        gtk_list_store_append(list_store2,&Iter);
        gtk_list_store_set(list_store2,&Iter,0,tramxe_sort[check],-1);
    }
    strcpy(tu_goi_y,word);
    for (i=1;i<number&&number<20;i++){
        if (prefix(word,tramxe_sort[i+check])==1){
            gtk_list_store_append(list_store2,&Iter);
            gtk_list_store_set(list_store2,&Iter,0,tramxe_sort[check+i],-1);
        }
        else number++;
    }
}

//Goi y thanh search
gboolean g_from_key_press( GtkEntry *g_from, GdkEvent *event, gpointer none){
    char word[30];
    GdkEventKey *key=(GdkEventKey *)event;
    int l;
    strcpy(word,gtk_entry_get_text(GTK_ENTRY(g_from)));
    l=strlen(word);
    if(key->keyval != GDK_KEY_BackSpace)
		{
			if((key->keyval != 65364)&&(key->keyval != 65362))
				word[l]=key->keyval;
			word[l+1]='\0';
		}
    else word[l-1]='\0';
    show_completion1(word);
	return FALSE;
}

gboolean g_to_key_press( GtkEntry *g_to, GdkEvent *event, gpointer none){
    char word[30];
    GdkEventKey *key=(GdkEventKey *)event;
    int l;
    strcpy(word,gtk_entry_get_text(GTK_ENTRY(g_to)));
    l=strlen(word);
    if(key->keyval != GDK_KEY_BackSpace)
		{
			if((key->keyval != 65364)&&(key->keyval != 65362))
				word[l]=key->keyval;
			word[l+1]='\0';
		}
    else word[l-1]='\0';
    show_completion2(word);
	return FALSE;
}


int main(int argc, char *argv[])
{
    //Xu li phan du lieu xe bus
    g=createGraph();
    id=fopen("id.txt","r");
    //Khoi tao Btree
    btinit();
    //Doc cac tram xe theo id
    i=0;
    while (i<nn){
        fgets(str,10000,id);
        if (str[strlen(str)-1]=='\n') str[strlen(str)-1]='\0';
        strcpy(tramxe[idxe++],str);
        i++;
    }
    printf("IDXE: %d\n",idxe);
    //Copy va sap xep de tao completion
    for (i=0;i<idxe;i++){
        strcpy(tramxe_sort[i],tramxe[i]);
    }
    char tmpp[1000];
    for (i=0;i<idxe-1;i++){
        for (j=i+1;j<idxe;j++){
            if (strcmp(tramxe_sort[i],tramxe_sort[j])>0){
                strcpy(tmpp,tramxe_sort[i]);
                strcpy(tramxe_sort[i],tramxe_sort[j]);
                strcpy(tramxe_sort[j],tmpp);
            }
        }
    }
    //Them cac dinh vao
    for (i=0;i<idxe;i++){
        addVertex(g,i,tramxe[i]);
    }
    //In ra kiem tra qua trinh them dinh
    JRB node=NULL;
//    jrb_traverse(node,g.vertices){
//        printf("ID: %d \"%s\"\n",jval_i(node->key),jval_s(node->val));
//    }
    //Khoi tao mang Line
    for (i=0;i<10000;i++){
        Line[i][0]=0;
    }
    //Doc cac canh

    strcpy(line[0],"[01]");
    strcpy(line[1],"[02]");
    strcpy(line[2],"[03A]");
    strcpy(line[3],"[03B]");
    strcpy(line[4],"[04]");
    strcpy(line[5],"[05]");
    strcpy(line[6],"[06A]");
    strcpy(line[7],"[06B]");
    strcpy(line[8],"[06C]");
    strcpy(line[9],"[06D]");
    strcpy(line[10],"[06E]");
    strcpy(line[11],"[07]");
    strcpy(line[12],"[08A]");
    strcpy(line[13],"[08B]");
    strcpy(line[14],"[09A]");
    strcpy(line[15],"[09B]");
    strcpy(line[16],"[10A]");
    strcpy(line[17],"[10B]");
    strcpy(line[18],"[11]");
    strcpy(line[19],"[12]");
    strcpy(line[20],"[13]");
    strcpy(line[21],"[14]");
    strcpy(line[22],"[15]");
    strcpy(line[23],"[16]");
    strcpy(line[24],"[17]");
    strcpy(line[25],"[18]");
    strcpy(line[26],"[19]");
    strcpy(line[27],"[20A]");
    strcpy(line[28],"[20B]");
    strcpy(line[29],"[21A]");
    strcpy(line[30],"[21B]");
    strcpy(line[31],"[22A]");
    strcpy(line[32],"[22B]");
    strcpy(line[33],"[22C]");
    strcpy(line[34],"[23]");
    strcpy(line[35],"[24]");
    strcpy(line[36],"[25]");
    strcpy(line[37],"[26]");
    strcpy(line[38],"[27]");
    strcpy(line[39],"[28]");
    strcpy(line[40],"[29]");
    strcpy(line[41],"[30]");
    strcpy(line[42],"[31]");
    strcpy(line[43],"[32]");
    strcpy(line[44],"[33]");
    strcpy(line[45],"[34]");
    strcpy(line[46],"[35A]");
    strcpy(line[47],"[35B]");
    strcpy(line[48],"[36]");
    strcpy(line[49],"[37]");

    int lineCount=0;
    FILE *f=fopen("bus_data.txt","r");
    int v1,v2,value;
    while (lineCount<50){
        fgets(str,5000,f);
        n=0;
        catString(str,&n,temp);
        //printf("n: %d\n",n);
        //while (temp[n-1][strlen(temp[n-1])-1]<32||temp[n-1][strlen(temp[n-1])-1]>126) temp[n-1][strlen(temp[n-1])-1]='\0';
        while (temp[n-1][strlen(temp[n-1])-1]==13) temp[n-1][strlen(temp[n-1])-1]='\0';
        for (i=0;i<n-1;i++){
            v1=getID(temp[i],tramxe,idxe);
            v2=getID(temp[i+1],tramxe,idxe);
            value=ValueOfEdge(g,v1,v2);
            if (value==-1) addEdge(g,v1,v2);
            value=ValueOfEdge(g,v1,v2);
            if (Line[value][0]==0){
                Line[value][1]=lineCount;
                Line[value][0]++;
            }
            else{
                k=0;
                for (j=1;j<=Line[value][0];j++){
                    if (Line[value][j]==lineCount){
                        k=1;
                        break;
                    }
                }
                if (k==0){
                    Line[value][Line[value][0]+1]=lineCount;
                    Line[value][0]++;
                }
            }
            //printf("ID: %d ID: %d   %s   %s\n",getID(temp[i],tramxe,idxe),getID(temp[i+1],tramxe,idxe),temp[i],temp[i+1]);
        }
        lineCount++;
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
    completion1 = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(completion1, 0);
    list_store1 = gtk_list_store_new(10, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    gtk_entry_completion_set_model(completion1, GTK_TREE_MODEL(list_store1));
    gtk_entry_set_completion(GTK_ENTRY(g_from), completion1);


    //Tao ra cai goi y va hoan thien tu tren thanh tim kiem
    completion2 = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(completion2, 0);
    list_store2 = gtk_list_store_new(10, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    gtk_entry_completion_set_model(completion2, GTK_TREE_MODEL(list_store2));
    gtk_entry_set_completion(GTK_ENTRY(g_to), completion2);


    //Ket noi widget vs function
    g_signal_connect(g_search_button,"clicked",G_CALLBACK(seach_button_clicked),NULL);
    g_signal_connect(g_from, "key-press-event", G_CALLBACK(g_from_key_press), NULL);
    g_signal_connect(g_to, "key-press-event", G_CALLBACK(g_to_key_press), NULL);


    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();
    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
