% File:         testPolarCodesCASCL.m
% Brief:       ����PolarCodesCASCL��������
% Author��  SherlockHsu
% Version:   3.2.0
% Date:        2017/11/10

clear
close all;

addpath('./fun');

gcrc24A = [1 0 0 0 0 1 1 0 0 1 0 0 1 1 0 0 1 1 1 1 1 0 1 1]';
gcrc16 = [0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 1]';
gcrc4 = [0 0 1 1]';

% �������
N = 1024;       % �볤
R = 0.5;        % ����
frozenbit = 0;      % �������
mapMethod = 'BPSK';     % ���Ʒ�ʽ
gcrc = gcrc24A;     % CRCУ�鷽ʽ
Lcrc = length(gcrc);        % CRCУ��λ����
K = N*R;
A = K - Lcrc;
n = log2(N);

Ns = 2^10*A;        % ��Դ������
ListnL = 0:1:4;     % ���������б�
ListSNR = 0:0.5:6;      % ����ȷ����б�

ListL = 2.^ListnL;      % �����볤�б�
NListnL = size(ListnL,2);       % �볤������Ŀ
NListSNR = size(ListSNR,2);     % ����ȷ�����Ŀ
FZlookup = getFZlookup(N,K,0,frozenbit);        % ��ö���λ��ѯ�б�

% ��ʼ����
s = randi([0,1],Ns,1);                                           % ��Դ����
BERSCL = zeros(NListnL,NListSNR);                      % SC�������б�
BERCASCL = zeros(NListnL,NListSNR);                    % SCL�������б�
TimeSCL = zeros(NListnL,NListSNR);                    % SCʱ���б�
TimeCASCL = zeros(NListnL,NListSNR);                  % SCLʱ���б�

% ��ͬ�볤
for index_L = 1:NListnL
    
    L = ListL(index_L);                                         % ����    
            
    % ��ͬ�����
    for index_SNR = 1:NListSNR
                
        SNR = ListSNR(index_SNR);

        NerrCASCL = 0;
        NerrSCL = 0;
        
        % �ֿ����
        for i = 1:Ns/A
            
            a = s(1+(i-1)*A:A*i);                                % �ź�λ
            u = addCRC(a,gcrc);
            x = PCencode(u,N,K,FZlookup);             % Polar����                                
            xMap = map(x,mapMethod);                    % Map
            y = awgn(xMap,SNR);                             % ͨ��AWGN�ŵ�
            llr = demap_llr(y,SNR,mapMethod);              % ���������Ȼ��
            
            tic;
            urSCL = PCdecodeSCL(llr,N,K,L,FZlookup);
            TimeSCL(index_L,index_SNR) =TimeSCL(index_L,index_SNR) + toc;               % ��ʱ����
                        
            tic;
            urCASCL = PCdecodeCASCL(llr,N,K,L,FZlookup,gcrc);
            TimeCASCL(index_L,index_SNR) =TimeCASCL(index_L,index_SNR) + toc;               % ��ʱ����
            
            arSCL = deleteCRC(urSCL,gcrc);
            arCASCL = deleteCRC(urCASCL,gcrc);
            
            NerrCASCL = NerrCASCL + sum(abs(a-arCASCL));
            NerrSCL = NerrSCL + sum(abs(a-arSCL));
            
        end
        
        BERSCL(index_L,index_SNR) = NerrSCL/Ns; 
        BERCASCL(index_L,index_SNR) = NerrCASCL/Ns; 
        
        
        fprintf('L: %d\tSNR: %.2f(dB)\tBERSCL: %f TimeSCL: %.2fs\tBERCA: %f TimeCA: %.2fs\n',...
            ListL(index_L),...
            ListSNR(index_SNR),...
            BERSCL(index_L,index_SNR),...
            TimeSCL(index_L,index_SNR),...
            BERCASCL(index_L,index_SNR),...
            TimeCASCL(index_L,index_SNR));
                
    end
    
end

save('data_0.5_1024_QPSK01.mat');