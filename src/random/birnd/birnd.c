/********************************************************
*   birnd - binomial random number generator for Max    *
*   developed by Masaki Nabeshima                       *
*********************************************************/

#include "ext.h"
#include "ext_obex.h"
#include <math.h>
typedef struct _birnd {
	t_object obj;
    double b_p;
    long b_n;
    void *b_outlet1;
} t_birnd;

/* function prototypes */
void *birnd_new(t_symbol *s, long argc, t_atom *argv);
void birnd_free(t_birnd *x);
void birnd_bang(t_birnd *x);
void birnd_in1(t_birnd *x, long n);
void birnd_ft2(t_birnd *x, double f);//
double trunc_helper_p(double f);
long trunc_helper_n(long n);

void *birnd_class;

int C74_EXPORT main(void) {
	t_class *c;
	
	c = class_new("birnd", (method)birnd_new, (method)birnd_free, (long)sizeof(t_birnd), 
				  0L , A_GIMME, 0);
	
    class_addmethod(c, (method)birnd_bang, "bang", 0);
    class_addmethod(c, (method)birnd_in1, "in1", A_LONG, 0);
    class_addmethod(c, (method)birnd_ft2, "ft2", A_FLOAT, 0);
    
	class_register(CLASS_BOX, c);
	birnd_class = c;

	return 0;
}

void birnd_bang(t_birnd *x) {
    /* n<=0 -> 0, n>0 -> OK              *
     * p<=0 -> 0, 1>p>0 ->OK, p>=1 -> 1  */
    long num = 0;
    double rnd = 0;
    long i;
    for(i = 0; i < x->b_n; i++) {
        rnd = (double)rand()/(double)RAND_MAX;
        if(rnd <= x->b_p) {
            num++;
        }
    }
    outlet_int(x->b_outlet1, num);
}

void birnd_ft2(t_birnd *x, double f) {
    /* f must be in range [0, 1] */
    x->b_p = trunc_helper_p(f);
}

void birnd_in1(t_birnd *x, long n) {
    /* n must be greater than or equal to 0 */
    x->b_n = trunc_helper_n(n);
}

double trunc_helper_p(double f) {
    if(f > (double)1) {
        f = (double)1;
    } else if (f < (double)0) {
        f = (double)0;
    }
    return f;
}

long trunc_helper_n(long n) {
    if(n < 0) {
        n = 0;
    }
    return n;
}


void birnd_free(t_birnd *x) {
    ;
}

void *birnd_new(t_symbol *s, long argc, t_atom *argv)
{
	t_birnd *x = NULL;

    x = (t_birnd *)object_alloc(birnd_class);
    
    intin(x,1);
    floatin(x,2);
    x->b_outlet1 = intout((t_object *)x);
    
    x->b_p = (double)0.5;
    x->b_n = (long)10;
    
    long i;
    for(i = 0; i < argc && i < 2; i++) {
        if((argv+i)->a_type == A_FLOAT || (argv+i)->a_type == A_LONG) {
            if(i == 0) {
                x->b_p = trunc_helper_p((double)atom_getfloat(argv+i));
            } else {
                x->b_n = trunc_helper_n((long)atom_getlong(argv+i));
            }
        }
    }

	return (x);
}

