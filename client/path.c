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

int getLine(FILE* f, int i, char* line)
{
    rewind(f);
    char ch;
    int length=0;
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
            length++;
        }
        return length;
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
void dijkstra(struct node * adjacencyList[], int vertices, int start_Vertex, int distances[], int parent[])
{
    int i, visited[vertices + 1];

    // Initially no routes to vertices are know, so all are infinity
    for (i = 1; i <= vertices; ++i) {
        distances[i] = INT_MAX;
        parent[i] = 0;
        visited[i] = 0;
    }

    // Setting distance to source to zero
    distances[start_Vertex] = 0;

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
void fprintPath(FILE *stream, int parent[], int vertex, int start_Vertex, tCoord* coord_correspond)
{
    if (vertex == start_Vertex) {  // reached the source vertex
        fprintf_tCoord_to_str(stream, coord_correspond[start_Vertex]);
        return;
    } else if (parent[vertex] == 0) { // current vertex has no parent
        fprintf_tCoord_to_str(stream, coord_correspond[vertex]     );
        return;
    } else {  // go for the current vertex's parent
        fprintPath(stream, parent, parent[vertex], start_Vertex, coord_correspond);
    }
}

int get_pos_in_list(tCoord* coord_correspond, size_t size_coord_correspond, tCoord coord)
{
    int i;
    while(i<size_coord_correspond && !tCoord_eq(coord_correspond[i], coord ))
        i++;
    return i;
}

int getPathTo(tCoord start_coord, tCoord goal_coord, int width, int height, char* map)
//clears previous path file and creates a new one.
{

    int i,j;
    int v1=0;
    int v2=0;
    tCoord coord;
    int vertices = width*height;
    struct node * adjacencyList[vertices + 1];  // to use the array as 1-indexed for simplicity
    int distances[vertices + 1];
    int parent[vertices + 1];
    tCoord coord_correspond[vertices + 1];

    for (i = 0; i <= vertices; ++i) { // Must initialize array
          adjacencyList[i] = NULL;
    }
    // get all of the free pixels in the same list
    int k = 0;
    for(i=0;i<height;i++)
    for(j=0;j<width;j++)
    {
        coord = tCoord_new(i,j);
        if(get_char(coord,width,height,map)==FREE_PIXEL)
        {
          coord_correspond[k] = tCoord_new(coord.i,coord.j);
          //printf("coords new : %d,%d\n", coord_correspond[k].i,coord_correspond[k].j);
          k++;
        }
    }
    int number_vertices = k; //int vertices

    for(v1=0;v1<number_vertices;v1++)
    {
        tCoord neighbours[4];
        //printf("coords new : %d,%d\n", coord_correspond[v1].i,coord_correspond[v1].j);
        get_neighbours_of_same_char(coord_correspond[v1],FREE_PIXEL,neighbours,width,height,map);
        // only consider the right and the bottom neighbours
        // because we read the list of free cells from the top left.
        int l;
        for(l=0;l<4;l++)
        {
            if(neighbours[l].i>coord_correspond[v1].i
            || neighbours[l].j>coord_correspond[v1].j)
            {
                printf("parent : %d,%d    tested son : %d,%d\n",coord_correspond[v1].i,coord_correspond[v1].j, neighbours[l].i,neighbours[l].j);
                v2 = get_pos_in_list(coord_correspond, number_vertices, neighbours[l]);
                adjacencyList[v1] = addEdge(adjacencyList[v1], v2, 1); // the weight will always be 1
            }
        }
    }

    int start_Vertex = get_pos_in_list(coord_correspond, number_vertices, start_coord);
    int goal_Vertex =  get_pos_in_list(coord_correspond, number_vertices, goal_coord);

    dijkstra(adjacencyList, number_vertices, start_Vertex, distances, parent);

    //The file onto which we will write the nodes for when the robot moves around
    remove(PATH_PATH);
    FILE *path = fopen(PATH_PATH, "w");
    if (goal_Vertex == start_Vertex) {  // reached the source goal_Vertex
        fprintf_tCoord_to_str(path, coord_correspond[goal_Vertex] );
        return -1; //we are already at the right position
    } else if (parent[goal_Vertex] == 0) { // current goal_Vertex has no parent
        fprintf_tCoord_to_str(path, coord_correspond[goal_Vertex] );
        return -2; // path impossible to find
    } else {  // go for the current goal_Vertex's parent
        fprintPath(path, parent, parent[goal_Vertex], start_Vertex, coord_correspond);
        return 0;
    }
    fclose(path);
    return 0;
}

tCoord getCheckpoint(int i, int pathLen)
{
    FILE *path = fopen(PATH_PATH, "r");
    char str[256];
    int length = getLine(path, pathLen-i, str);
    tCoord tc = tCoord_init_str(str, (size_t)length);
    fclose(path);
    return tc;
}
