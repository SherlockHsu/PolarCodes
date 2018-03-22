function xMap = map_16QAM(x)
% data为数据输入
    if mod(length(x),4)==1
      x=[x; 0; 0; 0];
    elseif mod(length(x),4)==2
      x=[x; 0; 0];
    elseif mod(length(x),4)==3
      x=[x; 0];
    end
n_data_bits=length(x);
n_dataSymbol=n_data_bits/4;
b=(x);
d=zeros(n_dataSymbol,1);

%for k=0:(n_dataSymbol-1)
%    P=(1-2*b(4*k+1))*(2-(1-2*b(4*k+3)))/sqrt(10);
%    ImP=(1-2*b(4*k+2))*(2-(1-2*b(4*k+4)))/sqrt(10);
%    d(k+1)=P+1i*ImP;
%end

P=zeros(n_dataSymbol,1);
ImP=zeros(n_dataSymbol,1);


P(1:1:end)=(1-2*b(1:4:end)).*(2-(1-2*b(3:4:end)))/sqrt(10);
ImP(1:1:end)=(1-2*b(2:4:end)).*(2-(1-2*b(4:4:end)))/sqrt(10);

d(1:1:end)=P(1:1:end)+1i*ImP(1:1:end);

xMap=d;

end

