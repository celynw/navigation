/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2008, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: TKruse
 *********************************************************************/

#ifndef SIMPLE_TRAJECTORY_GENERATOR_H_
#define SIMPLE_TRAJECTORY_GENERATOR_H_

#include <base_local_planner/trajectory_sample_generator.h>
#include <base_local_planner/local_planner_limits.h>
#include <Eigen/Core>

namespace base_local_planner {

/**
 * generates trajectories based on equi-distant discretisation of the degrees of freedom.
 * This is supposed to be a simple and robust implementation of the TrajectorySampleGenerator
 * interface, more efficient implementations are thinkable.
 */
class SimpleTrajectoryGenerator: public base_local_planner::TrajectorySampleGenerator {
public:

  SimpleTrajectoryGenerator() {
    limits_ = NULL;
  }

  ~SimpleTrajectoryGenerator() {}

  /**
   * @param pos current robot position
   * @param vel current robot velocity
   * @param limits Current velocity limits
   * @param sim_period distance between points in one trajectory
   * @param vsamples: in how many samples to divide the given dimension
   * @param use_acceleration_limits: if true use physical model, else idealized robot model
   * @param additional_samples (deprecated): Additional velocity samples to generate individual trajectories from.
   */
  void initialise(
      const Eigen::Vector3f& pos,
      const Eigen::Vector3f& vel,
      base_local_planner::LocalPlannerLimits* limits,
      const double sim_period,
      const Eigen::Vector3f& vsamples,
      bool use_acceleration_limits,
      std::vector<Eigen::Vector3f> additional_samples);

  /**
   * @param pos current robot position
   * @param vel current robot velocity
   * @param limits Current velocity limits
   * @param sim_period distance between points in one trajectory
   * @param vsamples: in how many samples to divide the given dimension
   * @param use_acceleration_limits: if true use physical model, else idealized robot model
   */
  void initialise(
      const Eigen::Vector3f& pos,
      const Eigen::Vector3f& vel,
      base_local_planner::LocalPlannerLimits* limits,
      const double sim_period,
      const Eigen::Vector3f& vsamples,
      bool use_acceleration_limits);

  void setParameters(double sim_time, double sim_granularity, double angular_sim_granularity);

  /**
   * Whether this generator can create more trajectories
   */
  bool hasMoreTrajectories();

  /**
   * Whether this generator can create more trajectories
   */
  bool nextTrajectory(Trajectory &traj);


  static Eigen::Vector3f computeNewPositions(const Eigen::Vector3f& pos,
      const Eigen::Vector3f& vel, double dt);

  Eigen::Vector3f computeNewVelocities(const Eigen::Vector3f& sample_target_vel, const Eigen::Vector3f& vel, double dt);

  bool generateTrajectory(
        Eigen::Vector3f pos,
        Eigen::Vector3f vel,
        Eigen::Vector3f sample_target_vel,
        base_local_planner::Trajectory& traj);

protected:

  unsigned int next_sample_index_;
  // to store sample params of each sample between init and generation
  std::vector<Eigen::Vector3f> sample_params_;
  base_local_planner::LocalPlannerLimits* limits_;
  Eigen::Vector3f pos_;
  Eigen::Vector3f vel_;

  // whether velocity of trajectory changes over time or not
  bool use_acceleration_limits_;

  double sim_time_, sim_granularity_, angular_sim_granularity_;
};

} /* namespace base_local_planner */
#endif /* SIMPLE_TRAJECTORY_GENERATOR_H_ */
