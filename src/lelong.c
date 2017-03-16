#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>

/*
 * Calcul du prix du call am�ricain dans le
 * modele de Cox Ross Rubinstein
 *
 */

/*
 * Conventions:
 *  - un arbre constitu� uniquement d'une racine a une profondeur 0
 *  - un arbre binaire recombinant de profondeur n a (n+1) feuilles
 */

typedef struct
{
    double T;        /* maturity time                      */
    double spot;     /* initial value                      */
    double strike;
    double sigma;    /* volatility                         */
    double r;        /* instantaneous interest rate        */
    double dt;       /* time step                          */
    double up;       /* up factor                          */
    double down;     /* down factor                        */
    double p_up;     /* probability of going up            */
    double p_down;   /* probability of going down          */
    double discount; /* Discount factor over one time step */
} Model;

/* Comment the definition of VERBOSE to avoir printing trees */
#define VERBOSE
#ifdef VERBOSE
#define DO_PRINT(call) call
#else
#define DO_PRINT(call)
#endif

#define GET(A,i,j) (A[(j)+((i)*((i)+1))/2])

double payoff(double stock, double strike)
{
    return fmax(strike - stock, 0.);
}

/**
 * Compute the backward induction
 *
 * @param P line wise storage matrix of size (depth+2)
 * @param depth number of time steps
 * @param root_i
 * @param root_J (root_, i, root_j) are the coordinates of the root of the
 * tree within the full tree
 * @param m Model structure
 */
void solve_crr_tree(double *P, int depth, int root_i, int root_j, const Model *m)
{
    int i, j;
    double stock_root = m->spot * pow(m->down, root_i - root_j) * pow(m->up, root_j);
    for (i = depth + root_i ; i >= root_i ; i--)
    {
        double stock = stock_root * pow(m->down, (i - root_i));
        for (j = root_j ; j <= (i - root_i) + root_j ; j++)
        {
            if (j > i) continue;
            GET(P, i, j) = fmax(payoff(stock, m->strike),
                                m->discount * (m->p_up * GET(P, i + 1, j + 1) + m->p_down * GET(P, i + 1, j)));
            stock *= m->up / m->down;
        }
    }
}

/**
 * Compute the backward induction on an upsidedown triangle
 *
 * @param P line wise storage matrix of size (depth+2)
 * @param depth number of time steps
 * @param root_i
 * @param root_J (root_, i, root_j) are the coordinates of the lower point of
 * the tree within the full tree
 * @param m Model structure
 */
void solve_crr_tree_upsidedown(double *P, int depth, int root_i, int root_j, const Model *m)
{
    int i, j;
    double stock_root = m->spot * pow(m->down, root_i - root_j) * pow(m->up, root_j);
    for (i = root_i ; i >= root_i - depth ; i--)
    {
        double stock = stock_root / pow(m->up, (root_i - i));
        for (j = root_j - (root_i - i) ; j <= root_j ; j++)
        {
            if (j > i) continue;
            GET(P, i, j) = fmax(payoff(stock, m->strike),
                                m->discount * (m->p_up * GET(P, i + 1, j + 1) + m->p_down * GET(P, i + 1, j)));
            stock *= m->up / m->down;
        }
    }
}

/**
 * Compute the final values
 *
 * @param P line wise storage matrix of size (depth+1)
 * @param depth number of time steps
 * @param m Model structure
 */
void fill_leaves(double *P, int depth, const Model *m)
{
    int i;
    double stock = m->spot * pow(m->down, depth);
    for (i = 0 ; i < depth + 1 ; i++)
    {
        GET(P, depth, i) = payoff(stock, m->strike);
        stock *= m->up / m->down;
    }
}

/**
 * Compare two trees
 *
 * @param P
 * @param Q
 * @param depth
 *
 * @return 1 or 0
 */
int cmp_tree(const double *P, const double *Q, int depth)
{
    int i, len;
    len = ((depth + 1) * (depth + 2)) / 2;

    for (i = 0 ; i < len ; i++)
    {
        if (fabs(P[i] - Q[i]) > 1E-12)
        {
            DO_PRINT(printf("First difference at index %d (%f).\n", i, fabs(P[i] - Q[i]));)
            return 0;
        }
    }
    return 1;
}

/**
 * Copy a tree
 *
 * @param Q
 * @param depth
 *
 * @return
 */
double *copy_tree(const double *Q, int depth)
{
    int len;
    double *P;
    len = ((depth + 1) * (depth + 2)) / 2;

    P = malloc(len * sizeof(double));
    memcpy(P, Q, len * sizeof(double));
    return P;
}

/**
 * Print a subtree a lower triangular matrix with time varying with the row
 * index of the matrix
 *
 * @param P
 * @param depth
 * @param root_i
 * @param root_j
 */
void print_tree(const double *P, int depth, int root_i, int root_j)
{
    int i, j;
    for (i = root_i ; i <= root_i + depth ; i++)
    {
        for (j = root_j ; j <= root_j + (i - root_i) ; j++)
        {
            printf("%6.2f ", GET(P, i, j));
        }
        printf("\n");
    }
}

int main()
{
    int N;
    double *P;
    double debut, fin;
    Model m;

    N = 7;

    m.spot = 100;
    m.strike = 100;
    m.r = 0.05;
    m.sigma = 0.2;
    m.T = 3;

    m.dt = m.T / (double)N;
    m.up = exp(m.sigma * sqrt(m.dt));
    m.down = 1. / m.up;
    m.p_up = (m.down - exp(m.r * m.dt)) / (m.down - m.up);
    m.p_down = 1. - m.p_up;
    m.discount = exp(-m.r * m.dt);

    /* Tridiagonal matrix with size N+1 */
    P = malloc(((N + 1) * (N + 2)) / 2 * sizeof(double));

    /* Compute terminal values */
    fill_leaves(P, N, &m);

    /* Backward induction */
    //debut = omp_get_wtime();
    solve_crr_tree(P, N - 1, 0, 0, &m);
    DO_PRINT(print_tree(P, N, 0, 0));
   // fin = omp_get_wtime();
   // printf("price at time 0: %f \n", GET(P, 0, 0));
   // printf("Computation time: %f\n", fin - debut);

    double *P2;
    P2 = malloc(((N + 1) * (N + 2)) / 2 * sizeof(double));
    fill_leaves(P2, N, &m);
    solve_crr_tree_upsidedown(P2, N - 1, 0, 0, &m);
    DO_PRINT(print_tree(P, N, 0, 0));

    printf("Spot %f\n", m.spot);
    printf("Strike %f\n", m.strike);
    printf("r %f\n", m.r);
    printf("vol %f\n", m.sigma);
    printf("Maturity %f \n", m.T);

    printf("Step number %i\n", N);
    printf("time step %f\n", m.dt);
    printf("u %f\n", m.up);
    printf("d %f\n", m.down);
    printf("Pu %f \n", m.p_up);
    printf("Pd %f \n", m.p_down);

    DO_PRINT(print_tree(P, N, 0, 0));


    free(P);
    free(P2);
    exit(0);
}
