function llr = demap_llr(y,SNR,mapMethod)

switch mapMethod
    case 'QPSK'
        llr = demap_llr_QPSK(y,SNR);
    case '16QAM'
        llr = demap_llr_16QAM(y,SNR);
    otherwise
        llr = demap_llr_BPSK(y,SNR);
end

end