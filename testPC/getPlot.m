% File:         getPlot.m
% Brief:       ��ò�ͬ�볤�µ�SC������������
% Author�� SherlockHsu
% Email��   SherlockHsu@foxmail.com
% Version:  1.2.0
% Date:       2017/11/19

load('data02.mat');

figure;
for i = 1:NListn
    semilogy(ListSNR,BER(i,:));
    hold on;
end
legend(['N=',num2str(ListN(1))],...
    ['N=',num2str(ListN(2))],...
    ['N=',num2str(ListN(3))],...
    ['N=',num2str(ListN(4))],...
    ['N=',num2str(ListN(5))],...
    ['N=',num2str(ListN(6))],...
    ['N=',num2str(ListN(7))],...
    ['N=',num2str(ListN(8))],...
    ['N=',num2str(ListN(9))],...
    ['N=',num2str(ListN(10))],...
    ['N=',num2str(ListN(11))],...
    ['N=',num2str(ListN(12))]...
    );

% figure;
% for i = 5:2:11
%     semilogy(ListSNR,BER(i,:));
%     hold on;
% end
% legend(['N=',num2str(ListN(5))],...
%     ['N=',num2str(ListN(7))],...
%     ['N=',num2str(ListN(9))],...
%     ['N=',num2str(ListN(11))]...
%     );

xlabel('Eb/N0(dB)');
ylabel('BER');
title('��ͬ�볤�µ�SC������������(R=0.5, N_s=2\^23)');
grid on;

figure;
plot(Listn,mean(Time,2),'*');
xlabel('n');
ylabel('Time(s)');
title('��ͬ�볤�µ�SC����ʱ��(R=0.5, N_s=2\^23)');
grid on;