cmd_/home/pi/Sicomp-projects/tp3/gpio_rbp/Module.symvers := sed 's/ko$$/o/' /home/pi/Sicomp-projects/tp3/gpio_rbp/modules.order | scripts/mod/modpost -m -a   -o /home/pi/Sicomp-projects/tp3/gpio_rbp/Module.symvers -e -i Module.symvers   -T -
