% File:         testPolarCodesSC.m
% Brief:       ����PolarCodesSC��������
% Author��  SherlockHsu
% Email��    SherlockHsu@foxmail.com
% Version:   3.2.0
% Date:        2017/11/10

clear
close all;

addpath('./fun');

% �������
R = 0.5;                                                               % ����
frozenbit = 0;                                                      % �������

Ns = 2^20;                                                         % ��Դ������
Listn = 4:1:10;                                                     % ���������б�
ListSNR = 0:0.5:6;                                                % ����ȷ����б�

ListN = 2.^Listn;                                                 % �����볤�б�
NListn = size(Listn,2);                                         % �볤������Ŀ
NListSNR = size(ListSNR,2);                               % ����ȷ�����Ŀ

% ��ʼ����
s = randi([0,1],Ns,1);                                           % ��Դ����
BER = zeros(NListn,NListSNR);                          % �������б�
Time = zeros(NListn,NListSNR);                        % ʱ���б�

% ��ͬ�볤
for index_n = 1:NListn
    
    n = Listn(index_n);                                          % ����
    N = 2^n;                                                         % �볤
    K = N*R;                                                         % ��Ϣλ��    
    FZlookup = getFZlookup(N,K,0,frozenbit);   % ��ö���λ��ѯ�б�
            
    % ��ͬ�����
    for index_SNR = 1:NListSNR
        
        tic;                                                               % ��ʼ��ʱ
        
        SNR = ListSNR(index_SNR);

        Nerr = 0;
        
        % �ֿ����
        for i = 1:Ns/K
            
            u = s(1+(i-1)*K:K*i);                                % �ź�λ
            x = PCencode(u,N,K,FZlookup);             % Polar����
            xMap = BPSK(x);                                     % BPSK
            y = awgn(xMap,SNR);                             % ͨ��AWGN�ŵ�
            llr = -2*y*10^(SNR/10);                  % ���������Ȼ��
            ur = SCL_decoder(llr,N,K,4,FZlookup);      % SC����
            Nerr = Nerr + sum(abs(u-ur));
            
        end
        
        BER(index_n,index_SNR) = Nerr/Ns; 
        Time(index_n,index_SNR) = toc;               % ��ʱ����
        
        fprintf('N: %d\tSNR: %.2f(dB)\tBER: %f\tTime: %fs\n',...
            ListN(index_n),...
            ListSNR(index_SNR),...
            BER(index_n,index_SNR),...
            Time(index_n,index_SNR));
                
    end
    
end

save('data.mat');