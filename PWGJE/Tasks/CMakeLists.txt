# Copyright 2019-2020 CERN and copyright holders of ALICE O2.
# See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
# All rights not expressly granted are reserved.
#
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "COPYING".
#
# In applying this license CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

o2physics_add_dpl_workflow(emc-cellmonitor
                    SOURCES emccellmonitor.cxx
                    PUBLIC_LINK_LIBRARIES O2::Framework O2::EMCALBase O2::EMCALCalib O2Physics::AnalysisCore
                     COMPONENT_NAME Analysis)
o2physics_add_dpl_workflow(emc-clustermonitor
                    SOURCES emcclustermonitor.cxx
                    PUBLIC_LINK_LIBRARIES O2::Framework O2::EMCALBase O2::EMCALCalib O2Physics::AnalysisCore
                    COMPONENT_NAME Analysis)
o2physics_add_dpl_workflow(emc-eventselection-qa
                    SOURCES emceventselectionqa.cxx
                    PUBLIC_LINK_LIBRARIES O2::Framework O2::EMCALBase O2::EMCALCalib O2Physics::AnalysisCore
                    COMPONENT_NAME Analysis)
o2physics_add_dpl_workflow(emc-tmmonitor
                    SOURCES emctmmonitor.cxx
                    PUBLIC_LINK_LIBRARIES O2::Framework O2::EMCALBase O2::EMCALCalib O2Physics::AnalysisCore
                    COMPONENT_NAME Analysis)

if(FastJet_FOUND)
    o2physics_add_dpl_workflow(jet-substructure
                        SOURCES jetsubstructure.cxx
                        PUBLIC_LINK_LIBRARIES O2::Framework O2Physics::PWGJECore O2Physics::AnalysisCore
                        COMPONENT_NAME Analysis)
    o2physics_add_dpl_workflow(jet-charged-trigger-qa
                        SOURCES ChJetTriggerQATask.cxx
                        PUBLIC_LINK_LIBRARIES O2::Framework O2Physics::PWGJECore O2Physics::AnalysisCore
                        COMPONENT_NAME Analysis)
    o2physics_add_dpl_workflow(jet-full-trigger-qa
                        SOURCES FullJetTriggerQATask.cxx
                        PUBLIC_LINK_LIBRARIES O2::Framework O2Physics::PWGJECore O2Physics::AnalysisCore
                        COMPONENT_NAME Analysis)
    o2physics_add_dpl_workflow(jet-matching-hf-qa
                        SOURCES jetmatchinghfqa.cxx
                        PUBLIC_LINK_LIBRARIES O2::Framework O2Physics::PWGJECore O2Physics::AnalysisCore
                        COMPONENT_NAME Analysis)
    o2physics_add_dpl_workflow(jet-substructure-hf
                        SOURCES jetsubstructurehf.cxx
                        PUBLIC_LINK_LIBRARIES O2::Framework O2Physics::PWGJECore O2Physics::AnalysisCore
                        COMPONENT_NAME Analysis)
    o2physics_add_dpl_workflow(jet-substructure-hf-output
                        SOURCES jetsubstructurehfoutput.cxx
                        PUBLIC_LINK_LIBRARIES O2::Framework O2Physics::PWGJECore O2Physics::AnalysisCore
                        COMPONENT_NAME Analysis)
endif()
