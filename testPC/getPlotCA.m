%load('dataCA03.mat');
%load('dataSC00');

figure;

semilogy(ListSNR,BERSCL(1,:),'g*-.');
hold on;
semilogy(ListSNR,BERSCL(2,:),'m+--');
semilogy(ListSNR,BERCASCL(2,:),'b+-');
semilogy(ListSNR,BERSCL(3,:),'m<--');
semilogy(ListSNR,BERCASCL(3,:),'b<-');
semilogy(ListSNR,BERSCL(4,:),'ms--');
semilogy(ListSNR,BERCASCL(4,:),'bs-');
semilogy(ListSNR,BERSCL(5,:),'md--');
semilogy(ListSNR,BERCASCL(5,:),'bd-');

legend('SC',...
    ['SCL, L=',num2str(ListL(2))],...
    ['CA-SCL, L=',num2str(ListL(2))],...
    ['SCL, L=',num2str(ListL(3))],...
    ['CA-SCL, L=',num2str(ListL(3))],...
    ['SCL, L=',num2str(ListL(4))],...
    ['CA-SCL, L=',num2str(ListL(4))],...
    ['SCL, L=',num2str(ListL(5))],...
    ['CA-SCL, L=',num2str(ListL(5))]...
    );

xlabel('Eb/N0(dB)');
ylabel('BER');
title('PolarÂëÒëÂëÎóÂëÇúÏß(N = 1024, R=0.5, Lcrc=24, 16QAM)');
grid on;