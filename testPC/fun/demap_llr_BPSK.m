function llr = demap_llr_BPSK(y,SNR)

sigma2 = 10^(SNR/10);
llr = 2*y*sigma2;

end