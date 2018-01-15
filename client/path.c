/**
* @Author: Axel_Soll <amrsoll>
* @Date:   13/01/2018
* @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 15/01/2018
*/


#include "path.h"

void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

int countlines(FILE* f)
{
    rewind(f);
    int lines = 0;
    char ch;
    while(!feof(f))
    {
        ch = fgetc(f);
        if(ch == '\n')
            lines++;
    }
    return lines;
}

char* getLine(FILE* f, int i)
{
    rewind(f);
    char* line[256];
    char ch;
    while(!feof(f)&&i>1)
    {
        ch = fgetc(f);
        if(ch == '\n')
            i--;
    }
    if(feof(f))
    {
        printf("error : the file does not have that many lines\n");
        exit(EXIT_FAILURE);
    } else
    {
        while(!feof(f)&&ch!='\n')
        {
            ch = fgetc(f);
            append(line, ch);
        }
        return line;
    }
}

// Follows head insertion to give O(1) insertion (binary heaps)
struct node * addEdge(struct node * head, int vertex, int weight)
{
    struct node * newNode = (struct node *) calloc(1, sizeof(struct node));

    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = head;

    return newNode;
}

// Retuns the vertex which is not visited and has least distance
int getMinVertex(int distances[], int visited[], int vertices)
{
  int min = INT_MAX, index = -1, i;

  for (i = 1; i <= vertices; ++i) {
    if (visited[i] == 0 && min > distances[i]) {
      min = distances[i];
      index = i;
    }
  }

  return index;
}

// Dijkstra's Algorithm function
void dijkstra(struct node * adjacencyList[], int vertices, int startVertex, int distances[], int parent[])
{
    int i, visited[vertices + 1];

    // Initially no routes to vertices are know, so all are infinity
    for (i = 1; i <= vertices; ++i) {
        distances[i] = INT_MAX;
        parent[i] = 0;
        visited[i] = 0;
    }

    // Setting distance to source to zero
    distances[startVertex] = 0;

    for (i = 1; i <= vertices; ++i) {     // Untill there are vertices to be processed
        int minVertex = getMinVertex(distances, visited, vertices); // Greedily process the nearest vertex
    struct node * trav = adjacencyList[minVertex];    // Checking all the vertices adjacent to 'min'
    visited[minVertex] = 1;

        while (trav != NULL) {
          int u = minVertex;
          int v = trav->vertex;
          int w = trav->weight;

            if (distances[u] != INT_MAX && distances[v] > distances[u] + w) {
                // a new shortest route, make the neccesary adjustments in data
                distances[v] = distances[u] + w;
                parent[v] = u;
            }
            trav = trav->next;
        }
    }
}

// Recursively looks at a vertex's parent to print the path
void printPath(int parent[], int vertex, int startVertex)
{
    if (vertex == startVertex) {  // reached the source vertex
        printf("%d ", startVertex);
        return;
    } else if (parent[vertex] == 0) { // current vertex has no parent
      printf("%d ", vertex);
      return;
  } else {  // go for the current vertex's parent
        printPath(parent, parent[vertex], startVertex);
        printf("%d ", vertex);
    }
}

int get_pos_in_list(tCoord* coord_correspond, size_t size_coord_correspond, tCoord coord)
{
    int i;
    while(i<size_coord_correspond && !tCoord_eq(coord_correspond[i]))
        i++;
    return i;
}

int getPathTo(tCoord)
//clears previous path file and creates a new one.
{
    //The file onto which we will write the nodes for when the robot moves around
    FILE *path = open("~/path", O_RDWR ,0666);

    close(path);
    return 0;
}

tCoord getCheckpoint(int i, int pathLen, int width, int height, char* map)
{
    FILE *path = open("~/path", O_RDWR ,0666);

    int i,j;
    int v1=0;
    int v2=0;
    tCoord coord;
    int vertices = width*height;
    struct node * adjacencyList[vertices + 1];  // to use the array as 1-indexed for simplicity
    int distances[vertices + 1];
    int parent[vertices + 1];
    tCoord* coord_correspond[vertices + 1];

    for (i = 0; i <= vertices; ++i) { // Must initialize array
          adjacencyList[i] = NULL;
    }
    // get all of the free pixels in the same list
    int k = 0;
    for(i=0;i<width;i++)
    for(j=0;j<height;j++)
    {
        coord = tCoord_new(i,j);
        if(get_char(coord,width,height,map)==FREE_PIXEL)
        {
          coord_correspond[k++] = coord;
        }
    }
    number_vertices = k; //int vertices

    for(v1=0;v1<number_vertices;v1++)
    {
        tCoord neighbours[4];
        get_neighbours_of_same_char(coord_correspond[k],FREE_PIXEL,neighbours,width,height,map);
        // only consider the right and the bottom neighbours
        // because we read the list of free cells from the top left.
        int l;
        for(l=1;
            l<3
            &&neighbours[2]!=coord_correspond[k]
            &&neighbours[3]!=coord_correspond[k];
            l++)
        {
            v2 = get_pos_in_list(coord_correspond, number_vertices, neighbours[l])
            adjacencyList[v1] = addEdge(adjacencyList[v1], v2, 1); // the weight will always be 1
        }
    }

    int startVertex =



    tCoord tc = tCoord_init_str(getLine(path, pathLen-i));
    close(path);
    return tc;
}
