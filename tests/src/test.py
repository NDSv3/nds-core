
#!/usr/bin/env python3
import sys
import os


# The test allows the user to run all available nds-core tests as many times
# as desired. If any test fails, it will be recorded in
# a new file called ErrorFile available in the same folder where this script is located.



#Number of times that every test is going to be executed in a row:
val1="200"
val2="2000"

# Unit test are stored into a list 
tests = [
    
# These test take a long time to run, so it is better to use a lower value.  
        ["testDataAcquisition.testPushDataAcquiredVFloat", val1],
        ["testDataAcquisition.testPushDataAcquiredVFloatInit", val1],
        ["testDataAcquisition.testPushDataAcquiredVDBL", val1],
        ["testDataAcquisition.testPushDataAcquiredVDBLInit", val1],
        ["testDataAcquisition.testPushDataAcquiredVI32", val1],
        ["testDataAcquisition.testPushDataAcquiredVI32Init", val1],
        ["testDataAcquisition.testPushDataAcquiredVI64", val1],
        ["testDataAcquisition.testPushDataAcquiredVI64Init", val1],
        ["testDataAcquisition.testPushDataAcquiredVI8", val1],
        ["testDataAcquisition.testPushDataAcquiredVI8Init", val1],
        ["testDataAcquisition.testPushDataAcquiredVUI8Init", val1],
        ["testDataAcquisition.testPushDataAcquiredVUI8", val1],
        ["testDataAcquisition.testPushDataAcquiredFloatInit", val1],
        ["testDataAcquisition.testPushDataAcquiredFloat", val1],
        ["testDataAcquisition.testPushDataAcquiredDBLInit", val1],
        ["testDataAcquisition.testPushDataAcquiredDBL", val1],
        ["testDataAcquisition.testPushDataAcquiredI32Init", val1],
        ["testDataAcquisition.testPushDataAcquiredI32", val1],
        ["testDataAcquisition.testPushDataAcquiredI64", val1],
        ["testDataAcquisition.testPushDataAcquiredI64Init", val1],
        ["testDataAcquisition.testDecimationInit", val1],
        ["testDataAcquisition.testDecimation", val1],
        ["testDataAcquisition.testDMAParametersInit", val1],
        ["testDataAcquisition.testDMAParameters", val1],
        ["testDeviceTimestamping.SetGetInitializationTest", val1],
           
#These tests are faster. 
        ["testDataAcquisition.testStateMachine", val2], 
        ["testDeviceAllocation.testAllocationMissingDevice", val2],
        ["testDeviceAllocation.testDoubleAllocation", val2],
        ["testDeviceAllocation.testInitDeinit", val2],
        ["testDeviceAllocation.testTwoAllocations", val2],
        ["testDeviceDataMultiplexing.Self", val2],
        ["testDeviceFirmware.StateMachineTest", val2],
        ["testDeviceFirmware.PVsTest", val2],
        ["testDeviceFirmware.PVsTestInitialization", val2],
        ["testDeviceHQMonitor.stateMachine",val2],
        ["testDeviceHQMonitor.PVs",val2],
        ["testDeviceHQMonitor.PVsInitialization",val2],
        ["testDevicePVs.PVTypes", val2],
        ["testDevicePVs.PVTypesInit", val2],
        ["testDevicePVs.DataSharing", val2],
        ["testDevicePVs.PVUnsubscribe", val2],
        ["testDeviceStateMachine.stateMachine", val2],
        ["testDeviceStateMachine.stateMachineStructure",   val2],
        ["testDeviceTimestamping.StateMachineTest", val2],
        ["testDeviceTimestamping.SetGetTest", val2],
        ["testDeviceTiming.fullTest", val2],
        ["testDeviceTiming.fullTestStructure", val2],
        ["testTrigAndClk.testSWTrig", val2],
        ["testTrigAndClk.testSWTrigInit", val2],
        ["testTrigAndClk.testConfigTrig", val2],
        ["testTrigAndClk.testConfigTrigInit", val2],
        ["testTrigAndClk.testPLLSync", val2],
        ["testTrigAndClk.testPLLSyncInit", val2],
        ["testTrigAndClk.testEnablePLL", val2],
        ["testTrigAndClk.testEnablePLLInit", val2],
        ["testTrigAndClk.testResetConfigTrigger", val2],
        ["testTrigAndClk.testResetConfigTriggerInit", val2],
        ["testTrigAndClk.testStateMachine_TrigAndClk", val2],
        ["testTrigAndClk.testStateMachine_TrigAndClk_RoutingNode", val2],
        ["testTrigAndClk.testClockSet", val2],
        ["testTrigAndClk.testTermSet", val2],
        ["testDigitalIO.testStateMachineBool", val2],
        ["testDigitalIO.testPushDataBool", val2],
        ["testDigitalIO.testPushDataBoolInit", val2],
        ["testDigitalIO.testStateMachineI8", val2],
        ["testDigitalIO.testPushDataI8", val2],
        ["testDigitalIO.testPushDataI8Init", val2],
        ["testDigitalIO.testStateMachineI16", val2],
        ["testDigitalIO.testPushDataI16", val2],
        ["testDigitalIO.testPushDataI16Init", val2],
        ["testDigitalIO.testStateMachineI32", val2],
        ["testDigitalIO.testPushDataI32", val2],
        ["testDigitalIO.testPushDataI32Init", val2],
        ["testDigitalIO.testStateMachineI64", val2],
        ["testDigitalIO.testPushDataI64", val2],
        ["testDigitalIO.testPushDataI64Init", val2],
        ["testFTE.testStateMachineFTE", val2],
        ["testFTE.testSetPVManaging", val2],
        ["testFTE.testSetPVManagingInit", val2],
        ["testFTE.testSuppressPVManaging", val2],
        ["testFTE.testSuppressPVManagingInit", val2],
        ["testFTE.testChgPeriodPVManaging", val2],
        ["testFTE.testChgPeriodPVManagingInit", val2],
        ["testFTE.testPendingAndMaximumPVManaging", val2],
        ["testFTE.testPendingAndMaximumPVManagingInit", val2],
        ["testIniParser.parseFile", val2],
        ["testIniParser.testEmptySectionAndComments", val2],
        ["testNamingRules.testDefaultRules", val2],
        ["testNamingRules.testFallbackRules", val2],
        ["testPVs.testVariable", val2],
        ["testPVs.testDelegate", val2],
        ["testPVs.testDelegateInitialized", val2],
        ["testPVs.testSubscription0", val2],
        ["testPVs.testSubscription1", val2],
        ["testPVs.testReplication", val2],
        ["testRouting.testStateMachineRouting", val2],
        ["testRouting.testClockSet", val2],
        ["testRouting.testClockSetInit", val2],
        ["testRouting.testTermSet", val2],
        ["testRouting.testTermSetInit", val2],
        ["testStateMachine.testLocalGlobalState", val2],
        ["testStateMachine.testNodeStates", val2],
        ["testStateMachine.testChildrenStates", val2],
        ["testStateMachineAutoEnable.testSuccesfulTransitionState", val2],
        ["testStateMachineAutoEnable.testErrorTransitionState", val2],
        ["testStateMachineAutoEnable.testAsynTransitionState", val2],
        ["testThreads.testThreads", val2],
        ["testWFG.testStateMachine", val2],
        ["testWFG.testPushDataGeneratedVDBL", val2],
        ["testWFG.testPushDataGeneratedVDBLInit", val2],
        ["testWFG.testPushDataGeneratedVI8", val2],
        ["testWFG.testPushDataGeneratedVI8Init", val2],
        ["testWFG.testPushDataGeneratedVUI8", val2],
        ["testWFG.testPushDataGeneratedVUI8Init", val2],
        ["testWFG.testPushDataGeneratedVI32", val2],
        ["testWFG.testPushDataGeneratedVI32Init", val2],
        ["testWFG.testPushDataGeneratedVI64", val2],
        ["testWFG.testPushDataGeneratedVI64Init", val2],
        ["testWFG.testPushDataGeneratedDBL", val2],
        ["testWFG.testPushDataGeneratedDBLInit", val2],
        ["testWFG.testPushDataGeneratedI32", val2],
        ["testWFG.testPushDataGeneratedI32Init", val2],
        ["testWFG.testPushDataGeneratedI64", val2],
        ["testWFG.testPushDataGeneratedI64Init", val2],
        ["testWFG.testdecimation", val2],
        ["testWFG.testdecimationInit", val2],  
        ["testDeviceError.statusError", val2],

# This test can only be passed once in a row.
        ["testLogging.testLotOfPVs", "1"]

         
         ]
f = open('ErrorFile', 'w')
    
for test in tests:
    if os.system('./nds3core-unit-tests --gtest_filter='+test[0]+' --gtest_repeat='+test[1]+' --gtest_break_on_failure') != 0:
        f.write(test[0]+'\n')
f.close()

