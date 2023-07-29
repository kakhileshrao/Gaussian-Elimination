Commands to run software simulation-
1. make
2. make aa2c
3. ./testbench_sw

Commands to run hardware simulation-
1. make
2. tmux
3. ./testbench_hw
4. ./ahir_system_test_bench --stop-time=1100us --wave=run.ghw

To run GTKwave-
Open command prompt in the project folder and type the command-
1. gtkwave run.ghw