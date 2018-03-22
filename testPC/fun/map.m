function xMap = map(x,mapMethod)

switch mapMethod
    case 'QPSK'
        xMap = map_QPSK(x);
    case '16QAM'
        xMap = map_16QAM(x);
    otherwise
        xMap = map_BPSK(x);
end

end