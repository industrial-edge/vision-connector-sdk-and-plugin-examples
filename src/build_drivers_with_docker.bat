@ECHO OFF

set IMAGENAME="driver_build_img:0.0.1"

call docker build . -t %IMAGENAME% --target local_build_img
call docker run --rm -v %cd%\installed_drivers:/vca-drivers/installed_drivers %IMAGENAME% ./build_drivers.sh
pause