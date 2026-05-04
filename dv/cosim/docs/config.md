## CoSim Configuration

CoSim can be configured in three different ways:

- environment variables
- cosim.toml (preferred)
- arg_desc.txt
- plusargs (only available on VCS)

These options are evaluated in order, i.e. when a `arg_desc.txt` file is
present, plusargs will be ignored.
Note that at this time there is no way to specify a path to these configuration
files, so they have to be in the directory where the executable is called.

For available options, see [cosim.toml](cosim.toml).
These options map directly to the member variables of a `cosim_config` object (see [cosim_config.h](../src/cosim_config.h]).
This is the preferred method, though the other options are still maintained.

Refer to [arg_desc.txt](arg_desc.txt) for an alternative way to specify runtime arguments.
Plusargs map 1-to-1 to these options, e.g. `CHECKER: 0` becomes `+CHECKER=0`.
Environment variables are equivalent to plusargs except prefixed with `COSIM_`, e.g.: `+CHECKER=0` becomes `COSIM_CHECKER=0`.
Options that already start with `COSIM_` are not changed, e.g.: `+COSIM_LOG_FILE=cosim.log` stays `COSIM_LOG_FILE=cosim.log`.
