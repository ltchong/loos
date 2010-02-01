/*
  This file is part of LOOS.

  LOOS (Lightweight Object-Oriented Structure library)
  Copyright (c) 2008, Tod D. Romo, Alan Grossfield
  Department of Biochemistry and Biophysics
  School of Medicine & Dentistry, University of Rochester

  This package (LOOS) is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation under version 3 of the License.

  This package is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#if !defined(ENSEMBLES_HPP)
#define ENSEMBLES_HPP

#include <vector>
#include <boost/tuple/tuple.hpp>

#include <loos_defs.hpp>
#include <MatrixImpl.hpp>
#include <MatrixOps.hpp>


namespace loos {
  class XForm;

  //! Compute the average structure of a set of AtomicGroup objects
  AtomicGroup averageStructure(const std::vector<AtomicGroup>& ensemble);

  //! Compute the average structure from a trajectory reading only certain frames
  AtomicGroup averageStructure(const AtomicGroup&, const std::vector<XForm>&, pTraj& traj, std::vector<uint>& indices);

  //! Compute the average structure using all frames in a trajectory
  AtomicGroup averageStructure(const AtomicGroup&, const std::vector<XForm>&, pTraj& traj);

  //! Compute an iterative superposition (a la Alan)
  boost::tuple<std::vector<XForm>, greal, int> iterativeAlignment(std::vector<AtomicGroup>& ensemble, greal threshold = 1e-6, int maxiter=1000);

  //! Compute an iterative superposition by reading in frames from the Trajectory.
  /*!
   * This function will internally cache an AtomicGroup copy for each
   * frame of the trajectory.  This could chew up a lot of memory, but
   * we make the assumption that you will usually be aligning against
   * a fairly small subset of each frame...
   */
  boost::tuple<std::vector<XForm>, greal, int> iterativeAlignment(const AtomicGroup& g, pTraj& traj, std::vector<uint>& frame_indices, greal threshold = 1e-6, int maxiter=1000);

  boost::tuple<std::vector<XForm>, greal, int> iterativeAlignment(const AtomicGroup& g, pTraj& traj, greal threshold = 1e-6, int maxiter=1000);


  void applyTransforms(std::vector<AtomicGroup>& ensemble, std::vector<XForm>& xforms);

  void readTrajectory(std::vector<AtomicGroup>& ensemble, const AtomicGroup& model, pTraj trajectory);
  void readTrajectory(std::vector<AtomicGroup>& ensemble, const AtomicGroup& model, pTraj trajectory, std::vector<uint>& frames);

  RealMatrix extractCoords(std::vector<AtomicGroup>& ensemble);
  RealMatrix extractCoords(std::vector<AtomicGroup>& ensemble, std::vector<XForm>& xforms);

  void subtractAverage(RealMatrix& M);

  boost::tuple<RealMatrix, RealMatrix, RealMatrix> svd(std::vector<AtomicGroup>& ensemble, const bool align = true);


  //! Computes a PCA given an ensemble and it's average.
  /**
   * In contrast with the SVD functions, this computes a PCA using an
   * eigendecomposition.  This means that the eigenvalues are the
   * squares of the corresponding singular values...
   */
  boost::tuple<RealMatrix, RealMatrix> pca(std::vector<AtomicGroup>& ensemble, const AtomicGroup& avg);
};



#endif
