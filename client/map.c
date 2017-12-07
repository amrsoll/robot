#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include "gsyst.h" /* Also contains the ev3 libraries*/

#include "constants.m"

sem_t * semNodesw    = sem_open(LOCK_FOR_WRITE_NODES, O_CREAT, 0644, 0);
sem_t * semVerticesw = sem_open(LOCK_FOR_WRITE_VERTI, O_CREAT, 0644, 0);
sem_t * semNodesr    = sem_open(LOCK_FOR_READ_NODES, O_CREAT, 0644, 0);
sem_t * semVerticesr = sem_open(LOCK_FOR_READ_VERTI, O_CREAT, 0644, 0);

Nodes* nodes;

int countlines(FILE f)
{
    int ch=0;
    int lines=0;
    sem_wait(semNodesr);
    sem_wait(semVerticesr);
    sem_wait(semNodesw);
    sem_wait(semVerticesw);
    sem_post(semNodesr);
    sem_post(semVerticesr);
    if (f == NULL);
        exit(EXIT_FAILURE);
    while(!feof(f))
    {
        ch = fgetc(f);
        if(ch == '\n')
        {
            lines++;
        }
    }
    sem_post(semNodesw);
    sem_post(semVerticesw);
    return lines;
}

Node getNode(int ID)
{
    sem_wait(semNodesr);
    sem_post(semNodesr)
    FILE *nodes = fopen("map/nodes", "r", 0666);
    if (nodes == NULL)
    {
        printf("Error opening the node file!\n");
        exit(1);
    }
    char ch = 0;
    int nodeID = 0;
    while(!feof(f))
    {
        ch = fgetc(f);
        while(ch != ',')
        {
            nodeID = nodeID*10 + (int)(ch-'0');
            ch = fgetc(f);
        }
        if(nodeID == ID)
        {
            int x = 0;
            while(ch != ',')
            {
                x = x*10 + (int)(ch-'0');
                ch = fgetc(f);
            }
            int y = 0;
            while(ch != ',')
            {
                y = y*10 + (int)(ch-'0');
                ch = fgetc(f);
            }
            Node node = node_init(ID,x,y);
            return node;
        }
        while(ch != '\n')
            ch = fgetc(f);
    }
    return -1;

}

int addNode(Node node)
{
    sem_wait(semNodesw);
    sem_wait(semNodesr);
    FILE *nodes = fopen("map/nodes", "a+", 0666);
    if (nodes == NULL)
    {
        printf("Error opening the node file!\n");
        return -1;
    }
    sem_post(semNodesw);
    sem_post(semNodesr);
    int ID = countlines(nodes);
    sem_wait(semNodesw);
    sem_wait(semNodesr);
    fprintf(f, "%s\n", node.toString());
    fclose(f);
    sem_post(semNodesw);
    sem_post(semNodesr);
    return 0;
}

int addVertice(Node node1,Node node2)
{

    sem_wait(semVerticesr);
    sem_wait(semVerticesw);
    FILE *vertices = fopen("map/vertices", "a+", 0666);
    if (vertices == NULL)
    {
        printf("Error opening the vertice file!\n");
        return -1;
    }
    sem_post(semVerticesw);
    sem_post(semVerticesr);
    int ID = countlines(nodes);
    sem_wait(semVerticesw);
    sem_wait(semVerticesr);
    Vertice vertice = vertice_init(ID,node1,node2);
    fprintf(f,  "%s\n", vertice.toString());
    fclose(f);
    sem_post(semVerticesr);
    sem_post(semVerticesw);
    return 0;
}

int eqNodes(Node nodeA, Node nodeB)
{
    return (nodeA.x-nodeB.x)*(nodeA.x-nodeB.x) +
           (nodeA.y-nodeB.y)*(nodeA.y-nodeB.y) <=
           ULTRASONIC_SENSOR_PRECISION*ULTRASONIC_SENSOR_PRECISION;
}

int makeClockwise()
{
    FILE *nodes = fopen("map/nodes","r+");
    int totalNodes = countlines(nodes);
    sem_wait(semNodesr);
    sem_wait(semVerticesr);
    sem_wait(semNodesw);
    sem_wait(semVerticesw);
    sem_post(semNodesr);
    sem_post(semVerticesr);
    int visitedNodes[totalNodes];



}

int map(int posX,int posY)
{
    /*TODO*/
}
