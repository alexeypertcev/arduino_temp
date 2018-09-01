
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

cmd={}
cmd.pos = "0";
cmd.state = "0";

print("configure http server...")
srv=net.createServer(net.TCP) 
srv:listen(80,function(conn) 
    conn:on("receive", function(client,request)
        local buf = "";
        local _, _, method, path, vars = string.find(request, "([A-Z]+) (.+)?(.+) HTTP");
        if(method == nil)then 
            _, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP"); 
        end
        local _GET = {}

        if (vars ~= nil)then 
            --print(vars)
            for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do 
                _GET[k] = v
                if (k == "pos") then
                    cmd.pos = v;
                elseif(k == "state") then
                    cmd.state = v;
                end
                print(k)
                print(v)
            end 
        end
        buf = buf.."<h1> Hello, NodeMcu.</h1>";
        --buf = buf.."<h1> Hello, NodeMcu.</h1><form src=\"/\">Turn GPIO2 <select name=\"pin\" onchange=\"form.submit()\">";
        --local _on,_off = "",""
        --if(_GET.pin == "ON")then
        --      _on = " selected=true";
        --      gpio.write(4, gpio.HIGH);
        --elseif(_GET.pin == "OFF")then
        --      _off = " selected=\"true\"";
        --      gpio.write(4, gpio.LOW);
        --end
        --buf = buf.."<option".._on..">ON</opton><option".._off..">OFF</option></select></form>";
        client:send(buf);
        client:close();
        collectgarbage();
    end)
end)



print("done.")
