% File:         testPolarCodesSC.m
% Brief:       测试PolarCodesSC译码性能
% Author：  SherlockHsu
% Email：    SherlockHsu@foxmail.com
% Version:   3.2.0
% Date:        2017/11/10

clear
close all;

addpath('./fun');

% 仿真参数
R = 0.5;                                                               % 码率
frozenbit = 0;                                                      % 冻结比特

Ns = 2^20;                                                         % 信源比特数
Listn = 4:1:10;                                                     % 层数仿真列表
ListSNR = 0:0.5:6;                                                % 信噪比仿真列表

ListN = 2.^Listn;                                                 % 仿真码长列表
NListn = size(Listn,2);                                         % 码长仿真数目
NListSNR = size(ListSNR,2);                               % 信噪比仿真数目

% 开始仿真
s = randi([0,1],Ns,1);                                           % 信源序列
BER = zeros(NListn,NListSNR);                          % 误码率列表
Time = zeros(NListn,NListSNR);                        % 时长列表

% 不同码长
for index_n = 1:NListn
    
    n = Listn(index_n);                                          % 层数
    N = 2^n;                                                         % 码长
    K = N*R;                                                         % 信息位长    
    FZlookup = getFZlookup(N,K,0,frozenbit);   % 获得冻结位查询列表
            
    % 不同信噪比
    for index_SNR = 1:NListSNR
        
        tic;                                                               % 开始计时
        
        SNR = ListSNR(index_SNR);

        Nerr = 0;
        
        % 分块仿真
        for i = 1:Ns/K
            
            u = s(1+(i-1)*K:K*i);                                % 信号位
            x = PCencode(u,N,K,FZlookup);             % Polar编码
            xMap = BPSK(x);                                     % BPSK
            y = awgn(xMap,SNR);                             % 通过AWGN信道
            llr = -2*y*10^(SNR/10);                  % 计算对数似然比
            ur = SCL_decoder(llr,N,K,4,FZlookup);      % SC译码
            Nerr = Nerr + sum(abs(u-ur));
            
        end
        
        BER(index_n,index_SNR) = Nerr/Ns; 
        Time(index_n,index_SNR) = toc;               % 计时结束
        
        fprintf('N: %d\tSNR: %.2f(dB)\tBER: %f\tTime: %fs\n',...
            ListN(index_n),...
            ListSNR(index_SNR),...
            BER(index_n,index_SNR),...
            Time(index_n,index_SNR));
                
    end
    
end

save('data.mat');