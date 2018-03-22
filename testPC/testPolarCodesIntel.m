% File:         testPolarCodesCASCL.m
% Brief:       测试PolarCodesCASCL译码性能
% Author：  SherlockHsu
% Version:   3.2.0
% Date:        2017/11/10

clear
close all;

addpath('./fun');


% 仿真参数
R = 0.5;                                                               % 码率
frozenbit = 0;                                                      % 冻结比特
N = 1024;
mapMethod = '4PSK';

gcrc24A = [1 0 0 0 0 1 1 0 0 1 0 0 1 1 0 0 1 1 1 1 1 0 1 1]';
gcrc16 = [0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 1]';
gcrc = gcrc24A;
Lcrc = length(gcrc);
K = N*R;
A = K - Lcrc;
n = log2(N);

Ns = 2^15*A;                                                     % 信源比特数
ListnL = 0:1:4;                                                     % 层数仿真列表
ListSNR = 0:0.5:6;                                               % 信噪比仿真列表

ListL = 2.^ListnL;                                                 % 仿真码长列表
NListnL = size(ListnL,2);                                         % 码长仿真数目
NListSNR = size(ListSNR,2);                               % 信噪比仿真数目
FZlookup = getFZlookup(N,K,0,frozenbit);        % 获得冻结位查询列表

% 开始仿真
s = randi([0,1],Ns,1);                                           % 信源序列
BERCASCL_ICC = zeros(NListnL,NListSNR);                      % SC误码率列表
BERCASCL = zeros(NListnL,NListSNR);                    % SCL误码率列表
TimeCASCL_ICC = zeros(NListnL,NListSNR);                    % SC时长列表
TimeCASCL = zeros(NListnL,NListSNR);                  % SCL时长列表

% 不同码长
for index_L = 1:NListnL
    
    L = ListL(index_L);                                         % 层数    
            
    % 不同信噪比
    for index_SNR = 1:NListSNR
                
        SNR = ListSNR(index_SNR);

        NerrCASCL = 0;
        NerrCASCL_ICC = 0;
        
        % 分块仿真
        for i = 1:Ns/A
            
            a = s(1+(i-1)*A:A*i);                                % 信号位
            u = addCRC(a,gcrc);
            x = PCencode(u,N,K,FZlookup);             % Polar编码                                
            xMap = map(x,mapMethod);                    % Map
            y = awgn(xMap,SNR);                             % 通过AWGN信道
            llr = getLLR(y,SNR,mapMethod);              % 计算对数似然比
            
            tic;
            urCASCL_ICC = PCdecodeCASCL_up(llr,N,K,L,FZlookup,gcrc);
            TimeCASCL_ICC(index_L,index_SNR) =TimeCASCL_ICC(index_L,index_SNR) + toc;               % 计时结束
            
            tic;
            urCASCL = PCdecodeCASCL_intel(llr,N,K,L,FZlookup,gcrc);
            TimeCASCL(index_L,index_SNR) =TimeCASCL(index_L,index_SNR) + toc;               % 计时结束
            
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