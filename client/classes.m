

typedef struct Node Node {
    int ID;
    int x;
    int y;
    char* toString(){
        char str[3*sizeOf(int)+2];
        sprintf(str, "%d,%d,%d",ID,x,y);
        return str;
    }
};
Node node_init(int ID, int x, int y)
{
    Node node;
    node.ID = ID;
    node.x = x;
    node.y = y;
    return node;
}
Node node_init_str(char *str)
{
    Node node;
    int i = 0;
    int Int = 0;
    while( *(str+i)!=",")
    {
        Int=(*(str+i)-'0')+Int*10;
        i++;
    }
    node.ID = Int;
    Int = 0;
    i++;
    while( *(str+i)!=",")
    {
        Int=(*(str+i)-'0')+Int*10;
        i++;
    }
    node.x = Int;
    Int = 0;
    i++;
    while( *(str+i)!=",")
    {
        Int=(*(str+i)-'0')+Int*10;
        i++;
    }
    node.y = Int;
    return node;
}

typedef struct Vertice Vertice {
    int ID;
    Node node1;
    Node node2;
    char* toString(){
        char str[3*sizeOf(int)+2];
        sprintf(str, "%d,%d,%d",ID,node1.ID,node2.ID);
        return str;
    }
};
Vertice vertice_init(int ID, Node node1, Node node2) {
    Vertice vertice;
    vertice.ID = ID;
    vertice.node1 = node1;
    vertice.node2 = node2;
    return vertice;
}
Node vertice_init_str(char *str)
{
    Vertice vertice;
    int i = 0;
    int Int = 0;
    while( *(str+i)!=",")
    {
        Int=(*(str+i)-'0')+Int*10;
        i++;
    }
    vertice.ID = Int;
    Int = 0;
    i++;
    while( *(str+i)!=",")
    {
        Int=(*(str+i)-'0')+Int*10;
        i++;
    }
    int ID1 = Int;
    Int = 0;
    i++;
    while( *(str+i)!=",")
    {
        Int=(*(str+i)-'0')+Int*10;
        i++;
    }
    int ID2 = Int;

    return node;
}
