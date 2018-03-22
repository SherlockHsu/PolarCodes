function b = addCRC(a,gcrc)

Lcrc = length(gcrc);
A = length(a);
K = A+Lcrc;
atem = [a;zeros(Lcrc,1)];
reg = a(1:Lcrc);
for i = Lcrc+1:K
    tem = reg(1);
    for j =1:Lcrc-1
        reg(j) = reg(j+1);
    end
    reg(Lcrc) = atem(i);
    if(tem==1)
        reg = mod((reg+gcrc),2);
    end
end
b = [a;reg];

end