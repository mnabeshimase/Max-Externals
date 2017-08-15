/********************************************************
*   poirnd - poisson random number generator for Max    *
*   developed by Masaki Nabeshima                       *
*********************************************************/

#include "ext.h"
#include "ext_obex.h"
#include <math.h>
#include <float.h>
typedef struct _poirnd {
	t_object obj;
    double p_lambda;
    void *p_outlet1;
} t_poirnd;

/* function prototypes */
void *poirnd_new(t_symbol *s, long argc, t_atom *argv);
void poirnd_free(t_poirnd *x);
void poirnd_bang(t_poirnd *x);
void poirnd_ft1(t_poirnd *x, double lambda);
double trunc_helper(double f);

void *poirnd_class;

int C74_EXPORT main(void) {
	t_class *c;
	
	c = class_new("poirnd", (method)poirnd_new, (method)poirnd_free, (long)sizeof(t_poirnd), 
				  0L , A_GIMME, 0);
	
    class_addmethod(c, (method)poirnd_bang, "bang", 0);
    class_addmethod(c, (method)poirnd_ft1, "ft1", A_FLOAT, 0);
	
	class_register(CLASS_BOX, c);
	poirnd_class = c;

	return 0;
}

void poirnd_bang(t_poirnd *x) {
    if(x->p_lambda <= (double)0) {
        outlet_int(x->p_outlet1, (long)0);
    } else {
        double lambdaLeft = x->p_lambda;
        long k = 0;
        double p = 1;
        double step = floor(log(DBL_MAX)-1);//e^step must be less or equal to DBL_MAX
        do {
            k++;
            p = p * ((double)rand() / (double)RAND_MAX);
            if(p < M_E && lambdaLeft > 0) {
                if(lambdaLeft > step) {
                    p = p * exp(step);
                    lambdaLeft -= step;
                } else {
                    p = p * exp(lambdaLeft);
                    lambdaLeft = -1;
                }
            }
        } while(p > 1);
        outlet_int(x->p_outlet1, (long)(k - 1));
    }
}

void poirnd_ft1(t_poirnd *x, double lambda) {
    /* f must be in range [0, Infinity) */
    x->p_lambda = trunc_helper(lambda);
}

double trunc_helper(double f) {
    if(f < (double)0) {
        f = (double)0;
    }
    return f;
}

void poirnd_free(t_poirnd *x) {
    ;
}

void *poirnd_new(t_symbol *s, long argc, t_atom *argv)
{
	t_poirnd *x = NULL;

    x = (t_poirnd *)object_alloc(poirnd_class);
    
    floatin(x,1);
    x->p_outlet1 = intout((t_object *)x);
    
    if(argc > 0 && argv->a_type == A_FLOAT) {
        x->p_lambda = trunc_helper((double)atom_getfloat(argv));
    } else if(argc > 0 && argv->a_type == A_LONG) {
        x->p_lambda = trunc_helper((double)atom_getlong(argv));
    } else {
        x->p_lambda = (double)10;
    }
	return (x);
}

