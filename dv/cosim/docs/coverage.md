## Cosim Coverage

### Run command

    run-replay -cov cosim_cov.txt cosim_evt.trace
    run-replay -cov cosim_cov.txt -info cosim_evt.trace > commits.log

### File formats

For `cosim_cov.txt`:

    group <NAME>
    commits <NUM_COMMITS>
    exec_time <MEAN> <STDEV> <MIN> <MAX> <SUM>
    queue_size <MEAN> <STDEV> <MIN> <MAX> <SUM>
    cross <NAME> <COUNT>
    [...]


For `commits.log`:

    RETIRE  COMMIT  HART                ID                      GROUP       INSTRUCTION
    11468   11468   [H0:S0:N0:C0:T0]    8100000000-0000000001   [rv_misc]   auipc x1,0x1
    11469   11469   [H0:S0:N0:C0:T0]    8100000004-0000000002   [rv_misc]   addi x1,x1,128
    [...]

### Coverage Groups

The groups are defined in `src/coverage/def`. Files are as follows:

 - `group.h`  List of all groups
 - `insns.h`  List of group/instruction pairings
 - `csrs.h`   List of group/CSR pairings

In order to change the way instructions/csrs are mapped to the respective
groups you'll need to edit to respective header files.

In order to add a new group, you'll need to add the group to the `group.h`
header file. Make sure to also update the mappings, and other constants, e.g.:
`num_groups` in [src/coverage/group.h](../src/coverage/group.h).

### Support script

In [tools/coverage](../tools/coverage) you can find the following support scripts that
are used to post-process the coverage output:

 - `tabular`   Convert txt to tsv file
 - `merge`     Merge tsv files into a single dataset
 - `concat`    Concatenate tsv files
