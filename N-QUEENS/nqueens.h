#include <stdio.h>
#include <stdlib.h>
int **initboard(int n)
{
    /*
    retourne la table du jeu qui est une matrice de n*n initialise avec des 0 
    qui indiquent qu'aucune case de la table n'est attaquée initialement, si une 
    case est négative |case| indique le nombre de reines sur la table qui attaque 
    la case, si une case est positive ça indique qu'une raine est dans cette 
    case.
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
    affiche la table du jeu (board)
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
      place une reine en position 'position' de la table du jeu (board) et 
      soustrait 1 à toutes les positions attaquées par cette reine.
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
     supprime la reine dans la position 'position' 
     et ajoute 1 à toutes les positions précisément attaquées 
     par cette reine
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
    retourne la NP eme position non attaque dans la 
    table du jeu (board), si on utilise cette fonction 
    pour trouver les solutions, l'algorithme trouvera 
    toutes les solutions possibles y compris la permutation 
    des reines qui conduira à une complexité tomporelle supérieure à n! ,
    mais nous savons que si nous trouvons une solution et changeons 
    la position des reines, cela conduira à la même solution,
    la fonction suivante (getNPthNotAttackedPositionOfLine) éliminera la permutation entre les reines.
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
   retourne la NP eme position non attaque dans la ligne 'line' de la table du jeu (board),
   dans cette fonction nous utilisons le fait que si une solution existe chaque reine est 
   placée dans une ligne différente des autres reines ( si deux reines sont dans la même ligne elles s'attaquent ),
   ce qui limitera la recherche à ne pas explorer la permutation de reines (une reine peut être placée sur une seule 
   ligne dans toutes les solutions).
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
    vérifier si une solution est valide, cette fonction n'est pas utilisée dans la recherche 
    d'une manière directe ,la vérification de la validite se fait avant de placer une reine dans notre solution.
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
Pour trouver une solution nous utiliserons trois composants , 
la table du jeu (board), une table qui tiendra la position des reines dans 
la table du jeu (poses), une table (state) qui contient pour chaque reine les 
NP qui indiquent la place prise entre les places disponibles après que les reines 
précédentes aient été placées sur la table du jeu.
*/
void getFirstSolution(int n)
{
    /*
    trouve et affiche la première solution si elle existe.
    */
    int poses[n], state[n], pos = 0, i = 0;
    int **board = initboard(n);
    for (i = 0; i < n; i++)
    {
        state[i] = 1;
    }
    i = 0;
    while (i < n) // tant qu'on n'a pas placé toutes les reines
    {
        pos = getNPthNotAttackedPosition(board, n, state[i]); // isi , l'utilisation de getNPthNotAttackedPositionOfLine va mener au meme resultat
        if (pos == -1) /* blockage !,
        nous notons que l'utilisation de cette condition 
        et de la fonction ci-dessus pour trouver la position éliminera 
        le besoin de vérifier si après avoir placé la reine la solution 
        est valide car nous ne plaçons la reine que si la position n'est 
        pas attaquée si elle est attaquée pos sera égal à -1.
        */
        {
            /*
            dans ce cas la reine ne peut pas être mise à la position désirée (NP eme position n'est pas disponible) 
            ce qui signifie que tous les NP plus grands que le NP actuel sont automatiquement éliminés (si on ne peut pas 
            placer une reine dans la NP eme position disponible car le les positions disponibles sont inférieures à NP nous ne pouvons pas 
            la placer dans la NP+1 eme position) et donc on remet NP à 1 et on change la position de la reine précédente
            */
            state[i] = 1;
            i--;
            if (i < 0)
            {
                /*
                ici, l'algorithme a essayé toutes les positions possibles et a 
                conclu que si nous voulons trouver une solution valide, nous devons 
                changer la position de la reine -1 ce qui n'est pas logique donc il n'y a pas 
                de solution.
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
            on peut plasser la reine dans la position désirée (pos)
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
    trouve et retourne la première solution si elle existe.
    */
    int *poses = malloc(n*sizeof(int)), state[n], pos = 0, i = 0;
    int **board = initboard(n);
    for (i = 0; i < n; i++)
    {
        state[i] = 1;
    }
    i = 0;
    while (i < n) // tant qu'on n'a pas placé toutes les reines
    {
        pos = getNPthNotAttackedPositionOfLine(board, n, state[i],i); // isi , l'utilisation de getNPthNotAttackedPositionOfLine va mener au meme resultat
        if (pos == -1) /* blockage !,
        nous notons que l'utilisation de cette condition 
        et de la fonction ci-dessus pour trouver la position éliminera 
        le besoin de vérifier si après avoir placé la reine la solution 
        est valide car nous ne plaçons la reine que si la position n'est 
        pas attaquée si elle est attaquée pos sera égal à -1.
        */
        {
            /*
            dans ce cas la reine ne peut pas être mise à la position désirée (NP eme position n'est pas disponible) 
            ce qui signifie que tous les NP plus grands que le NP actuel sont automatiquement éliminés (si on ne peut pas 
            placer une reine dans la NP eme position disponible car le les positions disponibles sont inférieures à NP nous ne pouvons pas 
            la placer dans la NP+1 eme position) et donc on remet NP à 1 et on change la position de la reine précédente
            */
            state[i] = 1;
            i--;
            if (i < 0)
            {
                /*
                ici, l'algorithme a essayé toutes les positions possibles et a 
                conclu que si nous voulons trouver une solution valide, nous devons 
                changer la position de la reine -1 ce qui n'est pas logique donc il n'y a pas 
                de solution.
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
            on peut plasser la reine dans la position désirée (pos)
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
    while (i < n) // on a pas trouver tous les solutions
    {
        pos = getNPthNotAttackedPositionOfLine(board, n, state[i],i); 
        /* 
        isi , l'utilisation de getNPthNotAttackedPosition va mener a trouver
        toutes les permutation des solutions possibles qui conduira à une complexité 
        tomporelle supérieure à n!
        */
        if (pos == -1) /* blockage !,
        nous notons que l'utilisation de cette condition 
        et de la fonction ci-dessus pour trouver la position éliminera 
        le besoin de vérifier si après avoir placé la reine la solution 
        est valide car nous ne plaçons la reine que si la position n'est 
        pas attaquée si elle est attaquée pos sera égal à -1.
        */
        {
            /*
            dans ce cas la reine ne peut pas être mise à la position désirée (NP eme position n'est pas disponible) 
            ce qui signifie que tous les NP plus grands que le NP actuel sont automatiquement éliminés (si on ne peut pas 
            placer une reine dans la NP eme position disponible car le les positions disponibles sont inférieures à NP nous ne pouvons pas 
            la placer dans la NP+1 eme position) et donc on remet NP à 1 et on change la position de la reine précédente
            */
            state[i] = 1;
            i--;
            if (i < 0)
            {
                /*
                ici, l'algorithme a essayé toutes les positions possibles partant de la derniere solution et a 
                conclu que si nous voulons trouver une solution valide, nous devons 
                changer la position de la reine -1 ce qui n'est pas logique donc on a trouver tous les solution.
                */
                return nbsolu;
            }
            removeQueen(board, n, poses[i]);
            state[i]++;
        }
        else
        {
            /*
            on peut plasser la reine dans la position désirée (pos)
            */
            poses[i] = pos;
            placeQueen(board, n, pos);
            i++;
        }
        if (i >= n) 
        /* 
        si nous avons trouvé une solution nous essayons
        de trouver la prochaine solution on changant le NP de 
        la dernière reine
        */
        {
            i--;
            removeQueen(board, n, poses[i]);
            nbsolu++;
            state[i]++;
            
        }
    }
}

