#include <stdio.h>
#include <stdlib.h>
int **initboard(int n)
{
    /*
    returns the game table which is an n*n matrix initialized with 0s
    which indicate that no box of the table is attacked initially, if a
    case is negative |case| indicates the number of queens on the table that is attacking
    box, if a box is positive it indicates that a groove is in this
    box.
    */
    int **board = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        board[i] = (int *)malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            board[i][j] = 0;
        }
    }
    return board;
}
void printboard(int **board, int n)
{
    /*
    displays the game table (board)
    */
    for (int i = 0; i < n; i++)
    {

        for (int j = 0; j < n; j++)
        {
            if(board[i][j]<=0)
            {
              printf("%4d ", board[i][j]);  
            }
            else{
                printf("%4c ", 'Q');  
            }
            
            
        }
        printf("\n\n\n");
    }
}
void placeQueen(int **board, int n, int position)
{
    /*
      places a queen in position 'position' of the game table (board) and
      subtracts 1 from all positions attacked by this queen.
    */
    int posx = position % n, posy = position / n;
    board[posy][posx] = position + 1;
    for (int x = 0; x < n; x++)
    {
        if (x != posx)
            board[posy][x]--;
    }
    for (int y = 0; y < n; y++)
    {
        if (y != posy)
            board[y][posx]--;
    }
    int xy = 1;
    while (posx + xy < n && posy + xy < n)
    {
        board[posy + xy][posx + xy]--;
        xy++;
    }
    int mxy = 1;
    while (posx - mxy >= 0 && posy + mxy < n)
    {
        board[posy + mxy][posx - mxy]--;
        mxy++;
    }
    int xmy = 1;
    while (posx + xmy < n && posy - xmy >= 0)
    {
        board[posy - xmy][posx + xmy]--;
        xmy++;
    }
    int mxmy = 1;
    while (posx - mxmy >= 0 && posy - mxmy >= 0)
    {
        board[posy - mxmy][posx - mxmy]--;
        mxmy++;
    }
}
void removeQueen(int **board, int n, int position)
{
    /*
     removes the queen in position 'position'
     and adds 1 to all precisely attacked positions
     by this queen
    */
    int posx = position % n, posy = position / n;
    board[posy][posx] = 0;
    for (int x = 0; x < n; x++)
    {
        if (x != posx)
            board[posy][x]++;
    }
    for (int y = 0; y < n; y++)
    {
        if (y != posy)
            board[y][posx]++;
    }
    int xy = 1;
    while (posx + xy < n && posy + xy < n)
    {
        board[posy + xy][posx + xy]++;
        xy++;
    }
    int mxy = 1;
    while (posx - mxy >= 0 && posy + mxy < n)
    {
        board[posy + mxy][posx - mxy]++;
        mxy++;
    }
    int xmy = 1;
    while (posx + xmy < n && posy - xmy >= 0)
    {
        board[posy - xmy][posx + xmy]++;
        xmy++;
    }
    int mxmy = 1;
    while (posx - mxmy >= 0 && posy - mxmy >= 0)
    {
        board[posy - mxmy][posx - mxmy]++;
        mxmy++;
    }
}
int getNPthNotAttackedPosition(int **board, int n, int NP)
{
    /*
    returns the NP th position not attacked in the
    game table (board), if this function is used
    to find the solutions, the algorithm will find
    all possible solutions including permutation
    queens which will lead to a tomporal complexity of n^n ,
    but we know that if we find a solution and change
    the position of the queens, this will lead to the same solution,
    the following function (getNPthNotAttackedPositionOfLine) will eliminate swapping between queens.
    */
    for (int position = 0; position < n * n; position++)
    {
        if (board[position / n][position % n] == 0)
        {
            NP--;
            if (NP == 0)
            {
                return position;
            }
        }
    }
    return -1;
}
int getNPthNotAttackedPositionOfLine(int **board, int n, int NP,int line)
{
   /*
   returns the NP th position not attacked in the line 'line' of the game table (board),
   in this function we use the fact that if a solution exists each queen is
   placed in a line different from the other queens (if two queens are in the same line they attack each other),
   which will limit the search not to explore the permutation of queens (a queen can be placed on a single
   row in all solutions).
   */
    for (int position = 0; position < n; position++)
    {
        if (board[line][position] == 0)
        {
            NP--;
            if (NP == 0)
            {
                return line*n+position;
            }
        }
    }
    return -1;
}
int checkSolution(int* poses,int n)
{
    /*
    check if a solution is valid, this function is not used in search
    in a direct way, the validity check is done before placing a queen in our solution.
    */
    int **board = initboard(n);
    int x,y;
    if(poses == NULL)
    {
        return 0;
    }
    for(int i = 0 ; i < n ; i++)
    {
        x = poses[i]%n;
        y = poses[i]/n;
        if(poses[i]<0 || poses[i]> n*n)
        {
            return 0;
        }

        if(board[y][x]!=0)
        {
            return 0;
        }
        placeQueen(board,n,poses[i]);
    }
    return 1;
}
/*
To find a solution we will use three components,
the game table (board), a table that will hold the position of the queens in
the game table (poses), a table (state) which contains for each queen the
NP which indicate the place taken between the places available after the queens
previous ones have been placed on the game table.
*/
void getFirstSolution(int n)
{
    /*
     finds and displays the first solution if it exists.
    */
    int poses[n], state[n], pos = 0, i = 0;
    int **board = initboard(n);
    for (i = 0; i < n; i++)
    {
        state[i] = 1;
    }
    i = 0;
    while (i < n) // as long as we haven't placed all the queens
    {
        pos = getNPthNotAttackedPosition(board, n, state[i]); 
        if (pos == -1) /* blockage!,
        we note that the use of this condition
        and the above function to find the position will eliminate
        the need to check if after placing the queen the solution
        is valid because we only place the queen if the position is not
        not attacked if attacked pos will be -1.
        */
        {
            /*
            in this case the queen cannot be put in the desired position (NP th position is not available)
            which means that all NPs larger than the current NP are automatically eliminated (if we cannot
            place a queen in the NP th available position because the available positions are lower than NP we cannot
            place it in the NP+1 th position) and therefore we reset NP to 1 and we change the position of the previous queen
            */
            state[i] = 1;
            i--;
            if (i < 0)
            {
                /*
                here the algorithm has tried all possible positions and has
                concluded that if we want to find a valid solution, we must
                change the position of the queen -1 which does not make sense so there is no
                of solution.
                */
                printf("pas de solu ! \n");
                return;
            }
            removeQueen(board, n, poses[i]);
            state[i]++;
        }
        else
        {
            /*
             we can place the queen in the desired position (pos)
            */
            poses[i] = pos;
            placeQueen(board, n, pos);
            i++;
        }
    }
    int **demo = initboard(n);
    for (int i = 0; i < n; i++)
    {
        printf("%d (%d,%d)", poses[i], poses[i] / n, poses[i] % n);
        placeQueen(demo, n, poses[i]);
    }
    printf("\n-------- demo ------- \n");
    printboard(demo, n);
    printf("\n");
}

int* getFirstSolutionVect(int n)
{
    /*
     finds and returns the first solution if it exists.
    */
    int *poses = malloc(n*sizeof(int)), state[n], pos = 0, i = 0;
    int **board = initboard(n);
    for (i = 0; i < n; i++)
    {
        state[i] = 1;
    }
    i = 0;
    while (i < n) // as long as we haven't placed all the queens
    {
        pos = getNPthNotAttackedPositionOfLine(board, n, state[i],i); // isi , l'utilisation de getNPthNotAttackedPositionOfLine va mener au meme resultat
        if (pos == -1) /* blockage!,
        we note that the use of this condition
        and the above function to find the position will eliminate
        the need to check if after placing the queen the solution
        is valid because we only place the queen if the position is not
        not attacked if attacked pos will be -1.
        */
        {
            /*
            in this case the queen cannot be put in the desired position (NP th position is not available)
            which means that all NPs larger than the current NP are automatically eliminated (if we cannot
            place a queen in the NP th available position because the available positions are lower than NP we cannot
            place it in the NP+1 th position) and therefore we reset NP to 1 and we change the position of the previous queen
            */
            state[i] = 1;
            i--;
            if (i < 0)
            {
                /*
                here the algorithm has tried all possible positions and has
                concluded that if we want to find a valid solution, we must
                change the position of the queen -1 which does not make sense so there is no
                of solution.
                */
                printf("pas de solu ! \n");
                return NULL;
            }
            removeQueen(board, n, poses[i]);
            state[i]++;
        }
        else
        {
            /*
             we can place the queen in the desired position (pos)
            */
            poses[i] = pos;
            placeQueen(board, n, pos);
            i++;
        }
    }
    return poses;
}

int getNbSolutionsOfNQueen(int n)
{
    int poses[n], state[n], pos = 0, i = 0, nbsolu = 0;
    int **board = initboard(n);
    for (i = 0; i < n; i++)
    {
        state[i] = 1;
    }
    i = 0;
    while (i < n) // we haven't found all the solutions
    {
        pos = getNPthNotAttackedPositionOfLine(board, n, state[i],i); 
        /* 
        here , using getNPthNotAttackedPosition will lead to finding
        all permutations of possible solutions that will lead to complexity
        n^n tomporelle
        */
        if (pos == -1) /* blockage!,
        we note that the use of this condition
        and the above function to find the position will eliminate
        the need to check if after placing the queen the solution
        is valid because we only place the queen if the position is not
        not attacked if attacked pos will be -1.
        */
        {
            /*
            in this case the queen cannot be put in the desired position (NP th position is not available)
            which means that all NPs larger than the current NP are automatically eliminated (if we cannot
            place a queen in the NP th available position because the available positions are lower than NP we cannot
            place it in the NP+1 th position) and therefore we reset NP to 1 and we change the position of the previous queen
            */
            state[i] = 1;
            i--;
            if (i < 0)
            {
                /*
                here, the algorithm has tried all possible positions starting from the last solution and has
                concluded that if we want to find a valid solution, we must
                change the position of the queen -1 which is not logical so we have found all the solutions.
                */
                return nbsolu;
            }
            removeQueen(board, n, poses[i]);
            state[i]++;
        }
        else
        {
            /*
             we can place the queen in the desired position (pos)
            */
            poses[i] = pos;
            placeQueen(board, n, pos);
            i++;
        }
        if (i >= n) 
        /* 
        if we found a solution we try
        to find the next solution by changing the NP of
        the last queen
        */
        {
            i--;
            removeQueen(board, n, poses[i]);
            nbsolu++;
            state[i]++;
            
        }
    }
}

