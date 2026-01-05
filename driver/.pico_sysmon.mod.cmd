savedcmd_pico_sysmon.mod := printf '%s\n'   pico_sysmon.o | awk '!x[$$0]++ { print("./"$$0) }' > pico_sysmon.mod
