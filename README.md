# car_dasboard


This is a 7 years old project just ran into. Work in progress...

   * If you run it start the start-xephir.sh and run the app on display 1, otherwise would iterfere with curent desktop.
   
   
   carutza -display ":1"
   
   
   #R-PI install
   
   * Have a non UI image installed. Like: Raspbian GNU/Linux 8 \n \l

   ## carutza (qt4)
   
```   
   sudo apt-get update && sudo apt-get install libtool libx11-dev xinput autoconf libx11-dev libxi-dev x11proto-input-dev xorg -y
   sudo apt-get install x11-xserver-utils xinit
   sudo apt-get install -y qt4-dev-tools
   sudo apt-get install rtl-sdr
   
##########################################   
   sudo nano /etc/X11/Xwrapper.config
   
#   dpkg-reconfigure xserver-xorg-legacy
allowed_users=anybody # make this change

##########################################
   
   cd carutza
   make clean
   rm moc_*
   qmake .
   make
xset s off
xset -dpms  
xset s noblank

   cp ./caruthza ../bin
   # for arm R-PI etc...
   cd ../bin
   ./start-arm.sh  
```

On a PC where already there is a desktop runnning

```
   cd car_dasboard
   ./start<tab> CR   start xephir
   ./carutza -display :1

```

#### For RTL radio  (qt5)

```
sudo apt-get  install gnuradio
```

#### Navigaton

 * using navit
 
NEO-6M GPS WITH R-PI: follow: http://comarius.blogspot.com/2020/09/neo-6m-gps-with-raspberry-pi.html



```

sudo apt-get install gpsd
stty -F /dev/serial1 9600
 
sudo nano /etc/default/gpsd
# add this content
START_DAEMON="true"
USBAUTO="true"
DEVICES="/dev/serial0"
GPSD_OPTIONS=""
GPSD_SOCKET="/var/run/gpsd.sock"
 
 ```


### Configure desktops and panels

todo

#### Bluetooth


  * WIP

#### Camera's

  * ESP32 cam & ethernet cam

#### For Media player

  * mplayer


#### For OBD2

   * pyobd

#### For temperature / Humidity

   * builtin

### For acceleration, orientation compass

  * builtin



 [](https://raw.githubusercontent.com/circinusX1/car_dasboard/master/docs/car_d.png?token=AQTKLGMS4SGXTPBL2WNGAJK7NUNSI)



