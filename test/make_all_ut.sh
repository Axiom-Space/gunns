#!/bin/tcsh -f
####################### BASH HEADER ######################
# PROGRAMMERS:
#    (
#       (Jason Harvey)  (L-3 Communications Corp) (File Creation)                                (A time long ago...)
#       (Syam Evani)    (Axiom Space)             (Converting to automatically find all folders) (08-30-2023)
#    )
##########################################################

# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.

set OUT = `pwd`/make_all_ut.out
echo ' ' > $OUT
echo '**** make_all_ut summary ****' >> $OUT

# clean & re-build the test compiled lib
cd $GUNNS_HOME/lib/test; make clean; make;
cd $GUNNS_HOME

# Array of partial paths of excluded subfolders
set excluded_subfolder_partials = ("ThermAspectGenerate/test" "radiation/test" "radiation/network/test" "hi-fi/test" "networks/electrical/test" "networks/fluid/test" "networks/basic/test" "cuda/test" "intersect/test" "hs/test" "software/unit_test/test" "software/exceptions/test" "lib/test")

# Find all "test" folders below the current directory
set test_folders = `find . -type d -name "test" -exec test -e "{}/Makefile" \; -print`

# Loop through each test folder
foreach test_folder ($test_folders)
    # Check if the current test folder's path contains any excluded subfolder partial
    set is_excluded = 0
    foreach excluded_subfolder_partial ($excluded_subfolder_partials)
        if ("$test_folder:q" =~ "*$excluded_subfolder_partial*") then
            set is_excluded = 1
            break
        endif
    end

    # If not excluded, execute the commands
    if ($is_excluded == 0) then
        echo "Executing commands in: $test_folder"
        
        # Set the FOLDER variable to the path of the current test folder
        setenv FOLDER "$test_folder"
        
        # Run
        echo "Running commands in: $FOLDER"
        cd $GUNNS_HOME/$FOLDER; make clean; make;
        if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
            echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
        else
            echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
        endif
    else
        echo "Excluding folder: $test_folder"
    endif
end

########################################################
# Summarize results to screen
########################################################
echo ' ' >> $OUT
tail -n 100 $OUT
cd $GUNNS_HOME


# # test core
# set FOLDER = core/test

# # TODO these lines are the same for each test folder so a loop would be good...
# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# set FOLDER = core/network/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif


# ########################################################
# # test dynamics aspect
# ########################################################
# set FOLDER = aspects/dynamics/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif


# ########################################################
# # test fluid aspect
# ########################################################
# set FOLDER = aspects/fluid/capacitor/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/fluid/conductor/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/fluid/fluid/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# # Commented out until memory leaks are addressed
# # set FOLDER = aspects/fluid/hi-fi/test

# # cd $GUNNS_HOME/$FOLDER; make clean; make;
# # if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
# #     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# # else
# #     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# # endif

# #
# set FOLDER = aspects/fluid/potential/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/fluid/source/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif


# ########################################################
# # test thermal aspect
# ########################################################
# set FOLDER = aspects/thermal/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/thermal/network/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif


# ########################################################
# # test electrical aspect
# ########################################################
# set FOLDER = aspects/electrical/Batt/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/ConstantPowerLoad/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/Converter/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/Diode/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# #set FOLDER = aspects/electrical/FetSwitch/test
# #
# #cd $GUNNS_HOME/$FOLDER; make clean; make;
# #if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
# #    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# #else
# #    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# #endif

# #
# set FOLDER = aspects/electrical/IPS/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/PowerBus/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/resistive/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/sar/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/SolarArray/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/Switch/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/SwitchCard/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = aspects/electrical/UserLoad/test

# cd $GUNNS_HOME/$FOLDER; make clean; make;
# if ( -f $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $GUNNS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif


# ########################################################
# # test relevent folders in ms-utils
# ########################################################
# set FOLDER = math/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = math/approximation/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = math/linear_algebra/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = math/root_finding/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = math/time/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = parsing/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = properties/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# # Can't be unit-tested because it has Trick dependencies
# #set FOLDER = simulation/hs/test
# #
# #cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# #if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
# #    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# #else
# #    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# #endif

# # Can't be unit-tested because it has Trick dependencies
# #set FOLDER = software/exceptions/test
# #
# #cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# #if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
# #    echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# #else
# #    echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# #endif

# #
# set FOLDER = software/SimBus/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = software/SimCompatibility/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = fileSearch/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = strings/test

# cd $MS_UTILS_HOME/$FOLDER; make clean; make;
# if ( -f $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $MS_UTILS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# ########################################################
# # test gunns-ts-models
# ########################################################
# set FOLDER = aspects/signal/effectors/thermostat/test

# cd $TS_MODELS_HOME/$FOLDER; make clean; make;
# if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = common/controllers/fluid/test

# cd $TS_MODELS_HOME/$FOLDER; make clean; make;
# if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = common/controllers/generic/test

# cd $TS_MODELS_HOME/$FOLDER; make clean; make;
# if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = common/controllers/valveAssemblies/test

# cd $TS_MODELS_HOME/$FOLDER; make clean; make;
# if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = common/effectors/mechanical/motor/test

# cd $TS_MODELS_HOME/$FOLDER; make clean; make;
# if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif

# #
# set FOLDER = common/sensors/test

# cd $TS_MODELS_HOME/$FOLDER; make clean; make;
# if ( -f $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log ) then
#     echo $FOLDER\: `grep -E 'OK \(*|FAILURES\!|Failures \!' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log` `grep 'ERROR SUMMARY' $TS_MODELS_HOME/$FOLDER/output/unit-tests-valgrind.log | grep -v ' 0 errors'` >> $OUT
# else
#     echo $FOLDER\: NO TEST OUTPUT, possibly failed to build! >> $OUT
# endif


