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
// O2 includes

#include <iostream>
#include <TFile.h>
#include <TTree.h>

#include "CommonDataFormat/InteractionRecord.h"
#include "CommonDataFormat/IRFrame.h"

using o2::InteractionRecord;
using o2::dataformats::IRFrame;

void checkBCRange(const char* filename = "AO2D.root")
{

  // Open the ROOT file and get the trees
  TFile inputFile(filename, "READ");

  // Loop over the TDirectories in the input file
  for (auto directoryKey : *inputFile.GetListOfKeys()) {

    // std::cout << "Processing directory " << directoryKey->GetName() << std::endl;
    auto dirName = directoryKey->GetName();
    TTree* treeRanges = dynamic_cast<TTree*>(inputFile.Get(Form("%s/O2bcranges", dirName)));
    TTree* treeDecision = dynamic_cast<TTree*>(inputFile.Get(Form("%s/O2cefpdecision", dirName)));
    if (!treeRanges || !treeDecision) {
      std::cerr << "Error: could not find the required trees in directory " << dirName << std::endl;
      continue;
    }

    // Get the branches we need from the trees
    ULong64_t bcstart, bcend, globalBCId, cefpSelected = 0;
    treeRanges->SetBranchAddress("fBCstart", &bcstart);
    treeRanges->SetBranchAddress("fBCend", &bcend);
    treeDecision->SetBranchAddress("fGlobalBCId", &globalBCId);
    treeDecision->SetBranchAddress("fCefpSelected", &cefpSelected);

    std::vector<InteractionRecord> bcids;
    // Loop over the entries in the decision tree and check if the BC range is valid
    int nEntriesDecision = treeDecision->GetEntries();
    for (int iEntryDecision = 0; iEntryDecision < nEntriesDecision; ++iEntryDecision) {
      treeDecision->GetEntry(iEntryDecision);
      if (cefpSelected == 0)
        continue;
      InteractionRecord ir;
      ir.setFromLong(globalBCId);
      bcids.push_back(ir);
    }
    std::vector<bool> found(bcids.size(), false);

    // Loop over the entries in the ranges tree and check if the BC range is valid
    int nEntriesRanges = treeRanges->GetEntries();
    for (int iEntryRanges = 0; iEntryRanges < nEntriesRanges; ++iEntryRanges) {
      treeRanges->GetEntry(iEntryRanges);
      InteractionRecord irstart, irend;
      irstart.setFromLong(bcstart);
      irend.setFromLong(bcend);
      IRFrame frame(irstart, irend);
      for (uint32_t i = 0; i < bcids.size(); i++) {
        auto& bcid = bcids[i];
        if (!frame.isOutside(bcid)) {
          found[i] = true;
        }
      }
    }
    int notFound = 0;
    for (uint32_t i = 0; i < bcids.size(); i++) {
      notFound += !found[i];
    }
    std::cout << "Found " << notFound << " BCs not in ranges out of " << bcids.size() << std::endl;
  }
}

void checkBCRange(const char* filename, const char* rangeFileName)
{

  // Open the ROOT file and get the trees
  TFile inputFile(filename);
  TFile rangeFile(rangeFileName);

  std::vector<IRFrame> frames;
  for (auto key : *rangeFile.GetListOfKeys()) {
    auto dir = dynamic_cast<TDirectory*>(rangeFile.Get(key->GetName()));
    if (!dir) {
      continue;
    }
    auto tree = dynamic_cast<TTree*>(dir->Get("O2bcranges"));
    if (!tree) {
      continue;
    }
    ULong64_t bcstart, bcend;
    tree->SetBranchAddress("fBCstart", &bcstart);
    tree->SetBranchAddress("fBCend", &bcend);
    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);
      InteractionRecord irstart, irend;
      irstart.setFromLong(bcstart);
      irend.setFromLong(bcend);
      frames.emplace_back(irstart, irend);
    }
  }

  TList* directoryList = inputFile.GetListOfKeys();
  std::vector<InteractionRecord> bcids;
  for (auto key : *inputFile.GetListOfKeys()) {
    TTree* treeDecision = dynamic_cast<TTree*>(inputFile.Get(Form("%s/O2cefpdecision", key->GetName())));
    if (!treeDecision) {
      continue;
    }
    // Get the branches we need from the trees
    ULong64_t globalBCId, cefpSelected = 0;
    treeDecision->SetBranchAddress("fGlobalBCId", &globalBCId);
    treeDecision->SetBranchAddress("fCefpSelected", &cefpSelected);

    // Loop over the entries in the decision tree and check if the BC range is valid
    int nEntriesDecision = treeDecision->GetEntries();
    for (int iEntryDecision = 0; iEntryDecision < nEntriesDecision; ++iEntryDecision) {
      treeDecision->GetEntry(iEntryDecision);
      if (cefpSelected == 0)
        continue;
      InteractionRecord ir;
      ir.setFromLong(globalBCId);
      bcids.push_back(ir);
    }
  }

  std::vector<bool> found(bcids.size(), false);
  for (uint32_t i{0}; i < bcids.size(); i++) {
    for (auto& frame : frames) {
      if (!frame.isOutside(bcids[i])) {
        found[i] = true;
        break;
      }
    }
  }
  int notFound = 0;
  for (uint32_t i = 0; i < bcids.size(); i++) {
    notFound += !found[i];
  }
  std::cout << "Found " << notFound << " BCs not in ranges out of " << bcids.size() << std::endl;
}