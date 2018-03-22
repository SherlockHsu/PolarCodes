function xMap = map_QPSK(x)
% data为输入数据

if mod(length(x),2)
    x=[x 0];
end
n_data_bits=length(x);

n_dataSymbol=n_data_bits/2;
b=(x);
d=zeros(1,n_dataSymbol);
for k=0:(n_dataSymbol-1)
    P=(1-2*b(2*k+1))/sqrt(2);
    ImP=(1-2*b(2*k+2))/sqrt(2);
    d(k+1)=P+1i*ImP;
end
 
xMap=d;
    
  
end

