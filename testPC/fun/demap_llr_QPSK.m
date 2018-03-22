function llr = demap_llr_QPSK(y,SNR)
%QPSK_LLR �˴���ʾ�йش˺�����ժҪ
%  inputΪ���ն��յ��ĵ��Ʒ���,SNRΪ�����

i_length=length(y);
LLR_length=i_length*2;
llr=zeros(1,LLR_length);
delta2=1/(10^(SNR/10));
llr(1:2:end) = sqrt(2)*real(y)./delta2;
llr(2:2:end) = sqrt(2)*imag(y)./delta2;     

end

