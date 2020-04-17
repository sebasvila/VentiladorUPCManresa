clear all

T=1;    % 100 seconds
A=800;  % 80000 steps (10 revolutions)

N=11;

D=T/N

us=1e-6;
ms=1e-3;
t=[0:D:T];
n=A/2*(1-cos(pi*t/T))

dn=diff(n);

out_dn=round(dn);

disp(['This should be ',num2str(A),' : ',num2str(sum(out_dn))])


# out_dn(round(N/2))=out_dn(round(N/2))-1    % <---if odd
#  out_dn(round(N/2):round(N/2)+1)=out_dn(round(N/2):round(N/2)+1)-1

s=D./out_dn/us;

out=round([t(2:end)'/ms , s' ,(out_dn)'])
sum(out)

% format as nice string
s='';
for ix=[1:length(out)]
  s=[s, '{ ',num2str(out(ix,3)), ' , ', num2str(out(ix,2)) " }, \n"];
endfor

% save to text file
save s.txt s
