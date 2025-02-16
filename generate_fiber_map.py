print("ap_uint<6> fiber_map[24][12]={")
startbin=1
fiber_map=[]
for nrow in range(24):
    fiber_map.append([])
    for ncol in range(12):
        if nrow==0 or nrow==23 or ncol==0 or ncol==11:   # permieter blocks
           fiber_map[nrow].append(0)
        else:
           nbin=startbin+(ncol-1)//2
           fiber_map[nrow].append(nbin)
    if nrow>0 and (nrow-1)%2==1:
       startbin=nbin+1
           

print(fiber_map)
print("};")
