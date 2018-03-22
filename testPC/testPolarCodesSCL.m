% File:         testPolarCodesSC.m
% Brief:       ����PolarCodesSC��������
% Author��  SherlockHsu
% Email��    SherlockHsu@foxmail.com
% Version:   1.2.0
% Date:        2017/11/10

clear
close all;

addpath('./fun');


% �������
R = 0.5;                                                               % ����
frozenbit = 0;                                                      % �������
L = 16;

Ns = 2^20;                                                         % ��Դ������
Listn = 10:1:10;                                                     % ���������б�
ListSNR = 1.5:0.5:6;                                               % ����ȷ����б�

ListN = 2.^Listn;                                                 % �����볤�б�
NListn = size(Listn,2);                                         % �볤������Ŀ
NListSNR = size(ListSNR,2);                               % ����ȷ�����Ŀ

% ��ʼ����
s = randi([0,1],Ns,1);                                           % ��Դ����
BERSC = zeros(NListn,NListSNR);                      % SC�������б�
BERSCL = zeros(NListn,NListSNR);                    % SCL�������б�
TimeSC = zeros(NListn,NListSNR);                    % SCʱ���б�
TimeSCL = zeros(NListn,NListSNR);                  % SCLʱ���б�

% ��ͬ�볤
for index_n = 1:NListn
    
    n = Listn(index_n);                                         % ����
    N = 2^n;                                                         % �볤
    K = N*R;                                                         % ��Ϣλ��    
    FZlookup = getFZlookup(N,K,0,frozenbit);   % ��ö���λ��ѯ�б�
            
    % ��ͬ�����
    for index_SNR = 1:NListSNR
        

        
        SNR = ListSNR(index_SNR);

        NerrSC = 0;
        NerrSCL = 0;
        
        % �ֿ����
        for i = 1:Ns/K
            
            u = s(1+(i-1)*K:K*i);                                % �ź�λ
            x = PCencode(u,N,K,FZlookup);             % Polar����
            xMap = BPSK(x);                                     % BPSK
            y = awgn(xMap,SNR);                             % ͨ��AWGN�ŵ�
            llr = -2*y*10^(SNR/10);                  % ���������Ȼ��
            
            tic;
            urSC = PCdecodeSCL(llr,N,K,2,FZlookup);      % SC����
            TimeSC(index_n,index_SNR) =TimeSC(index_n,index_SNR) + toc;               % ��ʱ����
            
            tic;
            urSCL = PCdecodeSCL(llr,N,K,4,FZlookup);
            TimeSCL(index_n,index_SNR) =TimeSCL(index_n,index_SNR) + toc;               % ��ʱ����
            
            NerrSC = NerrSC + sum(abs(u-urSC));
            NerrSCL = NerrSCL + sum(abs(u-urSCL));
            
        end
        
        BERSC(index_n,index_SNR) = NerrSC/Ns; 
        BERSCL(index_n,index_SNR) = NerrSCL/Ns; 
        
        
        fprintf('N: %d\tSNR: %.2f(dB)\tBERSC: %f\tBERSCL: %f\tTimeSC: %fs \tTimeSCL: %fs\n',...
            ListN(index_n),...
            ListSNR(index_SNR),...
            BERSC(index_n,index_SNR),...
            BERSCL(index_n,index_SNR),...
            TimeSC(index_n,index_SNR),...
            TimeSCL(index_n,index_SNR));
                
    end
    
end

save('data.mat');