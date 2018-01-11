/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   19/12/2017
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 11/01/2018
 */



tCoord* get_neighbours_of_same_char(tCoord tc, char c, int width, int height, char* map)
{
    int* offset[4] = {{0,1},{0,-1},{1,0},{-1,0}};
    int i=0;
    tCoord neighbour;
    tCoord output[4];
    int j=0
    while(i<4)
    {
        neighbour = tCoord_new(tc.i+offset[i][0] , tc.j+offset[i][1]);
        if(c==get_char(neighbour,width,height,map)
            &&neighbour.i>=0
            &&neighbour.i<height
            &&neighbour.j>=0) // &&neighbour.j<width not necesary because we compare char to \n to the right already

            output[j] = neighbour;
        else
            output[j] = NULL;
        j++;
    }
    return output;
}

bool tCoord_explored(tCoord tc, tCoord* connextCoords, size_t sizeof_connextCoords)
{
    int i=0;
    while(i<sizeof_connextCoords)
    {
        if(tCoord_eq(tc, connextCoords[i]))
            return 1;
    }
    return 0;
}

void get_connex_tCoord_of_same_char(tCoord tc,
                                   char c,
                                   int width,
                                   int height,
                                   char* map,
                                   tCoord* connextCoords,
                                   size_t* sizeof_connextCoords)
// tCoord* connextCoords[strlen(map)-height+1]; do not count the \n in the map
// size_t sizeof_connextCoords = 0;
{
    while(!tCoord_explored(tc, connextCoords, sizeof_connextCoords))
    {
        connextCoords[sizeof_connextCoords] = tc;
        sizeof_connextCoords++;
        tCoord* neighbours = get_neighbours_of_same_char(tc,c, width, height, map);
        int i=0;
        while(i<4)
            get_connex_tCoord_of_same_char(tc[i], c, width, height, map, connextCoords, sizeof_connextCoords);
    }
}

Point* get_connex_Pixels_coords(Pixel p, char* map)
{
    Pixel[strlen(map)];
}

int* getNewSpot()
{
    /*TODO*/
}
