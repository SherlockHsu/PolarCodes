% File:         testPCsystem.m
% Brief:       测试Polar码系统码相关性能
% Author：  SherlockHsu
% Date:        2018/03/06

clear
close all;

addpath('./fun');

gcrc24A = [1 0 0 0 0 1 1 0 0 1 0 0 1 1 0 0 1 1 1 1 1 0 1 1]';
gcrc16 = [0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 1]';
gcrc4 = [0 0 1 1]';

% 仿真参数
N = 1024;       % 码长
R = 0.5;        % 码率
frozenbit = 0;      % 冻结比特
mapMethod = 'QPSK';     % 调制方式
gcrc = gcrc24A;     % CRC校验方式
Lcrc = length(gcrc);        % CRC校验位长度
K = N*R;
A = K - Lcrc;
n = log2(N);

Ns = 2^10*A;        % 信源比特数
ListnL = 0:1:4;     % 层数仿真列表
ListSNR = 0:0.5:6;      % 信噪比仿真列表

ListL = 2.^ListnL;      % 仿真码长列表
NListnL = size(ListnL,2);       % 码长仿真数目
NListSNR = size(ListSNR,2);     % 信噪比仿真数目
FZlookup = getFZlookup(N,K,0,frozenbit);        % 获得冻结位查询列表

% 开始仿真
s = randi([0,1],Ns,1);                                           % 信源序列
BERSYS = zeros(NListnL,NListSNR);                      % SC误码率列表
BERCASCL = zeros(NListnL,NListSNR);                    % SCL误码率列表
TimeSYS = zeros(NListnL,NListSNR);                    % SC时长列表
TimeCASCL = zeros(NListnL,NListSNR);                  % SCL时长列表

% 不同码长
for index_L = 1:NListnL
    
    L = ListL(index_L);                                         % 层数    
            
    % 不同信噪比
    for index_SNR = 1:NListSNR
                
        SNR = ListSNR(index_SNR);

        NerrCASCL = 0;
        NerrSYS = 0;
        
        % 分块仿真
        for i = 1:Ns/A
            
            a = s(1+(i-1)*A:A*i);       % 信号位
            u = addCRC(a,gcrc);     % 添加校验位
            
            x = PCencode(u,N,K,FZlookup);
            xSYS = PCencode(u,N,K,FZlookup);        % Polar编码
            xMap = map(x,mapMethod);
            xMap_sys = map(xSYS,mapMethod);     % Map
            y = awgn(xMap,SNR);
            ySYS = awgn(xMap_sys,SNR);      % 通过AWGN信道
            llr = demap_llr(y,SNR,mapMethod); 
            llr_sys = demap_llr(ySYS,SNR,mapMethod);        % 计算对数似然比
                        
            tic;        % 计时开始
            urCASCL = PCdecodeCASCL(llr,N,K,L,FZlookup,gcrc);
            TimeCASCL(index_L,index_SNR) =TimeCASCL(index_L,index_SNR) + toc;       % 计时结束
            
            tic;        % 计时开始
            urSYS = PCdecodeCASCL_up(llr_sys,N,K,L,FZlookup,gcrc);
            TimeSYS(index_L,index_SNR) =TimeSYS(index_L,index_SNR) + toc;       % 计时结束            
            
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