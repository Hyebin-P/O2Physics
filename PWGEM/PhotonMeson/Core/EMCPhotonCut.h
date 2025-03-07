// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

//
// Class for emcal photon selection
//

#ifndef PWGEM_PHOTONMESON_CORE_EMCPHOTONCUT_H_
#define PWGEM_PHOTONMESON_CORE_EMCPHOTONCUT_H_

#include <set>
#include <vector>
#include <utility>
#include <string>
#include "Framework/Logger.h"
#include "Framework/DataTypes.h"
#include "Rtypes.h"
#include "TNamed.h"

class EMCPhotonCut : public TNamed
{
 public:
  EMCPhotonCut() = default;
  EMCPhotonCut(const char* name, const char* title) : TNamed(name, title) {}

  enum class EMCPhotonCuts : int {
    // cluster cut
    kEnergy = 0,
    kNCell,
    kM02,
    kTiming,
    kTM,
    kExotic,
    kNCuts
  };

  static const char* mCutNames[static_cast<int>(EMCPhotonCuts::kNCuts)];

  // Temporary function to check if cluster passes selection criteria. To be replaced by framework filters.
  template <typename T>
  bool IsSelected(T const& cluster) const
  {
    if (!IsSelectedEMCal(cluster, EMCPhotonCuts::kEnergy)) {
      return false;
    }
    if (!IsSelectedEMCal(cluster, EMCPhotonCuts::kNCell)) {
      return false;
    }
    if (!IsSelectedEMCal(cluster, EMCPhotonCuts::kM02)) {
      return false;
    }
    if (!IsSelectedEMCal(cluster, EMCPhotonCuts::kTiming)) {
      return false;
    }
    if (!IsSelectedEMCal(cluster, EMCPhotonCuts::kTM)) {
      return false;
    }
    if (!IsSelectedEMCal(cluster, EMCPhotonCuts::kExotic)) {
      return false;
    }
    return true;
  }

  // Temporary function to check if cluster passes a given selection criteria. To be replaced by framework filters.
  // Returns true if a cluster survives the cuts!
  template <typename Cluster, typename Track>
  bool IsSelectedEMCal(Cluster const& cluster, Track const& track, const EMCPhotonCuts& cut) const
  {
    float dEta = fabs(track.tracketa() - cluster.eta());
    float dPhi = fabs(track.trackphi() - cluster.phi());
    switch (cut) {
      case EMCPhotonCuts::kEnergy:
        return cluster.e() > mMinE;

      case EMCPhotonCuts::kNCell:
        return cluster.nCells() >= mMinNCell;

      case EMCPhotonCuts::kM02:
        return mMinM02 <= cluster.m02() && cluster.m02() <= mMaxM02;

      case EMCPhotonCuts::kTiming:
        return mMinTime <= cluster.time() && cluster.time() <= mMaxTime;

      case EMCPhotonCuts::kTM:
        return (dEta > mTrackMatchingEta(track.trackpt())) || (dPhi > mTrackMatchingPhi(track.trackpt())) || (cluster.e() / track.trackp() >= mMinEoverP);

      case EMCPhotonCuts::kExotic:
        return mUseExoticCut ? cluster.isExotic() : true;

      default:
        return false;
    }
  }

  // Setters
  void SetMinE(float min = 0.7f);
  void SetMinNCell(int min = 1);
  void SetM02Range(float min = 0.1f, float max = 0.7f);
  void SetTimeRange(float min = -20.f, float max = 25.f);
  void SetTrackMatchingEta(std::function<float(float)> funcTM);
  void SetTrackMatchingPhi(std::function<float(float)> funcTM);
  void SetMinEoverP(float min = 0.7f);
  void SetUseExoticCut(bool flag = true);

  /// @brief Print the cluster selection
  void print() const;

 private:
  // EMCal cluster cuts
  float mMinE{0.7f};        ///< minimum energy
  int mMinNCell{1};         ///< minimum number of cells per cluster
  float mMinM02{0.1f};      ///< minimum M02 for a cluster
  float mMaxM02{0.7f};      ///< maximum M02 for a cluster
  float mMinTime{-20.f};    ///< minimum cluster timing
  float mMaxTime{25.f};     ///< maximum cluster timing
  float mMinEoverP{1.75f};  ///< minimum cluster energy over track momentum ratio needed for the pair to be considered matched
  bool mUseExoticCut{true}; ///< flag to decide if the exotic cluster cut is to be checked or not

  std::function<float(float)> mTrackMatchingEta{}; ///< function to get check if a pre matched track and cluster pair is considered an actual match for eta
  std::function<float(float)> mTrackMatchingPhi{}; ///< function to get check if a pre matched track and cluster pair is considered an actual match for phi

  ClassDef(EMCPhotonCut, 1);
};

#endif // PWGEM_PHOTONMESON_CORE_EMCPHOTONCUT_H_
