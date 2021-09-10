#!/bin/bash
CSC_FILE="$1"
SEARCH_STRING="Simulation main loop started"

make TARGET=cooja "$CSC_FILE" NO_GUI=1 |& tee >(grep -m 1 "$SEARCH_STRING" > /dev/null; echo "Finding process"; JAVA_SIM_PROC_ID=$(pgrep -f "java.*$CSC_FILE"); echo "Found PROC $JAVA_SIM_PROC_ID, running gdbserver"; gdbserver --attach :8000 "$JAVA_SIM_PROC_ID")
