%t0=632 Minimum step period

N=10;
Tins=0.5;
dt=Tins/N;
t=[0:dt:Tins];
dur=[dt/2, diff(t(1:end-1)),dt/2]; % [dt/2, dt, dt, ..., dt, dt/2]

% velocity is derivative of position:
%      x=pi*(1+cos(pi*t/Tin)
% Amplitude of movement: 0..2*pi
v=pi^2*sin(pi*t/Tins)/Tins;

% time is the inverse of speed:
%      tstep=Tins./(pi^2*sin(pi*t/Tins)); (analytic)
tstep=1./v;

% normalize to get a fixed minimum value
tstep=t0*1e-6*tstep/min(tstep);

% big steps should be truncated
sel=(tstep>1.1*dt);
tstep(sel)=1.1*dt;

% count how many steps are being done
nSteps=[];
for ix=[1:length(tstep)]
  n=ceil(dt/tstep(ix))
  nSteps=[nSteps,n];
endfor

% compute output for Arduino code
out=round([dur'/1e-3,tstep'/1e-6]);

% format as nice string
s='';
for ix=[1:length(out)]
  s=[s, '{ ',num2str(out(ix,1)),' , ', num2str(out(ix,2))," }, // steps : ", num2str(nSteps(ix)), "\n"];
endfor

s=[s, "// Total steps :", num2str(sum(nSteps))]

% save to text file
save s.txt s


ct=0;
n=0;
ix=0;
%while t<max(t)
%  while t<dur(ix)
%    ct=
%  endwhile
%  ix=ix+1;

%endwhile



