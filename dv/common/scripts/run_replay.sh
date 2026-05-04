#!/bin/sh

status=0

echo "Running CoSim Replay"
$REPOROOT/dv/cosim/replay/build/run-replay -check -stats -verbose cosim_evt.trace > cosim_replay.log 2>&1
exit $status
