% File:         testPolarCodesSC.m
% Brief:       测试PolarCodesSC译码性能
% Author：  SherlockHsu
% Email：    SherlockHsu@foxmail.com
% Version:   1.2.0
% Date:        2017/11/10

clear
close all;

addpath('./fun');


% 仿真参数
R = 0.5;                                                               % 码率
frozenbit = 0;                                                      % 冻结比特
L = 16;

Ns = 2^20;                                                         % 信源比特数
Listn = 10:1:10;                                                     % 层数仿真列表
ListSNR = 1.5:0.5:6;                                               % 信噪比仿真列表

ListN = 2.^Listn;                                                 % 仿真码长列表
NListn = size(Listn,2);                                         % 码长仿真数目
NListSNR = size(ListSNR,2);                               % 信噪比仿真数目

% 开始仿真
s = randi([0,1],Ns,1);                                           % 信源序列
BERSC = zeros(NListn,NListSNR);                      % SC误码率列表
BERSCL = zeros(NListn,NListSNR);                    % SCL误码率列表
TimeSC = zeros(NListn,NListSNR);                    % SC时长列表
TimeSCL = zeros(NListn,NListSNR);                  % SCL时长列表

% 不同码长
for index_n = 1:NListn
    
    n = Listn(index_n);                                         % 层数
    N = 2^n;                                                         % 码长
    K = N*R;                                                         % 信息位长    
    FZlookup = getFZlookup(N,K,0,frozenbit);   % 获得冻结位查询列表
            
    % 不同信噪比
    for index_SNR = 1:NListSNR
        

        
        SNR = ListSNR(index_SNR);

        NerrSC = 0;
        NerrSCL = 0;
        
        % 分块仿真
        for i = 1:Ns/K
            
            u = s(1+(i-1)*K:K*i);                                % 信号位
            x = PCencode(u,N,K,FZlookup);             % Polar编码
            xMap = BPSK(x);                                     % BPSK
            y = awgn(xMap,SNR);                             % 通过AWGN信道
            llr = -2*y*10^(SNR/10);                  % 计算对数似然比
            
            tic;
            urSC = PCdecodeSCL(llr,N,K,2,FZlookup);      % SC译码
            TimeSC(index_n,index_SNR) =TimeSC(index_n,index_SNR) + toc;               % 计时结束
            
            tic;
            urSCL = PCdecodeSCL(llr,N,K,4,FZlookup);
            TimeSCL(index_n,index_SNR) =TimeSCL(index_n,index_SNR) + toc;               % 计时结束
            
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