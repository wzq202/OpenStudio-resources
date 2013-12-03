/**********************************************************************
*  Copyright (c) 2008-2010, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include "ModelSimulationFixture.hpp"
#include <model_tests/ModelBin.hxx> 
#include <runmanager/Test/ToolBin.hxx>

#include <model/UtilityBill.hpp> 
#include <model/UtilityBill_Impl.hpp> 

#include <runmanager/lib/JobFactory.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/RubyJobUtils.hpp>

#include <utilities/core/Application.hpp>
#include <utilities/core/System.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Path.hpp>

#include <utilities/sql/SqlFile.hpp>

#include <boost/filesystem.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

#include <QDir>

#ifdef _MSC_VER
#include <Windows.h>
#endif

openstudio::SqlFile runSimulation(const std::string t_filename)
{ 

  openstudio::path filePath = Paths::testsPath() / openstudio::toPath(t_filename);
  openstudio::path outdir = Paths::testRunPath(); 

  outdir /= filePath.filename();
  boost::filesystem::remove_all(outdir); // Clean up test dir before starting
  boost::filesystem::create_directories(outdir);

  openstudio::path p(openstudio::toPath("rm.db"));

  openstudio::path db = outdir / p;
  openstudio::runmanager::RunManager kit(db, true);
  kit.setPaused(true);

  openstudio::runmanager::Workflow wf;

  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(
        energyPlusExePath().parent_path(), openstudio::path(), openstudio::path(), rubyExePath().parent_path(), openstudio::path(),
        openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path());


  openstudio::path epw = (resourcesPath() / openstudio::toPath("weatherdata") / openstudio::toPath("USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw"));

  if (filePath.extension() == openstudio::toPath(".rb"))
  {
    openstudio::runmanager::RubyJobBuilder rubyJobBuilder;

    rubyJobBuilder.setScriptFile(filePath);
    rubyJobBuilder.addToolArgument("-I" + rubyOpenStudioDir()) ;
    rubyJobBuilder.addToolArgument("-I" + openstudio::toString(sourcePath()) + "/model/simulationtests/") ;
    rubyJobBuilder.copyRequiredFiles("rb", "osm", "in.epw");
    rubyJobBuilder.addToWorkflow(wf);
  }

  wf.addWorkflow(openstudio::runmanager::Workflow("ModelToIdf->EnergyPlus"));

  wf.add(tools);
  openstudio::runmanager::Job j = wf.create(outdir, filePath, epw);

  kit.enqueue(j, false);

  kit.setPaused(false);

  kit.waitForFinished();

  return openstudio::SqlFile(j.treeAllFiles().getLastByFilename("eplusout.sql").fullPath);
}



TEST_F(ModelSimulationFixture, baseline_sys01_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys01.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys02_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys02.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys03_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys03.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys04_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys04.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys05_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys05.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys06_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys06.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys07_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys07.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys08_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys08.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys09_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys09.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys10_rb) {
  openstudio::SqlFile sql = runSimulation("baseline_sys10.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, dist_ht_cl_rb) {
  //testing to make sure that district heating and district cooling
  //get forward translated correctly; in particular that they get assigned
  //to a plant operation scheme
  openstudio::SqlFile sql = runSimulation("dist_ht_cl.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, dsn_oa_w_ideal_loads_rb) {
  //testing to make sure that design specification outdoor air objects set
  //at the space type are forward translated directly into use by the
  //zone hvac ideal loads objects
  openstudio::SqlFile sql = runSimulation("dsn_oa_w_ideal_loads.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  //for all zones avg mech ventilation should be > 0
  std::string query = "SELECT value FROM tabulardatawithstrings WHERE \
                      ReportName='OutdoorAirSummary' AND \
                      ReportForString='Entire Facility' AND \
                      TableName='Average Outdoor Air During Occupied Hours' AND \
                      ColumnName = 'Mechanical Ventilation' AND \
                      Units = 'ach'";
  boost::optional<std::vector<double> >  avgMechVents = sql.execAndReturnVectorOfDouble(query);
  ASSERT_TRUE(avgMechVents);
  BOOST_FOREACH(const double& avgMechVent,*avgMechVents) {
    EXPECT_GT(avgMechVent, 0); 
  }
  
}

TEST_F(ModelSimulationFixture, utility_bill01_rb) {
  openstudio::SqlFile sql = runSimulation("utility_bill01.rb");

  openstudio::path osm_path = sql.path().parent_path().parent_path().parent_path() / openstudio::toPath("out.osm");
  boost::optional<openstudio::model::Model> model = openstudio::model::Model::load(osm_path);
  ASSERT_TRUE(model);

  model->setSqlFile(sql);

  std::vector<openstudio::model::UtilityBill> utilityBills = model->getModelObjects<openstudio::model::UtilityBill>();
  EXPECT_EQ(2u, utilityBills.size());
  BOOST_FOREACH(const openstudio::model::UtilityBill& utilityBill, utilityBills){
    std::vector<openstudio::model::BillingPeriod> billingPeriods = utilityBill.billingPeriods();
    EXPECT_EQ(6, billingPeriods.size());
    BOOST_FOREACH(const openstudio::model::BillingPeriod& billingPeriod, billingPeriods){
      boost::optional<double> modelConsumption = billingPeriod.modelConsumption();
      ASSERT_TRUE(modelConsumption);

      boost::optional<double> modelPeakDemand = billingPeriod.modelPeakDemand();
      if (utilityBill.fuelType() == openstudio::FuelType::Electricity){
        ASSERT_TRUE(modelPeakDemand);
        EXPECT_LT(0, modelPeakDemand.get());
      }else{
        EXPECT_FALSE(modelPeakDemand);
      } 
    }

    EXPECT_EQ(6, utilityBill.numberBillingPeriodsInCalculations());
    boost::optional<double> nmbe = utilityBill.NMBE();
    ASSERT_TRUE(nmbe);
    boost::optional<double> cvrmse = utilityBill.CVRMSE();
    ASSERT_TRUE(cvrmse);
    EXPECT_LT(0, cvrmse.get());
  }

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, utility_bill02_rb) {
  openstudio::SqlFile sql = runSimulation("utility_bill02.rb");

  openstudio::path osm_path = sql.path().parent_path().parent_path().parent_path() / openstudio::toPath("out.osm");
  boost::optional<openstudio::model::Model> model = openstudio::model::Model::load(osm_path);
  ASSERT_TRUE(model);

  model->setSqlFile(sql);

  std::vector<openstudio::model::UtilityBill> utilityBills = model->getModelObjects<openstudio::model::UtilityBill>();
  EXPECT_EQ(2u, utilityBills.size());
  BOOST_FOREACH(const openstudio::model::UtilityBill& utilityBill, utilityBills){
    std::vector<openstudio::model::BillingPeriod> billingPeriods = utilityBill.billingPeriods();
    EXPECT_EQ(6, billingPeriods.size());
    BOOST_FOREACH(const openstudio::model::BillingPeriod& billingPeriod, billingPeriods){
      boost::optional<double> modelConsumption = billingPeriod.modelConsumption();
      ASSERT_TRUE(modelConsumption);

      boost::optional<double> modelPeakDemand = billingPeriod.modelPeakDemand();
      if (utilityBill.fuelType() == openstudio::FuelType::Electricity){
        ASSERT_TRUE(modelPeakDemand);
        EXPECT_LT(0, modelPeakDemand.get());
      }else{
        EXPECT_FALSE(modelPeakDemand);
      } 
    }

    EXPECT_EQ(6, utilityBill.numberBillingPeriodsInCalculations());
    boost::optional<double> nmbe = utilityBill.NMBE();
    ASSERT_TRUE(nmbe);
    boost::optional<double> cvrmse = utilityBill.CVRMSE();
    ASSERT_TRUE(cvrmse);
    EXPECT_LT(0, cvrmse.get());
  }

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}




TEST_F(ModelSimulationFixture, baseline_sys01_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys01.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys02_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys02.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys03_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys03.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys04_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys04.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys05_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys05.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys06_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys06.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys07_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys07.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys08_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys08.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys09_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys09.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseline_sys10_osm) {
  openstudio::SqlFile sql = runSimulation("baseline_sys10.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, dist_ht_cl_osm) {
  //testing to make sure that district heating and district cooling
  //get forward translated correctly; in particular that they get assigned
  //to a plant operation scheme
  openstudio::SqlFile sql = runSimulation("dist_ht_cl.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, dsn_oa_w_ideal_loads_osm) {
  //testing to make sure that design specification outdoor air objects set
  //at the space type are forward translated directly into use by the
  //zone hvac ideal loads objects
  openstudio::SqlFile sql = runSimulation("dsn_oa_w_ideal_loads.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  //for all zones avg mech ventilation should be > 0
  std::string query = "SELECT value FROM tabulardatawithstrings WHERE \
                      ReportName='OutdoorAirSummary' AND \
                      ReportForString='Entire Facility' AND \
                      TableName='Average Outdoor Air During Occupied Hours' AND \
                      ColumnName = 'Mechanical Ventilation' AND \
                      Units = 'ach'";
  boost::optional<std::vector<double> >  avgMechVents = sql.execAndReturnVectorOfDouble(query);
  ASSERT_TRUE(avgMechVents);
  BOOST_FOREACH(const double& avgMechVent,*avgMechVents) {
    EXPECT_GT(avgMechVent, 0); 
  }
  
}  

TEST_F(ModelSimulationFixture, baseboard_heater_rb) {
  openstudio::SqlFile sql = runSimulation("baseboard_heater.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, baseboard_heater_osm) {
  openstudio::SqlFile sql = runSimulation("baseboard_heater.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, airterminal_cooledbeam_rb) {
  openstudio::SqlFile sql = runSimulation("airterminal_cooledbeam.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, airterminal_cooledbeam_osm) {
  openstudio::SqlFile sql = runSimulation("airterminal_cooledbeam.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, unit_heater_rb) {
  openstudio::SqlFile sql = runSimulation("unit_heater.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, unit_heater_osm) {
  openstudio::SqlFile sql = runSimulation("unit_heater.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, lowtemprad_constflow_rb) {
  openstudio::SqlFile sql = runSimulation("lowtemprad_constflow.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
	
	boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, lowtemprad_constflow_osm) {
  openstudio::SqlFile sql = runSimulation("lowtemprad_constflow.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
	
  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, lowtemprad_varflow_rb) {
  openstudio::SqlFile sql = runSimulation("lowtemprad_varflow.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
	
  boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, lowtemprad_varflow_osm) {
  openstudio::SqlFile sql = runSimulation("lowtemprad_varflow.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
	
	boost::optional<double> hoursCoolingSetpointNotMet = sql.hoursCoolingSetpointNotMet();
  ASSERT_TRUE(hoursCoolingSetpointNotMet);
  EXPECT_LT(*hoursCoolingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, lowtemprad_electric_rb) {
  openstudio::SqlFile sql = runSimulation("lowtemprad_electric.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, lowtemprad_electric_osm) {
  openstudio::SqlFile sql = runSimulation("lowtemprad_electric.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, watertoair_HP_rb) {
  openstudio::SqlFile sql = runSimulation("watertoair_HP.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, watertoair_HP_osm) {
  openstudio::SqlFile sql = runSimulation("watertoair_HP.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, lifecyclecostparameters_rb) {
  openstudio::SqlFile sql = runSimulation("lifecyclecostparameters.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  std::string constructionCashFlowQuery = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Capital Cash Flow by Category (Without Escalation)' AND  ColumnName='Construction'";
  boost::optional<std::vector<double> > constructionCashFlow = sql.execAndReturnVectorOfDouble(constructionCashFlowQuery);
  ASSERT_TRUE(constructionCashFlow);
  ASSERT_EQ(25u, constructionCashFlow->size());
  for (int i = 0; i < 25; ++i){
    if (i == 0){
      EXPECT_DOUBLE_EQ(10460.0, constructionCashFlow.get()[0]);
    }else{
      EXPECT_DOUBLE_EQ(0.0, constructionCashFlow.get()[i]);
    }
  }

  std::string maintenanceCashFlowQuery = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Operating Cash Flow by Category (Without Escalation)' AND  ColumnName='Maintenance'";
  boost::optional<std::vector<double> > maintenanceCashFlow = sql.execAndReturnVectorOfDouble(maintenanceCashFlowQuery);
  ASSERT_TRUE(maintenanceCashFlow);
  ASSERT_EQ(25u, maintenanceCashFlow->size());
  for (int i = 0; i < 25; ++i){
    if (i == 0){
      EXPECT_DOUBLE_EQ(0.0, maintenanceCashFlow.get()[0]);
    }else{
      EXPECT_DOUBLE_EQ(1046.0, maintenanceCashFlow.get()[i]);
    }
  }

  std::string replacementCashFlowQuery = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Operating Cash Flow by Category (Without Escalation)' AND  ColumnName='Replacement'";
  boost::optional<std::vector<double> > replacementCashFlow = sql.execAndReturnVectorOfDouble(replacementCashFlowQuery);
  ASSERT_TRUE(replacementCashFlow);
  ASSERT_EQ(25u, replacementCashFlow->size());
  for (int i = 0; i < 25; ++i){
    if (i == 10 || i == 20){
      EXPECT_DOUBLE_EQ(1046.0 + 10460.0, replacementCashFlow.get()[i]);
    }else{
      EXPECT_DOUBLE_EQ(0.0, replacementCashFlow.get()[0]);
    }
  }
}

TEST_F(ModelSimulationFixture, lifecyclecostparameters_osm) {
  openstudio::SqlFile sql = runSimulation("lifecyclecostparameters.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);

  std::string constructionCashFlowQuery = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Capital Cash Flow by Category (Without Escalation)' AND  ColumnName='Construction'";
  boost::optional<std::vector<double> > constructionCashFlow = sql.execAndReturnVectorOfDouble(constructionCashFlowQuery);
  ASSERT_TRUE(constructionCashFlow);
  ASSERT_EQ(25u, constructionCashFlow->size());
  for (int i = 0; i < 25; ++i){
    if (i == 0){
      EXPECT_DOUBLE_EQ(10460.0, constructionCashFlow.get()[0]);
    }else{
      EXPECT_DOUBLE_EQ(0.0, constructionCashFlow.get()[i]);
    }
  }

  std::string maintenanceCashFlowQuery = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Operating Cash Flow by Category (Without Escalation)' AND  ColumnName='Maintenance'";
  boost::optional<std::vector<double> > maintenanceCashFlow = sql.execAndReturnVectorOfDouble(maintenanceCashFlowQuery);
  ASSERT_TRUE(maintenanceCashFlow);
  ASSERT_EQ(25u, maintenanceCashFlow->size());
  for (int i = 0; i < 25; ++i){
    if (i == 0){
      EXPECT_DOUBLE_EQ(0.0, maintenanceCashFlow.get()[0]);
    }else{
      EXPECT_DOUBLE_EQ(1046.0, maintenanceCashFlow.get()[i]);
    }
  }

  std::string replacementCashFlowQuery = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Operating Cash Flow by Category (Without Escalation)' AND  ColumnName='Replacement'";
  boost::optional<std::vector<double> > replacementCashFlow = sql.execAndReturnVectorOfDouble(replacementCashFlowQuery);
  ASSERT_TRUE(replacementCashFlow);
  ASSERT_EQ(25u, replacementCashFlow->size());
  for (int i = 0; i < 25; ++i){
    if (i == 10 || i == 20){
      EXPECT_DOUBLE_EQ(1046.0 + 10460.0, replacementCashFlow.get()[i]);
    }else{
      EXPECT_DOUBLE_EQ(0.0, replacementCashFlow.get()[0]);
    }
  }
}

TEST_F(ModelSimulationFixture, heatexchanger_airtoair_sensibleandlatent_rb) {
  openstudio::SqlFile sql = runSimulation("heatexchanger_airtoair_sensibleandlatent.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, heatexchanger_airtoair_sensibleandlatent_osm) {
  openstudio::SqlFile sql = runSimulation("heatexchanger_airtoair_sensibleandlatent.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);

  boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  ASSERT_TRUE(hoursHeatingSetpointNotMet);
  EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, air_terminals_rb) {
  openstudio::SqlFile sql = runSimulation("air_terminals.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
  
  // This one is not going to hit heating setpoint because it has VAV air terminals without reheat
  // Possibly update control strategy
  //boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  //ASSERT_TRUE(hoursHeatingSetpointNotMet);
  //EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, air_terminals_osm) {
  openstudio::SqlFile sql = runSimulation("air_terminals.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
  
  // This one is not going to hit heating setpoint because it has VAV air terminals without reheat
  // Possibly update control strategy
  //boost::optional<double> hoursHeatingSetpointNotMet = sql.hoursHeatingSetpointNotMet();
  //ASSERT_TRUE(hoursHeatingSetpointNotMet);
  //EXPECT_LT(*hoursHeatingSetpointNotMet, 350);
}

TEST_F(ModelSimulationFixture, scheduled_infiltration_osm) {
  openstudio::SqlFile sql = runSimulation("scheduled_infiltration.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
  
}

TEST_F(ModelSimulationFixture,vrf_rb) {
  openstudio::SqlFile sql = runSimulation("vrf.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
}

TEST_F(ModelSimulationFixture, vrf_osm) {
  openstudio::SqlFile sql = runSimulation("vrf.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
}

TEST_F(ModelSimulationFixture,coolingtowers_rb) {
  openstudio::SqlFile sql = runSimulation("coolingtowers.rb");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
}

TEST_F(ModelSimulationFixture, coolingtowers_osm) {
  openstudio::SqlFile sql = runSimulation("coolingtowers.osm");

  boost::optional<double> totalSiteEnergy = sql.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_LT(*totalSiteEnergy, 1000000);
}