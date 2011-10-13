/* -*- c++ -*- -------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
   Contributing author: Axel Kohlmeyer (Temple U)
------------------------------------------------------------------------- */

#ifndef LMP_THR_DATA_H
#define LMP_THR_DATA_H

#if defined(_OPENMP)
#include <omp.h>
#endif

namespace LAMMPS_NS {

// per thread data accumulators
// there should be one instance
// of this class for each thread.
class ThrData {
  friend class FixOMP;
  friend class ThrOMP;

 public:
  ThrData(int tid) : _tid(tid) {};
  ~ThrData() {};

  // erase accumulator contents
  void clear(int, double **, double **, double *, double *, double *);

  void check_tid(int);    // thread id consistency check
  int get_tid() const { return _tid; }; // our thread id.

  // give access to per-thread offset arrays
  double **get_f() const { return _f; };
  double **get_torque() const { return _torque; };
  double *get_de() const { return _de; };
  double *get_drho() const { return _drho; };

 protected:
  double eng_vdwl;        // non-bonded non-coulomb energy
  double eng_coul;        // non-bonded coulomb energy
  double virial_pair[6];  // virial contribution from non-bonded
  double eng_bond;        // bond energy
  double virial_bond[6];  // virial contribution from bonds
  double eng_angle;       // angle energy
  double virial_angle[6]; // virial contribution from angles
  double eng_dihed;       // dihedral energy
  double virial_dihed[6]; // virial contribution from dihedrals
  double eng_imprp;       // improper energy
  double virial_imprp[6]; // virial contribution from impropers
  double eng_kspce;       // kspace energy
  double virial_kspce[6]; // virial contribution from kspace

  // per thread segments of various force or similar arrays
  double **_f;
  double **_torque;
  double *_erforce;
  double *_de;
  double *_drho;
  // these are re-assigned per style
  double *_eatom;
  double **_vatom;

 private:
  int _tid;               // my thread id

 public:
  // compute global per thread virial contribution from per-thread force
  void virial_fdotr_compute(double **, int, int, int);

  double memory_usage();

 // disabled default methods
 private:
  ThrData() {};
};

////////////////////////////////////////////////////////////////////////
//  helper functions operating on data replicated for thread support  //
////////////////////////////////////////////////////////////////////////
// generic per thread data reduction for continous arrays of nthreads*nmax size
void data_reduce_thr(double *, int, int, int, int);
}
#endif
