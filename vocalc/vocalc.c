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
  void *m_outlet1;
  void *m_outlet2;
} t_vocalc;

// function declarations
void ext_main(void *r);
void *vocalc_new();
void vocalc_bang(t_vocalc *v);
void vocalc_float(t_vocalc *v, double lo);
void vocalc_ft1(t_vocalc *v, double hi);
void vocalc_in2(t_vocalc *v, long bands);
void vocalc_ft3(t_vocalc *v, double bw);
void calculateValues(t_vocalc *v, double min, double max, long bands,
                     double bw);
double getIdealQ(double f1, double f2);

// function definitions
void ext_main(void *r)

{
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
  t_vocalc *v = (t_vocalc *)object_alloc(vocalc_class);

  // instantiate inlets and outlets
  floatin(v, 3); // bandwidth inlet
  intin(v, 2);   // band num inlet
  floatin(v, 1); // freq_hi inlet
  v->m_outlet2 = floatout((t_vocalc *)v);
  v->m_outlet1 = listout((t_vocalc *)v);

  // INITIALIZE YOUR CLASS VALUES HERE !!!!
  v->freq_lo   = 80.;
  v->freq_hi   = 12000.;
  v->num_bands = 20;
  v->bandwidth = 100;
  return v;
}

void vocalc_bang(t_vocalc *v)
{
  calculateValues(v, v->freq_lo, v->freq_hi, v->num_bands, v->bandwidth);
}

void vocalc_float(t_vocalc *v, double lo)
{
  v->freq_lo = lo;
  calculateValues(v, v->freq_lo, v->freq_hi, v->num_bands, v->bandwidth);
}

void vocalc_ft1(t_vocalc *v, double hi)
{
  v->freq_hi = hi;
  calculateValues(v, v->freq_lo, v->freq_hi, v->num_bands, v->bandwidth);
}
void vocalc_in2(t_vocalc *v, long bands)
{
  v->num_bands = bands;
  calculateValues(v, v->freq_lo, v->freq_hi, v->num_bands, v->bandwidth);
}
void vocalc_ft3(t_vocalc *v, double bw)
{
  v->bandwidth = bw;
  calculateValues(v, v->freq_lo, v->freq_hi, v->num_bands, v->bandwidth);
}

void calculateValues(t_vocalc *v, double min, double max, long bands, double bw)
{
  double scalar = (log(max) / log(min) - 1.) / (bands - 1);

  t_atom centerFreqs[bands];
  double f1, f2;
  for (int i = 0; i < bands; i++) {
    double f = pow(min, (1. + (scalar * i)));
    if (i == 0)
      f1 = f;
    else if (i == 1)
      f2 = f;

    atom_setfloat(centerFreqs + i, f);
  }

  double idealQ = getIdealQ(f1, f2);
  outlet_list(v->m_outlet1, 0L, bands, centerFreqs);
  outlet_float(v->m_outlet2, idealQ);
}

double getIdealQ(double f1, double f2) { return sqrt(f1 * f2) / (f2 - f1); }
