sudo bash
apt-get install build-essentials kernel-package
apt-get install linux-source
cd /usr/src
tar jxvf linux-source-4.4.0.tar.bz2
ln -s linux-source-4.4.0 linux
exit
echo 'READY'
