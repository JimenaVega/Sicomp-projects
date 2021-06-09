cmd_/home/pi/Sicomp-projects/tp3/test/modules.order := {   echo /home/pi/Sicomp-projects/tp3/test/test_read.ko; :; } | awk '!x[$$0]++' - > /home/pi/Sicomp-projects/tp3/test/modules.order
