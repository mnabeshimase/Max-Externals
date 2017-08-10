/********************************************************
*   berrnd - bernoullian random number generator        *
*            for Max                                    *
*   developed by Masaki Nabeshima                       *
*********************************************************/

#include "ext.h"
#include "ext_obex.h"
#include <math.h>
typedef struct _berrnd {
	t_object obj;
    double b_p;
    void *b_outlet1;
} t_berrnd;

/* function prototypes */
void *berrnd_new(t_symbol *s, long argc, t_atom *argv);
void berrnd_free(t_berrnd *x);
void berrnd_bang(t_berrnd *x);
void berrnd_ft1(t_berrnd *x, double f);
double trunc_helper(double f);

void *berrnd_class;

int C74_EXPORT main(void) {
	t_class *c;
	
	c = class_new("berrnd", (method)berrnd_new, (method)berrnd_free, (long)sizeof(t_berrnd), 
				  0L , A_GIMME, 0);
	
    class_addmethod(c, (method)berrnd_bang, "bang", 0);
    class_addmethod(c, (method)berrnd_ft1, "ft1", A_FLOAT, 0);
	
	class_register(CLASS_BOX, c);
	berrnd_class = c;

	return 0;
}

void berrnd_bang(t_berrnd *x) {
    if((double)rand()/(double)RAND_MAX <= x->b_p) {
        outlet_int(x->b_outlet1, (long)1);
    } else {
        outlet_int(x->b_outlet1, (long)0);
    }
}

void berrnd_ft1(t_berrnd *x, double f) {
    /* f must be in range [0, 1] */
    x->b_p = trunc_helper(f);
}

double trunc_helper(double f) {
    if(f > (double)1) {
        f = (double)1;
    } else if (f <= (double)0) {
        f = (double)0;
    }
    return f;
}

void berrnd_free(t_berrnd *x) {
    ;
}

void *berrnd_new(t_symbol *s, long argc, t_atom *argv)
{
	t_berrnd *x = NULL;

    x = (t_berrnd *)object_alloc(berrnd_class);
    
    floatin(x,1);
    x->b_outlet1 = intout((t_object *)x);
    
    if(argc > 0 && argv->a_type == A_FLOAT) {
        x->b_p = trunc_helper((double)atom_getfloat(argv));
    } else if(argc > 0 && argv->a_type == A_LONG) {
        x->b_p = trunc_helper((double)atom_getlong(argv));
    } else {
        x->b_p = (double)0.5;
    }
	return (x);
}

