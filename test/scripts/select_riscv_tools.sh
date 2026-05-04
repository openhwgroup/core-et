
# if FORCE_RISCV_VERSION is 1 do nothing
if [ "$FORCE_RISCV_VERSION" != "1" ]; then
    # select release directory ( different on BCN and MTV servers)
    for i in /tools/aifoundry/riscv/; do
        if [ -d $i ]; then
            export riscv_tools_dir=$i
            break
        fi
    done

    version=`cat $REPOROOT/.riscv_tools_version |head -1`
    export RISCV=$riscv_tools_dir/$version
fi
