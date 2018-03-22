% File:         testPolarCodesCASCL.m
% Brief:       ����PolarCodesCASCL��������
% Author��  SherlockHsu
% Version:   3.2.0
% Date:        2017/11/10

clear
close all;

addpath('./fun');


% �������
R = 0.5;                                                               % ����
frozenbit = 0;                                                      % �������
N = 1024;
mapMethod = '4PSK';

gcrc24A = [1 0 0 0 0 1 1 0 0 1 0 0 1 1 0 0 1 1 1 1 1 0 1 1]';
gcrc16 = [0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 1]';
gcrc = gcrc24A;
Lcrc = length(gcrc);
K = N*R;
A = K - Lcrc;
n = log2(N);

Ns = 2^15*A;                                                     % ��Դ������
ListnL = 0:1:4;                                                     % ���������б�
ListSNR = 0:0.5:6;                                               % ����ȷ����б�

ListL = 2.^ListnL;                                                 % �����볤�б�
NListnL = size(ListnL,2);                                         % �볤������Ŀ
NListSNR = size(ListSNR,2);                               % ����ȷ�����Ŀ
FZlookup = getFZlookup(N,K,0,frozenbit);        % ��ö���λ��ѯ�б�

% ��ʼ����
s = randi([0,1],Ns,1);                                           % ��Դ����
BERCASCL_ICC = zeros(NListnL,NListSNR);                      % SC�������б�
BERCASCL = zeros(NListnL,NListSNR);                    % SCL�������б�
TimeCASCL_ICC = zeros(NListnL,NListSNR);                    % SCʱ���б�
TimeCASCL = zeros(NListnL,NListSNR);                  % SCLʱ���б�

% ��ͬ�볤
for index_L = 1:NListnL
    
    L = ListL(index_L);                                         % ����    
            
    % ��ͬ�����
    for index_SNR = 1:NListSNR
                
        SNR = ListSNR(index_SNR);

        NerrCASCL = 0;
        NerrCASCL_ICC = 0;
        
        % �ֿ����
        for i = 1:Ns/A
            
            a = s(1+(i-1)*A:A*i);                                % �ź�λ
            u = addCRC(a,gcrc);
            x = PCencode(u,N,K,FZlookup);             % Polar����                                
            xMap = map(x,mapMethod);                    % Map
            y = awgn(xMap,SNR);                             % ͨ��AWGN�ŵ�
            llr = getLLR(y,SNR,mapMethod);              % ���������Ȼ��
            
            tic;
            urCASCL_ICC = PCdecodeCASCL_up(llr,N,K,L,FZlookup,gcrc);
            TimeCASCL_ICC(index_L,index_SNR) =TimeCASCL_ICC(index_L,index_SNR) + toc;               % ��ʱ����
            
            tic;
            urCASCL = PCdecodeCASCL_intel(llr,N,K,L,FZlookup,gcrc);
            TimeCASCL(index_L,index_SNR) =TimeCASCL(index_L,index_SNR) + toc;               % ��ʱ����
            
            arCASCL_ICC = deleteCRC(urCASCL_ICC,gcrc);
            arCASCL = deleteCRC(urCASCL,gcrc);
            
            NerrCASCL = NerrCASCL + sum(abs(a-arCASCL));
            NerrCASCL_ICC = NerrCASCL_ICC + sum(abs(a-arCASCL_ICC));
            
        end
        
        BERCASCL_ICC(index_L,index_SNR) = NerrCASCL_ICC/Ns; 
        BERCASCL(index_L,index_SNR) = NerrCASCL/Ns; 
        
        
        fprintf('L: %d\tSNR: %.2f(dB)\tBER(ICC): %f Time(ICC): %.2fs\tBER(AVX): %f Time(AVXs): %.2fs\n',...
            ListL(index_L),...
            ListSNR(index_SNR),...
            BERCASCL_ICC(index_L,index_SNR),...
            TimeCASCL_ICC(index_L,index_SNR),...
            BERCASCL(index_L,index_SNR),...
            TimeCASCL(index_L,index_SNR));
                
    end
    
end

save('data_0.5_1024_QPSK01.mat');