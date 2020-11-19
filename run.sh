#! /bin/bash
##author:guoxun
##run my task
echo 'shell is running'
cd build/
#rm -rf *
#cmake ..
make 
cd ..
cd bin/
./test_config

