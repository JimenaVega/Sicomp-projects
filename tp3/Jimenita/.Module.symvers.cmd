cmd_/home/pi/Sicomp-projects/tp3/Jimenita/Module.symvers := sed 's/ko$$/o/' /home/pi/Sicomp-projects/tp3/Jimenita/modules.order | scripts/mod/modpost -m -a   -o /home/pi/Sicomp-projects/tp3/Jimenita/Module.symvers -e -i Module.symvers   -T -