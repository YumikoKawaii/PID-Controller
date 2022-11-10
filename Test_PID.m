Kp = 1;
Ki = 0;
Kd = 0.025;
setPoint = 1;
prevE = 0;
integralE = 0;
u = 0;
timeDiff = 0.1;

counter = 0;
t = [0:0.1:2];
value = [0];

while (counter < 20)
   e = getError(u, setPoint);
   i = getIntegral(e, integralE, timeDiff);
   d = getDerivative(e, prevE, timeDiff);
   u = u + getPV(Kp, e, Ki, i, Kd, d);
   value(end + 1) = u;   
   prevE = e;      
   integralE = i;  
   counter = counter + 1;
end

plot(t,value);

function u = getPV(Kp, e,Ki, i,Kd, d)
    u = Kp*e + Ki*i + Kd*d;    
end

function e = getError(u, SP)
    e = SP - u;
end

function i = getIntegral(e, integralE, timeDiff)    
    i = integralE + e*timeDiff;
end

function d = getDerivative(e, prevE, timeDiff)
    d = (e - prevE)/timeDiff;
end