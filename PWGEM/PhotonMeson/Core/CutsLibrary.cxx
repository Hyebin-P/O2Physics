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
// Contact: daiki.sekihata@cern.ch
//
#include "PWGEM/PhotonMeson/Core/CutsLibrary.h"

V0PhotonCut* o2::aod::pcmcuts::GetCut(const char* cutName)
{
  V0PhotonCut* cut = new V0PhotonCut(cutName, cutName);
  std::string nameStr = cutName;

  if (!nameStr.compare("analysis")) {
    cut->SetPtRange(0.01f, 1e10f);
    cut->SetEtaRange(-0.9, +0.9);
    cut->SetMinNCrossedRowsTPC(30);
    cut->SetMinNCrossedRowsOverFindableClustersTPC(0.6);
    cut->SetMaxChi2PerClusterTPC(4.0);

    cut->SetTPCNsigmaElRange(-3, +3);
    cut->SetRxyKFRange(1, 90);
    cut->SetMaxMeePsiPairDep([](float psipair) { return psipair < 0.4 ? 0.06 : 0.015; });
    return cut;
  }
  if (!nameStr.compare("qc")) {
    cut->SetPtRange(0.01f, 1e10f);
    cut->SetEtaRange(-0.9, +0.9);
    cut->SetMinNCrossedRowsTPC(30);
    cut->SetMinNCrossedRowsOverFindableClustersTPC(0.6);
    cut->SetMaxChi2PerClusterTPC(4.0);

    cut->SetTPCNsigmaElRange(-3, +3);
    cut->SetRxyKFRange(0, 180);
    cut->SetMaxMeePsiPairDep([](float psipair) { return psipair < 0.4 ? 0.06 : 0.015; });
    return cut;
  }
  if (!nameStr.compare("nocut")) {
    cut->SetPtRange(0.01f, 1e10f);
    cut->SetEtaRange(-0.9, +0.9);
    cut->SetMinNCrossedRowsTPC(20);
    cut->SetMinNCrossedRowsOverFindableClustersTPC(0.6);
    cut->SetMaxChi2PerClusterTPC(4.0);
    return cut;
  }

  delete cut;
  LOGF(info, Form("Did not find cut %s", cutName));
  return nullptr;
}

// PHOSPhotonCut* o2::aod::phoscuts::GetCut(const char* cutName)
// {
// }

EMCPhotonCut* o2::aod::emccuts::GetCut(const char* cutName)
{
  EMCPhotonCut* cut = new EMCPhotonCut(cutName, cutName);
  std::string nameStr = cutName;

  if (!nameStr.compare("standard")) {
    cut->SetMinE(0.7f);
    cut->SetMinNCell(1);
    cut->SetM02Range(0.1f, 0.7f);
    cut->SetTimeRange(-20.f, 25.f);

    cut->SetTrackMatchingEta([](float pT) {
      return 0.01f + pow(pT + 4.07f, -2.5f);
    });
    cut->SetTrackMatchingPhi([](float pT) {
      return 0.015f + pow(pT + 3.65f, -2.f);
    });
    cut->SetMinEoverP(1.75f);
    cut->SetUseExoticCut(true);
    return cut;
  }
  if (!nameStr.compare("nocut")) {
    cut->SetMinE(0.f);
    cut->SetMinNCell(1);
    cut->SetM02Range(0.0f, 1000.f);
    cut->SetTimeRange(-500.f, 500.f);

    cut->SetTrackMatchingEta([](float pT) {
      return -1.f;
    });
    cut->SetTrackMatchingPhi([](float pT) {
      return -1.f;
    });
    cut->SetMinEoverP(0.f);
    cut->SetUseExoticCut(false);
    return cut;
  }

  delete cut;
  LOGF(info, Form("Did not find cut %s", cutName));
  return nullptr;
}
