/********************************************************
*   geornd - geometric random number generator for Max  *
*   developed by Masaki Nabeshima                       *
*********************************************************/

#include "ext.h"
#include "ext_obex.h"
#include <math.h>
typedef struct _geornd {
	t_object obj;
    double g_p;
    void *g_outlet1;
} t_geornd;

/* function prototypes */
void *geornd_new(t_symbol *s, long argc, t_atom *argv);
void geornd_free(t_geornd *x);
void geornd_bang(t_geornd *x);
void geornd_ft1(t_geornd *x, double f);
double trunc_helper(double f);

void *geornd_class;

int C74_EXPORT main(void) {
	t_class *c;
	
	c = class_new("geornd", (method)geornd_new, (method)geornd_free, (long)sizeof(t_geornd), 
				  0L , A_GIMME, 0);
	
    class_addmethod(c, (method)geornd_bang, "bang", 0);
    class_addmethod(c, (method)geornd_ft1, "ft1", A_FLOAT, 0);
	
	class_register(CLASS_BOX, c);
	geornd_class = c;

	return 0;
}

void geornd_bang(t_geornd *x) {
    /* -p => INFINITY, INFINITY => 0, (0,1] => OK, 1<p => 1 */
    long num = 0;
    double rnd = 0;
    if(x->g_p <= (double)0) {
        num = INFINITY;
    } else {
        do {
            rnd = (double)rand()/(double)RAND_MAX;
            num++;
        } while(rnd > x->g_p);
    }
    
    outlet_int(x->g_outlet1, num);
}

void geornd_ft1(t_geornd *x, double f) {
    /* f must be in range [0, 1] */
    x->g_p = trunc_helper(f);
}

double trunc_helper(double f) {
    if(f > (double)1) {
        f = (double)1;
    } else if (f <= (double)0) {
        f = (double)0;
    }
    return f;
}

void geornd_free(t_geornd *x) {
    ;
}

void *geornd_new(t_symbol *s, long argc, t_atom *argv)
{
	t_geornd *x = NULL;

    x = (t_geornd *)object_alloc(geornd_class);
    
    floatin(x,1);
    x->g_outlet1 = intout((t_object *)x);
    
    if(argc > 0 && argv->a_type == A_FLOAT) {
        x->g_p = trunc_helper((double)atom_getfloat(argv));
    } else if(argc > 0 && argv->a_type == A_LONG) {
        x->g_p = trunc_helper((double)atom_getlong(argv));
    } else {
        x->g_p = (double)0.5;
    }
	return (x);
}

