function llr = demap_llr_QPSK(y,SNR)
%QPSK_LLR 此处显示有关此函数的摘要
%  input为接收端收到的调制符号,SNR为信噪比

i_length=length(y);
LLR_length=i_length*2;
llr=zeros(1,LLR_length);
delta2=1/(10^(SNR/10));
llr(1:2:end) = sqrt(2)*real(y)./delta2;
llr(2:2:end) = sqrt(2)*imag(y)./delta2;     

end

