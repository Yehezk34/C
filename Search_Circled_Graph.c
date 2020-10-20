#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define bool int
#define true 1
#define false 0

#define pos_center 1
#define pos_edge_top 2
#define pos_edge_right 3
#define pos_edge_left 4
#define pos_edge_bottom 5

#define pos_corner_right_top 6
#define pos_corner_left_bottom 7
#define pos_corner_left_top 8
#define pos_corner_right_bottom 9
#define INT_MAX __INT_MAX__

typedef struct {
    int pos;
    int x;
    int y;
    int cost;
} City;

int minDistance(int dist[], bool sptSet[], int n) {

    int min = INT_MAX, min_index;
    min_index = 0;
    for (int v = 0; v < n; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void dijkstra(int **graph, int n, int src, int to) {

    int dist[n], sptSet[n];

    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    dist[src] = 0;
    for (int count = 0; count < n-1; count++) {
        int u = minDistance(dist, sptSet, n);
        sptSet[u] = true;
        for (int v = 0; v < n; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u]+graph[u][v] < dist[v])
                dist[v] = dist[u] + (graph[u][v] ==1 ? 0 : graph[u][v]);
    }
    printf("%d\n", dist[to] == INT_MAX ? -1 : dist[to]);
}

int getPos(int x, int y, int max_x, int max_y){
    return ( x == 0 && y == max_y) ? pos_corner_left_bottom : (x == max_x && y == 0) ? pos_corner_right_top : (x == 0 && y == 0) ? pos_corner_left_top : (x == max_x && y == max_y) ? pos_corner_right_bottom :
                                                                                                                                                         (x == 0) ? pos_edge_left : (y == 0) ? pos_edge_top : (x == max_x) ? pos_edge_right : (y == max_y) ? pos_edge_bottom : pos_center;
}

int getAdjMatCost(City *cit, int i, int j, int rest_x, int rest_y, int offsetx) {

    int b, a, t, sig_x, sig_y, index, last_index;
    int cost_x, cost_y;
    bool jump_c;

    if(cit[i].cost == -1 && cit[j].cost == -1) return 1;
    if(!cit[i].cost || !cit[j].cost ) return 0;
    for(t = 1; t <= 4; t++) {

        sig_x = t > 2 ? -1 : 1;
        sig_y = t % 2 == 0 ? -1 : 1;

        if(sig_x == 1)
        if(cit[i].pos == pos_edge_right || cit[i].pos == pos_corner_right_top || cit[i].pos == pos_corner_right_bottom)
            continue;
        if(sig_x == -1)
        if(cit[i].pos == pos_edge_left || cit[i].pos == pos_corner_left_top || cit[i].pos == pos_corner_left_bottom)
            continue;

        if(sig_y == 1)
        if(cit[i].pos == pos_edge_bottom || cit[i].pos == pos_corner_right_bottom || cit[i].pos == pos_corner_left_bottom)
            continue;
        if(sig_y == -1)
        if(cit[i].pos == pos_edge_top || cit[i].pos == pos_corner_left_top || cit[i].pos == pos_corner_right_top)
            continue;

        jump_c = false;

        if( (!rest_x && !rest_y) ){
            if( (i + (sig_x)) == j ){
                rest_x = 1;
                jump_c = true;
            }
            if( (i + (sig_y*offsetx)) == j ){
                rest_y = 1;
                jump_c = true;
            }
        }

        if( (i + (rest_x * sig_x) + (sig_y*offsetx*rest_y)) == j || jump_c ){

            // cost x
            cost_x = index = false;
            last_index = i;

            for(a = 1; a <= rest_x; a++){
                index = sig_x*a + i;
                if(!cit[index].cost ||
                   ((cit[index].pos != pos_edge_top && cit[index].pos != pos_center
                     && cit[index].pos != pos_edge_bottom) && (a < rest_x))){
                    cost_x = 0;
                    break;
                }
                cost_x += (cit[index].cost == -1 ? 0 : cit[index].cost )+
                          (cit[last_index].cost == -1 ? 0 : cit[last_index].cost );
                last_index = index;
            }

            if(cost_x){
                for(b = 1; b <= rest_y; b++){
                    index += sig_y*offsetx;
                    if(!cit[index].cost ||
                       ((cit[index].pos != pos_center && cit[index].pos != pos_edge_left &&
                         cit[index].pos != pos_edge_right)&& (a < rest_y))){
                        cost_x = 0;
                        break;
                    }
                    cost_x += (cit[index].cost == -1 ? 0 : cit[index].cost )+
                            (cit[last_index].cost == -1 ? 0 : cit[last_index].cost );
                    last_index = index;
                }
            }

            // cost y
            cost_y = index = false;
            last_index = i;
            for(a = 1; a <= rest_y; a++){
                index = i + (sig_y*a*offsetx);
                if(!cit[index].cost ||
                   ((cit[index].pos != pos_center && cit[index].pos != pos_edge_left &&
                     cit[index].pos != pos_edge_right)&& (a < rest_y))){
                    cost_y = 0;
                    break;
                }
                cost_y += (cit[index].cost == -1 ? 0 : cit[index].cost )+
                          (cit[last_index].cost == -1 ? 0 : cit[last_index].cost );
                last_index = index;
            }
            if(cost_y){
                for(b = 1; b <= rest_x; b++) {
                    index += sig_x;
                    if(!cit[index].cost ||
                       ((cit[index].pos != pos_edge_top && cit[index].pos != pos_center &&
                         cit[index].pos != pos_edge_bottom) && (b < rest_x))){
                        cost_y = 0;
                        break;
                    }
                    cost_y += (cit[index].cost == -1 ? 0 : cit[index].cost )+
                              (cit[last_index].cost == -1 ? 0 : cit[last_index].cost );
                    last_index = index;
                }
            }
            return !cost_x ? cost_y : !cost_y ? cost_x : (cost_x > cost_y) ? cost_y : cost_x;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {

    int size_x, size_y, size_adj_matrix;
    int **adj_matrix;
    int i, j, a, from, to, value;
    int start_for, stop_for;
    int from_x, from_y, to_x, to_y;

    FILE *map;
    City *cit;

    clock_t begin, end;
    double time_spent;

    begin = clock();

    map = fopen(argv[1], "r");
    fscanf(map, "%d %d", &size_x, &size_y);

    from = to = a = 0;

    from_x= atoi(argv[2]);
    from_y= atoi(argv[3]);
    to_x= atoi(argv[4]);
    to_y= atoi(argv[5]);

    cit = (City *) malloc(sizeof(City) * size_x * size_y);

    for(i = 0; i < size_y; i++)
        for(j = 0; j < size_x; j++) {
            fscanf( map, "%d", &cit[a].cost );
            a++;
        }
    fclose(map);
    a = 0;
    for(i = 0; i < size_y; i++)
        for(j = 0; j < size_x; j++){
            cit[a].x = j;
            cit[a].y = i;
            cit[a].pos = (i == 0 && j == 0) || (j == size_x && i == size_y) ? 0 : getPos(j, i, size_x-1, size_y-1);
            if(cit[a].x == from_x && cit[a].y == from_y) from = a;
            if(cit[a].x == to_x && cit[a].y == to_y ) to = a;
            a++;
        }

    size_adj_matrix = size_x * size_y;
    adj_matrix = (int **) malloc( (size_t)size_adj_matrix* sizeof(int *) );
    for(i = 0; i <  size_adj_matrix; i++)
        adj_matrix[i] = (int *) malloc( (size_t)size_adj_matrix* sizeof(int ) );

    for(i = 0; i < size_adj_matrix; i++)
        for(j = 0; j < size_adj_matrix; j++){
            adj_matrix[i][j] = 0;
        }

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("get matrix = %lf \n", time_spent);
    begin = clock();

    start_for =  from < to ? from : to;
    stop_for =  from > to ? from : to;
    for(i = start_for; i <= stop_for; i++)
        for(j = start_for; j <= stop_for; j++){
            value = ( (i == j || adj_matrix[i][j]) ? adj_matrix[i][j] : (getAdjMatCost(cit, i, j, atoi(argv[6]), atoi(argv[7]), size_x)));
            adj_matrix[i][j] = value;
            adj_matrix[j][i] = value;
        }
    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("getMatCost = %lf \n", time_spent);
    begin = clock();

    dijkstra(adj_matrix, size_adj_matrix, from, to);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("dijk = %lf \n", time_spent);

    free(cit);
    for(i = 0; i <  size_adj_matrix; i++) free(adj_matrix[i]);
    free(adj_matrix);

    return EXIT_SUCCESS;
}

//Coded by Yehezkiel
