
-- set pin
print("configure gpio...")
gpio.mode(3, gpio.OUTPUT)
gpio.mode(4, gpio.OUTPUT)
tmr.delay(100)
gpio.write(3, gpio.HIGH) 
gpio.write(4, gpio.HIGH) 
tmr.delay(2000)
gpio.write(3, gpio.LOW) 
gpio.write(4, gpio.LOW)

--set wifi
print("configure wifi...")
cfg={}
cfg.ssid="robokot";
cfg.pwd="12345678";
wifi.ap.config(cfg);

cfg={}
cfg.ip="192.168.1.1";
cfg.netmask="255.255.255.0";
cfg.gateway="192.168.1.1";
wifi.ap.setip(cfg);
wifi.setmode(wifi.SOFTAP);

cmd="none";

print("configure udp server...")
srv=net.createServer(net.UDP)
srv:on("receive", function(srv, pl)
   --print(pl)
   cmd=pl;
   end)
srv:listen(4444)



print("done.")
