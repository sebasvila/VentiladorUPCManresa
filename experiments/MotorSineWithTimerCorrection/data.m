clear all

N=10;
T=10000;
D=T/N
A=80000;
us=1e-6;
ms=1e-3;
t=[0:D:T];
n=A/2*(1-cos(pi*t/T))

dn=diff(n);


disp(['This should be ',num2str(A),' : ',num2str(sum(round(dn)))])

out_dn=round(dn);
% out_dn(round(N/2))=out_dn(round(N/2))-1    % <---if odd
out_dn(round(N/2):round(N/2)+1)=out_dn(round(N/2):round(N/2)+1)-1

s=D./out_dn/us;

out=round([t(2:end)'/ms , s' ,cumsum(out_dn)'])
sum(out)

% format as nice string
s='';
for ix=[1:length(out)]
  s=[s, '{ ',num2str(out(ix,1)), ' , ', num2str(out(ix,2)), ' , ', num2str(out(ix,3)), " }, \n"];
endfor

% save to text file
save s.txt s
