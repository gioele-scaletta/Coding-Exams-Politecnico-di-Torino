#include "Library\util.h"

#define MAXL 100
/* structures included in the library util.h
typedef struct edge
{
    struct edge *next;
    struct vertex *dst;
} edge_t;

typedef struct vertex
{
    char *name;
    int color;
    edge_t *head;
    struct vertex *next;
    int traversed;
} vertex_t;

typedef struct graph
{
    vertex_t *v;
} graph_t;
*/

graph_t *graphload(char *filename,int *n);
vertex_t *vertexfind(graph_t *g,char name[MAXL]);
vertex_t *newvertex(graph_t *g,char name[MAXL]);
int assigncolors(int pos,vertex_t *v,int k,graph_t *g);
int check(graph_t *g);
void newedge(vertex_t *src,vertex_t *dst);
void graphdispose (graph_t *g);
void printoutgraph(graph_t *g);

int main(int argc,char *argv[])
{
    int i,n=0,flag=0;
    graph_t *g;

    if(argc!=2)
    {
        exit(1);
    }

    g=graphload(argv[1],&n);

    for(i=1; i<=n+1; i++)
    {
        if(flag==0)
        {
            flag=assigncolors(n,g->v,i,g);
        }
        else
        {
            printf("Solution found using %d colors\n",(i-1));
            printoutgraph(g);
            break;
        }
    }
    graphdispose(g);
    return 0;
}


graph_t *graphload(char *filename,int *n)
{
    FILE *fp;
    char name1[MAXL],name2[MAXL];
    vertex_t *tmp,*tmp1;
    graph_t *g;

    g=(graph_t*)util_malloc(sizeof(graph_t));
    g->v=NULL;

    fp=util_fopen(filename,"r");

    while(fscanf(fp,"%s %s",name1,name2)!=EOF)
    {
        tmp=vertexfind(g,name1);
        tmp1=vertexfind(g,name2);

        if(tmp==NULL)
        {
            tmp=newvertex(g,name1);
            *n=*n+1;
        }

        if(tmp1==NULL)
        {
            tmp1=newvertex(g,name2);
            *n=*n+1;
        }

        newedge(tmp,tmp1);

    }
    fclose(fp);
    return g;
}


int assigncolors(int pos,vertex_t *v,int k,graph_t *g)//recursive function which creates arrangments with repetitions of k colors in n vertices (k value ranges from 1 to n)
{
    int i;
    edge_t *e;

    if(pos==0)
    {
        if(check(g)==1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    if(v->traversed==0)
    {
        v->traversed=1;
        for(i=1; i<=k; i++)
        {
            v->color=i;
            e=v->head;
            while(e!=NULL)
            {
                if(assigncolors(pos-1,e->dst,k,g)==1)
                {
                    return 1;
                }
                e=e->next;
            }
            v->color=0;       //backtrack
        }
        v->traversed=0;       //backtrack
    }
    return 0;
}


int check(graph_t *g)  //function "check" checks whether there are connected vertex with the same color(return 0) or not(return 1)
{
    vertex_t *tmp;
    edge_t *tmp1;

    for(tmp=g->v; tmp!=NULL; tmp=tmp->next)
    {
        for(tmp1=tmp->head; tmp1!=NULL; tmp1=tmp1->next)
        {
            if(tmp->color==tmp1->dst->color)
            {
                return 0;
            }
        }
    }
    return 1;
}


vertex_t *vertexfind(graph_t *g,char name[MAXL])  //find the vertex given its id
{

    vertex_t *tmp;

    for(tmp=g->v; tmp!=NULL; tmp=tmp->next)
    {
        if(strcmp(name,tmp->name)==0)
        {
            return tmp;
        }
    }

    return NULL;
}


vertex_t *newvertex(graph_t *g,char name[MAXL])  //allocate and initialize new vertex
{

    vertex_t *tmp=NULL,*tmp1;
    tmp=g->v;

    if(tmp!=NULL)
    {
        while(tmp->next!=NULL)
        {
            tmp=tmp->next;
        }
    }
    tmp1=(vertex_t *)util_malloc(sizeof(vertex_t));
    if(tmp==NULL)
    {
        g->v=tmp1;
    }
    else
    {
        tmp->next=tmp1;
    }
    tmp1->next=NULL;
    tmp1->name=util_strdup(name);
    tmp1->traversed=0;
    tmp1->color=0;
    tmp1->head=NULL;

    return tmp1;
}


void newedge(vertex_t *src,vertex_t *dst)  //allocate and initialize a new edge (undirected graph) given two vertices
{
    edge_t *tmp,*tmp1;

    if(src->head==NULL)
    {
        tmp=(edge_t*)util_malloc(sizeof(edge_t));
        tmp->next=NULL;
        src->head=tmp;
        tmp->dst=dst;
    }
    else
    {
        tmp=src->head;

        while(tmp->next!=NULL)
        {
            tmp=tmp->next;
        }

        tmp1=(edge_t*)util_malloc(sizeof(edge_t));
        tmp->next=tmp1;
        tmp1->next=NULL;
        tmp1->dst=dst;
    }


    if(dst->head==NULL)//same operation of lines 215-235 backwards: edge is created also from vertex dst to src(graph is undirected)
    {
        tmp=(edge_t*)util_malloc(sizeof(edge_t));
        tmp->next=NULL;
        dst->head=tmp;
        tmp->dst=src;
    }
    else
    {

        tmp=dst->head;

        while(tmp->next!=NULL)
        {
            tmp=tmp->next;
        }

        tmp1=(edge_t*)util_malloc(sizeof(edge_t));
        tmp->next=tmp1;
        tmp1->next=NULL;
        tmp1->dst=src;
    }
    return;
}


void printoutgraph(graph_t *g)                   //print vertex colors
{

    vertex_t *tmp;

    for(tmp=g->v; tmp!=NULL; tmp=tmp->next)
    {

        printf("Vertex: %s  Color %d\n",tmp->name,tmp->color);

    }
    return;
}
