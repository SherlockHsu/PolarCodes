% File:         testPCsystem.m
% Brief:       ����Polar��ϵͳ���������
% Author��  SherlockHsu
% Date:        2018/03/06

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
mapMethod = 'QPSK';     % ���Ʒ�ʽ
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
BERSYS = zeros(NListnL,NListSNR);                      % SC�������б�
BERCASCL = zeros(NListnL,NListSNR);                    % SCL�������б�
TimeSYS = zeros(NListnL,NListSNR);                    % SCʱ���б�
TimeCASCL = zeros(NListnL,NListSNR);                  % SCLʱ���б�

% ��ͬ�볤
for index_L = 1:NListnL
    
    L = ListL(index_L);                                         % ����    
            
    % ��ͬ�����
    for index_SNR = 1:NListSNR
                
        SNR = ListSNR(index_SNR);

        NerrCASCL = 0;
        NerrSYS = 0;
        
        % �ֿ����
        for i = 1:Ns/A
            
            a = s(1+(i-1)*A:A*i);       % �ź�λ
            u = addCRC(a,gcrc);     % ���У��λ
            
            x = PCencode(u,N,K,FZlookup);
            xSYS = PCencode(u,N,K,FZlookup);        % Polar����
            xMap = map(x,mapMethod);
            xMap_sys = map(xSYS,mapMethod);     % Map
            y = awgn(xMap,SNR);
            ySYS = awgn(xMap_sys,SNR);      % ͨ��AWGN�ŵ�
            llr = demap_llr(y,SNR,mapMethod); 
            llr_sys = demap_llr(ySYS,SNR,mapMethod);        % ���������Ȼ��
                        
            tic;        % ��ʱ��ʼ
            urCASCL = PCdecodeCASCL(llr,N,K,L,FZlookup,gcrc);
            TimeCASCL(index_L,index_SNR) =TimeCASCL(index_L,index_SNR) + toc;       % ��ʱ����
            
            tic;        % ��ʱ��ʼ
            urSYS = PCdecodeCASCL_up(llr_sys,N,K,L,FZlookup,gcrc);
            TimeSYS(index_L,index_SNR) =TimeSYS(index_L,index_SNR) + toc;       % ��ʱ����            
            
            arCASCL = deleteCRC(urCASCL,gcrc);
            arSYS = deleteCRC(urSYS,gcrc);
            
            NerrCASCL = NerrCASCL + sum(abs(a-arCASCL));
            NerrSYS = NerrSYS + sum(abs(a-arSYS));
            
        end
                
        BERCASCL(index_L,index_SNR) = NerrCASCL/Ns; 
        BERSYS(index_L,index_SNR) = NerrSYS/Ns;         
        
        fprintf('L: %d\tSNR: %.2f(dB)\tCA(BER: %f Time: %.2fs)\tSYS(BER: %f Time: %.2fs)\n',...
            ListL(index_L),...
            ListSNR(index_SNR),...
            BERCASCL(index_L,index_SNR),...
            TimeCASCL(index_L,index_SNR),...
            BERSYS(index_L,index_SNR),...
            TimeSYS(index_L,index_SNR));
                
    end
    
end

save('data_SYS.mat');