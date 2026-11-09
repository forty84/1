add_test([=[TapeTest.ExpandLeftAndRight]=]  D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe [==[--gtest_filter=TapeTest.ExpandLeftAndRight]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TapeTest.ExpandLeftAndRight]=]
  PROPERTIES
    
    _CMAKE_TEST_BUILD_DEPENDS [==[D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe]==]
    DEF_SOURCE_LINE [==[D:\code\PPOIS\1\TuringMachine\test.cpp:6]==]
    WORKING_DIRECTORY [==[D:/code/PPOIS/1/build/TuringMachine]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[TapeTest.DeepCopy]=]  D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe [==[--gtest_filter=TapeTest.DeepCopy]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TapeTest.DeepCopy]=]
  PROPERTIES
    
    _CMAKE_TEST_BUILD_DEPENDS [==[D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe]==]
    DEF_SOURCE_LINE [==[D:\code\PPOIS\1\TuringMachine\test.cpp:19]==]
    WORKING_DIRECTORY [==[D:/code/PPOIS/1/build/TuringMachine]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[MachineTest.AddAndRemoveRules]=]  D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe [==[--gtest_filter=MachineTest.AddAndRemoveRules]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[MachineTest.AddAndRemoveRules]=]
  PROPERTIES
    
    _CMAKE_TEST_BUILD_DEPENDS [==[D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe]==]
    DEF_SOURCE_LINE [==[D:\code\PPOIS\1\TuringMachine\test.cpp:32]==]
    WORKING_DIRECTORY [==[D:/code/PPOIS/1/build/TuringMachine]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[MachineTest.StreamLoadAndBitInversion]=]  D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe [==[--gtest_filter=MachineTest.StreamLoadAndBitInversion]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[MachineTest.StreamLoadAndBitInversion]=]
  PROPERTIES
    
    _CMAKE_TEST_BUILD_DEPENDS [==[D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe]==]
    DEF_SOURCE_LINE [==[D:\code\PPOIS\1\TuringMachine\test.cpp:50]==]
    WORKING_DIRECTORY [==[D:/code/PPOIS/1/build/TuringMachine]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[MachineTest.InfiniteLoopProtection]=]  D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe [==[--gtest_filter=MachineTest.InfiniteLoopProtection]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[MachineTest.InfiniteLoopProtection]=]
  PROPERTIES
    
    _CMAKE_TEST_BUILD_DEPENDS [==[D:/code/PPOIS/1/build/TuringMachine/TuringTests.exe]==]
    DEF_SOURCE_LINE [==[D:\code\PPOIS\1\TuringMachine\test.cpp:71]==]
    WORKING_DIRECTORY [==[D:/code/PPOIS/1/build/TuringMachine]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
set(TuringTests_TESTS [==[TapeTest.ExpandLeftAndRight]==] [==[TapeTest.DeepCopy]==] [==[MachineTest.AddAndRemoveRules]==] [==[MachineTest.StreamLoadAndBitInversion]==] [==[MachineTest.InfiniteLoopProtection]==])
