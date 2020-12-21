clear all

nSteps=1000;
dur_ms=1000;

tab=[
[ 16 , 5682 ]; 
[ 47 , 1934 ]; 
[ 75 , 1212 ]; 
[ 96 , 947 ]; 
[ 109 , 834 ]; 
[ 114 , 797 ]; 
[ 109 , 834 ]; 
[ 96 , 947 ]; 
[ 75 , 1212 ]; 
[ 47 , 1934 ]; 
[ 16 , 5682 ]]

N=length(tab);
s=zeros(1,N)
p=zeros(1,N)
for ix=1:N
  s(ix)=nSteps * tab(ix,1) / 800;
  p(ix)=dur_ms * tab(ix,2) * 4 / (5 * nSteps);
end
