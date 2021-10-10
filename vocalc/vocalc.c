#include "ext.h"
#include "math.h"

// global pointer
static t_class *vocalc_class;

// declare the new object
typedef struct _vocalc {
  t_object b_obj;
  double freq_lo;
  double freq_hi;
  int num_bands;
  double bandwidth;
} t_vocalc;

// function declarations
void ext_main(void *r);
void *vocalc_new();
void vocalc_bang(t_vocalc *v);
void vocalc_float(t_vocalc *v, double lo);
void vocalc_ft1(t_vocalc *v, double hi);
void vocalc_in2(t_vocalc *v, long bands);
void vocalc_ft3(t_vocalc *v, double bw);

// function definitions
void ext_main(void *r)

{
  post("entering ext_main");
  t_class *c;

  c = class_new("vocalc", (method)vocalc_new, (method)NULL, sizeof(t_vocalc),
                0L, 0);
  class_addmethod(c, (method)vocalc_float, "float", A_FLOAT, 0);
  class_addmethod(c, (method)vocalc_bang, "bang", 0);
  class_addmethod(c, (method)vocalc_ft1, "ft1", A_FLOAT, 0);
  class_addmethod(c, (method)vocalc_in2, "in2", A_LONG, 0);
  class_addmethod(c, (method)vocalc_ft3, "ft3", A_FLOAT, 0);

  class_register(CLASS_BOX, c);

  vocalc_class = c;
}

void *vocalc_new()
{
  t_vocalc *n = (t_vocalc *)object_alloc(vocalc_class);

  // instantiate inlets
  floatin(n, 3); // bandwidth inlet
  intin(n, 2);   // band num inlet
  floatin(n, 1); // freq_hi inlet

  // INITIALIZE YOUR CLASS VALUES HERE !!!!
  n->freq_lo   = 80.;
  n->freq_hi   = 12000.;
  n->num_bands = 20;
  n->bandwidth = 100;
  return n;
}

void vocalc_bang(t_vocalc *v)
{
  post("vocalc holds values: lo: % f hi: % f bands: % d bw: % f ", v->freq_lo,
       v->freq_hi, v->num_bands, v->bandwidth);
}

void vocalc_float(t_vocalc *v, double lo)
{
  v->freq_lo = lo;
  post("inlet 0 recieved float %f", lo);
}

void vocalc_ft1(t_vocalc *v, double hi)
{
  v->freq_hi = hi;
  post("inlet 1 received message %f", hi);
}
void vocalc_in2(t_vocalc *v, long bands)
{
  v->num_bands = bands;
  post("inlet 2 received message %i", bands);
}
void vocalc_ft3(t_vocalc *v, double bw)
{
  v->bandwidth = bw;
  post("inlet 3 received message %f", bw);
}

double getScalar(double min, double max, long bands, double bw)
{
  return (log(max) / log(min) - 1.) / (bands - 1);
}
