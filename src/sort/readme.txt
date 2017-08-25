1. 使用google-pprof，先加入链接库profiler.
2. env CPUPROFILE=./myprogram.prof ./myprogram
3. google-pprof --gc ./myprogram ./myprogram.prof
